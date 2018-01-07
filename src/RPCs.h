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

extern BYTE RPC_Gravity;
extern BYTE RPC_CreatePickup;
extern BYTE RPC_DestroyPickup;
extern BYTE RPC_SetPlayerTeam;
extern BYTE RPC_CreateObject;
extern BYTE RPC_DestroyObject;
extern BYTE RPC_AttachObject;
extern BYTE RPC_Widescreen;
extern BYTE RPC_ShowGangZone;
extern BYTE RPC_HideGangZone;
extern BYTE RPC_FlashGangZone;
extern BYTE RPC_StopFlashGangZone;
extern BYTE RPC_RemovePlayerAttachedObject;
extern BYTE RPC_WorldPlayerAdd;
extern BYTE RPC_WorldPlayerRemove;
extern BYTE RPC_ChatBubble;
extern BYTE RPC_SetPlayerSkin;
extern BYTE RPC_SetPlayerName;
extern BYTE RPC_SetFightingStyle;
extern BYTE RPC_ScrApplyAnimation;
extern BYTE RPC_ClientMessage;
extern BYTE RPC_ScrDisplayGameText;
extern BYTE RPC_Chat;
extern BYTE RPC_ClientCheck;
extern BYTE RPC_SetPlayerColor;
extern BYTE RPC_SetTextDrawString;
extern BYTE RPC_SetPlayerAttachedObject;

extern BYTE RPC_UpdateScoresPingsIPs;
extern BYTE RPC_PickedUpPickup;
extern BYTE RPC_Spawn;
extern BYTE RPC_Death;
extern BYTE RPC_DeathBroadcast;

extern BYTE RPC_ServerJoin;
extern BYTE RPC_ServerQuit;

void InitRPCs();

enum PacketEnumeration : unsigned char
{	
	RPC_InitGame = 0x8B, 
	RPC_ScmEvent = 96,

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