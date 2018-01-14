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

#ifndef YSF_RPCS_H
#define YSF_RPCS_H

#include "includes/platform.h"

class RPCID
{
	static BYTE tmp;
	const BYTE val;
public:
	constexpr RPCID(BYTE value) : val(value)
	{

	}

	constexpr operator BYTE() const
	{
		return val;
	}

	BYTE* operator&() const
	{
		return &(tmp = val);
	}
};

constexpr RPCID RPC_Gravity = 0x92;
constexpr RPCID RPC_CreatePickup = 95;
constexpr RPCID RPC_DestroyPickup = 63;

constexpr RPCID RPC_SetPlayerTeam = 45;
constexpr RPCID RPC_CreateObject = 0x2C;

constexpr RPCID RPC_DestroyObject = 0x2F;
constexpr RPCID RPC_AttachObject = 0x4B;
constexpr RPCID RPC_Widescreen = 111;
constexpr RPCID RPC_ShowGangZone = 0x6C;
constexpr RPCID RPC_HideGangZone = 0x78;
constexpr RPCID RPC_FlashGangZone = 0x79;
constexpr RPCID RPC_StopFlashGangZone = 0x55;
constexpr RPCID RPC_RemovePlayerAttachedObject = 0x71;
constexpr RPCID RPC_WorldPlayerAdd = 32;
constexpr RPCID RPC_WorldPlayerRemove = 163;
constexpr RPCID RPC_ChatBubble = 0x3B;
constexpr RPCID RPC_SetPlayerSkin = 0x99;
constexpr RPCID RPC_SetPlayerName = 0x0B;
constexpr RPCID RPC_SetFightingStyle = 0x59;
constexpr RPCID RPC_ScrApplyAnimation = 0x56;
constexpr RPCID RPC_ClientMessage = 0x5D;
constexpr RPCID RPC_ScrDisplayGameText = 0x49;
constexpr RPCID RPC_Chat = 0x65;
constexpr RPCID RPC_ClientCheck = 103;
constexpr RPCID RPC_SetPlayerColor = 72;
constexpr RPCID RPC_SetTextDrawString = 0x69;
constexpr RPCID RPC_SetPlayerAttachedObject = 0x71;

constexpr RPCID RPC_UpdateScoresPingsIPs = 0x9B;
constexpr RPCID RPC_PickedUpPickup = 0x83;
constexpr RPCID RPC_Spawn = 0x34;
constexpr RPCID RPC_Death = 0x35;
constexpr RPCID RPC_DeathBroadcast = 0xA6;

constexpr RPCID RPC_ServerJoin = 0x89;
constexpr RPCID RPC_ServerQuit = 0x8A;

constexpr RPCID RPC_InitGame = 0x8B;
constexpr RPCID RPC_ScmEvent = 0x60;

void InitRPCs();

enum PacketEnumeration : unsigned char
{	
	ID_TIMESTAMP = 40,
	ID_VEHICLE_SYNC = 200,
	ID_AIM_SYNC = 203,
	ID_BULLET_SYNC = 206,
	ID_PLAYER_SYNC = 207,
	ID_MARKERS_SYNC,
	ID_UNOCCUPIED_SYNC = 209, 
	ID_TRAILER_SYNC = 210,
	ID_PASSENGER_SYNC = 211,
	ID_SPECTATOR_SYNC = 212,
	ID_RCON_COMMAND = 201,
	ID_RCON_RESPONCE,
	ID_WEAPONS_UPDATE = 204,
	ID_STATS_UPDATE = 205,
};

#endif