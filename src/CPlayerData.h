#ifndef YSF_CPLAYERDATA_H
#define YSF_CPLAYERDATA_H

//#include "CServer.h"
#include "Structs.h"
#include <bitset>
#include <sampgdk/sampgdk.h>

#include "CGangZonePool.h"
#include "CPickupPool.h"

//#define MAX_PLAYERS					500
//#define MAX_OBJECTS					1000
//#define MAX_GANG_ZONES				1024
//#define MAX_PICKUPS					4096
//#define	MAX_TEXT_DRAWS				2048
//#define	MAX_PLAYER_TEXT_DRAWS		256
//#define	MAX_3DTEXT_GLOBAL			1024
//#define MAX_MENUS					128
//#define INVALID_PLAYER_ID			65535
#define MAX_FILTER_SCRIPTS			16
/*
#define PLAYER_STATE_NONE						0
#define PLAYER_STATE_ONFOOT						1
#define PLAYER_STATE_DRIVER						2
#define PLAYER_STATE_PASSENGER					3
#define PLAYER_STATE_EXIT_VEHICLE				4
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER		5
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER	6
#define PLAYER_STATE_WASTED						7
#define PLAYER_STATE_SPAWNED					8
#define PLAYER_STATE_SPECTATING					9

#define UPDATE_TYPE_NONE		0
#define UPDATE_TYPE_ONFOOT		1
#define UPDATE_TYPE_INCAR		2
#define UPDATE_TYPE_PASSENGER	3

#define SPECTATE_TYPE_NONE		0
#define SPECTATE_TYPE_PLAYER	1
#define SPECTATE_TYPE_VEHICLE	2

#define SPECIAL_ACTION_NONE				0
#define SPECIAL_ACTION_USEJETPACK		2
#define SPECIAL_ACTION_DANCE1			5
#define SPECIAL_ACTION_DANCE2			6
#define SPECIAL_ACTION_DANCE3			7
#define SPECIAL_ACTION_DANCE4			8
#define SPECIAL_ACTION_HANDSUP			10
#define SPECIAL_ACTION_USECELLPHONE		11
#define SPECIAL_ACTION_SITTING			12
#define SPECIAL_ACTION_STOPUSECELLPHONE 13
*/

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

	WORD GetGangZoneIDFromClientSide(WORD zoneid, bool bPlayer = false);
	bool DestroyObject_(WORD objectid);

	void Process(void);

	struct sObj
	{
		WORD usObjectID;
		WORD usAttachPlayerID;
		CVector vecOffset;
		CVector vecRot;
	} stObj[MAX_OBJECTS];

	WORD wPlayerID;
	
	float fGravity;
	BYTE byteWeather;
	float fBounds[4];

	BYTE byteTeam;
	
	// Special shits for store sync data
	CSyncData *pCustomSyncData;

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

	// Pickpus - clientside (global/player)
	PickupMap ClientPlayerPickups;
	std::bitset<MAX_PICKUPS> bClientPickupSlots;
	std::bitset<MAX_PICKUPS> bClientPickupStreamedIn;

	// Pickups - per-player
	PickupMap PlayerPickups;
	std::bitset<MAX_PICKUPS> bPlayerPickup;

	DWORD dwFakePingValue;
	DWORD dwLastUpdateTick;
	
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

private:
	int m_iTeams[MAX_PLAYERS];
	int m_iSkins[MAX_PLAYERS];
	int m_iFightingStyles[MAX_PLAYERS];
	char m_szNames[MAX_PLAYERS][MAX_PLAYER_NAME];
};

void RebuildSyncData(RakNet::BitStream *bsSync, WORD toplayerid);
#endif