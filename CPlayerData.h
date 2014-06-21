#ifndef __YSF_CPLAYER
#define __YSF_CPLAYER

//#include "CServer.h"
#include "Structs.h"

#define MAX_PLAYERS					500
#define MAX_OBJECTS					1000
#define MAX_GANG_ZONES				1024
#define MAX_PICKUPS					4096
#define	MAX_TEXT_DRAWS				2048
#define	MAX_PLAYER_TEXT_DRAWS		256
#define	MAX_3DTEXT_GLOBAL			1024
#define MAX_MENUS					128
#define INVALID_PLAYER_ID			65535
#define MAX_FILTER_SCRIPTS			16

class CGangZone;

class CPlayerData
{
public:
	CPlayerData(WORD playerid);
	~CPlayerData(void);

	void Process(void);

	struct sObj
	{
		unsigned short usObjectID;
		unsigned short usAttachPlayerID;
		CVector vecOffset;
		CVector vecRot;
	} stObj[MAX_OBJECTS];

	WORD wPlayerID;
	bool bObjectsRemoved;
	int iPlayerPickupCount;
	float fGravity;
	BYTE byteWeather;
	bool bWidescreen;
	float fBounds[4];

	BYTE byteTeam;
	BYTE bytePlayersTeam[MAX_PLAYERS]; // for GetPlayerTeamForPlayer

	CGangZone *pPlayerZone[MAX_GANG_ZONES];

	// [clientsideid] = serversideid
	BYTE byteClientSideZoneIDUsed[MAX_GANG_ZONES];
	WORD wClientSideGlobalZoneID[MAX_GANG_ZONES];
	WORD wClientSidePlayerZoneID[MAX_GANG_ZONES];

	bool bInGangZone[MAX_GANG_ZONES];
	bool bIsGangZoneFlashing[MAX_GANG_ZONES];
	DWORD dwClientSideZoneColor[MAX_GANG_ZONES];
	DWORD dwClientSideZoneFlashColor[MAX_GANG_ZONES];
};
#endif