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

#ifndef YSF_CPLAYERDATA_H
#define YSF_CPLAYERDATA_H

//#include "CServer.h"
#include "Structs.h"
#include <bitset>

#include "CGangZonePool.h"
#include "CPickupPool.h"

class CPlayerData
{
public:
	CPlayerData(WORD playerid);
	~CPlayerData(void);

	bool SetPlayerTeamForPlayer(WORD teamplayerid, int team);
	int GetPlayerTeamForPlayer(WORD teamplayerid);
	inline void ResetPlayerTeam(WORD playerid) { m_iTeams[playerid] = -1; }

	bool SetPlayerSkinForPlayer(WORD skinplayerid, int skin);
	int GetPlayerSkinForPlayer(WORD skinplayerid);
	inline void ResetPlayerSkin(WORD playerid) { m_iSkins[playerid] = -1; }

	bool SetPlayerNameForPlayer(WORD nameplayerid, char *name);
	char *GetPlayerNameForPlayer(WORD nameplayerid);
	inline void ResetPlayerName(WORD playerid) { m_szNames[playerid][0] = NULL; }

	bool SetPlayerFightingStyleForPlayer(WORD styleplayerid, int style);
	int GetPlayerFightingStyleForPlayer(WORD styleplayerid);
	inline void ResetPlayerFightingStyle(WORD playerid){ m_iFightingStyles[playerid] = -1; }

	void ResetPlayerMarkerForPlayer(WORD resetplayerid);
	
	WORD GetGangZoneIDFromClientSide(WORD zoneid, bool bPlayer = false);
	bool DestroyObject_(WORD objectid);

	void Process(void);

	struct sObj
	{
		WORD wObjectID;
		WORD wAttachPlayerID;
		CVector vecOffset;
		CVector vecRot;
	} stObj[MAX_OBJECTS];

	WORD wPlayerID;
	
	float fGravity;
	BYTE byteWeather;
	float fBounds[4];

	BYTE byteTeam;
	
	// Special shits for store sync data
	WORD wDisabledKeys;

		// Per-player pos
	bool bCustomPos[MAX_PLAYERS];
	bool bCustomQuat[MAX_PLAYERS];
	CVector *vecCustomPos[MAX_PLAYERS];
	float fCustomQuat[MAX_PLAYERS][4];

	CGangZone *pPlayerZone[MAX_GANG_ZONES];

	// [clientsideid] = serversideid
	BYTE byteClientSideZoneIDUsed[MAX_GANG_ZONES];
	WORD wClientSideGlobalZoneID[MAX_GANG_ZONES];
	WORD wClientSidePlayerZoneID[MAX_GANG_ZONES];

	std::bitset<MAX_GANG_ZONES> bInGangZone;
	std::bitset<MAX_GANG_ZONES> bIsGangZoneFlashing;
	DWORD dwClientSideZoneColor[MAX_GANG_ZONES];
	DWORD dwClientSideZoneFlashColor[MAX_GANG_ZONES];
#ifdef NEW_PICKUP_SYSTEM
	// Pickpus - clientside (global/player)
	PickupMap ClientPlayerPickups;
	std::bitset<MAX_PICKUPS> bClientPickupSlots;
	std::bitset<MAX_PICKUPS> bClientPickupStreamedIn;

	// Pickups - per-player
	PickupMap PlayerPickups;
	std::bitset<MAX_PICKUPS> bPlayerPickup;
#endif
	DWORD dwFakePingValue;
	DWORD dwLastUpdateTick;
	DWORD dwCreateAttachedObj;
	WORD dwObjectID;

	DWORD dwFPS;
	DWORD dwLastDrunkLevel;
	WORD wSurfingInfo;
	WORD wDialogID;
	WORD wLastDialogID;

	bool bObjectsRemoved : 1;
	bool bWidescreen : 1;
	bool bUpdateScoresPingsDisabled : 1;
	bool bFakePingToggle : 1;
	bool bAFKState : 1;
	bool bEverUpdated : 1; 
	bool bHidden : 1;
	bool bControllable : 1;
	bool bAttachedObjectCreated : 1;

private:
	int m_iTeams[MAX_PLAYERS];
	int m_iSkins[MAX_PLAYERS];
	int m_iFightingStyles[MAX_PLAYERS];
	char m_szNames[MAX_PLAYERS][MAX_PLAYER_NAME];
};

void RebuildSyncData(RakNet::BitStream *bsSync, WORD toplayerid);
void RebuildRPCData(BYTE uniqueID, RakNet::BitStream *bsSync, WORD playerid);
#endif