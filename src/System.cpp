#include <string>
#include <sstream>

#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <winternl.h>

#include <comdef.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#else
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#endif

#include "System.h"

char* GetNPCCommandLine(int processId)
{
	char *line = NULL;

#ifdef _WIN32
	HRESULT hr = 0;
	IWbemLocator         *WbemLocator = NULL;
	IWbemServices        *WbemServices = NULL;
	IEnumWbemClassObject *EnumWbem = NULL;

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&WbemLocator);
	hr = WbemLocator->ConnectServer(SysAllocString(L"ROOT\\CIMV2"), NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);

	std::ostringstream query;
	query << "SELECT CommandLine FROM Win32_Process WHERE ProcessId=" << processId;
	hr = WbemServices->ExecQuery(SysAllocString(L"WQL"), bstr_t(query.str().c_str()), WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);

	if (EnumWbem == NULL) return 0;

	IWbemClassObject *result = NULL;
	ULONG returnedCount = 0;

	while ((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK)
	{
		VARIANT CommandLine;
		hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);

		line = _com_util::ConvertBSTRToString(CommandLine.bstrVal);

		VariantClear(&CommandLine);
		result->Release();

		break;
	}

	EnumWbem->Release();
	WbemServices->Release();
	WbemLocator->Release();

	CoUninitialize();
#else
	char fname[32];
	sprintf(fname, "/proc/%d/cmdline", pid);

	FILE *fcmd = fopen(fname, "r");
	if (fcmd != NULL)
	{
		size_t size = 128;
		line = (char*)malloc(size);
		size_t total = 0, read;
		while ((read = fread(line + total, 1, size - total, fcmd)) >= size - total)
		{
			total += read;
			size *= 2;
			line = (char*)realloc(line, size);
		}
		fclose(fcmd);
		total += read;
		for (unsigned int i = 0; i < total - 1; i++)
		{
			if (!line[i]) line[i] = ' ';
		}
	}
#endif

	return line;
}

int FindNPCProcessID(const char *name)
{
#ifdef _WIN32
	DWORD cpid = GetCurrentProcessId();

	HRESULT hr = 0;
	IWbemLocator         *WbemLocator = NULL;
	IWbemServices        *WbemServices = NULL;
	IEnumWbemClassObject *EnumWbem = NULL;

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&WbemLocator);
	hr = WbemLocator->ConnectServer(SysAllocString(L"ROOT\\CIMV2"), NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);

	std::ostringstream query;
	query << "SELECT ProcessId, CommandLine FROM Win32_Process WHERE ParentProcessId=" << cpid;
	hr = WbemServices->ExecQuery(SysAllocString(L"WQL"), bstr_t(query.str().c_str()), WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);


	if (EnumWbem == NULL) return 0;

	IWbemClassObject *result = NULL;
	ULONG returnedCount = 0;

	DWORD pid = 0;
	while ((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK)
	{
		VARIANT ProcessId, CommandLine;
		hr = result->Get(L"ProcessId", 0, &ProcessId, 0, 0);
		hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);

		char *line = _com_util::ConvertBSTRToString(CommandLine.bstrVal);

		char *iter = line;

		bool found = false;
		while (*iter)
		{
			if (*iter == ' ')
			{
				if (*++iter == '-' && *++iter == 'n' && *++iter == ' ')
				{
					if (!strncmp(++iter, name, strlen(name)))
					{
						pid = ProcessId.uintVal;
						found = true;
						break;
					}
				}
			}
			else {
				iter++;
			}
		}

		free(line);
		VariantClear(&ProcessId);
		VariantClear(&CommandLine);
		result->Release();

		if (found) break;
	}

	EnumWbem->Release();
	WbemServices->Release();
	WbemLocator->Release();

	CoUninitialize();

	return pid;
#else
	pid_t pid, cpid = getpid();

	DIR *dp = opendir("/proc/");
	if (dp != NULL)
	{
		struct dirent *ep;

		char line[1024], fname[32];
		while (ep = readdir(dp))
		{
			if (sscanf(ep->d_name, "%d", &pid) == 1)
			{
				sprintf(fname, "/proc/%d/stat", pid);

				FILE *fstat = fopen(fname, "r");
				if (fstat != NULL)
				{
					fread(line, 1, sizeof(line), fstat);
					fclose(fstat);

					int gpid;
					if (sscanf(line, "%*d (samp-npc) %*c %*d %d", &gpid) == 1)
					{
						if (gpid == cpid)
						{
							sprintf(fname, "/proc/%d/cmdline", pid);

							FILE *fcmd = fopen(fname, "r");
							if (fcmd != NULL)
							{
								size_t total = fread(line, 1, sizeof(line), fcmd);
								fclose(fcmd);

								char *iter = line;
								char *end = line + total;

								bool found = false;
								while (iter < end)
								{
									if (*iter == '\0')
									{
										if (*++iter == '-' && *++iter == 'n' && *++iter == '\0')
										{
											if (!strncmp(++iter, name, strlen(name)))
											{
												found = true;
												break;
											}
										}
									}
									else {
										iter++;
									}
								}
								if (found) break;
							}
						}
					}
				}
			}
			pid = 0;
		}
		closedir(dp);
	}
	return pid;
#endif
}