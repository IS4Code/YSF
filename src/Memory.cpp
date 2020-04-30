#include <stdint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#include <link.h>
#endif

#include "includes/platform.h"
#include "Memory.h"

// Y_Less - original YSF
bool Unlock(void *address, size_t len)
{
#ifdef _WIN32
	DWORD
		oldp;
	// Shut up the warnings :D
	return !!VirtualProtect(address, len, PAGE_EXECUTE_READWRITE, &oldp);
#else
	size_t
		iPageSize = getpagesize(),
		iAddr = ((reinterpret_cast <uint32_t>(address) / iPageSize) * iPageSize);
	return !mprotect(reinterpret_cast <void*>(iAddr), len, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif
}

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask)
{
	for (; *mask; ++mask, ++data, ++pattern)
	{
		if (*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}

DWORD FindPattern(char *pattern, char *mask)
{
#ifdef _WIN32
	MODULEINFO info = {0};

	DWORD address = (DWORD)GetModuleHandle(NULL);
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &info, sizeof(MODULEINFO));
	DWORD size = (DWORD)info.SizeOfImage;

	for(DWORD i = 0; i < size; ++i)
	{
		if(memory_compare((BYTE *)(address + i), (BYTE *)pattern, mask))
		{
			return (DWORD)(address + i);
		}
	}
	return 0;
#else
	struct {
		DWORD result;
		char *pattern;
		char *mask;
	} pinfo;
	pinfo.result = 0;
	pinfo.pattern = pattern;
	pinfo.mask = mask;
	dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void *data) {
		auto info2 = reinterpret_cast<decltype(pinfo)*>(data);
		for(int s = 0; s < info->dlpi_phnum; s++)
		{
			DWORD address = info->dlpi_addr + info->dlpi_phdr[s].p_vaddr;
			DWORD size = info->dlpi_phdr[s].p_memsz;
			for(DWORD i = 0; i < size; ++i)
			{
				if(memory_compare((BYTE *)(address + i), (BYTE *)info2->pattern, info2->mask))
				{
					info2->result = (DWORD)(address + i);
					return 1;
				}
			}
		}
		return 1;
	}, reinterpret_cast<void*>(&pinfo));
	return pinfo.result;
#endif
}

void InstallJump(unsigned long addr, void *func)
{
#ifdef WIN32
	unsigned long dwOld;
	VirtualProtect((LPVOID)addr, 5, PAGE_EXECUTE_READWRITE, &dwOld);
#else
	int pagesize = sysconf(_SC_PAGE_SIZE);
	void *unpraddr = (void *)(((int)addr + pagesize - 1) & ~(pagesize - 1)) - pagesize;
	mprotect(unpraddr, pagesize, PROT_WRITE);
#endif
	*(unsigned char *)addr = 0xE9;
	*(unsigned long *)((unsigned long)addr + 0x1) = (unsigned long)func - (unsigned long)addr - 5;
#ifdef WIN32
	VirtualProtect((LPVOID)addr, 5, dwOld, &dwOld);
#else
	mprotect(unpraddr, pagesize, PROT_READ | PROT_EXEC);
#endif
}