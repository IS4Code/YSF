#include <string>
#include <sstream>

#ifdef _WIN32
#include "CPlugin.h"

#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <strsafe.h>
#include <tlhelp32.h>
#include <winternl.h>

#include <comdef.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

#define popen _popen
#define pclose  _pclose
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fnmatch.h>
#endif

#include "System.h"

bool CreateNewDirectory(const char *name)
{
#ifdef _WIN32
	TCHAR szDir[MAX_PATH] = TEXT("./scriptfiles/");
	StringCchCat(szDir, MAX_PATH, name);
	return CreateDirectory(szDir, NULL) != 0;
#else
	char *szDir = (char *)alloca(strlen(name) + 15);
	strcpy(szDir, "./scriptfiles/");
	strcpy(szDir + 14, name);
	return mkdir(szDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
}

#ifdef _WIN32
bool FindFileOrDir(const char *szSearch, std::string &result, int count, DWORD flags)
{
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH] = TEXT("./scriptfiles/");
	StringCchCat(szDir, MAX_PATH, szSearch);
	// Get a serch handle
	HANDLE hFind = FindFirstFile(szDir, &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (ffd.dwFileAttributes & flags)
			{
				if (!count)
				{
					// No files left to skip, return the data
					result = ffd.cFileName;
					FindClose(hFind);
					return true;
				}
				count--;
			}
		} while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);
	}
	return true;
}
#else
bool FindFileOrDir(const char *szSearch, std::string &result, int count, bool dir)
{
	int end = strlen(szSearch) - 1;
	if (end == -1)
	{
		return 0;
	}
	while (szSearch[end] != '\\' && szSearch[end] != '/')
	{
		if (!end)
		{
			break;
		}
		end--;
	}
	// Split up the information
	// Ensure that we search in scriptfiles
	// And separate out the filename and path
	char *szDir = (char *)alloca(end + 16);
	const char *szFile;
	strcpy(szDir, "./scriptfiles/");
	if (end)
	{
		szFile = &szSearch[end + 1];
		strncpy(szDir + 14, szSearch, end);
		strcpy(szDir + strlen(szDir), "/");
	}
	else
	{
		szFile = szSearch;
	}
	end = strlen(szDir);
	DIR *dp = opendir(szDir);
	if (dp)
	{
		// Loop through all files in the directory
		struct dirent *ep;
		while (ep = readdir(dp))
		{
			// Check if this file matches the pattern
			if (!fnmatch(szFile, ep->d_name, FNM_NOESCAPE))
			{
				// Check if this is a directory
				// There MUST be an easier way to do this!
				char *full = (char *)malloc(strlen(ep->d_name) + end + 1);
				if (!full)
				{
					closedir(dp);
					return false;
				}
				strcpy(full, szDir);
				strcpy(full + end, ep->d_name);
				DIR *xp = opendir(full);
				free(full);
				if (dir)
				{
					if (!xp)
					{
						continue;
					}
					closedir(xp);
				} else {
					if (xp)
					{
						closedir(xp);
						continue;
					}
				}
				// Check if there's any left to skip
				if (!count)
				{
					// No files left to skip, return the data
					result = ep->d_name;
					closedir(dp);
					return true;
				}
				count--;
			}
		}
		closedir(dp);
	}
	return false;
}
#endif

bool FindFile(const char *szSearch, std::string &result, int count)
{
#ifdef _WIN32
	return FindFileOrDir(szSearch, result, count, ~FILE_ATTRIBUTE_DIRECTORY);
#else
	return FindFileOrDir(szSearch, result, count, false);
#endif
}

bool FindDirectory(const char *szSearch, std::string &result, int count)
{
#ifdef _WIN32
	return FindFileOrDir(szSearch, result, count, FILE_ATTRIBUTE_DIRECTORY);
#else
	return FindFileOrDir(szSearch, result, count, true);
#endif
}

void ExecuteCommand(const std::string &cmd, bool saveOutput, int index)
{
#ifdef _WIN32
	auto thFunc = [](std::string command, int saveoutput, int index)
	{
		FILE *pPipe;
		char szBuffer[512];

		CPlugin::SysExec_t exec;
		exec.index = index;
		exec.output = "";
		exec.success = false;

		if ((pPipe = popen(command.c_str(), "r")) != NULL)
		{
			while (saveoutput && fgets(szBuffer, sizeof(szBuffer), pPipe))
				exec.output.append(szBuffer);

			exec.retval = pclose(pPipe);
			exec.success = true;
		}

		std::lock_guard<std::mutex> lock(CPlugin::Get()->m_SysExecMutex);
		CPlugin::Get()->m_SysExecQueue.push(exec);
	};

	std::thread(thFunc, cmd, static_cast<int>(saveOutput), index).detach();
#else
	system(cmd.c_str());
#endif
}

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
	sprintf(fname, "/proc/%d/cmdline", processId);

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