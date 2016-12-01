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
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

#include "main.h"

//----------------------------------------------------
#ifdef _WIN32
	// native ffind(const pattern[], filename[], len, &idx);
	AMX_DECLARE_NATIVE(Natives::ffind)
	{
		// Find a file, idx determines which one of a number of matches to use
		CHECK_PARAMS(4, "ffind");
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
				}
				while (FindNextFile(hFind, &ffd) != 0);
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
		CHECK_PARAMS(4, "dfind");
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
				}
				while (FindNextFile(hFind, &ffd) != 0);
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
		CHECK_PARAMS(4, "dfind");
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
		CHECK_PARAMS(4, "ffind");
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
	CHECK_PARAMS(1, "dcreate");
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
	CHECK_PARAMS(2, "frename");
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
	CHECK_PARAMS(2, "drename");
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

// native SetModeRestartTime(Float:time);
AMX_DECLARE_NATIVE(Natives::SetModeRestartTime)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	if (!CAddress::VAR_pRestartWaitTime) return 0;
	CHECK_PARAMS(1, "SetModeRestartTime");

	*(float*)CAddress::VAR_pRestartWaitTime = amx_ctof(params[1]);
	return 1;
}

// native Float:GetModeRestartTime();
AMX_DECLARE_NATIVE(Natives::GetModeRestartTime)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	if (!CAddress::VAR_pRestartWaitTime) return 0;

	float fRestartTime = *(float*)CAddress::VAR_pRestartWaitTime;
	return amx_ftoc(fRestartTime);
}

// native SetMaxPlayers(maxplayers);
AMX_DECLARE_NATIVE(Natives::SetMaxPlayers)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "SetMaxPlayers");

	int maxplayers = static_cast<int>(params[1]);
	if(maxplayers < 1 || maxplayers > MAX_PLAYERS) return 0;

	CSAMPFunctions::SetIntVariable("maxplayers", maxplayers);
	return 1;
}

// native SetMaxNPCs(maxnpcs);
AMX_DECLARE_NATIVE(Natives::SetMaxNPCs)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "SetMaxNPCs");

	int maxnpcs = static_cast<int>(params[1]);
	if(maxnpcs < 1 || maxnpcs > MAX_PLAYERS) return 0;

	CSAMPFunctions::SetIntVariable("maxnpc", maxnpcs);
	return 1;
}

// native SetPlayerAdmin(playerid, bool:admin);
AMX_DECLARE_NATIVE(Natives::SetPlayerAdmin)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerAdmin");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	pNetGame->pPlayerPool->bIsAnAdmin[playerid] = !!params[2];
	return 1;
}

// native LoadFilterScript(scriptname[]);
AMX_DECLARE_NATIVE(Natives::LoadFilterScript)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "LoadFilterScript");
	
	char
		*name;
	amx_StrParam(amx, params[1], name);
	if(name)
	{
		return CSAMPFunctions::LoadFilterscript(name);
	}
	return 0;
}

// UnLoadFilterScript(scriptname[]);
AMX_DECLARE_NATIVE(Natives::UnLoadFilterScript)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "UnLoadFilterScript");
	
	char
		*name;
	amx_StrParam(amx, params[1], name);
	if(name)
	{
		return CSAMPFunctions::UnLoadFilterscript(name);
	}
	return 0;
}

// native GetFilterScriptCount();
AMX_DECLARE_NATIVE(Natives::GetFilterScriptCount)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return pNetGame->pFilterScriptPool->iFilterScriptCount;
}

// native GetFilterScriptName(id, name[], len = sizeof(name));
AMX_DECLARE_NATIVE(Natives::GetFilterScriptName)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "GetFilterScriptName");

	int id = static_cast<int>(params[1]);
	if(id >= MAX_FILTER_SCRIPTS) return 0;

	return set_amxstring(amx, params[2], pNetGame->pFilterScriptPool->szFilterScriptName[id], params[3]);
}

// native AddServerRule(name[], value[], flags = CON_VARFLAG_RULE);
AMX_DECLARE_NATIVE(Natives::AddServerRule)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "AddServerRule");
	
	char *name, *value;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], value);
	if (name && value)
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(name);
		if (ConVar == NULL)
		{
			CSAMPFunctions::AddStringVariable(name, static_cast<int>(params[3]), value, NULL);
			return 1;
		}
	}
	return 0;
}

// native SetServerRule(name[], value[]);
AMX_DECLARE_NATIVE(Natives::SetServerRule)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetServerRule");

	char *name, *value;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], value);
	if (name && value)
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(name);
		if (ConVar != NULL)
		{
			CSAMPFunctions::SetStringVariable(name, value);
			return 1;
		}
	}
	return 0;
}

// native SetServerRuleInt(name[], value);
AMX_DECLARE_NATIVE(Natives::SetServerRuleInt)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetServerRuleInt");

	char *name;
	amx_StrParam(amx, params[1], name);
	if (name)
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(name);
		if (ConVar != NULL)
		{
			CSAMPFunctions::SetIntVariable(name, static_cast<int>(params[2]));
			return 1;
		}
		return 1;
	}
	return 0;
}

// native IsValidServerRule(name[]);
AMX_DECLARE_NATIVE(Natives::IsValidServerRule)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsValidServerRule");

	char *name;
	amx_StrParam(amx, params[1], name);
	if (name)
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(name);
		return ConVar != NULL;
	}
	return 0;
}

// native RemoveServerRule(name[]);
AMX_DECLARE_NATIVE(Natives::RemoveServerRule)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "RemoveServerRule");

	char *name;
	amx_StrParam(amx, params[1], name);
	if (name)
	{
		//RemoveServerRule(name);
		return 1;
	}
	return 0;
}

// native SetServerRuleFlags(name[], flags);
AMX_DECLARE_NATIVE(Natives::SetServerRuleFlags)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetServerRuleFlags");
	
	char *name;
	amx_StrParam(amx, params[1], name);
	if (name)
	{
		CSAMPFunctions::ModifyVariableFlags(name, (DWORD)params[2]);
		return 1;
	}
	return 0;
}

// native GetServerRuleFlags(name[]);
AMX_DECLARE_NATIVE(Natives::GetServerRuleFlags)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetServerRuleFlags");
	
	char *name;
	amx_StrParam(amx, params[1], name);
	if (name)
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(name);
		if (ConVar != NULL)
		{
			return ConVar->VarFlags;
		}
		return 0;
	}
	return 0;
}

// native GetServerSettings(&showplayermarkes, &shownametags, &stuntbonus, &useplayerpedanims, &bLimitchatradius, &disableinteriorenterexits, &nametaglos, &manualvehicleengine, 
//		&limitplayermarkers, &vehiclefriendlyfire, &defaultcameracollision, &Float:fGlobalchatradius, &Float:fNameTagDrawDistance, &Float:fPlayermarkerslimit);
AMX_DECLARE_NATIVE(Natives::GetServerSettings)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(14, "GetServerSettings");

	Utility::storeIntegerInNative(amx, params[1], pNetGame->bShowPlayerMarkers); 
	Utility::storeIntegerInNative(amx, params[2], pNetGame->byteShowNameTags);     
	Utility::storeIntegerInNative(amx, params[3], pNetGame->byteStuntBonus);   
	Utility::storeIntegerInNative(amx, params[4], pNetGame->bUseCJWalk);     
	Utility::storeIntegerInNative(amx, params[5], pNetGame->bLimitGlobalChatRadius);     
	Utility::storeIntegerInNative(amx, params[6], pNetGame->byteDisableEnterExits);  
	Utility::storeIntegerInNative(amx, params[7], pNetGame->byteNameTagLOS);   
	Utility::storeIntegerInNative(amx, params[8], pNetGame->bManulVehicleEngineAndLights);
	Utility::storeIntegerInNative(amx, params[9], pNetGame->bLimitPlayerMarkers);
	Utility::storeIntegerInNative(amx, params[10], pNetGame->bVehicleFriendlyFire);
	Utility::storeIntegerInNative(amx, params[11], pNetGame->byteDefaultCameraCollision);
	Utility::storeFloatInNative(amx, params[12], pNetGame->fGlobalChatRadius);
	Utility::storeFloatInNative(amx, params[13], pNetGame->fNameTagDrawDistance);
	Utility::storeFloatInNative(amx, params[14], pNetGame->fPlayerMarkesLimit);
	return 1;
}

// native IsValidNickName(name[]);
AMX_DECLARE_NATIVE(Natives::IsValidNickName)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsValidNickName");

	char *name;
	amx_StrParam(amx, params[1], name);
	if (name)
	{
		return CServer::Get()->IsValidNick(name);
	}
	return 0;
}

// native AllowNickNameCharacter(character, bool:allow);
AMX_DECLARE_NATIVE(Natives::AllowNickNameCharacter)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "AllowNickNameCharacter");
	
	char character = (char)params[1];

	// Enable %s is disallowed
	if(character == '%') return 0;

	CServer::Get()->AllowNickNameCharacter(character, !!params[2]);
	return 1;
}

// native IsNickNameCharacterAllowed(character);
AMX_DECLARE_NATIVE(Natives::IsNickNameCharacterAllowed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsNickNameCharacterAllowed");

	return CServer::Get()->IsNickNameCharacterAllowed((char)params[1]);
}

/////////////// Timers

// native GetAvailableClasses();
AMX_DECLARE_NATIVE(Natives::GetAvailableClasses)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return pNetGame->iSpawnsAvailable;
}

// native RemoveLastClass();
AMX_DECLARE_NATIVE(Natives::RemoveLastClass)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	if(pNetGame->iSpawnsAvailable <= 0)
		return 0;

	pNetGame->iSpawnsAvailable--;
	return 1;
}

// native GetPlayerClass(classid, &teamid, &modelid, &Float:spawn_x, &Float:spawn_y, &Float:spawn_z, &Float:z_angle, &weapon1, &weapon1_ammo, &weapon2, &weapon2_ammo,& weapon3, &weapon3_ammo);
AMX_DECLARE_NATIVE(Natives::GetPlayerClass)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(13, "GetPlayerClass");

	int classid = static_cast<int>(params[1]);
	if(classid < 0 || classid > pNetGame->iSpawnsAvailable) return 0;

	CPlayerSpawnInfo *pSpawn = &pNetGame->AvailableSpawns[classid];
	Utility::storeIntegerInNative(amx, params[2], pSpawn->byteTeam);
	Utility::storeIntegerInNative(amx, params[3], pSpawn->iSkin);
	Utility::storeVectorInNative(amx, params[4], pSpawn->vecPos);
	Utility::storeFloatInNative(amx, params[7], pSpawn->fRotation);
	Utility::storeIntegerInNative(amx, params[8], pSpawn->iSpawnWeapons[0]);
	Utility::storeIntegerInNative(amx, params[9], pSpawn->iSpawnWeaponsAmmo[0]);
	Utility::storeIntegerInNative(amx, params[10], pSpawn->iSpawnWeapons[1]);
	Utility::storeIntegerInNative(amx, params[11], pSpawn->iSpawnWeaponsAmmo[1]);
	Utility::storeIntegerInNative(amx, params[12], pSpawn->iSpawnWeapons[2]);
	Utility::storeIntegerInNative(amx, params[13], pSpawn->iSpawnWeaponsAmmo[2]);
	return 1;
}

// native EditPlayerClass(classid, teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
AMX_DECLARE_NATIVE(Natives::EditPlayerClass)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(13, "EditPlayerClass");

	int classid = static_cast<int>(params[1]);
	if(classid < 0 || classid > pNetGame->iSpawnsAvailable) return 0;

	CPlayerSpawnInfo *pSpawn = &pNetGame->AvailableSpawns[classid];

	pSpawn->byteTeam = static_cast<BYTE>(params[2]);
	pSpawn->iSkin = static_cast<int>(params[3]);
	pSpawn->vecPos = CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));
	pSpawn->fRotation = amx_ctof(params[7]);
	pSpawn->iSpawnWeapons[0] = static_cast<int>(params[8]);
	pSpawn->iSpawnWeaponsAmmo[0] = static_cast<int>(params[9]);
	pSpawn->iSpawnWeapons[1] = static_cast<int>(params[10]);
	pSpawn->iSpawnWeaponsAmmo[1] = static_cast<int>(params[11]);
	pSpawn->iSpawnWeapons[2] = static_cast<int>(params[12]);
	pSpawn->iSpawnWeaponsAmmo[2] = static_cast<int>(params[13]);
	return 1;
}

// native GetActiveTimers();
AMX_DECLARE_NATIVE(Natives::GetRunningTimers)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return pNetGame->pScriptTimers->dwTimerCount;
}

// native Float:GetGravity();
AMX_DECLARE_NATIVE(Natives::YSF_GetGravity)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	float fGravity = pNetGame->fGravity;
	return amx_ftoc(fGravity);
}

// native SetPlayerGravity(playerid, Float:gravity);
AMX_DECLARE_NATIVE(Natives::SetPlayerGravity)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerGravity");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	// Update stored values
	pPlayerData[playerid]->fGravity = amx_ctof(params[2]);

	RakNet::BitStream bs;
	bs.Write(pPlayerData[playerid]->fGravity);
	CSAMPFunctions::RPC(&RPC_Gravity, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
	return 1;
}

// native Float:GetPlayerGravity(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerGravity)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerGravity");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return amx_ftoc(pPlayerData[playerid]->fGravity);
}

// native SetPlayerTeamForPlayer(playerid, teamplayerid, teamid);
AMX_DECLARE_NATIVE(Natives::SetPlayerTeamForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "SetPlayerTeamForPlayer");

	int playerid = static_cast<int>(params[1]);
	int teamplayerid = static_cast<int>(params[2]);
	int team = static_cast<int>(params[3]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;
	if (team < 0 || team > 255) return 0;

	pPlayerData[playerid]->SetPlayerTeamForPlayer(static_cast<WORD>(teamplayerid), team);
	return 1;
}

// native GetPlayerTeamForPlayer(playerid, teamplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerTeamForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerTeamForPlayer");

	int playerid = static_cast<int>(params[1]);
	int teamplayerid = static_cast<int>(params[2]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;

	return pPlayerData[playerid]->GetPlayerTeamForPlayer(static_cast<WORD>(teamplayerid));
}

AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerTeam)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerTeam");

	int playerid = static_cast<int>(params[1]);
	if(pSetPlayerTeam(amx, params))
	{
		for(WORD i = 0; i != MAX_PLAYERS; i++)
		{
			if(IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerTeam(static_cast<WORD>(playerid));
		}
		return 1;
	}
	return 0;
}

// native SetPlayerSkinForPlayer(playerid, skinplayerid, skin);
AMX_DECLARE_NATIVE(Natives::SetPlayerSkinForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "SetPlayerSkinForPlayer");

	int playerid = static_cast<int>(params[1]);
	int skinplayerid = static_cast<int>(params[2]);
	int skin = static_cast<int>(params[3]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;
	if (skin < 0 || skin > 300) return 0;

	pPlayerData[playerid]->SetPlayerSkinForPlayer(static_cast<WORD>(skinplayerid), skin);
	return 1;
}

// native GetPlayerSkinForPlayer(playerid, skinplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSkinForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerSkinForPlayer");

	int playerid = static_cast<int>(params[1]);
	int skinplayerid = static_cast<int>(params[2]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;

	return pPlayerData[playerid]->GetPlayerSkinForPlayer(static_cast<WORD>(skinplayerid));
}

AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerSkin)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerSkin");

	int playerid = static_cast<int>(params[1]);
	if(pSetPlayerSkin(amx, params))
	{
		for(WORD i = 0; i != MAX_PLAYERS; i++)
		{
			if(IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerSkin(static_cast<WORD>(playerid));
		}
		return 1;
	}
	return 0;
}

// native SetPlayerNameForPlayer(playerid, nameplayerid, playername[]);
AMX_DECLARE_NATIVE(Natives::SetPlayerNameForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "SetPlayerNameForPlayer");

	int playerid = static_cast<int>(params[1]);
	int nameplayerid = static_cast<int>(params[2]);
	char *name = NULL;

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;

	amx_StrParam(amx, params[3], name);
	pPlayerData[playerid]->SetPlayerNameForPlayer(static_cast<WORD>(nameplayerid), name);
	return 1;
}

// native GetPlayerNameForPlayer(playerid, nameplayerid, playername[], size = sizeof(playername));
AMX_DECLARE_NATIVE(Natives::GetPlayerNameForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetPlayerNameForPlayer");

	int playerid = static_cast<int>(params[1]);
	int nameplayerid = static_cast<int>(params[2]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;

	return set_amxstring(amx, params[3], pPlayerData[playerid]->GetPlayerNameForPlayer(static_cast<WORD>(nameplayerid)), params[4]);
}

AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerName)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerName");

	int playerid = static_cast<int>(params[1]);
	int ret = pSetPlayerName(amx, params);

	if(ret == 1)
	{
		for(WORD i = 0; i != MAX_PLAYERS; i++)
		{
			if(IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerName(static_cast<WORD>(playerid));
		}
	}
	return ret;
}

// native SetPlayerFightStyleForPlayer(playerid, styleplayerid, style);
AMX_DECLARE_NATIVE(Natives::SetPlayerFightStyleForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "SetPlayerFightStyleForPlayer");

	int playerid = static_cast<int>(params[1]);
	int styleplayerid = static_cast<int>(params[2]);
	int style = static_cast<int>(params[3]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;

	pPlayerData[playerid]->SetPlayerFightingStyleForPlayer(static_cast<WORD>(styleplayerid), style);
	return 1;
}

// native GetPlayerFightStyleForPlayer(playerid, skinplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerFightStyleForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerFightStyleForPlayer");

	int playerid = static_cast<int>(params[1]);
	int styleplayerid = static_cast<int>(params[2]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;

	return pPlayerData[playerid]->GetPlayerFightingStyleForPlayer(static_cast<WORD>(styleplayerid));
}

AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerFightingStyle)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerFightingStyle");

	int playerid = static_cast<int>(params[1]);
	if(pSetPlayerFightingStyle(amx, params))
	{
		for(WORD i = 0; i != MAX_PLAYERS; i++)
		{
			if(IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerFightingStyle(static_cast<WORD>(playerid));
		}
		return 1;
	}
	return 0;
}

// native SetPlayerPosForPlayer(playerid, posplayerid, Float:fX, Float:fY, Float:fZ, bool:forcesync = true);
AMX_DECLARE_NATIVE(Natives::SetPlayerPosForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(6, "SetPlayerPosForPlayer");

	int playerid = static_cast<int>(params[1]);
	int posplayerid = static_cast<int>(params[2]);
	bool forcesync = static_cast<int>(params[6]) != 0;
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(posplayerid)) return 0;

	SAFE_DELETE(pPlayerData[playerid]->vecCustomPos[posplayerid]);

	if(!forcesync)
	{
		pPlayerData[playerid]->bCustomPos[posplayerid] = false;
		return 1;
	}

	CVector vecPos = CVector(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
		
	pPlayerData[playerid]->vecCustomPos[posplayerid] = new CVector;
	pPlayerData[playerid]->bCustomPos[posplayerid] = true;

	memcpy(pPlayerData[playerid]->vecCustomPos[posplayerid], &vecPos, sizeof(CVector));
	/*
	RakNet::BitStream bs;
	bs.Write(ID_PLAYER_SYNC);
	bs.Write((WORD)posplayerid);
	RebuildSyncData(&bs, playerid);
	CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false);
	*/
	return 1;
}

// native SetPlayerRotationQuatForPlayer(playerid, quatplayerid, Float:w, Float:x, Float:y, Float:z, bool:forcesync = true);
AMX_DECLARE_NATIVE(Natives::SetPlayerRotationQuatForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(7, "SetPlayerRotationQuatForPlayer");

	int playerid = static_cast<int>(params[1]);
	int posplayerid = static_cast<int>(params[2]);
	bool forcesync = static_cast<int>(params[4]) != 0;
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(posplayerid)) return 0;

	if(!forcesync)
	{
		pPlayerData[playerid]->bCustomQuat[posplayerid] = false;
		return 1;
	}
	
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[playerid];

	pPlayerData[playerid]->fCustomQuat[posplayerid][0] = amx_ctof(params[3]);
	pPlayerData[playerid]->fCustomQuat[posplayerid][1] = amx_ctof(params[4]);
	pPlayerData[playerid]->fCustomQuat[posplayerid][2] = amx_ctof(params[5]);
	pPlayerData[playerid]->fCustomQuat[posplayerid][3] = amx_ctof(params[6]);

	pPlayerData[playerid]->bCustomQuat[posplayerid] = true;

	RakNet::BitStream bs;
	bs.Write((BYTE)ID_PLAYER_SYNC);
	bs.Write((WORD)posplayerid);
	bs.Write((bool)0); // bHasLR
	bs.Write((bool)0); // bHasUD
	bs.Write(p->syncData.wKeys); // bHasUD
	
	if(pPlayerData[playerid]->bCustomPos[posplayerid])
		bs.Write(*pPlayerData[playerid]->vecCustomPos[posplayerid]);
	else 
		bs.Write(p->vecPosition);

	bs.Write(pPlayerData[playerid]->fCustomQuat[posplayerid][0]);
	bs.Write(pPlayerData[playerid]->fCustomQuat[posplayerid][1]);
	bs.Write(pPlayerData[playerid]->fCustomQuat[posplayerid][2]);
	bs.Write(pPlayerData[playerid]->fCustomQuat[posplayerid][3]);

	CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false);
	return 1;
}

// native ApplyAnimationForPlayer(playerid, animplayerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time);
AMX_DECLARE_NATIVE(Natives::ApplyAnimationForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(10, "ApplyAnimationForPlayer");
	
	RakNet::BitStream bsSend;
	char *szAnimLib;
	char *szAnimName;
	BYTE byteAnimLibLen;
	BYTE byteAnimNameLen;
	float fS;
	bool opt1,opt2,opt3,opt4;
	int time;
	
	int playerid = static_cast<int>(params[1]);
	int animplayerid = static_cast<int>(params[2]);
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(animplayerid)) return 0;

	amx_StrParam(amx, params[3], szAnimLib);
	amx_StrParam(amx, params[4], szAnimName);

	if(!szAnimLib || !szAnimName) return 0;

	byteAnimLibLen = static_cast<BYTE>(strlen(szAnimLib));
	byteAnimNameLen = static_cast<BYTE>(strlen(szAnimName));

	fS = amx_ctof(params[5]);
	opt1 = !!params[6];
	opt2 = !!params[7];
	opt3 = !!params[8];
	opt4 = !!params[9];
	time = static_cast<int>(params[10]);

	bsSend.Write((WORD)animplayerid);
	bsSend.Write(byteAnimLibLen);
	bsSend.Write(szAnimLib,byteAnimLibLen);
	bsSend.Write(byteAnimNameLen);
	bsSend.Write(szAnimName,byteAnimNameLen);
	bsSend.Write(fS);
	bsSend.Write(opt1);
	bsSend.Write(opt2);
	bsSend.Write(opt3);
	bsSend.Write(opt4);
	bsSend.Write(time);

	CSAMPFunctions::RPC(&RPC_ScrApplyAnimation, &bsSend, MEDIUM_PRIORITY, UNRELIABLE, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

// native SetPlayerWeather(playerid, weatherid);
AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerWeather)
{
	CHECK_PARAMS(2, "SetPlayerWeather");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	// Update stored values
	pPlayerData[playerid]->byteWeather = static_cast<BYTE>(params[2]);

	RakNet::BitStream bs;
	bs.Write(pPlayerData[playerid]->byteWeather);
	CSAMPFunctions::RPC(&RPC_Weather, &bs, MEDIUM_PRIORITY, RELIABLE, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
	return 1;
}

// native GetPlayerWeather(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerWeather)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerWeather");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->byteWeather;
}

// native SetPlayerWorldBounds(playerid, Float:x_max, Float:x_min, Float:y_max, Float:y_min)
AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerWorldBounds)
{
	int playerid = static_cast<int>(params[1]);
	if(pSetPlayerWorldBounds(amx, params))
	{
		for (BYTE i = 0; i != 4; i++)
		{
			pPlayerData[playerid]->fBounds[i] = amx_ctof(params[i + 2]);
		}
	}
	return 1;
}

// native DestroyObject(objectid)
AMX_DECLARE_NATIVE(Natives::YSF_DestroyObject)
{
	int objectid = static_cast<int>(params[1]);

	if(objectid < 0 || objectid > MAX_OBJECTS) return 0;
	if(!pNetGame->pObjectPool->pObjects[objectid]) return 0;

	if(pDestroyObject(amx, params))
	{
		CServer::Get()->COBJECT_AttachedObjectPlayer[objectid] = INVALID_PLAYER_ID;
		return 1;
	}
	return 0;
}

// native DestroyPlayerObject(playerid, objectid)
AMX_DECLARE_NATIVE(Natives::YSF_DestroyPlayerObject)
{
	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);

	if(objectid < 0 || objectid > MAX_OBJECTS) return 0;
	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	if(pDestroyPlayerObject(amx, params) && IsPlayerConnected(playerid))
	{
		if(pPlayerData[playerid]->stObj[objectid].wObjectID != 0xFFFF || pPlayerData[playerid]->stObj[objectid].wAttachPlayerID != INVALID_PLAYER_ID)
		{
			pPlayerData[playerid]->stObj[objectid].wObjectID = INVALID_OBJECT_ID;
			pPlayerData[playerid]->stObj[objectid].wAttachPlayerID = INVALID_PLAYER_ID;
			pPlayerData[playerid]->stObj[objectid].vecOffset = CVector(0.0f, 0.0f, 0.0f);
			pPlayerData[playerid]->stObj[objectid].vecRot = CVector(0.0f, 0.0f, 0.0f);		
			pPlayerData[playerid]->dwCreateAttachedObj = 0;
			pPlayerData[playerid]-> bAttachedObjectCreated = false;

			//logprintf("remove attached shit");
		}
		return 1;
	}
	return 0;
}

// native TogglePlayerControllable(playerid, bool:toggle)
AMX_DECLARE_NATIVE(Natives::YSF_TogglePlayerControllable)
{
	int playerid = static_cast<int>(params[1]);
	bool toggle = static_cast<int>(params[2]) != 0;

	if(pTogglePlayerControllable(amx, params) && IsPlayerConnected(playerid))
	{
		pPlayerData[playerid]->bControllable = toggle;
		return 1;
	}
	return 0;
}

// native ChangeVehicleColor(vehicleid, color1, color2)
AMX_DECLARE_NATIVE(Natives::YSF_ChangeVehicleColor)
{
	int vehicleid = static_cast<int>(params[1]);
	if(pChangeVehicleColor(amx, params))
	{
		CServer::Get()->bChangedVehicleColor[vehicleid] = true;
		return 1;
	}
	return 0;
}

AMX_DECLARE_NATIVE(Natives::YSF_DestroyVehicle)
{
	int vehicleid = static_cast<int>(params[1]);
	if(pDestroyVehicle(amx, params))
	{
		CServer::Get()->bChangedVehicleColor[vehicleid] = false;
		return 1;
	}
	return 0;
}

AMX_DECLARE_NATIVE(Natives::YSF_ShowPlayerDialog)
{
	int playerid = static_cast<int>(params[1]);
	int dialogid = static_cast<int>(params[2]);

	if (pShowPlayerDialog(amx, params))
	{
		pPlayerData[playerid]->wDialogID = dialogid;
		return 1;
	}
	return 0;
}

// native TogglePlayerWidescreen(playerid, bool:set);
AMX_DECLARE_NATIVE(Natives::TogglePlayerWidescreen)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "TogglePlayerWidescreen");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	BYTE set = static_cast<BYTE>(params[2]) != 0;
	pPlayerData[playerid]->bWidescreen = !!set;

	RakNet::BitStream bs;
	bs.Write(set);
	CSAMPFunctions::RPC(&RPC_Widescreen, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
	return 1;
}

// native IsPlayerWidescreenToggled(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerWidescreenToggled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerWideScreen");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->bWidescreen;
}

// native GetSpawnInfo(playerid, &teamid, &modelid, &Float:spawn_x, &Float:spawn_y, &Float:spawn_z, &Float:z_angle, &weapon1, &weapon1_ammo, &weapon2, &weapon2_ammo,& weapon3, &weapon3_ammo);
AMX_DECLARE_NATIVE(Natives::GetSpawnInfo)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(13, "GetSpawnInfo");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	CPlayerSpawnInfo *pSpawn = &pNetGame->pPlayerPool->pPlayer[playerid]->spawn;

	Utility::storeIntegerInNative(amx, params[2], pSpawn->byteTeam);
	Utility::storeIntegerInNative(amx, params[3], pSpawn->iSkin);
	Utility::storeVectorInNative(amx, params[4], pSpawn->vecPos);
	Utility::storeFloatInNative(amx, params[7], pSpawn->fRotation);
	Utility::storeIntegerInNative(amx, params[8], pSpawn->iSpawnWeapons[0]);
	Utility::storeIntegerInNative(amx, params[9], pSpawn->iSpawnWeaponsAmmo[0]);
	Utility::storeIntegerInNative(amx, params[10], pSpawn->iSpawnWeapons[1]);
	Utility::storeIntegerInNative(amx, params[11], pSpawn->iSpawnWeaponsAmmo[1]);
	Utility::storeIntegerInNative(amx, params[12], pSpawn->iSpawnWeapons[2]);
	Utility::storeIntegerInNative(amx, params[13], pSpawn->iSpawnWeaponsAmmo[2]);
	return 1;
}

// native GetPlayerSkillLevel(playerid, skill);
AMX_DECLARE_NATIVE(Natives::GetPlayerSkillLevel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerSkillLevel");

	int playerid = static_cast<int>(params[1]);
	int skillid = static_cast<int>(params[2]);
	
	if(!IsPlayerConnected(playerid)) return 0;
	if(skillid < 0 || skillid > 10) return -1;

	return pNetGame->pPlayerPool->pPlayer[playerid]->wSkillLevel[skillid];
}

// native IsPlayerCheckpointActive(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerCheckpointActive)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerCheckpointActive");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bShowCheckpoint;
}

// native GetPlayerCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fSize);
AMX_DECLARE_NATIVE(Natives::GetPlayerCheckpoint)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetPlayerCheckpoint");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[playerid]->vecCPPos;
	Utility::storeVectorInNative(amx, params[2], *vecPos);
	Utility::storeFloatInNative(amx, params[5], pNetGame->pPlayerPool->pPlayer[playerid]->fCPSize);
	return 1;
}

// native IsPlayerRaceCheckpointActive(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerRaceCheckpointActive)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerRaceCheckpointActive");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bShowRaceCheckpoint;
}

// native GetPlayerRaceCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fNextX, &Float:fNextY, &fNextZ, &Float:fSize);
AMX_DECLARE_NATIVE(Natives::GetPlayerRaceCheckpoint)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(8, "GetPlayerRaceCheckpoint");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[playerid]->vecRaceCPPos;
	CVector *vecNextPos = &pNetGame->pPlayerPool->pPlayer[playerid]->vecRaceCPNextPos;

	Utility::storeVectorInNative(amx, params[2], *vecPos);
	Utility::storeVectorInNative(amx, params[5], *vecNextPos);
	Utility::storeFloatInNative(amx, params[8], pNetGame->pPlayerPool->pPlayer[playerid]->fRaceCPSize);
	return 1;
}

// native GetPlayerWorldBounds(playerid, &Float:x_max, &Float:x_min, &Float:y_max, &Float:y_min);
AMX_DECLARE_NATIVE(Natives::GetPlayerWorldBounds)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetPlayerWorldBounds");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	for (BYTE i = 0; i != 4; i++)
	{
		Utility::storeFloatInNative(amx, params[2 + i], pPlayerData[playerid]->fBounds[i]);
	}
	return 1;
}

// native IsPlayerInModShop(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerInModShop)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerInModShop");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bIsInModShop;
}

// native GetPlayerSirenState(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSirenState)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerSirenState");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.byteSirenState;
}

// native GetPlayerLandingGearState(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerLandingGearState)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerLandingGearState");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.byteGearState;
}

// native GetPlayerHydraReactorAngle(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerHydraReactorAngle)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerHydraReactorAngle");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.wHydraReactorAngle[0];
}

// native Float:GetPlayerTrainSpeed(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerTrainSpeed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerTrainSpeed");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return amx_ftoc(pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.fTrainSpeed);
}

// native Float:GetPlayerZAim(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerZAim)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerZAim");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return amx_ftoc(pNetGame->pPlayerPool->pPlayer[playerid]->aimSyncData.fZAim);
}

// native GetPlayerSurfingOffsets(playerid, &Float:fOffsetX, &Float:fOffsetY, &Float:fOffsetZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerSurfingOffsets)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetPlayerSurfingOffsets");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	CVector vecPos = pNetGame->pPlayerPool->pPlayer[playerid]->syncData.vecSurfing;
	Utility::storeVectorInNative(amx, params[2], vecPos);
	return 1;
}

// native GetPlayerRotationQuat(playerid, &Float:w, &Float:x, &Float:y, &Float:z);
AMX_DECLARE_NATIVE(Natives::GetPlayerRotationQuat)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetPlayerRotationQuat");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];

	Utility::storeFloatInNative(amx, params[2], pPlayer->fQuaternion[0]);
	Utility::storeFloatInNative(amx, params[3], pPlayer->fQuaternion[1]);
	Utility::storeFloatInNative(amx, params[4], pPlayer->fQuaternion[2]);
	Utility::storeFloatInNative(amx, params[5], pPlayer->fQuaternion[3]);
	return 1;
}

// native GetPlayerDialogID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerDialogID)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerDialogID");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->wDialogID;
}

// native GetPlayerSpectateID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSpectateID)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerSpectateID");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->wSpectateID;
}

// native GetPlayerSpectateType(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSpectateType)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerSpectateType");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->byteSpectateType;
}

// native GetPlayerLastSyncedVehicleID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerLastSyncedVehicleID)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerLastSyncedVehicleID");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	return (cell)pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.wVehicleId;
}

// native GetPlayerLastSyncedTrailerID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerLastSyncedTrailerID)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerLastSyncedTrailerID");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->trailerSyncData.wTrailerID;  //* (WORD*)(((char*)pNetGame->pPlayerPool->pPlayer[playerid]) + 271);
}

// native GetActorSpawnInfo(actorid, &skinid, &Float:fX, &Float:fY, &Float:fZ, &Float:fAngle);
AMX_DECLARE_NATIVE(Natives::GetActorSpawnInfo)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(6, "GetActorSpawnInfo");

	int actorid = static_cast<int>(params[1]);
	if(actorid < 0 || actorid > MAX_PLAYERS) return 0;

	CActor *pActor = pNetGame->pActorPool->pActor[actorid];
	if(!pActor) return 0;

	Utility::storeIntegerInNative(amx, params[2], pActor->iSkinID);
	Utility::storeVectorInNative(amx, params[3], pActor->vecSpawnPos);
	Utility::storeFloatInNative(amx, params[6], pActor->fSpawnAngle);
	return 1;
}

// native GetActorSkin(actorid);
AMX_DECLARE_NATIVE(Natives::GetActorSkin)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetActorSkin");

	int actorid = static_cast<int>(params[1]);
	if(actorid < 0 || actorid > MAX_PLAYERS) return 0;

	CActor *pActor = pNetGame->pActorPool->pActor[actorid];
	if(!pActor) return 0;

	return pActor->iSkinID;
}


// native GetActorAnimation(actorid, animlib[], animlibsize = sizeof(animlib), animname[], animnamesize = sizeof(animname), &Float:fDelta, &loop, &lockx, &locky, &freeze, &time)
AMX_DECLARE_NATIVE(Natives::GetActorAnimation)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(11, "GetActorAnimation");

	int actorid = static_cast<int>(params[1]);
	if(actorid < 0 || actorid > MAX_PLAYERS) return 0;

	CActor *pActor = pNetGame->pActorPool->pActor[actorid];
	if(!pActor) return 0;

	set_amxstring(amx, params[2], pActor->anim.szAnimLib, params[3]);
	set_amxstring(amx, params[4], pActor->anim.szAnimName, params[5]);
	Utility::storeFloatInNative(amx, params[6], pActor->anim.fDelta);
	Utility::storeIntegerInNative(amx, params[7], pActor->anim.byteLoop);
	Utility::storeIntegerInNative(amx, params[8], pActor->anim.byteLockX);
	Utility::storeIntegerInNative(amx, params[9], pActor->anim.byteLockY);
	Utility::storeIntegerInNative(amx, params[10], pActor->anim.byteFreeze);
	Utility::storeIntegerInNative(amx, params[11], pActor->anim.iTime);
	return 1;
}

// native SendBulletData(sender, hitid, hittype, weaponid, Float:fHitOriginX, Float:fHitOriginY, Float:fHitOriginZ, Float:fHitTargetX, Float:fHitTargetY, Float:fHitTargetZ, Float:fCenterOfHitX, Float:fCenterOfHitY, Float:fCenterOfHitZ, forplayerid = -1);
AMX_DECLARE_NATIVE(Natives::SendBulletData) 
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(14, "SendBulletData");

	int playerid = static_cast<int>(params[1]);
	int forplayerid = static_cast<int>(params[14]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(forplayerid != -1)
	{
		if(!IsPlayerConnected(forplayerid)) return 0;
	}

	CBulletSyncData bulletSync;
	bulletSync.byteHitType = static_cast<BYTE>(params[3]);
	bulletSync.wHitID = static_cast<WORD>(params[2]);
	bulletSync.byteWeaponID = static_cast<BYTE>(params[4]);
	bulletSync.vecHitOrigin = CVector(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
	bulletSync.vecHitTarget = CVector(amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10]));
	bulletSync.vecCenterOfHit = CVector(amx_ctof(params[11]), amx_ctof(params[12]), amx_ctof(params[13]));

	RakNet::BitStream bs;
	bs.Write((BYTE)ID_BULLET_SYNC);
	bs.Write((WORD)playerid);
	bs.Write((char*)&bulletSync, sizeof(CBulletSyncData));

	if(forplayerid == -1)
	{
		CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
	}
	else
	{
		CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), false);
	}
	return 1;
}

// native ShowPlayerForPlayer(forplayerid, playerid);
AMX_DECLARE_NATIVE(Natives::ShowPlayerForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "ShowPlayerForPlayer");

	int forplayerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(forplayerid)) return 0;

	int playerid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(playerid == forplayerid) return 0;

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	CSAMPFunctions::RPC(&RPC_WorldPlayerAdd, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native HidePlayerForPlayer(forplayerid, playerid);
AMX_DECLARE_NATIVE(Natives::HidePlayerForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "HidePlayerForPlayer");

	int forplayerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(forplayerid)) return 0;

	int playerid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(playerid == forplayerid) return 0;

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	CSAMPFunctions::RPC(&RPC_WorldPlayerRemove, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native AddPlayerForPlayer(forplayerid, playerid, isnpc = 0);
AMX_DECLARE_NATIVE(Natives::AddPlayerForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "AddPlayerForPlayer");

	int forplayerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(forplayerid)) return 0;

	int playerid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(playerid == forplayerid) return 0;

	bool npc = static_cast<int>(params[3]) != 0;
	char* szName = GetPlayerName(playerid);
	BYTE len = static_cast<BYTE>(strlen(szName));

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	bs.Write((DWORD)0);
	bs.Write((BYTE)npc); //  // isNPC
	bs.Write(len);
	bs.Write(szName, len);
	CSAMPFunctions::RPC(&RPC_ServerJoin, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native RemovePlayerForPlayer(forplayerid, playerid);
AMX_DECLARE_NATIVE(Natives::RemovePlayerForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "RemovePlayerForPlayer");

	int forplayerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(forplayerid)) return 0;

	int playerid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(playerid == forplayerid) return 0;

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	bs.Write((BYTE)0); 
	CSAMPFunctions::RPC(&RPC_ServerQuit, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native SetPlayerChatBubbleForPlayer(forplayerid, playerid, text[], color, Float:drawdistance, expiretime);
AMX_DECLARE_NATIVE(Natives::SetPlayerChatBubbleForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(6, "SetPlayerChatBubbleForPlayer");

	int forplayerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(forplayerid)) return 0;

	int playerid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	int color = static_cast<int>(params[4]);
	float drawdistance = amx_ctof(params[5]);
	int expiretime = static_cast<int>(params[6]);

	char *str;
	amx_StrParam(amx, params[3], str);
	if(str)
	{
		BYTE len = static_cast<BYTE>(strlen(str));
		RakNet::BitStream bs;
		bs.Write((WORD)playerid);
		bs.Write(color);
		bs.Write(drawdistance);
		bs.Write(expiretime);
		bs.Write(len);
		bs.Write(str, len);
		CSAMPFunctions::RPC(&RPC_ChatBubble, &bs, LOW_PRIORITY, RELIABLE, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
		return 1;
	}
	return 0;
}

// native ResetPlayerMarkerForPlayer(playerid, resetplayerid)
AMX_DECLARE_NATIVE(Natives::ResetPlayerMarkerForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "ResetPlayerMarkerForPlayer");

	int playerid = static_cast<int>(params[1]);
	int resetplayerid = static_cast<int>(params[2]);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(resetplayerid)) return 0;
	
	pPlayerData[playerid]->ResetPlayerMarkerForPlayer(static_cast<WORD>(resetplayerid));
	return 1;
}
 
// native SetPlayerVersion(playerid, version[];
AMX_DECLARE_NATIVE(Natives::SetPlayerVersion)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerVersion");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;
	
	char *version;
	amx_StrParam(amx, params[2], version);

	if (version && strlen(version) < 28)
	{
		pNetGame->pPlayerPool->szVersion[playerid][0] = NULL;
		strcpy(pNetGame->pPlayerPool->szVersion[playerid], version);
		return 1;
	}
	return 0;
}

// native IsPlayerSpawned(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerSpawned)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerSpawned");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	int state = pNetGame->pPlayerPool->pPlayer[playerid]->byteState;
	return (state != PLAYER_STATE_NONE && state != PLAYER_STATE_WASTED && state != PLAYER_STATE_SPAWNED && pNetGame->pPlayerPool->pPlayer[playerid]->bHasSpawnInfo);
}

// native IsPlayerControllable(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerControllable)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerSpawned");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->bControllable;
}

// native SpawnForWorld(playerid);
AMX_DECLARE_NATIVE(Natives::SpawnForWorld)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "SpawnForWorld");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	CSAMPFunctions::SpawnPlayer_(playerid);
	return 1;
}

// native BroadcastDeath(playerid);
AMX_DECLARE_NATIVE(Natives::BroadcastDeath)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "BroadcastDeath");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	RakNet::BitStream bsData;
	bsData.Write((WORD)playerid);
	CSAMPFunctions::RPC(&RPC_DeathBroadcast, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), true, false);
	return 1;
}

// native IsPlayerCameraTargetEnabled(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerCameraTargetEnabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerCameraTargetEnabled");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bCameraTarget;
}

// native SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0);
AMX_DECLARE_NATIVE(Natives::SetPlayerDisabledKeysSync)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "SetPlayerDisabledKeySync");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->wDisabledKeys = static_cast<WORD>(params[2]);
	pPlayerData[playerid]->wDisabledKeysUD = static_cast<WORD>(params[3]);
	pPlayerData[playerid]->wDisabledKeysLR = static_cast<WORD>(params[4]);
	return 1;
}

// native GetPlayerDisabledKeysSync(playerid, &keys, &updown, &leftright);
AMX_DECLARE_NATIVE(Natives::GetPlayerDisabledKeysSync)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetPlayerDisabledKeySync");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	Utility::storeIntegerInNative(amx, params[2], pPlayerData[playerid]->wDisabledKeys);
	Utility::storeIntegerInNative(amx, params[3], static_cast<short>(pPlayerData[playerid]->wDisabledKeysUD));
	Utility::storeIntegerInNative(amx, params[4], static_cast<short>(pPlayerData[playerid]->wDisabledKeysLR));
	return 1;
}

// Scoreboard manipulation
// native TogglePlayerScoresPingsUpdate(playerid, bool:toggle);
AMX_DECLARE_NATIVE(Natives::TogglePlayerScoresPingsUpdate)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "TogglePlayerScoresPingsUpdate");

	int playerid = static_cast<int>(params[1]);
	bool toggle = static_cast<int>(params[2]) != 0;

	if(!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->bUpdateScoresPingsDisabled = !toggle;
	return 1;
}

// native TogglePlayerFakePing(playerid, bool:toggle);
AMX_DECLARE_NATIVE(Natives::TogglePlayerFakePing)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "TogglePlayerFakePing");

	int playerid = static_cast<int>(params[1]);
	bool toggle = static_cast<int>(params[2]) != 0;

	if(!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->bFakePingToggle = toggle;
	return 1;
}

// native SetPlayerFakePing(playerid, ping);
AMX_DECLARE_NATIVE(Natives::SetPlayerFakePing)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetPlayerFakePing");

	int playerid = static_cast<int>(params[1]);
	int fakeping = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->dwFakePingValue = fakeping;
	return 1;
}

// native TogglePlayerInServerQuery(playerid, bool:toggle);
AMX_DECLARE_NATIVE(Natives::TogglePlayerInServerQuery)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "TogglePlayerInServerQuery");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->bHidden = !(!!params[2]);
	return 1;
}

// native IsPlayerToggledInServerQuery(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerToggledInServerQuery)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsPlayerToggledInServerQuery");

	int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	return !pPlayerData[playerid]->bHidden;
}

// native IsPlayerPaused(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerPaused)
{
	CHECK_PARAMS(1, "IsPlayerPaused");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->bAFKState;
}

// native GetPlayerPausedTime(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPausedTime)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerPausedTime");

	int playerid = static_cast<int>(params[1]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(!pPlayerData[playerid]->bAFKState) return 0;

	return GetTickCount() - pPlayerData[playerid]->dwLastUpdateTick;
}

// Objects - global
// native Float:GetObjectDrawDistance(objectid);
AMX_DECLARE_NATIVE(Natives::GetObjectDrawDistance)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetObjectDrawDistance");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pObjects[objectid]->fDrawDistance);
}

// native Float:SetObjectMoveSpeed(objectid, Float:fSpeed);
AMX_DECLARE_NATIVE(Natives::SetObjectMoveSpeed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetObjectMoveSpeed");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	pNetGame->pObjectPool->pObjects[objectid]->fMoveSpeed = amx_ctof(params[2]);
	return 1;
}

// native Float:GetObjectMoveSpeed(objectid);
AMX_DECLARE_NATIVE(Natives::GetObjectMoveSpeed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetObjectMoveSpeed");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pObjects[objectid]->fMoveSpeed);
}

// native GetObjectTarget(objectid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetObjectTarget)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetObjectTarget");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	Utility::storeVectorInNative(amx, params[2], pObject->matTarget.pos);
	return 1;
}

// native GetObjectAttachedData(objectid, &vehicleid, &objectid, &attachedplayerid);
AMX_DECLARE_NATIVE(Natives::GetObjectAttachedData)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetObjectAttachedData");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	
	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	
	Utility::storeIntegerInNative(amx, params[2], pObject->wAttachedVehicleID);
	Utility::storeIntegerInNative(amx, params[3], pObject->wAttachedObjectID);
	Utility::storeIntegerInNative(amx, params[4], CServer::Get()->COBJECT_AttachedObjectPlayer[objectid]);
	return 1;
}

// native GetObjectAttachedOffset(objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
AMX_DECLARE_NATIVE(Natives::GetObjectAttachedOffset)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(7, "GetObjectAttachedOffset");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	Utility::storeVectorInNative(amx, params[2], pObject->vecAttachedOffset);
	Utility::storeVectorInNative(amx, params[5], pObject->vecAttachedRotation);
	return 1;
}

// native IsObjectMaterialSlotUsed(objectid, materialindex); // Return values: 1 = material, 2 = material text
AMX_DECLARE_NATIVE(Natives::IsObjectMaterialSlotUsed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsObjectMaterialSlotUsed");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	int materialindex = static_cast<int>(params[2]);
	if(materialindex < 0 || materialindex >= 16) return 0;

	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	
	// Nothing to comment here..
	while(i != 16)
	{
		if(pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if(i == 16) return 0;

	return pObject->Material[i].byteUsed;
}

// native GetObjectMaterial(objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor);
AMX_DECLARE_NATIVE(Natives::GetObjectMaterial)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(8, "GetObjectMaterial");

	int objectid = static_cast<int>(params[1]);
	int materialindex = static_cast<int>(params[2]);

	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(materialindex < 0 || materialindex >= 16) return 0;

	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];

	// Nothing to comment here..
	while(i != 16)
	{
		if(pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if(i == 16) return 0;

	Utility::storeIntegerInNative(amx, params[3], pObject->Material[i].wModelID);
	set_amxstring(amx, params[4], pObject->Material[i].szMaterialTXD, params[5]); // txdname[], txdnamelen = sizeof(txdname)
	set_amxstring(amx, params[6], pObject->Material[i].szMaterialTexture, params[7]); // texturenamelen = sizeof(txdnamelen),
	Utility::storeIntegerInNative(amx, params[8], ABGR_ARGB(pObject->Material[i].dwMaterialColor));
	return 1;
}

// native GetObjectMaterialText(objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
AMX_DECLARE_NATIVE(Natives::GetObjectMaterialText)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(12, "GetObjectMaterialText");

	int objectid = static_cast<int>(params[1]);
	int materialindex = static_cast<int>(params[2]);

	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(materialindex < 0 || materialindex >= 16) return 0;

	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];

	// Nothing to comment here..
	while(i != 16)
	{
		if(pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if(i == 16) return 0;

	set_amxstring(amx, params[3], pObject->szMaterialText[i], params[4]); 
	Utility::storeIntegerInNative(amx, params[5], pObject->Material[i].byteMaterialSize);
	set_amxstring(amx, params[6], pObject->Material[i].szFont, params[7]); 
	Utility::storeIntegerInNative(amx, params[8], pObject->Material[i].byteFontSize);
	Utility::storeIntegerInNative(amx, params[9], pObject->Material[i].byteBold);
	Utility::storeIntegerInNative(amx, params[10], pObject->Material[i].dwFontColor);
	Utility::storeIntegerInNative(amx, params[11], pObject->Material[i].dwBackgroundColor);
	Utility::storeIntegerInNative(amx, params[12], pObject->Material[i].byteAlignment);
	return 1;
}

// native IsObjectNoCameraCol(objectid);
AMX_DECLARE_NATIVE(Natives::IsObjectNoCameraCol)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsObjectNoCameraCol");

	int objectid = static_cast<int>(params[1]);
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if(!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	return pNetGame->pObjectPool->pObjects[objectid]->bNoCameraCol;
}

// native Float:GetPlayerObjectDrawDistance(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectDrawDistance)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerObjectDrawDistance");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= 1000) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->fDrawDistance);
}

// native Float:SetPlayerObjectMoveSpeed(playerid, objectid, Float:fSpeed);
AMX_DECLARE_NATIVE(Natives::SetPlayerObjectMoveSpeed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "SetPlayerObjectMoveSpeed");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->fMoveSpeed = amx_ctof(params[3]);
	return 1;
}

// native Float:GetPlayerObjectMoveSpeed(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectMoveSpeed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerObjectMoveSpeed");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->fMoveSpeed);
}

// native Float:GetPlayerObjectTarget(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectTarget)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetPlayerObjectTarget");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	Utility::storeVectorInNative(amx, params[3], pObject->matTarget.pos);
	return 1;
}

// native GetPlayerObjectAttachedData(playerid, objectid, &vehicleid, &objectid, &attachedplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectAttachedData)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetPlayerObjectAttachedData");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	
	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	Utility::storeIntegerInNative(amx, params[3], pObject->wAttachedVehicleID);
	Utility::storeIntegerInNative(amx, params[4], pPlayerData[playerid]->stObj[objectid].wObjectID);
	Utility::storeIntegerInNative(amx, params[5], pPlayerData[playerid]->stObj[objectid].wAttachPlayerID);
	return 1;
}

// native GetPlayerObjectAttachedOffset(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectAttachedOffset)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(8, "GetPlayerObjectAttachedOffset");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	CVector* vecOffset = NULL;
	CVector* vecRot = NULL;
	
	if(pObject->wAttachedVehicleID)
	{
		vecOffset = &pObject->vecAttachedOffset;
		vecRot = &pObject->vecAttachedRotation;
	}
	else
	{
		vecOffset = &pPlayerData[playerid]->stObj[objectid].vecOffset;
		vecRot = &pPlayerData[playerid]->stObj[objectid].vecRot;
	}
	
	Utility::storeVectorInNative(amx, params[3], *vecOffset);
	Utility::storeVectorInNative(amx, params[6], *vecRot);
	return 1;
}

// native IsPlayerObjectMaterialSlotUsed(playerid, objectid, materialindex); // Return values: 1 = material, 2 = material text
AMX_DECLARE_NATIVE(Natives::IsPlayerObjectMaterialSlotUsed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "IsPlayerObjectMaterialSlotUsed");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	int materialindex = static_cast<int>(params[3]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(materialindex < 0 || materialindex >= 16) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	
	// Nothing to comment here..
	while(i != 16)
	{
		if(pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if(i == 16) return 0;

	return pObject->Material[i].byteUsed;
}

// native GetPlayerObjectMaterial(playerid, objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectMaterial)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(9, "GetPlayerObjectMaterial");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	int materialindex = static_cast<int>(params[3]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(materialindex < 0 || materialindex >= 16) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	
	// Nothing to comment here..
	while(i != 16)
	{
		if(pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if(i == 16) return 0;

	Utility::storeIntegerInNative(amx, params[4], pObject->Material[i].wModelID); //  modelid
	set_amxstring(amx, params[5], pObject->Material[i].szMaterialTXD, params[6]); // txdname[], txdnamelen = sizeof(txdname)
	set_amxstring(amx, params[7], pObject->Material[i].szMaterialTexture, params[8]); // texturenamelen = sizeof(txdnamelen),
	Utility::storeIntegerInNative(amx, params[9], ABGR_ARGB(pObject->Material[i].dwMaterialColor)); // materialcolor
	return 1;
}

// native GetPlayerObjectMaterialText(playerid, objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectMaterialText)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(13, "GetPlayerObjectMaterialText");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	int materialindex = static_cast<int>(params[3]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if(materialindex < 0 || materialindex >= 16) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	
	// Nothing to comment here..
	while(i != 16)
	{
		if(pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if(i == 16) return 0;

	set_amxstring(amx, params[4], pObject->szMaterialText[i], params[5]); 
	Utility::storeIntegerInNative(amx, params[6], pObject->Material[i].byteMaterialSize); // materialsize
	set_amxstring(amx, params[7], pObject->Material[i].szFont, params[8]); 
	Utility::storeIntegerInNative(amx, params[9], pObject->Material[i].byteFontSize);
	Utility::storeIntegerInNative(amx, params[10], pObject->Material[i].byteBold);
	Utility::storeIntegerInNative(amx, params[11], pObject->Material[i].dwFontColor);
	Utility::storeIntegerInNative(amx, params[12], pObject->Material[i].dwBackgroundColor);
	Utility::storeIntegerInNative(amx, params[13], pObject->Material[i].byteAlignment);
	return 1;
}

// native IsPlayerObjectNoCameraCol(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::IsPlayerObjectNoCameraCol)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPlayerObjectNoCameraCol");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	return pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->bNoCameraCol;
}

// native GetPlayerSurfingPlayerObjectID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSurfingPlayerObjectID)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerSurfingPlayerObjectID");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return INVALID_OBJECT_ID;

	int surf = pPlayerData[playerid]->wSurfingInfo - MAX_VEHICLES;
	if(surf >= 0 && surf < MAX_OBJECTS)
	{
		if(pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][surf])
			return surf;
	}
	return INVALID_OBJECT_ID;
}

// native GetPlayerCameraTargetPlayerObj(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerCameraTargetPlayerObj)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPlayerCameraTargetPlayerObj");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return INVALID_OBJECT_ID;
	
	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
	if(!pPlayer->bCameraTarget) return INVALID_OBJECT_ID;

	int target = pNetGame->pPlayerPool->pPlayer[playerid]->aimSyncData.wCameraObject;
	if(target >= 0 && target < MAX_OBJECTS)
	{
		if(pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][target])
			return target;
	}
	return INVALID_OBJECT_ID;
}

// native GetObjectType(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::GetObjectType)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetObjectType");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	BYTE ret;
	if(pNetGame->pObjectPool->bObjectSlotState[objectid])
		ret = SELECT_OBJECT_GLOBAL_OBJECT;
	else if(pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid])
		ret = SELECT_OBJECT_PLAYER_OBJECT;
	else 
		ret = 0;
	return ret;
}

// native GetPlayerAttachedObject(playerid, index, &modelid, &bone, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ, Float:&fSacleX, Float:&fScaleY, Float:&fScaleZ, &materialcolor1, &materialcolor2);
AMX_DECLARE_NATIVE(Natives::GetPlayerAttachedObject)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(15, "GetPlayerAttachedObject");

	int playerid = static_cast<int>(params[1]);
	int slot = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(slot < 0 || slot >= MAX_PLAYER_ATTACHED_OBJECTS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->attachedObjectSlot[slot]) return 0;

	CAttachedObject *pObject = &pNetGame->pPlayerPool->pPlayer[playerid]->attachedObject[slot];
	Utility::storeIntegerInNative(amx, params[3], pObject->iBoneiD);
	Utility::storeIntegerInNative(amx, params[4], pObject->iBoneiD);
	Utility::storeVectorInNative(amx, params[5], pObject->vecPos);
	Utility::storeVectorInNative(amx, params[8], pObject->vecRot);
	Utility::storeVectorInNative(amx, params[11], pObject->vecScale);
	Utility::storeIntegerInNative(amx, params[14], RGBA_ABGR(pObject->dwMaterialColor1));
	Utility::storeIntegerInNative(amx, params[15], RGBA_ABGR(pObject->dwMaterialColor2));
	return 1;
}

// Vehicle functions
// native GetVehicleSpawnInfo(vehicleid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRot, &color1, &color2);
AMX_DECLARE_NATIVE(Natives::GetVehicleSpawnInfo)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(7, "GetVehicleSpawnInfo");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	CVehicle* pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];
	if(!pVehicle) 
		return 0;

	CVehicleSpawn spawn; 
	auto v = CServer::Get()->vehicleSpawnData.find(vehicleid);
	if(v == CServer::Get()->vehicleSpawnData.end())
	{
		spawn.vecPos = pVehicle->customSpawn.vecPos;
		spawn.fRot = pVehicle->customSpawn.fRot;
		spawn.iColor1 = pVehicle->customSpawn.iColor1;
		spawn.iColor2 = pVehicle->customSpawn.iColor2;

	}
	else
	{
		spawn.vecPos = v->second.vecPos;
		spawn.fRot = v->second.fRot;
		spawn.iColor1 = v->second.iColor1;
		spawn.iColor2 = v->second.iColor2;	
	}
	Utility::storeVectorInNative(amx, params[2], spawn.vecPos);
	Utility::storeFloatInNative(amx, params[5], spawn.fRot);
	Utility::storeIntegerInNative(amx, params[6], spawn.iColor1);
	Utility::storeIntegerInNative(amx, params[7], spawn.iColor2);
	return 1;
}

// native SetVehicleSpawnInfo(vehicleid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, respawntime = -2, interior = -2);
AMX_DECLARE_NATIVE(Natives::SetVehicleSpawnInfo)
{
	if (!CServer::Get()->IsInitialized() || !CAddress::FUNC_CVehicle__Respawn) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(10, "SetVehicleSpawnInfo");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	int modelid = static_cast<int>(params[2]);
	if(modelid < 400 || modelid > 611) return 0;

	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid]; 
	if(!pVehicle) 
		return 0;

	bool bStreamedIn = false;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;		

	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(pPlayerPool->pPlayer[i]->byteVehicleStreamedIn[pVehicle->wVehicleID])
			{
				bStreamedIn = true;
				break;
			}
		}
	}

	CVehicleSpawn spawn;
	spawn.iModelID = modelid;
	spawn.vecPos =  CVector(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
	spawn.fRot = amx_ctof(params[6]);
	spawn.iColor1 = static_cast<int>(params[7]);
	spawn.iColor2 = static_cast<int>(params[8]);
	spawn.iRespawnTime = pVehicle->customSpawn.iRespawnTime;
	spawn.iInterior = pVehicle->customSpawn.iInterior;

	// logprintf("spandata: %d, %d", spawn.iRespawnTime, spawn.iInterior);

	int respawntime = static_cast<int>(params[9]);
	if(respawntime >= -1)
	{
		spawn.iRespawnTime = respawntime;
	}

	int interior = static_cast<int>(params[10]);
	if(interior != -2)
	{
		spawn.iInterior = interior;
	}
	
	auto v = CServer::Get()->vehicleSpawnData.find(pVehicle->wVehicleID);
	if(v != CServer::Get()->vehicleSpawnData.end())
	{
		CServer::Get()->vehicleSpawnData.erase(v);
		// logprintf("add custom");
	}
	CServer::Get()->vehicleSpawnData.insert(std::make_pair(vehicleid, spawn));

	// logprintf("streamedin: %d, iRespawnTime: %d, interior: %d", bStreamedIn, respawntime, interior);

	if(!bStreamedIn)
	{
		pVehicle->customSpawn.iModelID = spawn.iModelID;
		pVehicle->customSpawn.fRot = spawn.fRot;
		pVehicle->customSpawn.iColor1 = spawn.iColor1;
		pVehicle->customSpawn.iColor2 = spawn.iColor2;
		pVehicle->customSpawn.iRespawnTime = spawn.iRespawnTime;
		pVehicle->customSpawn.iInterior= spawn.iInterior;
	}
	return 1;
}

// native GetVehicleModelCount(modelid);
AMX_DECLARE_NATIVE(Natives::GetVehicleModelCount)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehicleModelCount");

	int modelid = static_cast<int>(params[1]);
	if(modelid < 400 || modelid > 611) return 0;
	
	return pNetGame->pVehiclePool->byteVehicleModelsUsed[modelid - 400];
}

// native GetVehicleModelsUsed();
AMX_DECLARE_NATIVE(Natives::GetVehicleModelsUsed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	BYTE byteModelsUsed = 0;
	for(BYTE i = 0; i != 212; i++)
	{
		if(pNetGame->pVehiclePool->byteVehicleModelsUsed[i] != 0)
			byteModelsUsed++;
	}

	return byteModelsUsed;
}

// native GetVehicleColor(vehicleid, &color1, &color2);
AMX_DECLARE_NATIVE(Natives::GetVehicleColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "GetVehicleColor");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;
	
	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];
	int color1 = CServer::Get()->bChangedVehicleColor[vehicleid] ? pVehicle->vehModInfo.iColor1 : pVehicle->customSpawn.iColor1;
	int color2 = CServer::Get()->bChangedVehicleColor[vehicleid] ? pVehicle->vehModInfo.iColor2 : pVehicle->customSpawn.iColor2;

	Utility::storeIntegerInNative(amx, params[2], color1);
	Utility::storeIntegerInNative(amx, params[3], color2);
	return 1;
}

// native GetVehiclePaintjob(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehiclePaintjob)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehiclePaintjob");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehModInfo.bytePaintJob - 1;
}

// native GetVehicleInterior(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleInterior)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehicleInterior");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iInterior;
}

// native GetVehicleNumberPlate(vehicleid, plate[], len = sizeof(plate));
AMX_DECLARE_NATIVE(Natives::GetVehicleNumberPlate)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "GetVehicleNumberPlate");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	char* szPlate = NULL;
	if (pNetGame->pVehiclePool->pVehicle[vehicleid]->szNumberplate[0])
	{
		szPlate = &pNetGame->pVehiclePool->pVehicle[vehicleid]->szNumberplate[0];
	}
	else
	{
		szPlate = "XYZSR998";
	}
	return set_amxstring(amx, params[2], szPlate, params[3]);
}

// native SetVehicleRespawnDelay(vehicleid, delay);
AMX_DECLARE_NATIVE(Natives::SetVehicleRespawnDelay)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetVehicleRespawnDelay");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iRespawnTime = (static_cast<int>(params[2]) * 1000);
	return 1;
}

// native GetVehicleRespawnDelay(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleRespawnDelay)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehicleRespawnDelay");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iRespawnTime / 1000;
}

// native SetVehicleOccupiedTick(vehicleid, ticks);
AMX_DECLARE_NATIVE(Natives::SetVehicleOccupiedTick)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetVehicleOccupiedTick");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	pNetGame->pVehiclePool->pVehicle[vehicleid]->vehOccupiedTick = static_cast<int>(params[2]);
	return 1;
}

// native GetVehicleOccupiedTick(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleOccupiedTick)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehicleOccupiedTick");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehOccupiedTick;
}

// native SetVehicleRespawnTick(vehicleid, ticks);
AMX_DECLARE_NATIVE(Natives::SetVehicleRespawnTick)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetVehicleRespawnTick");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	pNetGame->pVehiclePool->pVehicle[vehicleid]->vehRespawnTick = static_cast<int>(params[2]);
	return 1;
}

// native GetVehicleRespawnTick(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleRespawnTick)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehicleRespawnTick");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehRespawnTick;
}

// native GetVehicleLastDriver(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleLastDriver)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehicleLastDriver");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->wLastDriverID;
}

// native GetVehicleCab(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleCab)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetVehicleCab");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;
	
	CVehicle *pVeh;
	for(WORD i = 0; i != MAX_VEHICLES; i++)
	{
		pVeh = pNetGame->pVehiclePool->pVehicle[i];
		if(!pVeh) continue;

		if(pVeh->wTrailerID != 0 && pVeh->wTrailerID == vehicleid)
			return i;
	}
	return 0;
}

// native HasVehicleBeenOccupied(vehicleid);
AMX_DECLARE_NATIVE(Natives::HasVehicleBeenOccupied)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "HasVehicleBeenOccupied");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->bOccupied;
}

// native SetVehicleBeenOccupied(vehicleid, occupied);
AMX_DECLARE_NATIVE(Natives::SetVehicleBeenOccupied)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetVehicleBeenOccupied");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	pNetGame->pVehiclePool->pVehicle[vehicleid]->bOccupied = static_cast<BYTE>(params[2]) != 0;
	return 1;
}

// native IsVehicleOccupied(vehicleid);
AMX_DECLARE_NATIVE(Natives::IsVehicleOccupied)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsVehicleOccupied");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	CPlayer *pPlayer;
	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i)) continue; 
		pPlayer = pNetGame->pPlayerPool->pPlayer[i];

		if(pPlayer->wVehicleId == vehicleid && (pPlayer->byteState == PLAYER_STATE_DRIVER || pPlayer->byteState == PLAYER_STATE_PASSENGER)) 
			return 1;
	}
	return 0;
}

// native IsVehicleDead(vehicleid);
AMX_DECLARE_NATIVE(Natives::IsVehicleDead)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsVehicleDead");

	int vehicleid = static_cast<int>(params[1]);
	if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;
	
	if(!pNetGame->pVehiclePool->pVehicle[vehicleid]) 
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->bDead;
}

// Gangzone functions
// native IsValidGangZone(zoneid);
AMX_DECLARE_NATIVE(Natives::IsValidGangZone)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsValidGangZone");
	
	int zoneid = static_cast<int>(params[1]);
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	return CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid));
}

// native IsGangZoneVisibleForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsGangZoneVisibleForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsGangZoneVisibleForPlayer");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if(!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	return !!(pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid)) != 0xFF);
}

// native GangZoneGetColorForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::GangZoneGetColorForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GangZoneGetColorForPlayer");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->dwClientSideZoneColor[id];
	}
	return 0;
}

// native GangZoneGetFlashColorForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::GangZoneGetFlashColorForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GangZoneGetFlashColorForPlayer");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->dwClientSideZoneFlashColor[id];
	}
	return 0;
}

// native IsGangZoneFlashingForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsGangZoneFlashingForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsGangZoneFlashingForPlayer");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->bIsGangZoneFlashing[id];
	}
	return 0;
}

// native IsPlayerInGangZone(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerInGangZone)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPlayerInGangZone");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->bInGangZone[id];
	}
	return 0;
}

// native GangZoneGetPos(zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);
AMX_DECLARE_NATIVE(Natives::GangZoneGetPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GangZoneGetPos");

	int zoneid = static_cast<int>(params[1]);
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid)))  return 0;
	
	CGangZone *pGangZone = CServer::Get()->pGangZonePool->pGangZone[zoneid];
	Utility::storeFloatInNative(amx, params[2], pGangZone->fGangZone[0]);
	Utility::storeFloatInNative(amx, params[3], pGangZone->fGangZone[1]);
	Utility::storeFloatInNative(amx, params[4], pGangZone->fGangZone[2]);
	Utility::storeFloatInNative(amx, params[5], pGangZone->fGangZone[3]);
	return 1;
}

// Textdraw functions
// native IsValidTextDraw(textdrawid);
AMX_DECLARE_NATIVE(Natives::IsValidTextDraw)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsValidTextDraw");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	return pNetGame->pTextDrawPool->bSlotState[textdrawid];
}

// native IsTextDrawVisibleForPlayer(playerid, textdrawid);
AMX_DECLARE_NATIVE(Natives::IsTextDrawVisibleForPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server 
	CHECK_PARAMS(2, "IsTextDrawVisibleForPlayer");
	
	int playerid = static_cast<int>(params[1]);
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;

	int textdrawid = static_cast<int>(params[2]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;

	return pNetGame->pTextDrawPool->bHasText[textdrawid][playerid];
}

// native TextDrawGetString(textdrawid, text[], len = sizeof(text));
AMX_DECLARE_NATIVE(Natives::TextDrawGetString)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "TextDrawGetString");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	const char *szText = (pNetGame->pTextDrawPool->bSlotState[textdrawid]) ? pNetGame->pTextDrawPool->szFontText[textdrawid] : '\0';
	return set_amxstring(amx, params[2], szText, params[3]);
}

// native TextDrawSetPos(textdrawid, Float:fX, Float:fY);
AMX_DECLARE_NATIVE(Natives::TextDrawSetPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "TextDrawPos");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	pTD->vecPos.fX = amx_ctof(params[2]);
	pTD->vecPos.fY = amx_ctof(params[3]);
	return 1;
}

// native TextDrawGetLetterSize(textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::TextDrawGetLetterSize)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "TextDrawGetLetterSize");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[2], pTD->vecLetter);
	return 1;
}

// native TextDrawGetTextSize(textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::TextDrawGetTextSize)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "TextDrawGetTextSize");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[2], pTD->vecLine);
	return 1;
}

// native TextDrawGetPos(textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::TextDrawGetPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "TextDrawGetPos");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[2], pTD->vecPos);
	return 1;
}

// native TextDrawGetColor(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawGetColor");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return ABGR_RGBA(pTD->dwLetterColor);
}

// native TextDrawGetBoxColor(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetBoxColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawGetBoxColor");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return ABGR_RGBA(pTD->dwBoxColor);
}

// native TextDrawGetBackgroundColor(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetBackgroundColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawGetBackgroundColor");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return ABGR_RGBA(pTD->dwBackgroundColor);
}

// native TextDrawGetShadow(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetShadow)
{
	CHECK_PARAMS(1, "TextDrawGetShadow");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteShadow;
}

// native TextDrawGetOutline(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetOutline)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawGetOutline");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteOutline;
}

// native TextDrawGetFont(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetFont)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawGetOutline");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteStyle;
}

// native TextDrawIsBox(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawIsBox)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawIsBox");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteBox;
}

// native TextDrawIsProportional(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawIsProportional)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawIsProportional");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteProportional;
}

// native TextDrawIsSelectable(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawIsSelectable)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawIsSelectable");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteSelectable;
}

// native TextDrawGetAlignment(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetAlignment)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "TextDrawGetAlignment");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	BYTE ret = 0;

	if(pTD->byteCenter) ret = 2;
	else if(pTD->byteLeft) ret = 1;
	else if(pTD->byteRight) ret = 3;
	return ret;
}

// native TextDrawGetPreviewModel(textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetPreviewModel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server
	CHECK_PARAMS(1, "TextDrawGetPreviewModel");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->dwModelIndex;
}

// native TextDrawGetPreviewRot(textdrawid, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fZoom);
AMX_DECLARE_NATIVE(Natives::TextDrawGetPreviewRot)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server 
	CHECK_PARAMS(5, "TextDrawGetPreviewRot");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[2], pTD->vecRot);
	Utility::storeFloatInNative(amx, params[5], pTD->fZoom);
	return 1;
}

// native TextDrawGetPreviewVehCol(textdrawid, &color1, &color2);
AMX_DECLARE_NATIVE(Natives::TextDrawGetPreviewVehCol)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "TextDrawGetPreviewVehCol");
	
	int textdrawid = static_cast<int>(params[1]);
	if(textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;
	
	if(!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	Utility::storeIntegerInNative(amx, params[2], pTD->color1);
	Utility::storeIntegerInNative(amx, params[3], pTD->color2);
	return 1;
}

// Per-Player textdraws
// native IsValidPlayerTextDraw(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::IsValidPlayerTextDraw)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server 
	CHECK_PARAMS(2, "IsValidPlayerTextDraw");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid];
}

// native IsPlayerTextDrawVisible(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::IsPlayerTextDrawVisible)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPlayerTextDrawVisible");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bHasText[textdrawid];
}

// native PlayerTextDrawGetString(playerid, PlayerText:textdrawid, text[], len = sizeof(text));
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetString)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "PlayerTextDrawGetString");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	
	bool bIsValid = static_cast<int>(pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) != 0;
	if(!bIsValid) return 0;

	const char *szText = (bIsValid) ? pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->szFontText[textdrawid]: '\0';
	return set_amxstring(amx, params[3], szText, params[4]);
}

// native PlayerTextDrawSetPos(playerid, PlayerText:textdrawid, Float:fX, Float:fY);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawSetPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "PlayerTextDrawSetPos");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

	pTD->vecPos.fX = amx_ctof(params[3]);
	pTD->vecPos.fY = amx_ctof(params[4]);
	return 1;
}

// native PlayerTextDrawGetLetterSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetLetterSize)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server 
	CHECK_PARAMS(4, "PlayerTextDrawGetLetterSize");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[3], pTD->vecLetter);
	return 1;
}

// native PlayerTextDrawGetTextSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetTextSize)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "PlayerTextDrawGetTextSize");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[3], pTD->vecLine);
	return 1;
}

// native PlayerTextDrawGetPos(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "PlayerTextDrawGetPos");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[3], pTD->vecPos);
	return 1;
}

// native PlayerTextDrawGetColor(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawGetColor");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return ABGR_RGBA(pTD->dwLetterColor);
}

// native PlayerTextDrawGetBoxColor(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetBoxColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server 
	CHECK_PARAMS(2, "PlayerTextDrawGetBoxColor");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return ABGR_RGBA(pTD->dwBoxColor);
}

// native PlayerTextDrawGetBackgroundCol(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetBackgroundCol)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server 
	CHECK_PARAMS(2, "PlayerTextDrawGetBackgroundCol");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return ABGR_RGBA(pTD->dwBackgroundColor);
}

// native PlayerTextDrawGetShadow(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetShadow)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawGetShadow");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return pTD->byteShadow;
}

// native PlayerTextDrawGetOutline(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetOutline)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawGetOutline");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return pTD->byteOutline;
}

// native PlayerTextDrawGetFont(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetFont)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawGetFont");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return pTD->byteStyle;
}

// native PlayerTextDrawIsBox(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawIsBox)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server 
	CHECK_PARAMS(2, "PlayerTextDrawIsBox");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return pTD->byteBox;
}

// native PlayerTextDrawIsProportional(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawIsProportional)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawIsProportional");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return pTD->byteProportional;
}

// native PlayerTextDrawIsSelectable(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawIsSelectable)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawIsSelectable");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return pTD->byteSelectable;
}

// native PlayerTextDrawGetAlignment(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetAlignment)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawGetAlignment");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	BYTE ret = 0;

	if(pTD->byteCenter) ret = 2;
	else if(pTD->byteLeft) ret = 1;
	else if(pTD->byteRight) ret = 3;
	return ret;
}

// native PlayerTextDrawGetPreviewModel(playerid, PlayerText:textdrawid);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPreviewModel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerTextDrawGetPreviewModel");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
	return pTD->dwModelIndex;
}

// native PlayerTextDrawGetPreviewRot(playerid, PlayerText:textdrawid, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fZoom);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPreviewRot)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(6, "PlayerTextDrawGetPreviewRot");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

	Utility::storeVectorInNative(amx, params[3], pTD->vecRot);
	Utility::storeFloatInNative(amx, params[6], pTD->fZoom);
	return 1;
}

// native PlayerTextDrawGetPreviewVehCol(playerid, PlayerText:textdrawid, &color1, &color2);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPreviewVehCol)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "PlayerTextDrawGetPreviewVehCol");
	
	int playerid = static_cast<int>(params[1]);
	int textdrawid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

	Utility::storeIntegerInNative(amx, params[3], pTD->color1);
	Utility::storeIntegerInNative(amx, params[4], pTD->color2);
	return 1;
}

// 3D Text labels
// native IsValid3DTextLabel(Text3D:id);
AMX_DECLARE_NATIVE(Natives::IsValid3DTextLabel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsValid3DTextLabel");
	
	int id = static_cast<int>(params[1]);
	if(0 < id || id >= MAX_3DTEXT_GLOBAL) return 0;
	
	return pNetGame->p3DTextPool->bIsCreated[id];
}

// native Is3DTextLabelStreamedIn(playerid, Text3D:id);
AMX_DECLARE_NATIVE(Natives::Is3DTextLabelStreamedIn)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "Is3DTextLabelStreamedIn");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[1]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(0 < id || id >= MAX_3DTEXT_PLAYER) return 0;
	
	return pNetGame->pPlayerPool->pPlayer[playerid]->byte3DTextLabelStreamedIn[id];
}

// native Get3DTextLabelText(id, text[], len = sizeof(text));
AMX_DECLARE_NATIVE(Natives::Get3DTextLabelText)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "Get3DTextLabelText");
	
	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

	if(!pNetGame->p3DTextPool->bIsCreated[id]) return 0;

	const char *szText = pNetGame->p3DTextPool->TextLabels[id].szText;
	return set_amxstring(amx, params[2], szText, params[3]);
}

// native Get3DTextLabelColor(id);
AMX_DECLARE_NATIVE(Natives::Get3DTextLabelColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "Get3DTextLabelColor");
	
	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;
	
	if(!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
	C3DText &p3DText = pNetGame->p3DTextPool->TextLabels[id];

	return p3DText.dwColor;
}

// native Get3DTextLabelPos(id, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::Get3DTextLabelPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "Get3DTextLabelPos");
	
	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;
	
	if(!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
	C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

	Utility::storeVectorInNative(amx, params[2], p3DText.vecPos);
	return 1;
}

// native Float:Get3DTextLabelDrawDistance(id);
AMX_DECLARE_NATIVE(Natives::Get3DTextLabelDrawDistance)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "Get3DTextLabelDrawDistance");
	
	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;
	
	if(!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
	C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

	return amx_ftoc(p3DText.fDrawDistance);
}

// native Get3DTextLabelLOS(id);
AMX_DECLARE_NATIVE(Natives::Get3DTextLabelLOS)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "Get3DTextLabelLOS");
	
	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;
	
	if(!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
	C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

	return p3DText.bLineOfSight;
}

// native Get3DTextLabelVirtualWorld(id);
AMX_DECLARE_NATIVE(Natives::Get3DTextLabelVirtualWorld)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "Get3DTextLabelVirtualWorld");
	
	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;
	
	if(!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
	C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

	return p3DText.iWorld;
}

// native Get3DTextLabelAttachedData(id, &attached_playerid, &attached_vehicleid);
AMX_DECLARE_NATIVE(Natives::Get3DTextLabelAttachedData)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "Get3DTextLabelAttachedData");
	
	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;
	
	if(!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
	C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

	Utility::storeIntegerInNative(amx, params[2], p3DText.wAttachedToPlayerID);
	Utility::storeIntegerInNative(amx, params[3], p3DText.wAttachedToVehicleID);
	return 1;
}

// native IsValidPlayer3DTextLabel(playerid, PlayerText3D:id);
AMX_DECLARE_NATIVE(Natives::IsValidPlayer3DTextLabel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsValidPlayer3DTextLabel");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;
	
	return pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id];
}

// native GetPlayer3DTextLabelText(playerid, PlayerText3D:id, text[], len = sizeof(text));
AMX_DECLARE_NATIVE(Natives::GetPlayer3DTextLabelText)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetPlayer3DTextLabelText");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;
	
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

	const char *szText = (pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id].szText) ? pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id].szText : '\0';
	return set_amxstring(amx, params[3], szText, params[4]);
}

// native GetPlayer3DTextLabelColor(playerid, PlayerText3D:id);
AMX_DECLARE_NATIVE(Natives::GetPlayer3DTextLabelColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayer3DTextLabelColor");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;
	
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

	C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
	return p3DText.dwColor;
}

// native GetPlayer3DTextLabelPos(playerid, PlayerText3D:id, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPlayer3DTextLabelPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetPlayer3DTextLabelPos");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

	if(!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;
	C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];

	Utility::storeVectorInNative(amx, params[3], p3DText.vecPos);
	return 1;
}

// native Float:GetPlayer3DTextLabelDrawDist(playerid, PlayerText3D:id);
AMX_DECLARE_NATIVE(Natives::GetPlayer3DTextLabelDrawDist)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayer3DTextLabelDrawDist");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;
	
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

	C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
	return amx_ftoc(p3DText.fDrawDistance);
}

// native GetPlayer3DTextLabelLOS(playerid, PlayerText3D:id);
AMX_DECLARE_NATIVE(Natives::GetPlayer3DTextLabelLOS)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayer3DTextLabelLOS");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;
	
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

	C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
	return p3DText.bLineOfSight;
}

// native GetPlayer3DTextLabelVirtualW(playerid, PlayerText3D:id);
AMX_DECLARE_NATIVE(Natives::GetPlayer3DTextLabelVirtualW)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayer3DTextLabelVirtualW");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;
	
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

	C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
	return p3DText.iWorld;
}

// native GetPlayer3DTextLabelAttached(playerid, PlayerText3D:id, &attached_playerid, &attached_vehicleid);
AMX_DECLARE_NATIVE(Natives::GetPlayer3DTextLabelAttached)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetPlayer3DTextLabelAttached");
	
	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;
	
	if(!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

	C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
	Utility::storeIntegerInNative(amx, params[3], p3DText.wAttachedToPlayerID);
	Utility::storeIntegerInNative(amx, params[4], p3DText.wAttachedToVehicleID);
	return 1;
}

// native AttachObjectToPlayer(objectid, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ)
AMX_DECLARE_NATIVE(Natives::YSF_AttachObjectToPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(8, "AttachObjectToPlayer");

	int playerid = static_cast<int>(params[2]);
	int objectid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(objectid < 1 || objectid >= MAX_OBJECTS) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	if(!pObject) return 0;

	// FUCK SAMP -.- n_AttachObjectToPlayer always return 0
	pAttachObjectToPlayer(amx, params);

	CServer::Get()->COBJECT_AttachedObjectPlayer[objectid] = static_cast<WORD>(playerid);
	pObject->vecAttachedOffset = CVector(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
	pObject->vecAttachedRotation = CVector(amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]));
	return 1;
}

// native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ, onlyaddtoinstance = 0)
AMX_DECLARE_NATIVE(Natives::YSF_AttachPlayerObjectToPlayer)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	//	CHECK_PARAMS(9, "AttachPlayerObjectToPlayer");

	int playerid = static_cast<int>(params[1]);
	int objectid = static_cast<int>(params[2]);
	int attachplayerid = static_cast<int>(params[3]);
	bool bOnlyAddToInstance = static_cast<int>(params[10]) != 0;

	if(!IsPlayerConnected(playerid)) return 0;
	if(!IsPlayerConnected(attachplayerid)) return 0;

	if(objectid < 1 || objectid >= MAX_OBJECTS) return 0;
	if(!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;
	
	pPlayerData[playerid]->stObj[objectid].wAttachPlayerID = static_cast<WORD>(attachplayerid);
	pPlayerData[playerid]->stObj[objectid].vecOffset = CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));
	pPlayerData[playerid]->stObj[objectid].vecRot = CVector(amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]));

	if(!bOnlyAddToInstance)
	{
		if(pNetGame->pPlayerPool->pPlayer[playerid]->byteStreamedIn[attachplayerid])
		{
			RakNet::BitStream bs;
			bs.Write((WORD)objectid); // wObjectID
			bs.Write((WORD)attachplayerid); // playerid
			bs.Write(amx_ctof(params[4]));
			bs.Write(amx_ctof(params[5]));
			bs.Write(amx_ctof(params[6]));
			bs.Write(amx_ctof(params[7]));
			bs.Write(amx_ctof(params[8]));
			bs.Write(amx_ctof(params[9]));
			CSAMPFunctions::RPC(&RPC_AttachObject, &bs, LOW_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);

			pPlayerData[playerid]->bAttachedObjectCreated = true;
		}
	}
	else
	{
			pPlayerData[playerid]->dwCreateAttachedObj = GetTickCount();
			pPlayerData[playerid]->dwObjectID = static_cast<WORD>(objectid);
			pPlayerData[playerid]->bAttachedObjectCreated = true;
	}
	return 1;
}

// native AttachPlayerObjectToObject(playerid, objectid, attachtoid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ, SyncRotation = 1);
AMX_DECLARE_NATIVE(Natives::AttachPlayerObjectToObject)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(10, "AttachPlayerObjectToObject");

	int forplayerid = static_cast<int>(params[1]);
	int wObjectID = static_cast<int>(params[2]);
	int wAttachTo = static_cast<int>(params[3]);

	if(!IsPlayerConnected(forplayerid)) return 0;

	if(wObjectID < 1 || wObjectID >= MAX_OBJECTS) return 0;
	if(wAttachTo < 1 || wAttachTo >= MAX_OBJECTS) return 0;

	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	if(!pObjectPool->pPlayerObjects[forplayerid][wObjectID] || !pObjectPool->pPlayerObjects[forplayerid][wAttachTo]) return 0; // Check if object is exist

	// Get data
	CVector vecOffset = CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));
	CVector vecOffsetRot = CVector(amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]));
	BYTE byteSyncRot = !!params[10];

	// Store data
	pPlayerData[forplayerid]->stObj[wObjectID].wObjectID = static_cast<WORD>(wAttachTo);
	pPlayerData[forplayerid]->stObj[wObjectID].vecOffset = vecOffset;
	pPlayerData[forplayerid]->stObj[wObjectID].vecRot = vecOffsetRot;

	// Attach it
	int iModelID = pObjectPool->pPlayerObjects[forplayerid][wObjectID]->iModel;
	CVector vecPos = pObjectPool->pPlayerObjects[forplayerid][wObjectID]->matWorld.pos;
	CVector vecRot = pObjectPool->pPlayerObjects[forplayerid][wObjectID]->vecRot;
	float fDrawDistance = 299.0;
	BYTE byteNoCameraCol = pObjectPool->pPlayerObjects[forplayerid][wObjectID]->bNoCameraCol;

	RakNet::BitStream bs;
	bs.Write((WORD)wObjectID);
	bs.Write(iModelID);
	bs.Write(vecPos);
	bs.Write(vecRot);
	bs.Write(fDrawDistance); // 159
	bs.Write(byteNoCameraCol);
	bs.Write((WORD)-1); // attached vehicle
	bs.Write((WORD)wAttachTo); // attached object
	bs.Write(vecOffset);
	bs.Write(vecOffsetRot);	
	bs.Write(byteSyncRot);
	
	CSAMPFunctions::RPC(&RPC_CreateObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0); // Send this on same RPC as CreateObject
	return 1;
}

// native SetRecordingDirectory(const dir[]);
AMX_DECLARE_NATIVE(Natives::SetRecordingDirectory)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "SetRecordingDirectory");

	char *dir;
	amx_StrParam(amx, params[1], dir);
	if (!dir || !CAddress::ADDR_RecordingDirectory) return 0;

	strcpy(gRecordingDataPath, dir);
	strcat(gRecordingDataPath, "/%s.rec");
	return 1;
}

// native GetRecordingDirectory(dir[], len = sizeof(dir));
AMX_DECLARE_NATIVE(Natives::GetRecordingDirectory)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetRecordingDirectory");
	
	if(!CAddress::ADDR_RecordingDirectory) return 0;

	char temp[MAX_PATH];
	int len = strlen(gRecordingDataPath);
	strcpy(temp, gRecordingDataPath);
	temp[len - 7] = 0;

	return set_amxstring(amx, params[1], temp, params[2]);
}

// native SendClientMessagef(playerid, color, const message[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::SendClientMessagef)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 3, len);
	if(!szMessage) return 0;

	RakNet::BitStream bsParams;
	bsParams.Write((DWORD)params[2]);
	bsParams.Write((DWORD)len);
	bsParams.Write(szMessage, len);
	CSAMPFunctions::RPC(&RPC_ClientMessage, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

// native SendClientMessageToAllf(color, const message[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::SendClientMessageToAllf)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 2, len);
	if(!szMessage) return 0;

	RakNet::BitStream bsParams;
	bsParams.Write((DWORD)params[1]);
	bsParams.Write((DWORD)len);
	bsParams.Write(szMessage, len);
	CSAMPFunctions::RPC(&RPC_ClientMessage, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, false);
	return 1;
}

// native GameTextForPlayerf(playerid, displaytime, style, const message[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::GameTextForPlayerf)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 4, len);
	if(!szMessage) return 0;

	RakNet::BitStream bsParams;
	bsParams.Write((int)params[3]);
	bsParams.Write((int)params[2]);
	bsParams.Write(len);
	bsParams.Write(szMessage, len);
	CSAMPFunctions::RPC(&RPC_ScrDisplayGameText, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

// native GameTextForAllf(displaytime, style, const message[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::GameTextForAllf)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 3, len);
	if(!szMessage) return 0;

	RakNet::BitStream bsParams;
	bsParams.Write((int)params[2]);
	bsParams.Write((int)params[1]);
	bsParams.Write(len);
	bsParams.Write(szMessage, len);
	CSAMPFunctions::RPC(&RPC_ScrDisplayGameText, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, false);
	return 1;
}

// native SendPlayerMessageToPlayerf(playerid, senderid, const message[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::SendPlayerMessageToPlayerf)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	int senderid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(senderid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 3, len);
	if(!szMessage) return 0;

	RakNet::BitStream bsParams;
	bsParams.Write((WORD)senderid);
	bsParams.Write((BYTE)len);
	bsParams.Write(szMessage, len);
	CSAMPFunctions::RPC(&RPC_Chat, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

// native SendPlayerMessageToAllf(senderid, const message[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::SendPlayerMessageToAllf)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	int senderid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(senderid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 2, len);
	if(!szMessage) return 0;

	RakNet::BitStream bsParams;
	bsParams.Write((WORD)senderid);
	bsParams.Write((BYTE)len);
	bsParams.Write(szMessage, len);
	CSAMPFunctions::RPC(&RPC_Chat, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, false);
	return 1;
}

// native SendRconCommandf(command[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::SendRconCommandf)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 1, len);
	if(!szMessage) return 0;

	CSAMPFunctions::Execute(szMessage);
	return 1;
}

// native YSF_SetTickRate(ticks);
AMX_DECLARE_NATIVE(Natives::YSF_SetTickRate)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "YSF_SetTickRate");

	int rate = static_cast<int>(params[1]);
	if(rate < -1 || rate == 0) return 0; // -1 = no update

	CServer::Get()->SetTickRate(rate);
	return 1;
}

// native YSF_GetTickRate();
AMX_DECLARE_NATIVE(Natives::YSF_GetTickRate)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->GetTickRate());
}

// native YSF_EnableNightVisionFix(enable);
AMX_DECLARE_NATIVE(Natives::YSF_EnableNightVisionFix)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "YSF_EnableNightVisionFix");

	CServer::Get()->EnableNightVisionFix(static_cast<int>(params[1]) != 0);
	return 1;
}

// native YSF_IsNightVisionFixEnabled();
AMX_DECLARE_NATIVE(Natives::YSF_IsNightVisionFixEnabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->IsNightVisionFixEnabled());
}

// native YSF_SetExtendedNetStatsEnabled(enable);
AMX_DECLARE_NATIVE(Natives::YSF_SetExtendedNetStatsEnabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "YSF_SetExtendedNetStatsEnabled");

	CServer::Get()->SetExtendedNetStatsEnabled(static_cast<int>(params[1]) != 0);
	return 1;
}

// native YSF_IsExtendedNetStatsEnabled();
AMX_DECLARE_NATIVE(Natives::YSF_IsExtendedNetStatsEnabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->IsExtendedNetStatsEnabled());
}

// native YSF_SetAFKAccuracy(time_ms);
AMX_DECLARE_NATIVE(Natives::YSF_SetAFKAccuracy)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "YSF_SetAFKAccuracy");

	CServer::Get()->SetAFKAccuracy(static_cast<DWORD>(params[1]));
	return 1;
}

// native YSF_GetAFKAccuracy();
AMX_DECLARE_NATIVE(Natives::YSF_GetAFKAccuracy)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->GetAFKAccuracy());
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneCreate)
{
	CHECK_PARAMS(4, "GangZoneCreate");

	float fMinX = amx_ctof(params[1]);
	float fMinY = amx_ctof(params[2]);
	float fMaxX = amx_ctof(params[3]);
	float fMaxY = amx_ctof(params[4]);

	// If coordinates are wrong, then won't create bugged zone!
	if(fMaxX <= fMinX || fMaxY <= fMinY) 
	{
		//logprintf("GangZoneCreate: MaxX, MaxY must be bigger than MinX, MinY. Not inversely!");
		//logprintf("GangZoneCreate: %f, %f, %f, %f",fMinX, fMinY, fMaxX, fMaxY);
		return -1;
	}

	WORD ret = CServer::Get()->pGangZonePool->New(fMinX, fMinY, fMaxX, fMaxY);

	return (ret == 0xFFFF) ? -1 : ret;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneDestroy)
{
	CHECK_PARAMS(1, "GangZoneDestroy");

	CGangZonePool *pGangZonePool = CServer::Get()->pGangZonePool;
	int zoneid = static_cast<int>(params[1]);
	if(!pGangZonePool || !pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	pGangZonePool->Delete(static_cast<WORD>(zoneid));
	return 1;
}

// native YSF_GangZoneShowForPlayer(playerid, zone, color);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneShowForPlayer)
{
	CHECK_PARAMS(3, "GangZoneShowForPlayer");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	DWORD color = (DWORD)params[3];

	// For security
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	return CServer::Get()->pGangZonePool->ShowForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(color));
}

// native YSF_GangZoneHideForPlayer(playerid, zone);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneHideForPlayer)
{
	CHECK_PARAMS(2, "GangZoneHideForPlayer");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);

	// For security
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid));
	return 1;
}

// native YSF_GangZoneShowForAll(zone, color);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneShowForAll)
{
	CHECK_PARAMS(2, "GangZoneShowForAll");

	int zoneid = static_cast<int>(params[1]);
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->ShowForAll(static_cast<WORD>(zoneid), static_cast<DWORD>(params[2]));
	return 1;
}

// native YSF_GangZoneHideForAll(zone);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneHideForAll)
{
	CHECK_PARAMS(1, "GangZoneHideForAll");

	int zoneid = static_cast<int>(params[1]);
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->HideForAll(static_cast<WORD>(zoneid));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneFlashForPlayer)
{
	CHECK_PARAMS(3, "GangZoneFlashForPlayer");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);

	// For security
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->FlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(params[3]));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneFlashForAll)
{
	CHECK_PARAMS(2, "GangZoneFlashForAll");

	int zoneid = static_cast<int>(params[1]);
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->FlashForAll(static_cast<WORD>(zoneid), static_cast<DWORD>(params[2]));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneStopFlashForPlayer)
{
	CHECK_PARAMS(2, "GangZoneStopFlashForPlayer");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);

	// For security
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->StopFlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneStopFlashForAll)
{
	CHECK_PARAMS(1, "GangZoneStopFlashForAll");

	int zoneid = static_cast<int>(params[1]);
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->StopFlashForAll(static_cast<WORD>(zoneid));
	return 1;
}

// Menu functions
// native IsMenuDisabled(Menu:menuid);
AMX_DECLARE_NATIVE(Natives::IsMenuDisabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsMenuDisabled");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;
	
	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return !!(!pMenu->interaction.Menu);
}

// native IsMenuRowDisabled(Menu:menuid, row);
AMX_DECLARE_NATIVE(Natives::IsMenuRowDisabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsMenuRowDisabled");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;
	
	int itemid = static_cast<int>(params[2]);
	if(itemid < 0 || itemid >= 12) return 0;

	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return !!(!pMenu->interaction.Row[itemid]);
}

// native GetMenuColumns(menuid);
AMX_DECLARE_NATIVE(Natives::GetMenuColumns)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetMenuColumns");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;
	
	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return pMenu->byteColumnsNumber;
}

// native GetMenuItems(menuid, column);
AMX_DECLARE_NATIVE(Natives::GetMenuItems)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetMenuItems");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;

	int column = static_cast<int>(params[2]);
	if(menuid < 0 || menuid > 2) return 0;

	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return pMenu->byteItemsCount[column];
}

// native GetMenuPos(menuid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::GetMenuPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "GetMenuColumns");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;

	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	Utility::storeVectorInNative(amx, params[2], pMenu->vecPos);
	return 1;
}

// native GetMenuColumnWidth(menuid, &Float:fColumn1, &Float:fColumn2);
AMX_DECLARE_NATIVE(Natives::GetMenuColumnWidth)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetMenuColumnWidth");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;

	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	Utility::storeFloatInNative(amx, params[2], pMenu->fColumn1Width);
	Utility::storeFloatInNative(amx, params[3], pMenu->fColumn2Width);
	return 1;
}

// native GetMenuColumnHeader(menuid, column, header[], len = sizeof(header));
AMX_DECLARE_NATIVE(Natives::GetMenuColumnHeader)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetMenuColumnHeader");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;

	int column = static_cast<int>(params[2]);
	if(menuid < 0 || menuid > 2) return 0;

	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return set_amxstring(amx, params[3], pMenu->szHeaders[column], params[4]);
}

// native GetMenuItem(menuid, column, itemid, item[], len = sizeof(item));
AMX_DECLARE_NATIVE(Natives::GetMenuItem)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetMenuItem");
	
	int menuid = static_cast<int>(params[1]);
	if(menuid < 1 || menuid >= MAX_MENUS) return 0;

	int column = static_cast<int>(params[2]);
	if(menuid < 0 || menuid > 2) return 0;

	int itemid = static_cast<int>(params[3]);
	if(itemid < 0 || itemid >= 12) return 0;

	if(!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return set_amxstring(amx, params[4], pMenu->szItems[itemid][column], params[5]);
}

// native CreatePlayerGangZone(playerid, Float:minx, Float:miny, Float:maxx, Float:maxy);
AMX_DECLARE_NATIVE(Natives::CreatePlayerGangZone)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "CreatePlayerGangZone");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	float fMinX = amx_ctof(params[2]);
	float fMinY = amx_ctof(params[3]);
	float fMaxX = amx_ctof(params[4]);
	float fMaxY = amx_ctof(params[5]);

	// If coordinates are wrong, then won't create bugged zone!
	if(fMaxX <= fMinX || fMaxY <= fMinY) 
	{
		logprintf("CreatePlayerGangZone: MaxX, MaxY must be bigger than MinX, MinY. Not inversely!");
		logprintf("CreatePlayerGangZone: %f, %f, %f, %f", fMinX, fMinY, fMaxX, fMaxY);
		return -1;
	}

	WORD ret = CServer::Get()->pGangZonePool->New(static_cast<WORD>(playerid), fMinX, fMinY, fMaxX, fMaxY);
	if (ret == 0xFFFF) return -1;

	return ret;
}

// native PlayerGangZoneShow(playerid, zoneid, color);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneShow)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "PlayerGangZoneShow");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	DWORD dwColor = static_cast<DWORD>(params[3]);

	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->ShowForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), dwColor, true);
	return 1;
}

// native PlayerGangZoneHide(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneHide)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerGangZoneHide");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);

	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
	return 1;
}

// native PlayerGangZoneFlash(playerid, zoneid, color);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneFlash)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "PlayerGangZoneFlash");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	DWORD dwColor = static_cast<DWORD>(params[3]);

	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->FlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), dwColor, true);
	return 1;
}

// native PlayerGangZoneStopFlash(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneStopFlash)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerGangZoneStopFlash");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;

	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->StopFlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
	return 1;
}

// native PlayerGangZoneDestroy(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneDestroy)
{
	if(!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerGangZoneDestroy");

	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
	return 1;
}

// native IsValidPlayerGangZone(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsValidPlayerGangZone)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsValidPlayerGangZone");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	return pPlayerData[playerid]->pPlayerZone[zoneid] != NULL;
}

// native IsPlayerInPlayerGangZone(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerInPlayerGangZone)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPlayerInPlayerGangZone");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->bInGangZone[id];
	}
	return 0;
}

// native PlayerGangZoneGetPos(playerid, zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneGetPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(6, "PlayerGangZoneGetPos");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	int zoneid = static_cast<int>(params[2]);
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;
	
	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if(id != 0xFFFF) 
	{
		CGangZone *pGangZone = pPlayerData[playerid]->pPlayerZone[zoneid];
		Utility::storeFloatInNative(amx, params[3], pGangZone->fGangZone[0]);
		Utility::storeFloatInNative(amx, params[4], pGangZone->fGangZone[1]);
		Utility::storeFloatInNative(amx, params[5], pGangZone->fGangZone[2]);
		Utility::storeFloatInNative(amx, params[6], pGangZone->fGangZone[3]);
		return 1;
	}
	return 0;
}

// native IsPlayerGangZoneVisible(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerGangZoneVisible)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPlayerInPlayerGangZone");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	return pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true) != 0xFFFF;
}

// native PlayerGangZoneGetColor(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneGetColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerGangZoneGetColor");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->dwClientSideZoneColor[id];
	}
	return 0;
}

// native PlayerGangZoneGetFlashColor(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneGetFlashColor)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "PlayerGangZoneGetFlashColor");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->dwClientSideZoneFlashColor[id];
	}
	return 0;
}

// native IsPlayerGangZoneFlashing(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerGangZoneFlashing)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPlayerGangZoneFlashing");
	
	int playerid = static_cast<int>(params[1]);
	int zoneid = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	
	if(!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if(id != 0xFFFF) 
	{
		return pPlayerData[playerid]->bIsGangZoneFlashing[id];
	}
	return 0;
}

#ifdef NEW_PICKUP_SYSTEM

// native IsValidPickup(pickupid);
AMX_DECLARE_NATIVE(Natives::IsValidPickup)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsValidPickup");

	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_PICKUPS) return 0;

	return CServer::Get()->pPickupPool->FindPickup(id) != 0;
}

// native IsPickupStreamedIn(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsPickupStreamedIn)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPickupStreamedIn");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;
	if(id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if(!pPickup) return 0;

	return CServer::Get()->pPickupPool->IsStreamed(playerid, pPickup);
}

// native GetPickupPos(pickupid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPickupPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetPickupPos");

	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if(!pPickup) return 0;

	Utility::storeVectorInNative(amx, params[2], pPickup->vecPos);
	return 1;
}

// native GetPickupModel(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupModel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPickupModel");

	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if(!pPickup) return 0;

	return pPickup->iModel;
}

// native GetPickupType(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupType)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPickupType");

	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if(!pPickup) return 0;

	return pPickup->iType;
}

// native GetPickupVirtualWorld(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupVirtualWorld)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPickupVirtualWorld");

	int id = static_cast<int>(params[1]);
	if(id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if(!pPickup) return 0;

	return pPickup->iWorld;
}

// CreatePlayerPickup(playerid, model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
AMX_DECLARE_NATIVE(Natives::CreatePlayerPickup)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(7, "CreatePlayerPickup");

	int playerid = static_cast<int>(params[1]);
	if(!IsPlayerConnected(playerid)) return 0;

	return CServer::Get()->pPickupPool->New(playerid, (int)params[2], (int)params[3], CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])), (int)params[7]);
}

// native DestroyPlayerPickup(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::DestroyPlayerPickup)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "DestroyPlayerPickup");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;	
	if(id < 0 || id >= MAX_PICKUPS) return 0;
	
	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if(!pPickup) return 0;

	CServer::Get()->pPickupPool->Destroy((WORD)playerid, id);
	return 1;
}

// native IsValidPlayerPickup(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsValidPlayerPickup)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsValidPlayerPickup");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;	
	if(id < 0 || id >= MAX_PICKUPS) return 0;

	return CServer::Get()->pPickupPool->FindPickup(playerid, id) != 0;
}

// native IsPlayerPickupStreamedIn(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsPlayerPickupStreamedIn)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPlayerPickupStreamedIn");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;	
	if(id < 0 || id >= MAX_PICKUPS) return 0;
	
	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if(!pPickup) return 0;

	return CServer::Get()->pPickupPool->IsStreamed(playerid, pPickup);
}

// native GetPlayerPickupPos(playerid, pickupid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(5, "GetPlayerPickupPos");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;	
	if(id < 0 || id >= MAX_PICKUPS) return 0;
	
	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if(!pPickup) return 0;

	Utility::storeVectorInNative(amx, params[3], pPickup->vecPos);
	return 1;
}

// native GetPlayerPickupModel(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupModel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerPickupModel");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;	
	if(id < 0 || id >= MAX_PICKUPS) return 0;
	
	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if(!pPickup) return 0;

	return pPickup->iModel;
}

// native GetPlayerPickupType(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupType)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerPickupType");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;	
	if(id < 0 || id >= MAX_PICKUPS) return 0;
	
	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if(!pPickup) return 0;

	return pPickup->iType;
}

// native GetPlayerPickupVirtualWorld(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupVirtualWorld)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "GetPlayerPickupVirtualWorld");

	int playerid = static_cast<int>(params[1]);
	int id = static_cast<int>(params[2]);
	if(!IsPlayerConnected(playerid)) return 0;	
	if(id < 0 || id >= MAX_PICKUPS) return 0;
	
	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if(!pPickup) return 0;

	return pPickup->iWorld;
}

#else

// native IsValidPickup(pickupid);
AMX_DECLARE_NATIVE(Natives::IsValidPickup)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsValidPickup");

	int id = static_cast<int>(params[1]);
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	return pNetGame->pPickupPool->bActive[id];
}

// native IsPickupStreamedIn(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsPickupStreamedIn)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "IsPickupStreamedIn");

	int playerid = static_cast<int>(params[1]);
	int pickupid = static_cast<int>(params[2]);
	if (!IsPlayerConnected(playerid)) return 0;
	if (pickupid < 0 || pickupid >= MAX_PICKUPS) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bPickupStreamedIn[pickupid];
}

// native GetPickupPos(pickupid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPickupPos)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(4, "GetPickupPos");

	int id = static_cast<int>(params[1]);
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	if (!pNetGame->pPickupPool->bActive[id]) return 0;

	Utility::storeVectorInNative(amx, params[2], pNetGame->pPickupPool->Pickup[id].vecPos);
	return 1;
}

// native GetPickupModel(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupModel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPickupModel");

	int id = static_cast<int>(params[1]);
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	if (!pNetGame->pPickupPool->bActive[id]) return 0;

	return pNetGame->pPickupPool->Pickup[id].iModel;
}

// native GetPickupType(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupType)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPickupType");

	int id = static_cast<int>(params[1]);
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	if (!pNetGame->pPickupPool->bActive[id]) return 0;

	return pNetGame->pPickupPool->Pickup[id].iType;
}

// native GetPickupVirtualWorld(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupVirtualWorld)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "GetPickupVirtualWorld");

	int id = static_cast<int>(params[1]);
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	if (!pNetGame->pPickupPool->bActive[id]) return 0;

	return pNetGame->pPickupPool->iWorld[id];
}

#endif
// RakServer functions //
// native ClearBanList();
AMX_DECLARE_NATIVE(Natives::ClearBanList)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	CSAMPFunctions::ClearBanList();
	CServer::Get()->ClearBans();
	return 1;
}

// native IsBanned(ipaddress[]);
AMX_DECLARE_NATIVE(Natives::IsBanned)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(1, "IsBanned");

	char *ip;
	amx_StrParam(amx, params[1], ip);
	return (ip) ? CServer::Get()->IsBanned(ip) : 0;
}

// native SetTimeoutTime(playerid, time);
AMX_DECLARE_NATIVE(Natives::SetTimeoutTime)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, "SetTimeoutTime");
	
	PlayerID playerId = CSAMPFunctions::GetPlayerIDFromIndex(static_cast<int>(params[1]));
	if(playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress || !IsPlayerConnected(static_cast<int>(params[1])))
		return 0;

	CSAMPFunctions::SetTimeoutTime(static_cast<RakNetTime>(params[2]), playerId);
	return 1;
}

// native GetLocalIP(index, localip[], len = sizeof(localip));
AMX_DECLARE_NATIVE(Natives::GetLocalIP)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(3, "GetLocalIP");

	return set_amxstring(amx, params[2], CSAMPFunctions::GetLocalIP(static_cast<unsigned int>(params[1])), params[3]);
}

// native SendRPC(playerid, RPC, {Float,_}:...)
AMX_DECLARE_NATIVE(Natives::SendRPC)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	bool bBroadcast = static_cast<int>(params[1]) == -1;
	BYTE rpcid = static_cast<BYTE>(params[2]);
	
	PlayerID playerId = bBroadcast ? UNASSIGNED_PLAYER_ID : CSAMPFunctions::GetPlayerIDFromIndex(static_cast<int>(params[1]));
	
	if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress && !bBroadcast)
		return 0;
	
	RakNet::BitStream bs;
	cell *type = (cell*)0;
	cell *data = (cell*)0;

	for (int i = 0; i < (int)((params[0]/sizeof(cell)) - 2); i+=2)
	{
		amx_GetAddr(amx, params[i + 3], &type);
		amx_GetAddr(amx, params[i + 4], &data);
					
		if (type && data)
		{
			switch (*type)
			{
			case BS_BOOL:
				bs.Write((bool)(*data!=0));
				break;
			case BS_CHAR:
				bs.Write(*(char*)data);
				break;
			case BS_UNSIGNEDCHAR:
				bs.Write(*(unsigned char*)data);
				break;
			case BS_SHORT:
				bs.Write(*(short*)data);
				break;
			case BS_UNSIGNEDSHORT:
				bs.Write(*(unsigned short*)data);
				break;
			case BS_INT:
				bs.Write(*(int*)data);
				break;
			case BS_UNSIGNEDINT:
				bs.Write(*(unsigned int*)data);
				break;
			case BS_FLOAT:
				bs.Write(*(float*)data);
				break;
			case BS_STRING:
				{
					int len;
					amx_StrLen(data, &len);
					len++;
					char* str = new char[len];
					amx_GetString(str, data, 0, len);
					bs.Write(str, len - 1);
					//logprintf("str: %s", str);
					delete [] str;
				}
				break;
			}
		}
	}

	if(bBroadcast)
	{
		CSAMPFunctions::RPC(&rpcid, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, 0);
	}
	else
	{
		CSAMPFunctions::RPC(&rpcid, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, playerId, 0, 0);
	}
	return 1;
}

// native SendData(playerid, {Float,_}:...)
AMX_DECLARE_NATIVE(Natives::SendData)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	bool bBroadcast = static_cast<int>(params[1]) == -1;
	PlayerID playerId = bBroadcast ? UNASSIGNED_PLAYER_ID : CSAMPFunctions::GetPlayerIDFromIndex(static_cast<int>(params[1]));

	if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress && !bBroadcast)
		return 0;
	
	RakNet::BitStream bs;
	cell *type = (cell*)0;
	cell *data = (cell*)0;

	for (int i = 0; i < (int)((params[0]/sizeof(cell)) - 2); i+=2)
	{
		amx_GetAddr(amx, params[i + 2], &type);
		amx_GetAddr(amx, params[i + 3], &data);
					
		if (type && data)
		{
			switch (*type)
			{
			case BS_BOOL:
				bs.Write((bool)(*data!=0));
				break;
			case BS_CHAR:
				bs.Write(*(char*)data);
				break;
			case BS_UNSIGNEDCHAR:
				bs.Write(*(unsigned char*)data);
				break;
			case BS_SHORT:
				bs.Write(*(short*)data);
				break;
			case BS_UNSIGNEDSHORT:
				bs.Write(*(unsigned short*)data);
				break;
			case BS_INT:
				bs.Write(*(int*)data);
				break;
			case BS_UNSIGNEDINT:
				bs.Write(*(unsigned int*)data);
				break;
			case BS_FLOAT:
				bs.Write(*(float*)data);
				break;
			case BS_STRING:
				{
					int len;
					amx_StrLen(data, &len);
					len++;
					char* str = new char[len];
					amx_GetString(str, data, 0, len);
					bs.Write(str, len - 1);
					//logprintf("str: %s", str);
					delete [] str;
				}
				break;
			}
		}
	}

	if(bBroadcast)
	{
		CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
	}
	else
	{
		CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, playerId, 0);
	}
	return 1;
}

// native GetColCount();
AMX_DECLARE_NATIVE(Natives::GetColCount)
{
	return CModelSizes::GetColCount();
}

// native Float:GetColSphereRadius(modelid);
AMX_DECLARE_NATIVE(Natives::GetColSphereRadius)
{
	CHECK_PARAMS(1, "GetColSphereRadius");
	
	float fRet = CModelSizes::GetColSphereRadius(static_cast<int>(params[1]));
	return amx_ftoc(fRet);
}

// native GetColSphereOffset(modelid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetColSphereOffset)
{
	CHECK_PARAMS(4, "GetColSphereOffset");

	CVector vecOffset = CModelSizes::GetColSphereOffset(static_cast<int>(params[1]));

	Utility::storeVectorInNative(amx, params[2], vecOffset);
	return 1;
}

// native GetWeaponSlot(weaponid);
AMX_DECLARE_NATIVE(Natives::GetWeaponSlot)
{
	CHECK_PARAMS(1, "GetWeaponSlot");
	
	return Utility::GetWeaponSlot(static_cast<BYTE>(params[1]));
}

// native GetWeaponName(weaponid, weaponname[], len = sizeof(weaponname));
AMX_DECLARE_NATIVE(Natives::FIXED_GetWeaponName)
{
	CHECK_PARAMS(3, "GetWeaponName");

	return set_amxstring(amx, params[2], Utility::GetWeaponName(static_cast<BYTE>(params[1])), params[3]);
}

// native IsPlayerConnected(playerid);
AMX_DECLARE_NATIVE(Natives::FIXED_IsPlayerConnected)
{
	CHECK_PARAMS(1, "IsPlayerConnected");

	int playerid = static_cast<int>(params[1]);
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid] != NULL;
}

#ifdef NEW_PICKUP_SYSTEM
// native CreatePickup(model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
AMX_DECLARE_NATIVE(Natives::CreatePickup)
{
	CHECK_PARAMS(6, "CreatePickup");

	return CServer::Get()->pPickupPool->New((int)params[1], (int)params[2], CVector(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5])), (int)params[6]);
}

// native DestroyPickup(pickupid);
AMX_DECLARE_NATIVE(Natives::DestroyPickup)
{
	CHECK_PARAMS(1, "DestroyPickup");

	CServer::Get()->pPickupPool->Destroy((int)params[1]);
	return 1;
}

// native SetPickupStreamingEnabled(enabled);
AMX_DECLARE_NATIVE(Natives::SetPickupStreamingEnabled)
{
	CHECK_PARAMS(1, "SetPickupStreamingEnabled");

	CServer::Get()->pPickupPool->SetStreamingEnabled(!!params[1]);
	return 1;
}
#endif

// And an array containing the native function-names and the functions specified with them
AMX_NATIVE_INFO YSINatives [] =
{
	// File
	AMX_DEFINE_NATIVE(ffind)
	AMX_DEFINE_NATIVE(frename)
	
	// Directory
	AMX_DEFINE_NATIVE(dfind)
	AMX_DEFINE_NATIVE(dcreate)
	AMX_DEFINE_NATIVE(drename)

	// Generic
	AMX_DEFINE_NATIVE(SetModeRestartTime)
	AMX_DEFINE_NATIVE(GetModeRestartTime)
	AMX_DEFINE_NATIVE(SetMaxPlayers) // R8
	AMX_DEFINE_NATIVE(SetMaxNPCs) // R8

	AMX_DEFINE_NATIVE(SetPlayerAdmin)
	AMX_DEFINE_NATIVE(LoadFilterScript)
	AMX_DEFINE_NATIVE(UnLoadFilterScript)
	AMX_DEFINE_NATIVE(GetFilterScriptCount)
	AMX_DEFINE_NATIVE(GetFilterScriptName)

	AMX_DEFINE_NATIVE(AddServerRule)
	AMX_DEFINE_NATIVE(SetServerRule)
	AMX_DEFINE_NATIVE(SetServerRuleInt)
	AMX_DEFINE_NATIVE(IsValidServerRule)
	AMX_DEFINE_NATIVE(RemoveServerRule) // Doesn't work!
	AMX_DEFINE_NATIVE(SetServerRuleFlags)
	AMX_DEFINE_NATIVE(GetServerRuleFlags)

	// Server settings
	AMX_DEFINE_NATIVE(GetServerSettings)

	// Nick name
	AMX_DEFINE_NATIVE(IsValidNickName)	// R8
	AMX_DEFINE_NATIVE(AllowNickNameCharacter) // R7
	AMX_DEFINE_NATIVE(IsNickNameCharacterAllowed) // R7

	// Player classes
	AMX_DEFINE_NATIVE(GetAvailableClasses) // R6
	AMX_DEFINE_NATIVE(RemoveLastClass) // R16
	AMX_DEFINE_NATIVE(GetPlayerClass) // R6
	AMX_DEFINE_NATIVE(EditPlayerClass) // R6
	
	// Timers
	AMX_DEFINE_NATIVE(GetRunningTimers) // R8

	// Special
	AMX_DEFINE_NATIVE(SetPlayerGravity)
	AMX_DEFINE_NATIVE(GetPlayerGravity)
	
	AMX_DEFINE_NATIVE(SetPlayerTeamForPlayer) // R5
	AMX_DEFINE_NATIVE(GetPlayerTeamForPlayer) // R5
	AMX_DEFINE_NATIVE(SetPlayerSkinForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerSkinForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerNameForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerNameForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerFightStyleForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerFightStyleForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerPosForPlayer) // R12
	AMX_DEFINE_NATIVE(SetPlayerRotationQuatForPlayer) // R12
	AMX_DEFINE_NATIVE(ApplyAnimationForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerWeather)
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds)
	AMX_DEFINE_NATIVE(TogglePlayerWidescreen)
	AMX_DEFINE_NATIVE(IsPlayerWidescreenToggled)
	AMX_DEFINE_NATIVE(GetSpawnInfo) // R8
	AMX_DEFINE_NATIVE(GetPlayerSkillLevel) // R3
	AMX_DEFINE_NATIVE(IsPlayerCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerCheckpoint) // R4
	AMX_DEFINE_NATIVE(IsPlayerRaceCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerRaceCheckpoint) // R4
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds) // R5
	AMX_DEFINE_NATIVE(IsPlayerInModShop) // R4
	AMX_DEFINE_NATIVE(SendBulletData) // R6
	AMX_DEFINE_NATIVE(ShowPlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(HidePlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(AddPlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(RemovePlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(SetPlayerChatBubbleForPlayer) // R10
	AMX_DEFINE_NATIVE(SetPlayerVersion) // R9
	AMX_DEFINE_NATIVE(IsPlayerSpawned) // R9
	AMX_DEFINE_NATIVE(IsPlayerControllable) // R11
	AMX_DEFINE_NATIVE(SpawnForWorld) // R10
	AMX_DEFINE_NATIVE(BroadcastDeath) // R13
	AMX_DEFINE_NATIVE(IsPlayerCameraTargetEnabled) // R13
	AMX_DEFINE_NATIVE(SetPlayerDisabledKeysSync) // R16
	AMX_DEFINE_NATIVE(GetPlayerDisabledKeysSync) // R16
	
	// Special things from syncdata
	AMX_DEFINE_NATIVE(GetPlayerSirenState)
	AMX_DEFINE_NATIVE(GetPlayerLandingGearState)
	AMX_DEFINE_NATIVE(GetPlayerHydraReactorAngle)
	AMX_DEFINE_NATIVE(GetPlayerTrainSpeed)
	AMX_DEFINE_NATIVE(GetPlayerZAim)
	AMX_DEFINE_NATIVE(GetPlayerSurfingOffsets)
	AMX_DEFINE_NATIVE(GetPlayerRotationQuat) // R3
	AMX_DEFINE_NATIVE(GetPlayerDialogID) // R8
	AMX_DEFINE_NATIVE(GetPlayerSpectateID) // R8
	AMX_DEFINE_NATIVE(GetPlayerSpectateType) // R8
	AMX_DEFINE_NATIVE(GetPlayerLastSyncedVehicleID) // R10
	AMX_DEFINE_NATIVE(GetPlayerLastSyncedTrailerID) // R10

	// Actor
	AMX_DEFINE_NATIVE(GetActorSpawnInfo) // R13
	AMX_DEFINE_NATIVE(GetActorSkin) // R13
	AMX_DEFINE_NATIVE(GetActorAnimation) // R17

	// Scoreboard manipulation
	AMX_DEFINE_NATIVE(TogglePlayerScoresPingsUpdate) // R8
	AMX_DEFINE_NATIVE(TogglePlayerFakePing) // R8
	AMX_DEFINE_NATIVE(SetPlayerFakePing) // R8
	AMX_DEFINE_NATIVE(TogglePlayerInServerQuery) // R11
	AMX_DEFINE_NATIVE(IsPlayerToggledInServerQuery) // R11
	AMX_DEFINE_NATIVE(TogglePlayerInServerQuery) // R11
	AMX_DEFINE_NATIVE(IsPlayerToggledInServerQuery) // R11

	// AFK
	AMX_DEFINE_NATIVE(IsPlayerPaused)
	AMX_DEFINE_NATIVE(GetPlayerPausedTime)
	
	// Objects get - global
	AMX_DEFINE_NATIVE(GetObjectDrawDistance)
	AMX_DEFINE_NATIVE(SetObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetObjectTarget) // R6
	AMX_DEFINE_NATIVE(GetObjectAttachedData)
	AMX_DEFINE_NATIVE(GetObjectAttachedOffset)
	AMX_DEFINE_NATIVE(IsObjectMaterialSlotUsed) // R6
	AMX_DEFINE_NATIVE(GetObjectMaterial) // R6
	AMX_DEFINE_NATIVE(GetObjectMaterialText) // R6
	AMX_DEFINE_NATIVE(IsObjectNoCameraCol) // R13

	// Objects get - player
	AMX_DEFINE_NATIVE(GetPlayerObjectDrawDistance)
	AMX_DEFINE_NATIVE(SetPlayerObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectTarget) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectAttachedData)
	AMX_DEFINE_NATIVE(GetPlayerObjectAttachedOffset)
	AMX_DEFINE_NATIVE(IsPlayerObjectMaterialSlotUsed) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectMaterial) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectMaterialText) // R6
	AMX_DEFINE_NATIVE(IsPlayerObjectNoCameraCol) // R13
	AMX_DEFINE_NATIVE(GetPlayerSurfingPlayerObjectID) // R12
	AMX_DEFINE_NATIVE(GetPlayerCameraTargetPlayerObj) // R13
	AMX_DEFINE_NATIVE(GetObjectType)// R12

	// special - for attached objects
	AMX_DEFINE_NATIVE(GetPlayerAttachedObject) // R3
	
	// Vehicle functions
	AMX_DEFINE_NATIVE(GetVehicleSpawnInfo)
	AMX_DEFINE_NATIVE(SetVehicleSpawnInfo) // R16
	AMX_DEFINE_NATIVE(GetVehicleColor)
	AMX_DEFINE_NATIVE(GetVehiclePaintjob)
	AMX_DEFINE_NATIVE(GetVehicleInterior)
	AMX_DEFINE_NATIVE(GetVehicleNumberPlate)
	AMX_DEFINE_NATIVE(SetVehicleRespawnDelay)
	AMX_DEFINE_NATIVE(GetVehicleRespawnDelay)
	AMX_DEFINE_NATIVE(SetVehicleOccupiedTick) // R6
	AMX_DEFINE_NATIVE(GetVehicleOccupiedTick)
	AMX_DEFINE_NATIVE(SetVehicleRespawnTick)
	AMX_DEFINE_NATIVE(GetVehicleRespawnTick)
	AMX_DEFINE_NATIVE(GetVehicleLastDriver)
	AMX_DEFINE_NATIVE(GetVehicleCab) // R9
	AMX_DEFINE_NATIVE(HasVehicleBeenOccupied) // R9
	AMX_DEFINE_NATIVE(SetVehicleBeenOccupied) // R9
	AMX_DEFINE_NATIVE(IsVehicleOccupied) // R9
	AMX_DEFINE_NATIVE(IsVehicleDead) // R9
	AMX_DEFINE_NATIVE(GetVehicleModelCount) // R17
	AMX_DEFINE_NATIVE(GetVehicleModelsUsed) // R17

	// Gangzone - Global
	AMX_DEFINE_NATIVE(IsValidGangZone)
	AMX_DEFINE_NATIVE(IsPlayerInGangZone)
	AMX_DEFINE_NATIVE(IsGangZoneVisibleForPlayer)
	AMX_DEFINE_NATIVE(GangZoneGetColorForPlayer)
	AMX_DEFINE_NATIVE(GangZoneGetFlashColorForPlayer)
	AMX_DEFINE_NATIVE(IsGangZoneFlashingForPlayer) // R6
	AMX_DEFINE_NATIVE(GangZoneGetPos)

	// Gangzone - Player
	AMX_DEFINE_NATIVE(CreatePlayerGangZone)
	AMX_DEFINE_NATIVE(PlayerGangZoneDestroy)
	AMX_DEFINE_NATIVE(PlayerGangZoneShow)
	AMX_DEFINE_NATIVE(PlayerGangZoneHide)
	AMX_DEFINE_NATIVE(PlayerGangZoneFlash)
	AMX_DEFINE_NATIVE(PlayerGangZoneStopFlash)
	AMX_DEFINE_NATIVE(IsValidPlayerGangZone)
	AMX_DEFINE_NATIVE(IsPlayerInPlayerGangZone)
	AMX_DEFINE_NATIVE(IsPlayerGangZoneVisible)
	AMX_DEFINE_NATIVE(PlayerGangZoneGetColor)
	AMX_DEFINE_NATIVE(PlayerGangZoneGetFlashColor)
	AMX_DEFINE_NATIVE(IsPlayerGangZoneFlashing) // R6
	AMX_DEFINE_NATIVE(PlayerGangZoneGetPos)

	// Textdraw functions
	AMX_DEFINE_NATIVE(IsValidTextDraw)
	AMX_DEFINE_NATIVE(IsTextDrawVisibleForPlayer)
	AMX_DEFINE_NATIVE(TextDrawGetString)
	AMX_DEFINE_NATIVE(TextDrawSetPos)
	AMX_DEFINE_NATIVE(TextDrawGetLetterSize)
	AMX_DEFINE_NATIVE(TextDrawGetTextSize)
	AMX_DEFINE_NATIVE(TextDrawGetPos)
	AMX_DEFINE_NATIVE(TextDrawGetColor)
	AMX_DEFINE_NATIVE(TextDrawGetBoxColor)
	AMX_DEFINE_NATIVE(TextDrawGetBackgroundColor)
	AMX_DEFINE_NATIVE(TextDrawGetShadow)
	AMX_DEFINE_NATIVE(TextDrawGetOutline)
	AMX_DEFINE_NATIVE(TextDrawGetFont)
	AMX_DEFINE_NATIVE(TextDrawIsBox)
	AMX_DEFINE_NATIVE(TextDrawIsProportional)
	AMX_DEFINE_NATIVE(TextDrawIsSelectable) // R6
	AMX_DEFINE_NATIVE(TextDrawGetAlignment)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewModel)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewRot)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewVehCol)

	// Per-Player Textdraw functions - R4
	AMX_DEFINE_NATIVE(IsValidPlayerTextDraw)
	AMX_DEFINE_NATIVE(IsPlayerTextDrawVisible)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetString)
	AMX_DEFINE_NATIVE(PlayerTextDrawSetPos)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetLetterSize)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetTextSize)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPos)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetColor)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetBoxColor)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetBackgroundCol)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetShadow)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetOutline)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetFont)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsBox)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsProportional)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsSelectable) // R6
	AMX_DEFINE_NATIVE(PlayerTextDrawGetAlignment)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewModel)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewRot)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewVehCol)

	// 3D Text
	AMX_DEFINE_NATIVE(IsValid3DTextLabel) // R4
	AMX_DEFINE_NATIVE(Is3DTextLabelStreamedIn) // R9
	AMX_DEFINE_NATIVE(Get3DTextLabelText)
	AMX_DEFINE_NATIVE(Get3DTextLabelColor)
	AMX_DEFINE_NATIVE(Get3DTextLabelPos)
	AMX_DEFINE_NATIVE(Get3DTextLabelDrawDistance)
	AMX_DEFINE_NATIVE(Get3DTextLabelLOS)
	AMX_DEFINE_NATIVE(Get3DTextLabelVirtualWorld)
	AMX_DEFINE_NATIVE(Get3DTextLabelAttachedData)

	// Per-Player 3D Text
	AMX_DEFINE_NATIVE(IsValidPlayer3DTextLabel) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelText) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelColor) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelPos) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelDrawDist)
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelLOS) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelVirtualW) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelAttached) // R9

	// Menus
	AMX_DEFINE_NATIVE(IsMenuDisabled) // R5 
	AMX_DEFINE_NATIVE(IsMenuRowDisabled) // R5
	AMX_DEFINE_NATIVE(GetMenuColumns)
	AMX_DEFINE_NATIVE(GetMenuItems)
	AMX_DEFINE_NATIVE(GetMenuPos)
	AMX_DEFINE_NATIVE(GetMenuColumnWidth)
	AMX_DEFINE_NATIVE(GetMenuColumnHeader)
	AMX_DEFINE_NATIVE(GetMenuItem)
	
	// Pickups - Global
	AMX_DEFINE_NATIVE(IsValidPickup) // R10
	AMX_DEFINE_NATIVE(IsPickupStreamedIn) // R10
	AMX_DEFINE_NATIVE(GetPickupPos) // R10
	AMX_DEFINE_NATIVE(GetPickupModel) // R10
	AMX_DEFINE_NATIVE(GetPickupType) // R10
	AMX_DEFINE_NATIVE(GetPickupVirtualWorld) // R10
#ifdef NEW_PICKUP_SYSTEM
	// Pickups - Per-player
	AMX_DEFINE_NATIVE(CreatePlayerPickup) // R10
	AMX_DEFINE_NATIVE(DestroyPlayerPickup) // R10
	AMX_DEFINE_NATIVE(IsValidPlayerPickup) // R10
	AMX_DEFINE_NATIVE(IsPlayerPickupStreamedIn) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupPos) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupModel) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupType) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupVirtualWorld) // R10
#endif
	// RakServer functions
	AMX_DEFINE_NATIVE(ClearBanList)
	AMX_DEFINE_NATIVE(IsBanned)

	AMX_DEFINE_NATIVE(SetTimeoutTime)
	AMX_DEFINE_NATIVE(GetLocalIP)
	
	AMX_DEFINE_NATIVE(SendRPC)
	AMX_DEFINE_NATIVE(SendData)
	AMX_DEFINE_NATIVE(YSF_SetTickRate)
	AMX_DEFINE_NATIVE(YSF_GetTickRate)
	AMX_DEFINE_NATIVE(YSF_EnableNightVisionFix)
	AMX_DEFINE_NATIVE(YSF_IsNightVisionFixEnabled)
	AMX_DEFINE_NATIVE(YSF_SetExtendedNetStatsEnabled) // R17
	AMX_DEFINE_NATIVE(YSF_IsExtendedNetStatsEnabled) // R17
	AMX_DEFINE_NATIVE(YSF_SetAFKAccuracy) // R17
	AMX_DEFINE_NATIVE(YSF_GetAFKAccuracy) // R17 

	AMX_DEFINE_NATIVE(AttachPlayerObjectToObject)
	
	// Recording functions
	AMX_DEFINE_NATIVE(SetRecordingDirectory) // R17
	AMX_DEFINE_NATIVE(GetRecordingDirectory) // R17

	// Format functions
	AMX_DEFINE_NATIVE(SendClientMessagef)
	AMX_DEFINE_NATIVE(SendClientMessageToAllf)
	AMX_DEFINE_NATIVE(GameTextForPlayerf)
	AMX_DEFINE_NATIVE(GameTextForAllf)
	AMX_DEFINE_NATIVE(SendPlayerMessageToPlayerf)
	AMX_DEFINE_NATIVE(SendPlayerMessageToAllf)
	AMX_DEFINE_NATIVE(SendRconCommandf)

	// Other
	AMX_DEFINE_NATIVE(GetColCount)
	AMX_DEFINE_NATIVE(GetColSphereRadius)
	AMX_DEFINE_NATIVE(GetColSphereOffset)

	AMX_DEFINE_NATIVE(GetWeaponSlot)
#ifdef NEW_PICKUP_SYSTEM
	AMX_DEFINE_NATIVE(SetPickupStreamingEnabled)
#endif
	{ NULL,								NULL }
};

AMX_NATIVE_INFO RedirectedNatives[] =
{
	// File
	{ "AttachObjectToPlayer",			Natives::YSF_AttachObjectToPlayer },
	{ "AttachPlayerObjectToPlayer",		Natives::YSF_AttachPlayerObjectToPlayer },
	{ "GetGravity",						Natives::YSF_GetGravity },
	{ "SetPlayerWeather",				Natives::YSF_SetPlayerWeather },
	{ "SetPlayerWorldBounds",			Natives::YSF_SetPlayerWorldBounds },
	{ "DestroyObject",					Natives::YSF_DestroyObject },
	{ "DestroyPlayerObject",			Natives::YSF_DestroyPlayerObject },
	{ "TogglePlayerControllable",		Natives::YSF_TogglePlayerControllable},
	{ "ChangeVehicleColor",				Natives::YSF_ChangeVehicleColor},
	{ "DestroyVehicle",					Natives::YSF_DestroyVehicle},
	{ "ShowPlayerDialog",				Natives::YSF_ShowPlayerDialog },

	{ "GangZoneCreate",					Natives::YSF_GangZoneCreate },
	{ "GangZoneDestroy",				Natives::YSF_GangZoneDestroy },
	{ "GangZoneShowForPlayer",			Natives::YSF_GangZoneShowForPlayer },
	{ "GangZoneHideForPlayer",			Natives::YSF_GangZoneHideForPlayer },
	{ "GangZoneShowForAll",				Natives::YSF_GangZoneShowForAll },
	{ "GangZoneHideForAll",				Natives::YSF_GangZoneHideForAll },
								
	{ "GangZoneFlashForPlayer",			Natives::YSF_GangZoneFlashForPlayer },
	{ "GangZoneFlashForAll",			Natives::YSF_GangZoneFlashForAll },
	{ "GangZoneStopFlashForPlayer",		Natives::YSF_GangZoneStopFlashForPlayer },
	{ "GangZoneStopFlashForAll",		Natives::YSF_GangZoneStopFlashForAll },
#ifdef NEW_PICKUP_SYSTEM
	{ "CreatePickup",					Natives::CreatePickup },
	{ "AddStaticPickup",				Natives::CreatePickup },
	{ "DestroyPickup",					Natives::DestroyPickup },
#endif
	{ "GetWeaponName",					Natives::FIXED_GetWeaponName },
	{ "IsPlayerConnected",			Natives::FIXED_IsPlayerConnected },

	{ "SetPlayerTeam",					Natives::YSF_SetPlayerTeam },
	{ "SetPlayerSkin",					Natives::YSF_SetPlayerSkin },
	{ "SetPlayerName",					Natives::YSF_SetPlayerName },
	{ "SetPlayerFightingStyle",			Natives::YSF_SetPlayerFightingStyle },
	{ NULL,								NULL }
};

int InitScripting(AMX *amx)
{
	return amx_Register(amx, YSINatives, -1);
}