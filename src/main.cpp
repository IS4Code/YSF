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
#include "includes/platform.h"
#include "CPlugin.h"
#include "CScriptParams.h"
#include "CCallbackManager.h"
#include "CAddresses.h"
#include "Hooks.h"
#include "Natives.h"
#include "Utils.h"
#include "Globals.h"

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
	char szVersion[64];

	DWORD addr = reinterpret_cast<DWORD>(logprintf);
#ifdef SAMP_03DL
	if(addr == CAddress::FUNC_Logprintf_03DL_R1 || Utility::CFGLoad("SkipVersionCheck"))
	{
		version = SAMPVersion::VERSION_03DL_R1;
		strcpy(szVersion, "0.3.DL R1");
	}
	else if(addr == CAddress::FUNC_Logprintf_037_R2_1)
	{
		logprintf("This version of YSF doesn't support SA-MP 0.3.7");
		logprintf("Use another version of YSF");
	}
#else
	if(addr == CAddress::FUNC_Logprintf_037_R2_1 || Utility::CFGLoad("SkipVersionCheck"))
	{
		version = SAMPVersion::VERSION_037_R2;
		strcpy(szVersion, "0.3.7 R2-1");
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
		
		logprintf("");
		logprintf(" ===============================");
		logprintf("        " PROJECT_NAME " - kurta999's version " PROJECT_VERSION " loaded");
		logprintf("   (c) 2008 Alex \"Y_Less\" Cole - (c) 2010 - 2016 kurta999");
		logprintf("    Server version: %s", szVersion);
		logprintf("    Operating System: " OS_NAME);
		logprintf("    Built on: " __DATE__ " at " __TIME__);
		logprintf(" ===============================");
		logprintf("");
	}
	else
	{
		logprintf("Error: Unknown " OS_NAME " server version");
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

	// Corrected apperance in log file
	logprintf("");
	logprintf(" ==============");
	logprintf("  " PROJECT_NAME " - kurta999's version " PROJECT_VERSION " unloaded");
	logprintf(" ==============");
	logprintf("");
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