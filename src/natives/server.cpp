#include "../main.h"
#include "../Natives.h"

// native SetModeRestartTime(Float:time);
AMX_DECLARE_NATIVE(Natives::SetModeRestartTime)
{
	CHECK_PARAMS(1, LOADED);
	if (!CAddress::VAR_pRestartWaitTime) return 0;

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
	CHECK_PARAMS(1, LOADED);

	const int maxplayers = CScriptParams::Get()->ReadInt();
	if (maxplayers < 1 || maxplayers > MAX_PLAYERS) return 0;

	CSAMPFunctions::SetIntVariable("maxplayers", maxplayers);
	return 1;
}

// native SetMaxNPCs(maxnpcs);
AMX_DECLARE_NATIVE(Natives::SetMaxNPCs)
{
	CHECK_PARAMS(1, LOADED);

	const int maxnpcs = CScriptParams::Get()->ReadInt();
	if (maxnpcs < 0 || maxnpcs > MAX_PLAYERS) return 0;

	CSAMPFunctions::SetIntVariable("maxnpc", maxnpcs);
	return 1;
}

// native GetSyncBounds(&Float:hmin, &Float:hmax, &Float:vmin, &Float:vmax);
AMX_DECLARE_NATIVE(Natives::GetSyncBounds)
{
	CHECK_PARAMS(4, LOADED);

	float fBounds[4];
	for (BYTE i = 0; i != 4; ++i)
		fBounds[i] = *(float*)CAddress::VAR_pPosSyncBounds[i];

	CScriptParams::Get()->Add(fBounds[0], fBounds[1], fBounds[2], fBounds[3]);
	return 1;
}

// native SetSyncBounds(Float:hmin, Float:hmax, Float:vmin, Float:vmax);
AMX_DECLARE_NATIVE(Natives::SetSyncBounds)
{
	CHECK_PARAMS(4, LOADED);

	for (BYTE i = 0; i != 4; ++i)
		*(float*)CAddress::VAR_pPosSyncBounds[i] = CScriptParams::Get()->ReadFloat();
	return 1;
}

// native LoadFilterScript(scriptname[]);
AMX_DECLARE_NATIVE(Natives::LoadFilterScript)
{
	CHECK_PARAMS(1, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);
	if (!name.empty())
	{
		return CSAMPFunctions::LoadFilterscript(name.c_str());
	}
	return 0;
}

// UnLoadFilterScript(scriptname[]);
AMX_DECLARE_NATIVE(Natives::UnLoadFilterScript)
{
	CHECK_PARAMS(1, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);
	if (!name.empty())
	{
		return CSAMPFunctions::UnLoadFilterscript(name.c_str());
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
	CHECK_PARAMS(3, LOADED);

	int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_FILTER_SCRIPTS) return 0;

	CScriptParams::Get()->Add(&pNetGame->pFilterScriptPool->szFilterScriptName[id][0]);
	return 1;
}

// native AddServerRule(name[], value[], flags = CON_VARFLAG_RULE);
AMX_DECLARE_NATIVE(Natives::AddServerRule)
{
	CHECK_PARAMS(3, LOADED);

	std::string name, value;
	CScriptParams::Get()->Read(&name, &value);

	if (!name.empty() && !value.empty())
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(const_cast<char*>(name.c_str()));
		if (ConVar == NULL)
		{
			CSAMPFunctions::AddStringVariable(const_cast<char*>(name.c_str()), CScriptParams::Get()->ReadInt(), const_cast<char*>(value.c_str()), NULL);
			return 1;
		}
	}
	return 0;
}

// native SetServerRule(name[], value[]);
AMX_DECLARE_NATIVE(Natives::SetServerRule)
{
	CHECK_PARAMS(2, LOADED);

	std::string name, value;
	CScriptParams::Get()->Read(&name, &value);
	if (!name.empty() && !value.empty())
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(const_cast<char*>(name.c_str()));
		if (ConVar != NULL)
		{
			CSAMPFunctions::SetStringVariable(const_cast<char*>(name.c_str()), const_cast<char*>(value.c_str()));
			return 1;
		}
	}
	return 0;
}

// native SetServerRuleInt(name[], value);
AMX_DECLARE_NATIVE(Natives::SetServerRuleInt)
{
	CHECK_PARAMS(2, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);
	if (!name.empty())
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(const_cast<char*>(name.c_str()));
		if (ConVar != NULL)
		{
			CSAMPFunctions::SetIntVariable(const_cast<char*>(name.c_str()), CScriptParams::Get()->ReadInt());
			return 1;
		}
		return 1;
	}
	return 0;
}

// native IsValidServerRule(name[]);
AMX_DECLARE_NATIVE(Natives::IsValidServerRule)
{
	CHECK_PARAMS(1, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);
	if (!name.empty())
	{
		ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(const_cast<char*>(name.c_str()));
		return ConVar != NULL;
	}
	return 0;
}

// native RemoveServerRule(name[]);
AMX_DECLARE_NATIVE(Natives::RemoveServerRule)
{
	CHECK_PARAMS(1, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);
	if (!name.empty())
	{
		//RemoveServerRule(name.c_str());
		return 1;
	}
	return 0;
}

// native SetServerRuleFlags(name[], flags);
AMX_DECLARE_NATIVE(Natives::SetServerRuleFlags)
{
	CHECK_PARAMS(2, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);
	if (!name.empty())
	{
		CSAMPFunctions::ModifyVariableFlags(const_cast<char*>(name.c_str()), (DWORD)params[2]);
		return 1;
	}
	return 0;
}

// native GetServerRuleFlags(name[]);
AMX_DECLARE_NATIVE(Natives::GetServerRuleFlags)
{
	CHECK_PARAMS(1, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);

	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(const_cast<char*>(name.c_str()));
	if (ConVar != NULL)
	{
		return ConVar->VarFlags;
	}
	return 0;
}

// native GetServerSettings(&showplayermarkes, &shownametags, &stuntbonus, &useplayerpedanims, &bLimitchatradius, &disableinteriorenterexits, &nametaglos, &manualvehicleengine, 
//		&limitplayermarkers, &vehiclefriendlyfire, &defaultcameracollision, &Float:fGlobalchatradius, &Float:fNameTagDrawDistance, &Float:fPlayermarkerslimit);
AMX_DECLARE_NATIVE(Natives::GetServerSettings)
{
	CHECK_PARAMS(14, LOADED);

	CScriptParams::Get()->Add(
		pNetGame->bShowPlayerMarkers
		, pNetGame->byteShowNameTags
		, pNetGame->byteStuntBonus
		, pNetGame->bUseCJWalk
		, pNetGame->bLimitGlobalChatRadius
		, pNetGame->byteDisableEnterExits
		, pNetGame->byteNameTagLOS
		, pNetGame->bManulVehicleEngineAndLights
		, pNetGame->bLimitPlayerMarkers
		, pNetGame->bVehicleFriendlyFire
		, pNetGame->byteDefaultCameraCollision
		, pNetGame->fGlobalChatRadius
		, pNetGame->fNameTagDrawDistance
		, pNetGame->fPlayerMarkesLimit
	);
	return 1;
}

// native GetNPCCommandLine(npcid, npcscript[], length = sizeof(npcscript));
AMX_DECLARE_NATIVE(Natives::GetNPCCommandLine)
{
	CHECK_PARAMS(3, LOADED);

	const int npcid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(npcid)) return 0;

	char *szCommandLine = CServer::Get()->GetNPCCommandLine(static_cast<WORD>(npcid));
	if (szCommandLine == NULL) return 0;

	CScriptParams::Get()->Add(&szCommandLine[0]);
	return 1;
}

// native GetRCONCommandName(const cmdname[], changedname[]);
AMX_DECLARE_NATIVE(Natives::ChangeRCONCommandName)
{
	CHECK_PARAMS(2, LOADED);

	std::string name, newname;
	CScriptParams::Get()->Read(&name, &newname);

	return CServer::Get()->ChangeRCONCommandName(name, newname);
}

// native GetRCONCommandName(const cmdname[], changedname[], len = sizeof(changedname));
AMX_DECLARE_NATIVE(Natives::GetRCONCommandName)
{
	CHECK_PARAMS(3, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);

	std::string changedname;
	bool ret = CServer::Get()->GetRCONCommandName(name, changedname);
	CScriptParams::Get()->Add(changedname);
	return ret;
}

// This function based on maddinat0r's function - Thanks (MySQL plugin/CCallback.cpp)
// native CallFunctionInScript(const scriptname[], const function[], const format[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::CallFunctionInScript)
{
	if (CScriptParams::Get()->Setup(3, "CallFunctionInScript", CScriptParams::Flags::MORE_PARAMETER_ALLOWED, amx, params)) return CScriptParams::Get()->HandleError();

	std::string scriptname, function, formatparams;
	CScriptParams::Get()->Read(&scriptname, &function, &formatparams);

	AMX* pAMX = nullptr;
	if (scriptname == "GameMode")
	{
		pAMX = &pNetGame->pGameModePool->amx;
	}
	else
	{
		for (BYTE i = 0; i != 16; ++i)
		{
			if (scriptname == pNetGame->pFilterScriptPool->szFilterScriptName[i])
			{
				pAMX = pNetGame->pFilterScriptPool->pFilterScripts[i];
				break;
			}
		}
	}

	if (pAMX == nullptr)
	{
		logprintf("script \"%s\" does not exist", scriptname.c_str());
		return 0;
	}

	int cb_idx = -1;
	if (amx_FindPublic(pAMX, function.c_str(), &cb_idx) != AMX_ERR_NONE)
	{
		logprintf("callback \"%s\" does not exist", function.c_str());
		return 0;
	}

	size_t len = formatparams.length();
	char* format = new char[len + 1];
	strcpy(format, formatparams.c_str());

	const size_t param_offset = 4;
	const size_t num_params = len;

	if ((params[0] / sizeof(cell) - (param_offset - 1)) != num_params)
	{
		logprintf("parameter count does not match format specifier length %d - %d", num_params, (params[0] / sizeof(cell) - (param_offset - 1)));
		return 0;
	}

	cell param_idx = len - 1;
	cell *address_ptr = nullptr;
	cell *array_addr_ptr = nullptr;
	cell amx_address = -1;
	do
	{
		cell tmp_addr;
		switch (*(format + (len - 1)))
		{
			case 'd': //decimal
			case 'i': //integer
			case 'b':
			case 'f':
			{
				amx_GetAddr(amx, params[param_offset + param_idx], &address_ptr);
				cell value = *address_ptr;
				amx_Push(pAMX, value);
			}
			break;
			case 's': //string
			{
				char *str = nullptr;
				amx_StrParam(amx, params[param_offset + param_idx], str);

				if (str == nullptr)
				{
					str = new char[5];
					strcpy(str, "NULL");
				}

				amx_PushString(pAMX, &tmp_addr, nullptr, str, 0, 0);

				delete[] str;
			}
			break;
			case 'a': //array
			{
				cell *arraySize;
				amx_GetAddr(amx, params[param_offset + param_idx], &array_addr_ptr);
				if (amx_GetAddr(amx, params[param_offset + (param_idx + 1)], &arraySize) != AMX_ERR_NONE)
				{
					logprintf("missing 'd' / 'i' specifier for array size");
					return 0;
				}

				if ((*(format + (len))) != 'd' && (*(format + (len))) != 'i')
				{
					logprintf("expected 'd'/'i' specifier for array size (got '%c' instead)", *(format + (len)));
					return 0;
				}

				if (arraySize <= 0)
				{
					logprintf("invalid array size '%d'", arraySize);
					return 0;
				}

				cell *copied_array = static_cast<cell *>(malloc(*arraySize * sizeof(cell)));
				memcpy(copied_array, array_addr_ptr, *arraySize * sizeof(cell));

				amx_PushArray(pAMX, &tmp_addr, nullptr, copied_array, *arraySize);
				free(copied_array);

				if (amx_address < 0)
					amx_address = tmp_addr;

				array_addr_ptr = nullptr;
			}
			break;
			default:
			{
				logprintf("invalid format specifier '%c'", *(format + (len - 1)));
				return 0;
				break;
			}
		}
		param_idx--;
		len--;
	} while (len);

	cell ret;
	amx_Exec(pAMX, &ret, cb_idx);

	if (amx_address >= 0)
		amx_Release(pAMX, amx_address);

	delete[] format;
	return ret;
}

// native IsValidNickName(name[]);
AMX_DECLARE_NATIVE(Natives::IsValidNickName)
{
	CHECK_PARAMS(1, LOADED);

	std::string name;
	CScriptParams::Get()->Read(&name);
	return CServer::Get()->IsValidNick(const_cast<char*>(name.c_str()));
}

// native AllowNickNameCharacter(character, bool:allow);
AMX_DECLARE_NATIVE(Natives::AllowNickNameCharacter)
{
	CHECK_PARAMS(2, LOADED);

	const char character = static_cast<const char>(params[1]);

	// Enable %s is disallowed
	if (character == '%') return 0;

	CServer::Get()->AllowNickNameCharacter(character, static_cast<int>(params[2]) != 0);
	return 1;
}

// native IsNickNameCharacterAllowed(character);
AMX_DECLARE_NATIVE(Natives::IsNickNameCharacterAllowed)
{
	CHECK_PARAMS(1, LOADED);

	return CServer::Get()->IsNickNameCharacterAllowed(static_cast<char>(params[1]));
}

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

	if (pNetGame->iSpawnsAvailable <= 0)
		return 0;

	pNetGame->iSpawnsAvailable--;
	return 1;
}

// native GetPlayerClass(classid, &teamid, &modelid, &Float:spawn_x, &Float:spawn_y, &Float:spawn_z, &Float:z_angle, &weapon1, &weapon1_ammo, &weapon2, &weapon2_ammo,& weapon3, &weapon3_ammo);
AMX_DECLARE_NATIVE(Natives::GetPlayerClass)
{
	CHECK_PARAMS(13, LOADED);

	const int classid = CScriptParams::Get()->ReadInt();
	if (classid < 0 || classid > pNetGame->iSpawnsAvailable) return 0;

	CPlayerSpawnInfo *pSpawn = &pNetGame->AvailableSpawns[classid];

	CScriptParams::Get()->Add(pSpawn->byteTeam, pSpawn->iSkin, pSpawn->vecPos, pSpawn->fRotation,
		pSpawn->iSpawnWeapons[0], pSpawn->iSpawnWeaponsAmmo[0],
		pSpawn->iSpawnWeapons[1], pSpawn->iSpawnWeaponsAmmo[1],
		pSpawn->iSpawnWeapons[2], pSpawn->iSpawnWeaponsAmmo[2]);
	return 1;
}

// native EditPlayerClass(classid, teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
AMX_DECLARE_NATIVE(Natives::EditPlayerClass)
{
	CHECK_PARAMS(13, LOADED);

	const int classid = CScriptParams::Get()->ReadInt();
	if (classid < 0 || classid > pNetGame->iSpawnsAvailable) return 0;

	CPlayerSpawnInfo *pSpawn = &pNetGame->AvailableSpawns[classid];

	CScriptParams::Get()->Read(&pSpawn->byteTeam, &pSpawn->iSkin, &pSpawn->vecPos, &pSpawn->fRotation,
		&pSpawn->iSpawnWeapons[0], &pSpawn->iSpawnWeaponsAmmo[0],
		&pSpawn->iSpawnWeapons[1], &pSpawn->iSpawnWeaponsAmmo[1],
		&pSpawn->iSpawnWeapons[2], &pSpawn->iSpawnWeaponsAmmo[2]);
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

// native GetRecordingDirectory(dir[], len = sizeof(dir));
AMX_DECLARE_NATIVE(Natives::GetRecordingDirectory)
{
	CHECK_PARAMS(2, LOADED);

	if (!CAddress::ADDR_RecordingDirectory) return 0;

	char temp[MAX_PATH];
	const size_t len = strlen(gRecordingDataPath);
	strcpy(temp, gRecordingDataPath);
	temp[len - 7] = 0;

	CScriptParams::Get()->Add(&temp[0]);
	return 1;
}

// native SetRecordingDirectory(const dir[]);
AMX_DECLARE_NATIVE(Natives::SetRecordingDirectory)
{
	CHECK_PARAMS(1, LOADED);

	std::string dir;
	CScriptParams::Get()->Read(&dir);
	if (!CAddress::ADDR_RecordingDirectory) return 0;

	strcpy(gRecordingDataPath, dir.c_str());
	strcat(gRecordingDataPath, "/%s.rec");
	return 1;
}

// native SendClientMessagef(playerid, color, const message[], {Float,_}:...);
AMX_DECLARE_NATIVE(Natives::SendClientMessagef)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	const int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 3, len);
	if (!szMessage) return 0;

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
	if (!szMessage) return 0;

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

	const int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 4, len);
	if (!szMessage) return 0;

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
	if (!szMessage) return 0;

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

	const int playerid = static_cast<int>(params[1]);
	if (!IsPlayerConnected(playerid)) return 0;

	int senderid = static_cast<int>(params[2]);
	if (!IsPlayerConnected(senderid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 3, len);
	if (!szMessage) return 0;

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
	if (!IsPlayerConnected(senderid)) return 0;

	int len;
	char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 2, len);
	if (!szMessage) return 0;

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
	if (!szMessage) return 0;

	CSAMPFunctions::Execute(szMessage);
	return 1;
}
