#ifndef __YSF_CPLAYER
#define __YSF_CPLAYER

//#include "CServer.h"
#include "Structs.h"

class CGangZone;

class CPlayerData
{
public:
	CPlayerData(void);
	~CPlayerData(void);

	struct sObj
	{
		unsigned short usObjectID;
		unsigned short usAttachPlayerID;
		CVector vecOffset;
		CVector vecRot;
	} stObj[MAX_OBJECTS];

	int iPlayerPickupCount;
	float fGravity;
	BYTE byteWeather;
	bool bWidescreen;
	float fBounds[4];
	bool bGangZonePresent[MAX_GANG_ZONES];

	BYTE byteTeam;
	BYTE bytePlayersTeam[MAX_PLAYERS]; // for GetPlayerTeamForPlayer

	CGangZone *pPlayerZone[MAX_GANG_ZONES];

	// [clientsideid] = serversideid
	BYTE byteClientSideZoneIDUsed[MAX_GANG_ZONES];
	WORD wClientSideGlobalZoneID[MAX_GANG_ZONES];
	WORD wClientSidePlayerZoneID[MAX_GANG_ZONES];

	bool bInGangZone[MAX_GANG_ZONES];
	DWORD dwClientSideZoneColor[MAX_GANG_ZONES];
	DWORD dwClientSideZoneFlashColor[MAX_GANG_ZONES];
};
#endif