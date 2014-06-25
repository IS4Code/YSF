#include "main.h"

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
}

CPlayerData::~CPlayerData( void )
{

}

void CPlayerData::Process(void)
{
	for(WORD zoneid = 0; zoneid != MAX_GANG_ZONES; zoneid++)
	{
		// If zone id is unused client side, then continue
		if(this->byteClientSideZoneIDUsed[zoneid] == 0xFF) continue;

		CGangZone *pGangZone = NULL;
		if(this->byteClientSideZoneIDUsed[zoneid] == 0)
		{
			if(this->wClientSideGlobalZoneID[zoneid] == 0xFFFF)
			{
				logprintf("pPlayer->wClientSideGlobalZoneID[%d] = 0xFFFF", zoneid);
				return;
			}
					
			pGangZone = pNetGame->pGangZonePool->pGangZone[this->wClientSideGlobalZoneID[zoneid]];
		}
		else
		{
			if(this->wClientSidePlayerZoneID[zoneid] == 0xFFFF)
			{
				logprintf("pPlayer->wClientSidePlayerZoneID[%d] = 0xFFFF", zoneid);
				return;
			}

			pGangZone = this->pPlayerZone[this->wClientSidePlayerZoneID[zoneid]];
		}

		if(!pGangZone) continue;

		// Mutatók létrehozása
		CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[this->wPlayerID]->vecPosition;
		float *fMinX = &pGangZone->fGangZone[0];
		float *fMinY = &pGangZone->fGangZone[1];
		float *fMaxX = &pGangZone->fGangZone[2];
		float *fMaxY = &pGangZone->fGangZone[3];

		//logprintf("validzone: %d, %f, %f, %f, %f", pPlayer->wClientSideGlobalZoneID[zoneid], *fMinX, *fMinY, *fMaxX, *fMaxY);

		// Ha benne van
		if(vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY && !this->bInGangZone[zoneid])
		{
			this->bInGangZone[zoneid] = true;
			//logprintf("enterzone: %d", zoneid);
					
			if(this->byteClientSideZoneIDUsed[zoneid] == 0)
			{
				// Call callback
				int idx = -1;
				std::list<AMX*>::iterator second;
				for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
				{
					if(!amx_FindPublic(*second, "OnPlayerEnterGangZone", &idx))
					{
						cell
							ret;
						amx_Push(*second, this->wClientSideGlobalZoneID[zoneid]);
						amx_Push(*second, this->wPlayerID);

						amx_Exec(*second, &ret, idx);
					}
				}
			}
			else
			{
				// Call callback
				int idx = -1;
				std::list<AMX*>::iterator second;
				for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
				{
					if(!amx_FindPublic(*second, "OnPlayerEnterPlayerGangZone", &idx))
					{
						cell
							ret;
						amx_Push(*second, this->wClientSidePlayerZoneID[zoneid]);
						amx_Push(*second, this->wPlayerID);

						amx_Exec(*second, &ret, idx);
					}
				}					
			}
		}
		else if(!(vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY) && this->bInGangZone[zoneid])
		{
			this->bInGangZone[zoneid] = false;
			//logprintf("leavezone: %d", zoneid);
					
			if(this->byteClientSideZoneIDUsed[zoneid] == 0)
			{
				// Call callback
				int idx = -1;
				std::list<AMX*>::iterator second;
				for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
				{
					if(!amx_FindPublic(*second, "OnPlayerLeaveGangZone", &idx))
					{
						cell
							ret;
						amx_Push(*second, this->wClientSideGlobalZoneID[zoneid]);
						amx_Push(*second, this->wPlayerID);

						amx_Exec(*second, &ret, idx);
					}
				}
			}
			else
			{
				// Call callback
				int idx = -1;
				std::list<AMX*>::iterator second;
				for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
				{
					if(!amx_FindPublic(*second, "OnPlayerLeavePlayerGangZone", &idx))
					{
						cell
							ret;
						amx_Push(*second, this->wClientSidePlayerZoneID[zoneid]);
						amx_Push(*second, this->wPlayerID);

						amx_Exec(*second, &ret, idx);
					}
				}					
			}
		}
	}
}