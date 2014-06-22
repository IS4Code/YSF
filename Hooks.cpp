/*  
 *  Version: MPL 1.1
 *  
 *  The contents of this file are subject to the Mozilla Public License Version 
 *  1.1 (the "License"); you may not use this file except in compliance with 
 *  the License. You may obtain a copy of the License at 
 *  http://www.mozilla.org/MPL/
 *  
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *  
 *  The Original Code is the YSI 2.0 SA:MP plugin.
 *  
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (C) 2008
 *  the Initial Developer. All Rights Reserved.
 *  
 *  Contributor(s):
 *  
 *  Peter Beverloo
 *  Marcus Bauer
 *  MaVe;
 *  Sammy91
 *  Incognito
 *  
 *  Special Thanks to:
 *  
 *  SA:MP Team past, present and future
 */

#include "main.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <Windows.h>
	#include <Psapi.h>
#else
	#include <stdio.h>
	#include <sys/mman.h>
	#include <limits.h>
	#include <string.h>
	#include <algorithm>
#endif

#ifndef PAGESIZE
	#define PAGESIZE (4096)
#endif

//typedef void far            *LPVOID;

AddServerRule_t g_pCConsole__AddRule = 0;
SetServerRule_t g_pCConsole__SetRule = 0;
RemoveServerRule_t g_pCConsole__RemoveRule = 0;
ModifyFlag_t g_pCConsole__MFlag = 0;
LoadFS_t g_pCFilterscript__LoadFS_t = 0;
UnLoadFS_t g_pCFilterscript__UnLoadFS_t = 0;

// Y_Less - original YSF
bool Unlock(void *address, size_t len)
{
	#ifdef WIN32
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

// Y_Less - fixes2
void AssemblySwap(char * addr, char * dat, int len)
{
	char
		swp;
	while (len--)
	{
		swp = addr[len];
		addr[len] = dat[len];
		dat[len] = swp;
	}
}

void AssemblyRedirect(void * from, void * to, char * ret)
{
	#ifdef LINUX
		size_t
			iPageSize = getpagesize(),
			iAddr = ((reinterpret_cast <uint32_t>(from) / iPageSize) * iPageSize),
			iCount = (5 / iPageSize) * iPageSize + iPageSize * 2;
		mprotect(reinterpret_cast <void*>(iAddr), iCount, PROT_READ | PROT_WRITE | PROT_EXEC);
		//mprotect(from, 5, PROT_READ | PROT_WRITE | PROT_EXEC);
	#else
		DWORD
			old;
		VirtualProtect(from, 5, PAGE_EXECUTE_READWRITE, &old);
	#endif
	*((unsigned char *)ret) = 0xE9;
	*((char **)(ret + 1)) = (char *)((char *)to - (char *)from) - 5;
	AssemblySwap((char *)from, ret, 5);
}

#ifdef WIN32
DWORD FindPattern(char *pattern, char *mask)
{
	#ifdef WIN32
	MODULEINFO mInfo = {0};
 
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &mInfo, sizeof(MODULEINFO));
 
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size =  (DWORD)mInfo.SizeOfImage;
	#else
 
	#endif
 
	DWORD patternLength = (DWORD)strlen(mask);
 
	for(DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for(DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}
 
		if(found) 
		{
			return base + i;
		}
	}
 
	return NULL;
} 
#endif

// From "amx.c", part of the PAWN language runtime:
// http://code.google.com/p/pawnscript/source/browse/trunk/amx/amx.c

#define USENAMETABLE(hdr) \
	((hdr)->defsize==sizeof(AMX_FUNCSTUBNT))

#define NUMENTRIES(hdr,field,nextfield) \
	(unsigned)(((hdr)->nextfield - (hdr)->field) / (hdr)->defsize)

#define GETENTRY(hdr,table,index) \
	(AMX_FUNCSTUB *)((unsigned char*)(hdr) + (unsigned)(hdr)->table + (unsigned)index*(hdr)->defsize)

#define GETENTRYNAME(hdr,entry) \
	(USENAMETABLE(hdr) ? \
		(char *)((unsigned char*)(hdr) + (unsigned)((AMX_FUNCSTUBNT*)(entry))->nameofs) : \
		((AMX_FUNCSTUB*)(entry))->name)

void Redirect(AMX * amx, char const * const from, ucell to, AMX_NATIVE * store)
{
	int
		num,
		idx;
	// Operate on the raw AMX file, don't use the amx_ functions to avoid issues
	// with the fact that we've not actually finished initialisation yet.  Based
	// VERY heavilly on code from "amx.c" in the PAWN runtime library.
	AMX_HEADER *
		hdr = (AMX_HEADER *)amx->base;
	AMX_FUNCSTUB *
		func;
	num = NUMENTRIES(hdr, natives, libraries);
	//logprintf("Redirect 1");
	for (idx = 0; idx != num; ++idx)
	{
		func = GETENTRY(hdr, natives, idx);
		//logprintf("Redirect 2 \"%s\" \"%s\"", from, GETENTRYNAME(hdr, func));
		if (!strcmp(from, GETENTRYNAME(hdr, func)))
		{
			//logprintf("Redirect 3");
			// Intercept the call!
			if (store)
			{
				*store = (AMX_NATIVE)func->address;
			}
			func->address = to;
			break;
		}
	}
}
///////////////////////////////////////////////////////////////
// Hooks //
///////////////////////////////////////////////////////////////
char gContainsInvalidCharsAssembly[5];
std::vector <char> gValidNameCharacters;

bool YSF_ContainsInvalidChars(char * szString)
{
	int i = 0;
	bool bIllegal = false;

	while(*szString) 
	{
		if( (*szString >= '0' && *szString <= '9') || (*szString >= 'A' && *szString <= 'Z') || (*szString >= 'a' && *szString <= 'z')  ||
			*szString == ']' || *szString == '[' || *szString == '_'  || *szString == '$' || *szString == ':' || *szString == '=' || 
			*szString == '(' || *szString == ')' || *szString == '@' ||  *szString == '.' ) 
		{

			szString++;
		} 
		else 
		{
			if(Contains(gValidNameCharacters, *szString)) 
			{
				szString++;
			}
			else
			{
				return true;
			}

		}
	}
	return false;
}

void GetAddresses()
{
	DWORD temp;

	// Console for adding rules.
	POINTER_TO_MEMBER(g_pCConsole__AddRule, CAddress::FUNC_CConsole_AddStringVariable, AddServerRule_t);
	POINTER_TO_MEMBER(g_pCConsole__SetRule, CAddress::FUNC_CConsole_SetStringVariable, SetServerRule_t);
//	POINTER_TO_MEMBER(g_pCConsole__RemoveRule, CONSOLE_REMOVE_RULE_0341, RemoveServerRule_t);
	POINTER_TO_MEMBER(g_pCConsole__MFlag, CAddress::FUNC_CConsole_ModifyVariableFlags, ModifyFlag_t);

	POINTER_TO_MEMBER(g_pCFilterscript__LoadFS_t, CAddress::FUNC_CFilterscripts_LoadFilterscript, LoadFS_t);
	POINTER_TO_MEMBER(g_pCFilterscript__UnLoadFS_t, CAddress::FUNC_CFilterscripts_UnLoadFilterscript, UnLoadFS_t);
	 
	// Unlock restart wait time
	Unlock((void*)CAddress::VAR_pRestartWaitTime, 4);

	// Redirect ContainsInvalidChars function to our own function
	AssemblyRedirect((void *)CAddress::FUNC_ContainsInvalidChars, (void *)YSF_ContainsInvalidChars, gContainsInvalidCharsAssembly);
}

void InstallPreHooks()
{
	GetAddresses();
}
