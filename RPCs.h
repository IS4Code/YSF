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

#ifndef __YSF_RPCS
#define __YSF_RPCS

extern int RPC_Gravity;
extern int RPC_Weather;
extern int RPC_DeathMessage;
extern int RPC_Explosion;
extern int RPC_CreatePickup;
extern int RPC_DestroyPickup;
extern int RPC_SetPlayerTeam;
extern int RPC_CreateObject;
extern int RPC_DestroyObject;
extern int RPC_AttachObject;
extern int RPC_Widescreen;
extern int RPC_WorldBounds;
extern int RPC_ShowGangZone;
extern int RPC_HideGangZone;
extern int RPC_FlashGangZone;
extern int RPC_StopFlashGangZone;
extern int RPC_RemovePlayerAttachedObject;
extern int RPC_WorldPlayerAdd;
extern int RPC_WorldPlayerRemove;

extern int RPC_UpdateScoresPingsIPs;
extern int RPC_PickedUpPickup;
extern int RPC_Death;

void InitRPCs();

enum PacketEnumeration : unsigned char
{
	ID_PLAYER_SYNC = 212,
	ID_MARKERS_SYNC = 213,
	ID_UNOCCUPIED_SYNC = 214,
	ID_TRAILER_SYNC = 215,
	ID_PASSENGER_SYNC = 216,
	ID_SPECTATOR_SYNC = 217,
	ID_AIM_SYNC = 218,
	ID_VEHICLE_SYNC = 219,
	ID_RCON_COMMAND = 220,
	ID_RCON_RESPONCE = 221,
	ID_WEAPONS_UPDATE = 222,
	ID_STATS_UPDATE = 223,
	ID_BULLET_SYNC = 224,
};

#endif