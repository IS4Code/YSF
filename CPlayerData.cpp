#include "main.h"

#ifndef WIN32
	#include <string.h>
#endif

CPlayerData::CPlayerData( void )
{
	// Null object data
	for(int i = 0; i != MAX_OBJECTS; i++)
	{
		this->stObj[i].usObjectID = 0xFFFF;
		this->stObj[i].usAttachPlayerID = INVALID_PLAYER_ID;
		this->stObj[i].vecOffset = CVector(0.0f, 0.0f, 0.0f);
		this->stObj[i].vecRot = CVector(0.0f, 0.0f, 0.0f);
	}

	bObjectsRemoved = false;
	iPlayerPickupCount = 0;
	fGravity = pServer->GetGravity();
	byteWeather = pServer->GetWeather();
	bWidescreen = false;

	fBounds[0] = 20000.0f;
	fBounds[1] = -20000.0f;
	fBounds[2] = 20000.0f;
	fBounds[3] = -20000.0f;

	memset(bytePlayersTeam, 0xFF, sizeof(bytePlayersTeam));

	memset(pPlayerZone, NULL, sizeof(pPlayerZone));
	memset(byteClientSideZoneIDUsed, 0xFF, sizeof(byteClientSideZoneIDUsed));
	memset(wClientSideGlobalZoneID, 0xFFFF, sizeof(wClientSideGlobalZoneID));
	memset(wClientSidePlayerZoneID, 0xFFFF, sizeof(wClientSidePlayerZoneID));

	memset(bInGangZone, false, sizeof(bInGangZone));
	memset(dwClientSideZoneColor, NULL, sizeof(dwClientSideZoneColor));
	memset(dwClientSideZoneFlashColor, NULL, sizeof(dwClientSideZoneFlashColor));
	memset(bIsGangZoneFlashing, false, sizeof(bIsGangZoneFlashing));
}

CPlayerData::~CPlayerData( void )
{

}