#include "main.h"
#include "Utils.h"
#include "CCallbackManager.h"

#ifndef WIN32
	#include <string.h>
#endif

CPlayerData::CPlayerData( WORD playerid )
{
	// Null object data
	for(int i = 0; i != MAX_OBJECTS; i++)
	{
		this->stObj[i].usObjectID = 0xFFFF;
		this->stObj[i].usAttachPlayerID = INVALID_PLAYER_ID;
		this->stObj[i].vecOffset = CVector(0.0f, 0.0f, 0.0f);
		this->stObj[i].vecRot = CVector(0.0f, 0.0f, 0.0f);
	}

	wPlayerID = playerid;
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

	bUpdateScoresPingsDisabled = false;
	bFakePingToggle = false;
	dwFakePingValue = 0;

	bAFKState = false;
	bEverUpdated = false;
	dwLastUpdateTick = false;
}

CPlayerData::~CPlayerData( void )
{

}

WORD CPlayerData::GetGangZoneIDFromClientSide(WORD zoneid, bool bPlayer)
{
	// Loop though every global gang zone
	if(!bPlayer)
	{
		for(WORD wZone = 0; wZone != MAX_GANG_ZONES; wZone++)
		{
			if(wClientSideGlobalZoneID[wZone] == zoneid)
				return wZone;
		}
	}
	else
	{
		for(WORD wZone = 0; wZone != MAX_GANG_ZONES; wZone++)
		{
			if(wClientSidePlayerZoneID[wZone] == zoneid)
				return wZone;
		}	
	}
	return 0xFFFF;
}

bool CPlayerData::DestroyObject(WORD objectid)
{
	RakNet::BitStream bs;
	bs.Write(objectid);
	pRakServer->RPC(&RPC_DestroyObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 2, pRakServer->GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return 1;
}

void CPlayerData::Process(void)
{
	// Process AFK detection
#ifdef WIN32
	DWORD dwTickCount = GetTickCount();

	if(bEverUpdated)
	{
		if(bAFKState == false && dwTickCount - dwLastUpdateTick > AFK_ACCURACY)
		{
			bAFKState = true;

			CCallbackManager::OnPlayerPauseStateChange(wPlayerID, bAFKState);
		}

		else if(bAFKState == true && dwTickCount - dwLastUpdateTick < AFK_ACCURACY)
		{
			bAFKState = false;

			CCallbackManager::OnPlayerPauseStateChange(wPlayerID, bAFKState);
		}
	}
#endif
	for(WORD zoneid = 0; zoneid != MAX_GANG_ZONES; zoneid++)
	{
		// If zone id is unused client side, then continue
		if(byteClientSideZoneIDUsed[zoneid] == 0xFF) continue;

		CGangZone *pGangZone = NULL;
		if(byteClientSideZoneIDUsed[zoneid] == 0)
		{
			if(wClientSideGlobalZoneID[zoneid] == 0xFFFF)
			{
				logprintf("pPlayer->wClientSideGlobalZoneID[%d] = 0xFFFF", zoneid);
				return;
			}
					
			pGangZone = pNetGame->pGangZonePool->pGangZone[wClientSideGlobalZoneID[zoneid]];
		}
		else
		{
			if(wClientSidePlayerZoneID[zoneid] == 0xFFFF)
			{
				logprintf("pPlayer->wClientSidePlayerZoneID[%d] = 0xFFFF", zoneid);
				return;
			}

			pGangZone = pPlayerZone[wClientSidePlayerZoneID[zoneid]];
		}

		if(!pGangZone) continue;

		// Mutatók létrehozása
		CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[wPlayerID]->vecPosition;
		float *fMinX = &pGangZone->fGangZone[0];
		float *fMinY = &pGangZone->fGangZone[1];
		float *fMaxX = &pGangZone->fGangZone[2];
		float *fMaxY = &pGangZone->fGangZone[3];

		//logprintf("validzone: %d, %f, %f, %f, %f", this->wClientSideGlobalZoneID[zoneid], *fMinX, *fMinY, *fMaxX, *fMaxY);
		
		// Ha benne van
		if(vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY && !bInGangZone[zoneid])
		{
			bInGangZone[zoneid] = true;
			//logprintf("enterzone: %d", zoneid);
					
			if(byteClientSideZoneIDUsed[zoneid] == 0)
			{
				CCallbackManager::OnPlayerEnterGangZone(wPlayerID, wClientSideGlobalZoneID[zoneid]);
			}
			else if(byteClientSideZoneIDUsed[zoneid] == 1)
			{
				CCallbackManager::OnPlayerEnterPlayerGangZone(wPlayerID, wClientSidePlayerZoneID[zoneid]);
			}
		}
		else if(!(vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY) && bInGangZone[zoneid])
		{
			bInGangZone[zoneid] = false;
			//logprintf("leavezone: %d", zoneid);
					
			if(byteClientSideZoneIDUsed[zoneid] == 0)
			{
				CCallbackManager::OnPlayerLeaveGangZone(wPlayerID, wClientSideGlobalZoneID[zoneid]);
			}
			else if(byteClientSideZoneIDUsed[zoneid] == 1)
			{
				CCallbackManager::OnPlayerLeavePlayerGangZone(wPlayerID, wClientSidePlayerZoneID[zoneid]);
			}
		}
	}
}