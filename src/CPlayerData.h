#ifndef YSF_CPLAYERDATA_H
#define YSF_CPLAYERDATA_H

//#include "CServer.h"
#include "Structs.h"
#include <bitset>
#include <sampgdk/sampgdk.h>

#include "CGangZonePool.h"
#include "CPickupPool.h"

class CPlayerData
{
public:
	CPlayerData(WORD playerid);
	~CPlayerData(void);

	bool ResetPlayerMarkerForPlayer(WORD resetplayerid);

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
	WORD dwDisabledKeys;

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