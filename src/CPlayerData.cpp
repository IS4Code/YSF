#include "main.h"

CPlayerData::CPlayerData( WORD playerid )
{
	static int playercolors[100] = 
	{
		0xFF8C13FF,0xC715FFFF,0x20B2AAFF,0xDC143CFF,0x6495EDFF,0xf0e68cFF,0x778899FF,0xFF1493FF,0xF4A460FF,
		0xEE82EEFF,0xFFD720FF,0x8b4513FF,0x4949A0FF,0x148b8bFF,0x14ff7fFF,0x556b2fFF,0x0FD9FAFF,0x10DC29FF,
		0x534081FF,0x0495CDFF,0xEF6CE8FF,0xBD34DAFF,0x247C1BFF,0x0C8E5DFF,0x635B03FF,0xCB7ED3FF,0x65ADEBFF,
		0x5C1ACCFF,0xF2F853FF,0x11F891FF,0x7B39AAFF,0x53EB10FF,0x54137DFF,0x275222FF,0xF09F5BFF,0x3D0A4FFF,
		0x22F767FF,0xD63034FF,0x9A6980FF,0xDFB935FF,0x3793FAFF,0x90239DFF,0xE9AB2FFF,0xAF2FF3FF,0x057F94FF,
		0xB98519FF,0x388EEAFF,0x028151FF,0xA55043FF,0x0DE018FF,0x93AB1CFF,0x95BAF0FF,0x369976FF,0x18F71FFF,
		0x4B8987FF,0x491B9EFF,0x829DC7FF,0xBCE635FF,0xCEA6DFFF,0x20D4ADFF,0x2D74FDFF,0x3C1C0DFF,0x12D6D4FF,
		0x48C000FF,0x2A51E2FF,0xE3AC12FF,0xFC42A8FF,0x2FC827FF,0x1A30BFFF,0xB740C2FF,0x42ACF5FF,0x2FD9DEFF,
		0xFAFB71FF,0x05D1CDFF,0xC471BDFF,0x94436EFF,0xC1F7ECFF,0xCE79EEFF,0xBD1EF2FF,0x93B7E4FF,0x3214AAFF,
		0x184D3BFF,0xAE4B99FF,0x7E49D7FF,0x4C436EFF,0xFA24CCFF,0xCE76BEFF,0xA04E0AFF,0x9F945CFF,0xDCDE3DFF,
		0x10C9C5FF,0x70524DFF,0x0BE472FF,0x8A2CD7FF,0x6152C2FF,0xCF72A9FF,0xE59338FF,0xEEDC2DFF,0xD8C762FF,
		0xD8C762FF
	};

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
	fGravity = pServer->GetGravity_();
	byteWeather = pServer->GetWeather_();
	bWidescreen = false;

	// Fix for GetPlayerColor
	if(pNetGame->pPlayerPool->pPlayer[playerid])
	{
		pNetGame->pPlayerPool->pPlayer[playerid]->iNickNameColor = playercolors[playerid % 100];
	}

	fBounds[0] = 20000.0f;
	fBounds[1] = -20000.0f;
	fBounds[2] = 20000.0f;
	fBounds[3] = -20000.0f;

	memset(pPlayerZone, NULL, sizeof(pPlayerZone));
	memset(byteClientSideZoneIDUsed, 0xFF, sizeof(byteClientSideZoneIDUsed));
	memset(wClientSideGlobalZoneID, 0xFFFF, sizeof(wClientSideGlobalZoneID));
	memset(wClientSidePlayerZoneID, 0xFFFF, sizeof(wClientSidePlayerZoneID));

	memset(dwClientSideZoneColor, NULL, sizeof(dwClientSideZoneColor));
	memset(dwClientSideZoneFlashColor, NULL, sizeof(dwClientSideZoneFlashColor));
//	memset(bIsGangZoneFlashing, false, sizeof(bIsGangZoneFlashing));

	// Pickpus
	ClientPlayerPickups.clear();

	bUpdateScoresPingsDisabled = false;
	bFakePingToggle = false;
	dwFakePingValue = 0;

	pCustomSyncData = NULL;
	memset(bCustomPos, false, MAX_PLAYERS);
	memset(bCustomQuat, false, MAX_PLAYERS);
	memset(vecCustomPos, NULL, sizeof(CVector));

	dwFPS = 0;
	dwLastDrunkLevel = 0;
	wSurfingInfo = 0;

	bAFKState = false;
	bEverUpdated = false;
	dwLastUpdateTick = 0;

	bHidden = false;
	bControllable = true;

	// Private
	memset(m_iTeams, -1, sizeof(m_iSkins));
	memset(m_iSkins, -1, sizeof(m_iSkins));
	memset(m_iFightingStyles, -1, sizeof(m_iFightingStyles));
	memset(m_szNames, NULL, sizeof(m_szNames));
}

CPlayerData::~CPlayerData( void )
{

}

bool CPlayerData::SetPlayerTeamForPlayer(WORD teamplayerid, int team)
{
	m_iTeams[teamplayerid] = team;

	RakNet::BitStream bs;
	bs.Write((WORD)teamplayerid);
	bs.Write((BYTE)team);	
	pRakServer->RPC(&RPC_SetPlayerTeam, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

int CPlayerData::GetPlayerTeamForPlayer(WORD teamplayerid)
{
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[teamplayerid];
	if (m_iTeams[teamplayerid] == -1)
	{
		return p->spawn.byteTeam;
	}
	return m_iTeams[teamplayerid];
}


bool CPlayerData::SetPlayerSkinForPlayer(WORD skinplayerid, int skin)
{
	m_iSkins[skinplayerid] = skin;

	RakNet::BitStream bs;
	bs.Write((int)skinplayerid);
	bs.Write((int)skin);
	pRakServer->RPC(&RPC_SetPlayerSkin, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

int CPlayerData::GetPlayerSkinForPlayer(WORD skinplayerid)
{
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[skinplayerid];
	if (m_iSkins[skinplayerid] == -1)
	{
		return p->spawn.iSkin;
	}
	return m_iSkins[skinplayerid];
}

bool CPlayerData::SetPlayerNameForPlayer(WORD nameplayerid, char *name)
{
	memcpy(&m_szNames[nameplayerid], name, MAX_PLAYER_NAME);
	BYTE len = static_cast<BYTE>(strlen(name));

	RakNet::BitStream bs;
	bs.Write((WORD)nameplayerid);
	bs.Write((BYTE)len);
	bs.Write(name, len);
	bs.Write((BYTE)1);

	pRakServer->RPC(&RPC_SetPlayerName, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

char *CPlayerData::GetPlayerNameForPlayer(WORD nameplayerid)
{
	if (!m_szNames[nameplayerid][0])
	{
		return GetPlayerName_(nameplayerid);
	}
	return &m_szNames[nameplayerid][0];
}

bool CPlayerData::SetPlayerFightingStyleForPlayer(WORD styleplayerid, int style)
{
	m_iFightingStyles[styleplayerid] = style;

	RakNet::BitStream bs;
	bs.Write((WORD)styleplayerid);
	bs.Write((BYTE)style);
	pRakServer->RPC(&RPC_SetFightingStyle, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

int CPlayerData::GetPlayerFightingStyleForPlayer(WORD styleplayerid)
{
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[styleplayerid];
	if (m_iFightingStyles[styleplayerid] == -1)
	{
		return p->byteFightingStyle;
	}
	return m_iFightingStyles[styleplayerid];
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

bool CPlayerData::DestroyObject_(WORD objectid)
{
	RakNet::BitStream bs;
	bs.Write(objectid);
	pRakServer->RPC(&RPC_DestroyObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return 1;
}

void CPlayerData::Process(void)
{
	// Process AFK detection
	DWORD dwTickCount = GetTickCount();
	
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	if (bEverUpdated && pPlayerPool->pPlayer[wPlayerID]->byteState != PLAYER_STATE_NONE && pPlayerPool->pPlayer[wPlayerID]->byteState != PLAYER_STATE_WASTED)
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

	// Process gangzones
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

void RebuildSyncData(RakNet::BitStream *bsSync, WORD toplayerid)
{
	BYTE id;
	WORD playerid;

	bsSync->Read(id);
	bsSync->Read(playerid);
	
	if(!IsPlayerConnectedEx(playerid) || !IsPlayerConnectedEx(toplayerid)) return;

	//logprintf("RebuildSyncData %d - %d", id, playerid);
	switch(id)
	{
		case ID_PLAYER_SYNC:
		{
			//logprintf("playerid: %d", playerid);

			bsSync->Reset();
			CPlayer *p = pNetGame->pPlayerPool->pPlayer[playerid];
	
			bsSync->Write((BYTE)ID_PLAYER_SYNC);
			bsSync->Write(playerid);

			// LEFT/RIGHT KEYS
			if(p->syncData.wLRAnalog) 
			{
				bsSync->Write(true);
				bsSync->Write(p->syncData.wLRAnalog);
			} 
			else 
			{
				bsSync->Write(false);
			}

			// UP/DOWN KEYS
			if(p->syncData.wUDAnalog) 
			{
				bsSync->Write(true);
				bsSync->Write(p->syncData.wUDAnalog);
			} 
			else 
			{
				bsSync->Write(false);
			}

			// Keys
			bsSync->Write(p->syncData.wKeys);
	
			// Position
			if(pPlayerData[toplayerid]->bCustomPos[playerid])				
				bsSync->Write(*pPlayerData[toplayerid]->vecCustomPos[playerid]);	
			else
				bsSync->Write((char*)&p->syncData.vecPosition, sizeof(CVector));
			
			// Rotation (in quaternion)
			if(pPlayerData[toplayerid]->bCustomQuat[playerid])
				bsSync->WriteNormQuat(pPlayerData[toplayerid]->fCustomQuat[playerid][0], pPlayerData[toplayerid]->fCustomQuat[playerid][1], pPlayerData[toplayerid]->fCustomQuat[playerid][2], pPlayerData[toplayerid]->fCustomQuat[playerid][3]);	
			else
				bsSync->WriteNormQuat(p->syncData.fQuaternion[0], p->syncData.fQuaternion[1], p->syncData.fQuaternion[2], p->syncData.fQuaternion[3]);

			// Health & armour compression
			BYTE byteSyncHealthArmour = 0;
			if( p->syncData.byteHealth > 0 && p->syncData.byteHealth < 100 ) 
			{
				byteSyncHealthArmour = ((BYTE)(p->syncData.byteHealth / 7)) << 4;
			} 
			else if(p->syncData.byteHealth >= 100) 
			{
				byteSyncHealthArmour = 0xF << 4;
			}

			if( p->syncData.byteArmour > 0 && p->syncData.byteArmour < 100 ) 
			{
				byteSyncHealthArmour |=  (BYTE)(p->syncData.byteArmour / 7);
			}
			else if(p->syncData.byteArmour >= 100) 
			{
				byteSyncHealthArmour |= 0xF;
			}
	
			bsSync->Write(byteSyncHealthArmour);

			// Current weapon
			bsSync->Write(p->syncData.byteWeapon);

			// Special action
			bsSync->Write(p->syncData.byteSpecialAction);

			// Velocity
			bsSync->WriteVector(p->syncData.vecVelocity.fX, p->syncData.vecVelocity.fY, p->syncData.vecVelocity.fZ);

			// Vehicle surfing (POSITION RELATIVE TO CAR SYNC)
			if(p->syncData.wSurfingInfo) 
			{
				bsSync->Write(true);
				bsSync->Write(p->syncData.wSurfingInfo);
				bsSync->Write(p->syncData.vecSurfing.fX);
				bsSync->Write(p->syncData.vecSurfing.fY);
				bsSync->Write(p->syncData.vecSurfing.fZ);
			} 
			else 
			{
				bsSync->Write(false);
			}
	
			// Animation
			if(p->syncData.dwAnimationData)
			{
				bsSync->Write(true);
				bsSync->Write((int)p->syncData.dwAnimationData);
			}
			else bsSync->Write(false);
			break;
		}
		case ID_VEHICLE_SYNC:
		{
		
			break;
		}
	}
}