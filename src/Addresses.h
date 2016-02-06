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

#ifndef YSF_ADDRESSES_H
#define YSF_ADDRESSES_H

#ifdef _WIN32
	#define RAKNET_SEND_OFFSET			7
	#define RAKNET_RPC_OFFSET			32
	#define RAKNET_RECEIVE_OFFSET		10
#else
	#define RAKNET_SEND_OFFSET			9
	#define RAKNET_RPC_OFFSET			35
	#define RAKNET_RECEIVE_OFFSET		11
#endif

#include <vector>

enum eSAMPVersion
{
	SAMP_VERSION_UNKNOWN,
	SAMP_VERSION_037,
	SAMP_VERSION_037_R2,

	SAMP_VERSION_SKIPPED,
};

class CAddress
{
public:
	static void	Initialize(eSAMPVersion sampVersion);

	static DWORD			FUNC_Logprintf_03Z;
	static DWORD			FUNC_Logprintf_03ZR2_2;
	static DWORD			FUNC_Logprintf_03ZR3;
	static DWORD			FUNC_Logprintf_03ZR4;
	static DWORD			FUNC_Logprintf_037;
	static DWORD			FUNC_Logprintf_037_R2;
	static DWORD			FUNC_Logprintf_037_R2_1;

	// Pointers
	static DWORD			VAR_ppNetGame;
	static DWORD			VAR_ppConsole;
	static DWORD			VAR_ppRakServer;

	// Variables
	static DWORD			VAR_pRestartWaitTime;

	// Functions
	static DWORD			FUNC_CConsole__AddStringVariable;
	static DWORD			FUNC_CConsole__GetStringVariable;
	static DWORD			FUNC_CConsole__SetStringVariable;
	static DWORD			FUNC_CConsole__GetIntVariable;
	static DWORD			FUNC_CConsole__SetIntVariable;
	static DWORD			FUNC_CConsole__GetBoolVariable;
	static DWORD			FUNC_CConsole__ModifyVariableFlags;
	static DWORD			FUNC_CConsole__FindVariable;	 
	static DWORD			FUNC_CConsole__SendRules;
	static DWORD			FUNC_CConsole__Execute;

	static DWORD			FUNC_CNetGame__SetWeather;
	static DWORD			FUNC_CNetGame__SetGravity;

	static DWORD			FUNC_CFilterscripts__LoadFilterscript;
	static DWORD			FUNC_CFilterscripts__UnLoadFilterscript;
	static DWORD			FUNC_ContainsInvalidChars;
	static DWORD			FUNC_GetPacketID;

	static DWORD			FUNC_CPlayer__SpawnForWorld;

	static DWORD			FUNC_CVehicle__Respawn;
	static DWORD			FUNC_CPlayerPool__HandleVehicleRespawn;

	static DWORD			FUNC_ProcessQueryPacket;
	static DWORD			FUNC_Packet_WeaponsUpdate;
	static DWORD			FUNC_Packet_StatsUpdate;
	static DWORD			FUNC_format_amxstring;

	// Others
	static DWORD			ADDR_CNetGame_GMX_GangZoneDelete;
	static DWORD			ADDR_CNetGame_GMX_PckupDelete;

	static DWORD			ADDR_GetNetworkStats_VerbosityLevel;
	static DWORD			ADDR_GetPlayerNetworkStats_VerbosityLevel;
};

#endif