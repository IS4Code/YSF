#include "main.h"
#include "includes/platform.h"
#include "CPlugin.h"
#include "CScriptParams.h"
#include "CCallbackManager.h"
#include "CAddresses.h"
#include "Hooks.h"
#include "Natives.h"
#include "Utils.h"
#include "Globals.h"

#include <string>

//----------------------------------------------------------
// The Support() function indicates what possibilities this
// plugin has. The SUPPORTS_VERSION flag is required to check
// for compatibility with the server. 

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

//----------------------------------------------------------
// The Load() function gets passed on exported functions from
// the SA-MP Server, like the AMX Functions and logprintf().
// Should return true if loading the plugin has succeeded.

PLUGIN_EXPORT bool PLUGIN_CALL Load(void ** ppData)
{
	ppPluginData = ppData;
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);
	//logprintf("logprintf = 0x%08X\n", ppData[PLUGIN_DATA_LOGPRINTF]);
	
	// Check server version
	SAMPVersion version = SAMPVersion::VERSION_UNKNOWN;
	const char *szVersion;

	DWORD addr = reinterpret_cast<DWORD>(logprintf);
#ifdef SAMP_03DL
	if(addr == CAddress::FUNC_Logprintf_03DL_R1 || Utility::CFGLoad("SkipVersionCheck", 0))
	{
		version = SAMPVersion::VERSION_03DL_R1;
		szVersion = "0.3.DL R1";
	}else if(addr == CAddress::FUNC_Logprintf_037_R2_1)
	{
		logprintf("This version of YSF doesn't support SA-MP 0.3.7");
		logprintf("Use another version of YSF or build without SAMP_03DL");
	}
#else
	if(addr == CAddress::FUNC_Logprintf_037_R2_1 || Utility::CFGLoad("SkipVersionCheck", 0))
	{
		version = SAMPVersion::VERSION_037_R2;
		szVersion = "0.3.7 R2-1 or R2-2";
	}else if(addr == CAddress::FUNC_Logprintf_03DL_R1)
	{
		logprintf("This version of YSF doesn't support SA-MP 0.3.DL");
		logprintf("Use another version of YSF or build with SAMP_03DL");
	}
#endif
	else if(addr == CAddress::FUNC_Logprintf_03Z || CAddress::FUNC_Logprintf_03ZR2_2 || CAddress::FUNC_Logprintf_03ZR3 || CAddress::FUNC_Logprintf_03ZR4)
	{
		logprintf("This version of YSF doesn't support SA-MP 0.3z");
		logprintf("Update to 0.3.7! http://sa-mp.com/download.php");
	}
	
	CScriptParams::Init();
	if (version != SAMPVersion::VERSION_UNKNOWN)
	{
		CPlugin::Init(version);

		auto &header = " " PROJECT_NAME " " PROJECT_VERSION " (compiled " __DATE__ " " __TIME__") loaded";
		logprintf("");
		std::string row(sizeof(header) - 1, '=');
		row[0] = ' ';
		logprintf(row.c_str());
		logprintf(header);
		logprintf("  (c) 2008 Alex \"Y_Less\" Cole");
		logprintf("  (c) 2010 - 2018 kurta999");
		logprintf("  (c) 2018 - 2020 IllidanS4");
		logprintf(" Server version: %s, " OS_NAME, szVersion);
		logprintf(row.c_str());
		logprintf("");
	}
	else
	{
		logprintf("Error: Unknown " OS_NAME " server version (0x%08X)", ppData[PLUGIN_DATA_LOGPRINTF]);
		logprintf("Error: Big part of YSF will be unusable for you");
	}
	return true;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	UninstallHooks();

	CPlugin::Destroy();
	CScriptParams::Destroy();

	logprintf(" " PROJECT_NAME " " PROJECT_VERSION " unloaded");
}

//----------------------------------------------------------
// The AmxLoad() function gets called when a new gamemode or
// filterscript gets loaded with the server. In here we register
// the native functions we like to add to the scripts.

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX * amx) 
{
	CCallbackManager::RegisterAMX(amx);

	if(CPlugin::IsInitialized())
	{
		static bool bFirst = false;
		if(!bFirst)
		{
			bFirst = true;
			InstallPostHooks();
		}
	}
	return RegisterAllNatives(amx);
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
	if(CPlugin::IsInitialized())
	{
		CPlugin::Get()->Process();
	}
}