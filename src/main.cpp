//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2007 SA:MP Team
//
//----------------------------------------------------------

#include "main.h"

//----------------------------------------------------------

void **ppPluginData;
extern void *pAMXFunctions;

// Server instance
CServer *pServer = NULL;

// Internal server pointers
CNetGame *pNetGame = NULL;
void *pConsole = NULL;
RakServer *pRakServer = NULL;
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
	if (addr == CAddress::FUNC_Logprintf_037)
	{
		version = SAMP_VERSION_037;
		strcpy(szVersion, "0.3.7");
	}
	else if (addr == CAddress::FUNC_Logprintf_037_R2)
	{
		version = SAMP_VERSION_037_R2;
		strcpy(szVersion, "0.3.7 R2");
	}
	else if(addr == CAddress::FUNC_Logprintf_037_R2_1)
	{
		version = SAMP_VERSION_037_R2;
		strcpy(szVersion, "0.3.7 R2-1");
	}
	//logprintf("skipgeci: %d", GetServerCfgOption("ysf_skipversioncheck").c_str());

	if (1)
	{
		if (version != SAMP_VERSION_UNKNOWN)
		{
			// Create server instance
			pServer = new CServer(version);
		}
		else
		{
			logprintf("Error: Unknown " OS_NAME " server version\n");
			return true;
		}
	}
	else
	{
		version = SAMP_VERSION_037;
		strcpy(szVersion, "version check skipped");
		
		// Create server instance
		pServer = new CServer(version);
	}

	InstallPreHooks();

	logprintf("\n");
	logprintf(" ===============================\n");
	logprintf("        " PROJECT_NAME " - kurta999 version " PROJECT_VERSION " loaded\n");
	logprintf("   (c) 2008 Alex \"Y_Less\" Cole - (c) 2010 - 2015 kurta999\n");
	logprintf("    Server version: %s\n", szVersion);
	logprintf("    Operating System: " OS_NAME "\n");
	logprintf("    Built on: " __DATE__ " at " __TIME__ "\n");
	logprintf(" ===============================\n");
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
	logprintf("  %s unloaded\n", PROJECT_NAME);
	logprintf(" ==============");

	delete pNetGame->pGangZonePool;
	pNetGame->pGangZonePool = NULL;
#ifdef NEW_PICKUP_SYSTEM
	delete pNetGame->pPickupPool;
	pNetGame->pPickupPool = NULL;
#endif
	delete pServer;
	pServer = NULL;

	sampgdk_Unload();
}

//----------------------------------------------------------
// The AmxLoad() function gets called when a new gamemode or
// filterscript gets loaded with the server. In here we register
// the native functions we like to add to the scripts.

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX * amx) 
{
	CCallbackManager::RegisterAMX(amx);
	
	if(pServer)
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
	if(pServer)
	{
		pServer->Process();
	}
}