#include "../main.h"
#include "../Natives.h"

// native execute(const command[], saveoutput=0, index=0);
AMX_DECLARE_NATIVE(Natives::execute)
{
	CHECK_PARAMS(3, NO_FLAGS);

	std::string command;
	int saveoutput, index;
	CScriptParams::Get()->Read(&command, &saveoutput, &index);

	auto thFunc = [](std::string command, int saveoutput, int index)
	{
		FILE *pPipe;
		char szBuffer[512];

		CServer::SysExec_t exec;
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

		std::lock_guard<std::mutex> lock(CServer::Get()->m_SysExecMutex);
		CServer::Get()->m_SysExecQueue.push(exec);
	};

	std::thread(thFunc, command, saveoutput, index).detach();
	return 1;
}

#ifdef _WIN32
// native ffind(const pattern[], filename[], len, &idx);
AMX_DECLARE_NATIVE(Natives::ffind)
{
	// Find a file, idx determines which one of a number of matches to use
	CHECK_PARAMS(4, NO_FLAGS);
	cell
		*cptr;
	char
		*szSearch;
	// Get the search pattern
	amx_StrParam(amx, params[1], szSearch);
	if (szSearch)
	{
		// Get associated search information
		amx_GetAddr(amx, params[4], &cptr);
		cell
			count = *cptr;
		WIN32_FIND_DATA
			ffd;
		TCHAR
			szDir[MAX_PATH] = TEXT("./scriptfiles/");
		StringCchCat(szDir, MAX_PATH, szSearch);
		// Get a serch handle
		HANDLE
			hFind = FindFirstFile(szDir, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				// Check that this isn't a directory
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					// It's not - update idx
					if (!count)
					{
						// No files left to skip, return the data
						(*cptr)++;
						amx_GetAddr(amx, params[2], &cptr);
						amx_SetString(cptr, ffd.cFileName, 0, 0, params[3]);
						FindClose(hFind);
						return 1;
					}
					count--;
				}
			} while (FindNextFile(hFind, &ffd) != 0);
			FindClose(hFind);
		}
	}
	return 0;
}

// native dfind(const pattern[], filename[], len, &idx);
AMX_DECLARE_NATIVE(Natives::dfind)
{
	// Find a directory, idx determines which one of a number of matches to use
	// Identical to ffind in all but 1 line
	CHECK_PARAMS(4, NO_FLAGS);
	cell
		*cptr;
	char
		*szSearch;
	// Get the search pattern
	amx_StrParam(amx, params[1], szSearch);
	if (szSearch)
	{
		// Get associated search information
		amx_GetAddr(amx, params[4], &cptr);
		cell
			count = *cptr;
		WIN32_FIND_DATA
			ffd;
		TCHAR
			szDir[MAX_PATH] = TEXT("./scriptfiles/");
		StringCchCat(szDir, MAX_PATH, szSearch);
		// Get a serch handle
		HANDLE
			hFind = FindFirstFile(szDir, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				// Check that this is a directory
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// It is - update idx
					if (!count)
					{
						// No files left to skip, return the data
						(*cptr)++;
						amx_GetAddr(amx, params[2], &cptr);
						amx_SetString(cptr, ffd.cFileName, 0, 0, params[3]);
						FindClose(hFind);
						return 1;
					}
					count--;
				}
			} while (FindNextFile(hFind, &ffd) != 0);
			FindClose(hFind);
		}
	}
	return 0;
}
#else
// native ffind(const pattern[], filename[], len, &idx);
AMX_DECLARE_NATIVE(Natives::ffind)
{
	// Find a file, idx determines which one of a number of matches to use
	CHECK_PARAMS(4, NO_FLAGS);
	cell
		*cptr;
	char
		*szSearch;
	// Get the search pattern
	amx_StrParam(amx, params[1], szSearch);
	if (szSearch)
	{
		// Get associated search information
		amx_GetAddr(amx, params[4], &cptr);
		cell
			count = *cptr;
		// Find the end of the directory name
		int
			end = strlen(szSearch) - 1;
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
		char
			*szDir = (char *)alloca(end + 16),
			*szFile;
		strcpy(szDir, "./scriptfiles/");
		if (end)
		{
			szFile = &szSearch[end + 1];
			szSearch[end] = '\0';
			strcpy(szDir + 14, szSearch);
			strcpy(szDir + strlen(szDir), "/");
		}
		else
		{
			szFile = szSearch;
		}
		end = strlen(szDir);
		DIR
			*dp = opendir(szDir);
		if (dp)
		{
			// Loop through all files in the directory
			struct dirent
				*ep;
			while (ep = readdir(dp))
			{
				// Check if this file matches the pattern
				if (!fnmatch(szFile, ep->d_name, FNM_NOESCAPE))
				{
					// Check if this is a directory
					// There MUST be an easier way to do this!
					char
						*full = (char *)malloc(strlen(ep->d_name) + end + 1);
					if (!full)
					{
						closedir(dp);
						return 0;
					}
					strcpy(full, szDir);
					strcpy(full + end, ep->d_name);
					DIR
						*xp = opendir(full);
					free(full);
					if (xp)
					{
						closedir(xp);
						continue;
					}
					// Check if there's any left to skip
					if (!count)
					{
						// No files left to skip, return the data
						(*cptr)++;
						amx_GetAddr(amx, params[2], &cptr);
						amx_SetString(cptr, ep->d_name, 0, 0, params[3]);
						closedir(dp);
						return 1;
					}
					count--;
				}
			}
			closedir(dp);
			return 0;
		}
	}
	return 0;
}

// native dfind(const pattern[], filename[], len, &idx);
AMX_DECLARE_NATIVE(Natives::dfind)
{
	// Find a file, idx determines which one of a number of matches to use
	CHECK_PARAMS(4, NO_FLAGS);
	cell
		*cptr;
	char
		*szSearch;
	// Get the search pattern
	amx_StrParam(amx, params[1], szSearch);
	if (szSearch)
	{
		// Get associated search information
		amx_GetAddr(amx, params[4], &cptr);
		cell
			count = *cptr;
		// Find the end of the directory name
		int
			end = strlen(szSearch) - 1;
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
		char
			*szDir = (char *)alloca(end + 16),
			*szFile;
		strcpy(szDir, "./scriptfiles/");
		if (end)
		{
			szFile = &szSearch[end + 1];
			szSearch[end] = '\0';
			strcpy(szDir + 14, szSearch);
			strcpy(szDir + strlen(szDir), "/");
		}
		else
		{
			szFile = szSearch;
		}
		end = strlen(szDir);
		DIR
			*dp = opendir(szDir);
		if (dp)
		{
			// Loop through all files in the directory
			struct dirent
				*ep;
			while (ep = readdir(dp))
			{
				// Check if this file matches the pattern
				if (!fnmatch(szFile, ep->d_name, FNM_NOESCAPE))
				{
					// Check if this is a directory
					// There MUST be an easier way to do this!
					char
						*full = (char *)malloc(strlen(ep->d_name) + end + 1);
					if (!full)
					{
						closedir(dp);
						return 0;
					}
					strcpy(full, szDir);
					strcpy(full + end, ep->d_name);
					DIR
						*xp = opendir(full);
					free(full);
					if (!xp)
					{
						continue;
					}
					closedir(xp);
					// Check if there's any left to skip
					if (!count)
					{
						// No files left to skip, return the data
						(*cptr)++;
						amx_GetAddr(amx, params[2], &cptr);
						amx_SetString(cptr, ep->d_name, 0, 0, params[3]);
						closedir(dp);
						return 1;
					}
					count--;
				}
			}
			closedir(dp);
			return 0;
		}
	}
	return 0;
}
#endif

// native dcreate(const name[]);
AMX_DECLARE_NATIVE(Natives::dcreate)
{
	// Creates a directory
	CHECK_PARAMS(1, NO_FLAGS);
	char
		*szSearch;
	// Get the search pattern
	amx_StrParam(amx, params[1], szSearch);
	if (szSearch)
	{
#ifdef _WIN32
		TCHAR
			szDir[MAX_PATH] = TEXT("./scriptfiles/");
		StringCchCat(szDir, MAX_PATH, szSearch);
		return (cell)CreateDirectory(szDir, NULL);
#else
		char
			*szDir = (char *)alloca(strlen(szSearch) + 15);
		strcpy(szDir, "./scriptfiles/");
		strcpy(szDir + 14, szSearch);
		return (cell)mkdir(szDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	}
	return 0;
}

// native frename(const oldname[], const newname[]);
AMX_DECLARE_NATIVE(Natives::frename)
{
	// Creates a directory
	CHECK_PARAMS(2, NO_FLAGS);
	char
		*szOld,
		*szNew;
	// Get the search pattern
	amx_StrParam(amx, params[1], szOld);
	amx_StrParam(amx, params[2], szNew);
	if (szOld && szNew)
	{
		char
			*szO = (char *)alloca(strlen(szOld) + 16),
			*szN = (char *)alloca(strlen(szNew) + 16);
		strcpy(szO, "./scriptfiles/");
		strcpy(szO + 14, szOld);
		strcpy(szN, "./scriptfiles/");
		strcpy(szN + 14, szNew);
		return (cell)rename(szO, szN);
	}
	return 0;
}

// native drename(const oldname[], const newname[]);
AMX_DECLARE_NATIVE(Natives::drename)
{
	// Creates a directory
	CHECK_PARAMS(2, NO_FLAGS);
	char
		*szOld,
		*szNew;
	// Get the search pattern
	amx_StrParam(amx, params[1], szOld);
	amx_StrParam(amx, params[2], szNew);
	if (szOld && szNew)
	{
		char
			*szO = (char *)alloca(strlen(szOld) + 16),
			*szN = (char *)alloca(strlen(szNew) + 16);
		strcpy(szO, "./scriptfiles/");
		strcpy(szO + 14, szOld);
		int
			end;
		end = strlen(szO);
		if (szO[end - 1] != '/')
		{
			szO[end] = '/';
			szO[end + 1] = '\0';
		}
		strcpy(szN, "./scriptfiles/");
		strcpy(szN + 14, szNew);
		end = strlen(szN);
		if (szN[end - 1] != '/')
		{
			szN[end] = '/';
			szN[end + 1] = '\0';
		}
		return (cell)rename(szO, szN);
	}
	return 0;
}
