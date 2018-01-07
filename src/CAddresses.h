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

#ifndef YSF_ADDRESSES_H
#define YSF_ADDRESSES_H

#ifdef _WIN32
	#define RAKNET_START_OFFSET							1
	#define RAKNET_SEND_OFFSET							7
	#define RAKNET_RECEIVE_OFFSET						10
	#define RAKNET_SET_ALLOWED_PLAYERS_OFFSET			13
	#define RAKNET_GET_LAST_PING_OFFSET					19
	#define RAKNET_REGISTER_RPC_OFFSET					29
	#define RAKNET_UNREGISTER_RPC_OFFSET				31
	#define RAKNET_RPC_OFFSET							32
	#define RAKNET_GET_LOCAL_IP_OFFSET					52
	#define RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET		57
	#define RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET		58
	#define RAKNET_ADD_BAN_OFFSET						60
	#define RAKNET_REMOVE_BAN_OFFSET					61
	#define RAKNET_CLEAR_BAN_OFFSET						62
	#define RAKNET_SET_TIMEOUT_OFFSET					65
#else
	#define RAKNET_START_OFFSET							2
	#define RAKNET_SEND_OFFSET							9
	#define RAKNET_RECEIVE_OFFSET						11
	#define RAKNET_SET_ALLOWED_PLAYERS_OFFSET			14
	#define RAKNET_GET_LAST_PING_OFFSET					20
	#define RAKNET_REGISTER_RPC_OFFSET					30
	#define RAKNET_UNREGISTER_RPC_OFFSET				32
	#define RAKNET_RPC_OFFSET							35
	#define RAKNET_GET_LOCAL_IP_OFFSET					53
	#define RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET		58
	#define RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET		59
	#define RAKNET_ADD_BAN_OFFSET						61
	#define RAKNET_REMOVE_BAN_OFFSET					62
	#define RAKNET_CLEAR_BAN_OFFSET						63
	#define RAKNET_SET_TIMEOUT_OFFSET					65
#endif

#include <vector>

enum class SAMPVersion
{
	VERSION_UNKNOWN,
	VERSION_037,
	VERSION_037_R2,
};

class CAddress
{
public:
	static void	Initialize(SAMPVersion version);

	static DWORD			FUNC_Logprintf_03Z;
	static DWORD			FUNC_Logprintf_03ZR2_2;
	static DWORD			FUNC_Logprintf_03ZR3;
	static DWORD			FUNC_Logprintf_03ZR4;
	static DWORD			FUNC_Logprintf_037;
	static DWORD			FUNC_Logprintf_037_R2;
	static DWORD			FUNC_Logprintf_037_R2_1;

	// Variables
	static DWORD			VAR_pRestartWaitTime;
	static DWORD			VAR_pPosSyncBounds[4];
	static DWORD			VAR_wRCONUser;
	static DWORD			ARRAY_ConsoleCommands;

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

	static DWORD			FUNC_CPlayer__SpawnForWorld;
	static DWORD			FUNC_CVehicle__Respawn;
	static DWORD			FUNC_CPlayerPool__HandleVehicleRespawn;

	static DWORD			FUNC_ProcessQueryPacket;
	static DWORD			FUNC_Packet_WeaponsUpdate;
	static DWORD			FUNC_Packet_StatsUpdate;
	static DWORD			FUNC_format_amxstring;

	// Others
	static DWORD			ADDR_GetNetworkStats_VerbosityLevel;
	static DWORD			ADDR_GetPlayerNetworkStats_VerbosityLevel;

	static DWORD			ADDR_RecordingDirectory;

	// Callback hooks
	static DWORD			FUNC_CGameMode__OnPlayerConnect;
	static DWORD			FUNC_CGameMode__OnPlayerDisconnect;
	static DWORD			FUNC_CGameMode__OnPlayerSpawn;
	static DWORD			FUNC_CGameMode__OnPlayerStreamIn;
	static DWORD			FUNC_CGameMode__OnPlayerStreamOut;
	static DWORD			FUNC_CGameMode__OnDialogResponse;
};

#endif