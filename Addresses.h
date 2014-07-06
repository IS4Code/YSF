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

#ifndef __YSF_ADDRESSES
#define __YSF_ADDRESSES

typedef unsigned long       DWORD;

enum eSAMPVersion
{
	SAMP_VERSION_UNKNOWN,
	SAMP_VERSION_03Z,
	SAMP_VERSION_03Z_R2_2,
	SAMP_VERSION_03Z_R3,

	SAMP_VERSION_SKIPPED,
};

class CAddress
{
public:
	static void	Initialize(eSAMPVersion sampVersion);

	static DWORD			FUNC_Logprintf_03Z;
	static DWORD			FUNC_Logprintf_03ZR2_2;
	static DWORD			FUNC_Logprintf_03ZR3;

	// Pointers
	static DWORD			VAR_ppNetGame;
	static DWORD			VAR_ppConsole;
	static DWORD			VAR_ppRakServer;

	// Variables
	static DWORD			VAR_pRestartWaitTime;

	// Functions
	static DWORD			FUNC_CConsole_AddStringVariable;
	static DWORD			FUNC_CConsole_SetStringVariable;
	static DWORD			FUNC_CConsole_SetIntVariable;
	static DWORD			FUNC_CConsole_ModifyVariableFlags;

	static DWORD			FUNC_CFilterscripts_LoadFilterscript;
	static DWORD			FUNC_CFilterscripts_UnLoadFilterscript;
	static DWORD			FUNC_ContainsInvalidChars;

	// Others
	static DWORD			ADDR_CNetGame_GMX_GangZoneDelete;

	// Receive hook
	static DWORD			ADDR_RECEIVE_HOOKPOS;

	// Process timers
	static DWORD			ADDR_CScriptTimers__ProcessCall;
};

extern std::vector <char> gValidNameCharacters;

#endif