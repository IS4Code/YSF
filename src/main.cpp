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

//----------------------------------------------------------

void **ppPluginData;
extern void *pAMXFunctions;

// Internal server pointers
CNetGame *pNetGame = NULL;
void *pConsole = NULL;
void *pRakServer = NULL;
CPlayerData *pPlayerData[MAX_PLAYERS];

//----------------------------------------------------------
// The Support() function indicates what possibilities this
// plugin has. The SUPPORTS_VERSION flag is required to check
// for compatibility with the server. 

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return sampgdk_Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

//----------------------------------------------------------
// The Load() function gets passed on exported functions from
// the SA-MP Server, like the AMX Functions and logprintf().
// Should return true if loading the plugin has succeeded.
//typedef void(*logprintf_t)(char* format, ...);
//logprintf_t logprintf;

PLUGIN_EXPORT bool PLUGIN_CALL Load(void ** ppData)
{
	ppPluginData = ppData;
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	//logprintf = (logprintf_t);
	CServer::Get()->IsInitialized();
	bool ret = sampgdk_Load(ppData);
	logprintf("logprintf = 0x%08X\n", ppData[PLUGIN_DATA_LOGPRINTF]);
	
	//logprintf("offset: killer: %d", _OFFSET(CVehicle, wKillerID));

	// 10473
//	logprintf("object drawdistance offset: %d", _OFFSET2(RakServer, Send));

#ifndef _WIN32
	LoadTickCount();
#endif	
	// Check server version
	eSAMPVersion version = SAMP_VERSION_UNKNOWN;
	char szVersion[64];

	DWORD addr = (DWORD)ppData[PLUGIN_DATA_LOGPRINTF];
	if(addr == CAddress::FUNC_Logprintf_037_R2_1)
	{
		version = SAMP_VERSION_037_R2;
		strcpy(szVersion, "0.3.7 R2-1");
	}
	else if (addr == CAddress::FUNC_Logprintf_03Z || CAddress::FUNC_Logprintf_03ZR2_2 || CAddress::FUNC_Logprintf_03ZR3 || CAddress::FUNC_Logprintf_03ZR4)
	{
		logprintf("This version of YSF doesn't support SA-MP 0.3z");
		logprintf("Update to 0.3.7! http://sa-mp.com/download.php");
	}

	if (version != SAMP_VERSION_UNKNOWN)
	{
		CServer::Get()->Initialize(version);
		
		logprintf("\n");
		logprintf(" ===============================\n");
		logprintf("        " PROJECT_NAME " - kurta999's version " PROJECT_VERSION " loaded\n");
		logprintf("   (c) 2008 Alex \"Y_Less\" Cole - (c) 2010 - 2016 kurta999\n");
		logprintf("    Server version: %s\n", szVersion);
		logprintf("    Operating System: " OS_NAME "\n");
		logprintf("    Built on: " __DATE__ " at " __TIME__ "\n");
		logprintf(" ===============================\n");
	}
	else
	{
		logprintf("Error: Unknown " OS_NAME " server version\n");
		logprintf("Error: Big part of YSF will be unusable for you\n");
	}
	return ret;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// Corrected apperance in log file
	logprintf("\n");
	logprintf(" ==============\n");
	logprintf("  " PROJECT_NAME " - kurta999's version " PROJECT_VERSION " unloaded\n");
	logprintf(" ==============");

	UninstallHooks();
	CServer::CSingleton::Destroy();
	sampgdk_Unload();
}

//----------------------------------------------------------
// The AmxLoad() function gets called when a new gamemode or
// filterscript gets loaded with the server. In here we register
// the native functions we like to add to the scripts.

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX * amx) 
{
	CCallbackManager::RegisterAMX(amx);
	
	if(CServer::Get()->IsInitialized())
	{
		static bool bFirst = false;
		if(!bFirst)
		{
			bFirst = true;
			InstallPostHooks();
		}
	}
	return InitScripting(amx);
}

//----------------------------------------------------------
// When a gamemode is over or a filterscript gets unloaded, this
// function gets called. No special actions needed in here.

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX * amx) 
{
	// Remove AMX instance from our amxlist
	CCallbackManager::UnregisterAMX(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	if(CServer::Get()->IsInitialized())
	{
		CServer::Get()->Process();
	}
}

//----------------------------------------------------------
// SAMP GDK callbacks
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
{
	if(!CServer::Get()->IsInitialized())
		return true;

#ifndef NEW_PICKUP_SYSTEM
	CServer::Get()->AddPlayer(playerid);
#else
	// Initialize pickups
	if (CServer::Get()->AddPlayer(playerid))
		CServer::Get()->pPickupPool->InitializeForPlayer(playerid);
#endif
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	if(!CServer::Get()->IsInitialized())
		return true;

	CServer::Get()->RemovePlayer(playerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerStreamIn(int playerid, int forplayerid)
{
	if(!CServer::Get()->IsInitialized())
		return true;

	CServer::Get()->OnPlayerStreamIn(static_cast<WORD>(playerid), static_cast<WORD>(forplayerid));
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerStreamOut(int playerid, int forplayerid)
{
	if(!CServer::Get()->IsInitialized())
		return true;

	CServer::Get()->OnPlayerStreamOut(static_cast<WORD>(playerid), static_cast<WORD>(forplayerid));
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerSpawn(int playerid)
{
	if(!CServer::Get()->IsInitialized())
		return true;

	if (IsPlayerConnectedEx(playerid))
	{
		pPlayerData[playerid]->bControllable = true;
	}
	return true;
}