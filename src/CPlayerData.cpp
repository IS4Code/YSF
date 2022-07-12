#include "CPlayerData.h"
#include "includes/platform.h"
#include "CPlugin.h"
#include "CConfig.h"
#include "CFunctions.h"
#include "CCallbackManager.h"
#include "Globals.h"
#include "Utils.h"
#include "RPCs.h"

#include <cstring>

CPlayerData::CPlayerData(WORD playerid) : wPlayerID(playerid)
{
	static DWORD dwPlayerColors[100] = 
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

	// Fix for GetPlayerColor
	if (pNetGame->pPlayerPool->pPlayer[playerid])
	{
		pNetGame->pPlayerPool->pPlayer[playerid]->dwNickNameColor = dwPlayerColors[playerid % 100];
	}

	// Store NPC Process ID if it's an NPC
	if (pNetGame->pPlayerPool->bIsNPC[playerid])
	{
		if (CSAMPFunctions::GetPlayerIDFromIndex(playerid).binaryAddress == 0x0100007F)
			iNPCProcessID = CPlugin::Get()->FindNPCProcessID(playerid);
	}

	std::memset(byteClientSideZoneIDUsed, 0xFF, sizeof(byteClientSideZoneIDUsed));
	std::memset(wClientSideGlobalZoneID, 0xFF, sizeof(wClientSideGlobalZoneID));
	std::memset(wClientSidePlayerZoneID, 0xFF, sizeof(wClientSidePlayerZoneID));
}

CPlayerData::~CPlayerData( void )
{
	CPlugin::Get()->RemoveConsolePlayer(wPlayerID);

	for (WORD i = 0; i != MAX_OBJECTS; ++i)
	{
		DeleteObjectAddon(i);
	}
}

bool CPlayerData::SetPlayerTeamForPlayer(WORD teamplayerid, int team)
{
	m_iTeams[teamplayerid] = team + 1;

	RakNet::BitStream bs;
	bs.Write((WORD)teamplayerid);
	bs.Write((BYTE)team);	
	CSAMPFunctions::RPC(&RPC_SetPlayerTeam, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

int CPlayerData::GetPlayerTeamForPlayer(WORD teamplayerid)
{
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[teamplayerid];
	if (m_iTeams[teamplayerid] == 0)
	{
		return p->spawn.byteTeam;
	}
	return m_iTeams[teamplayerid] - 1;
}

bool CPlayerData::SetPlayerSkinForPlayer(WORD skinplayerid, int skin)
{
	m_iSkins[skinplayerid] = skin + 1;

	RakNet::BitStream bs;
	bs.Write((int)skinplayerid);
	bs.Write((int)skin);
	CSAMPFunctions::RPC(&RPC_SetPlayerSkin, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

int CPlayerData::GetPlayerSkinForPlayer(WORD skinplayerid)
{
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[skinplayerid];
	if (m_iSkins[skinplayerid] == 0)
	{
		return p->spawn.iSkin;
	}
	return m_iSkins[skinplayerid] - 1;
}

bool CPlayerData::SetPlayerNameForPlayer(WORD nameplayerid, const char *name)
{
	m_PlayerNames[nameplayerid] = std::string(name);
	BYTE len = static_cast<BYTE>(strlen(name));

	RakNet::BitStream bs;
	bs.Write((WORD)nameplayerid);
	bs.Write((BYTE)len);
	bs.Write(name, len);
	bs.Write((BYTE)1);

	CSAMPFunctions::RPC(&RPC_SetPlayerName, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

const char *CPlayerData::GetPlayerNameForPlayer(WORD nameplayerid)
{
	auto n = m_PlayerNames.find(nameplayerid);
	if (n == m_PlayerNames.end())
	{
		return GetPlayerName(nameplayerid);
	}
	return n->second.c_str();
}

bool CPlayerData::SetPlayerFightingStyleForPlayer(WORD styleplayerid, int style)
{
	m_iFightingStyles[styleplayerid] = style + 1;

	RakNet::BitStream bs;
	bs.Write((WORD)styleplayerid);
	bs.Write((BYTE)style);
	CSAMPFunctions::RPC(&RPC_SetFightingStyle, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return true;
}

int CPlayerData::GetPlayerFightingStyleForPlayer(WORD styleplayerid)
{
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[styleplayerid];
	if (m_iFightingStyles[styleplayerid] == 0)
	{
		return p->byteFightingStyle;
	}
	return m_iFightingStyles[styleplayerid] - 1;
}

void CPlayerData::ResetPlayerMarkerForPlayer(WORD resetplayerid)
{
	CPlayer *p = pNetGame->pPlayerPool->pPlayer[resetplayerid];
	
	RakNet::BitStream bs;
	bs.Write(resetplayerid);
	bs.Write(p->dwNickNameColor);
	CSAMPFunctions::RPC(&RPC_SetPlayerColor, &bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);
}

WORD CPlayerData::GetGangZoneIDFromClientSide(WORD zoneid, bool bPlayer)
{
	// Loop though every global gang zone
	if(!bPlayer)
	{
		for(WORD wZone = 0; wZone != MAX_GANG_ZONES; ++wZone)
		{
			if(wClientSideGlobalZoneID[wZone] == zoneid)
				return wZone;
		}
	}
	else
	{
		for(WORD wZone = 0; wZone != MAX_GANG_ZONES; ++wZone)
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
	CSAMPFunctions::RPC(&RPC_DestroyObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);
	return 1;
}

// Return a pointer from map if exists or add it if isn't - to save memory
std::shared_ptr<CPlayerObjectAttachAddon> CPlayerData::GetObjectAddon(WORD objectid)
{
	std::shared_ptr<CPlayerObjectAttachAddon> pAddon;

	try
	{
		auto it = m_PlayerObjectsAddon.find(static_cast<WORD>(objectid));
		if (it == m_PlayerObjectsAddon.end())
		{
			pAddon = std::make_shared<CPlayerObjectAttachAddon>();
			m_PlayerObjectsAddon.emplace(objectid, pAddon);
		}
		else
		{
			pAddon = it->second;
		}
	}
	catch (...)
	{
		//logprintf("CPlayerData::GetObjectAddon(WORD objectid) catch");
	}
	return pAddon;
}

std::shared_ptr<CPlayerObjectAttachAddon> const CPlayerData::FindObjectAddon(WORD objectid)
{
	auto it = m_PlayerObjectsAddon.find(static_cast<WORD>(objectid));
	if (it == m_PlayerObjectsAddon.end())
		return NULL;

	return it->second;
}

void CPlayerData::DeleteObjectAddon(WORD objectid)
{
	auto it = m_PlayerObjectsAddon.find(static_cast<WORD>(objectid));
	if (it != m_PlayerObjectsAddon.end())
	{
		m_PlayerObjectsAttachQueue.erase(it->first);
		m_PlayerObjectsAddon.erase(it);
	}
}

void CPlayerData::Process(void)
{
	// Processing AFK detection	
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	if (bEverUpdated && pPlayerPool->pPlayer[wPlayerID]->byteState != PLAYER_STATE_NONE && pPlayerPool->pPlayer[wPlayerID]->byteState != PLAYER_STATE_WASTED)
	{
		default_clock::duration passed_time = default_clock::now() - LastUpdateTick;
		if(bAFKState == false && std::chrono::duration_cast<std::chrono::milliseconds>(passed_time).count() > CPlugin::Get()->GetAFKAccuracy())
		{
			bAFKState = true;

			CCallbackManager::OnPlayerPauseStateChange(wPlayerID, bAFKState);
		}

		else if(bAFKState == true && std::chrono::duration_cast<std::chrono::milliseconds>(passed_time).count() < CPlugin::Get()->GetAFKAccuracy())
		{
			bAFKState = false;

			CCallbackManager::OnPlayerPauseStateChange(wPlayerID, bAFKState);
		}
	}
	
	// Processing Attached Objects
	if (!m_PlayerObjectsAttachQueue.empty())
	{
		for (std::set<WORD>::iterator o = m_PlayerObjectsAttachQueue.begin(); o != m_PlayerObjectsAttachQueue.end(); )
		{
			auto it = m_PlayerObjectsAddon.find(*(o));
			if (it != m_PlayerObjectsAddon.end() )
			{
				if(it->second && it->second->bCreated)
				{
					default_clock::duration passed_time = default_clock::now() - it->second->creation_timepoint;
					//logprintf("time passed: %d", std::chrono::duration_cast<std::chrono::milliseconds>(passed_time).count());
					if (std::chrono::duration_cast<std::chrono::milliseconds>(passed_time).count() > CConfig::Get()->m_iAttachObjectDelay)
					{
						RakNet::BitStream bs;
						bs.Write((WORD)it->first); // wObjectID
						bs.Write((WORD)it->second->wAttachPlayerID); // wAttachPlayerID
						bs.Write((char*)&it->second->vecOffset, sizeof(CVector));
						bs.Write((char*)&it->second->vecRot, sizeof(CVector));
						CSAMPFunctions::RPC(&RPC_AttachObject, &bs, LOW_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);

						it->second->bAttached = true;
						o = m_PlayerObjectsAttachQueue.erase(o);

						//logprintf("attached and removed: %d", it->first);
					}
					else
					{
						++o;
					}
				}
			}
			else
			{
				//logprintf("YSF: Error at looping trough attached object queue");
			}
		}
	}
	
	if (CConfig::Get()->m_bUsePerPlayerGangZones)
	{
		// Processing gangzones
		for (WORD zoneid = 0; zoneid != MAX_GANG_ZONES; ++zoneid)
		{
			// If zone id is unused client side, then continue
			if (byteClientSideZoneIDUsed[zoneid] == 0xFF) continue;

			CGangZone *pGangZone = NULL;
			if (byteClientSideZoneIDUsed[zoneid] == 0)
			{
				if (wClientSideGlobalZoneID[zoneid] == 0xFFFF)
				{
					logprintf("pPlayer->wClientSideGlobalZoneID[%d] = 0xFFFF", zoneid);
					return;
				}

				pGangZone = CPlugin::Get()->pGangZonePool->pGangZone[wClientSideGlobalZoneID[zoneid]];
			}
			else
			{
				if (wClientSidePlayerZoneID[zoneid] == 0xFFFF)
				{
					logprintf("pPlayer->wClientSidePlayerZoneID[%d] = 0xFFFF", zoneid);
					return;
				}

				pGangZone = pPlayerZone[wClientSidePlayerZoneID[zoneid]];
			}

			if (!pGangZone) continue;

			const CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[wPlayerID]->vecPosition;
			const float *fMinX = &pGangZone->fGangZone[0];
			const float *fMinY = &pGangZone->fGangZone[1];
			const float *fMaxX = &pGangZone->fGangZone[2];
			const float *fMaxY = &pGangZone->fGangZone[3];

			//logprintf("validzone: %d, %f, %f, %f, %f", this->wClientSideGlobalZoneID[zoneid], *fMinX, *fMinY, *fMaxX, *fMaxY);

			// Check for enters/exits
			if (vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY && !bInGangZone[zoneid])
			{
				bInGangZone[zoneid] = true;
				//logprintf("enterzone: %d", zoneid);

				if (byteClientSideZoneIDUsed[zoneid] == 0)
				{
					CCallbackManager::OnPlayerEnterGangZone(wPlayerID, wClientSideGlobalZoneID[zoneid]);
				}
				else if (byteClientSideZoneIDUsed[zoneid] == 1)
				{
					CCallbackManager::OnPlayerEnterPlayerGangZone(wPlayerID, wClientSidePlayerZoneID[zoneid]);
				}
			}
			else if (!(vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY) && bInGangZone[zoneid])
			{
				bInGangZone[zoneid] = false;
				//logprintf("leavezone: %d", zoneid);

				if (byteClientSideZoneIDUsed[zoneid] == 0)
				{
					CCallbackManager::OnPlayerLeaveGangZone(wPlayerID, wClientSideGlobalZoneID[zoneid]);
				}
				else if (byteClientSideZoneIDUsed[zoneid] == 1)
				{
					CCallbackManager::OnPlayerLeavePlayerGangZone(wPlayerID, wClientSidePlayerZoneID[zoneid]);
				}
			}
		}
	}
}

void CPlayerData::HideObject(WORD objectid, bool sync)
{
	if (sync)
	{
		RakNet::BitStream bs;
		bs.Write(objectid);
		CSAMPFunctions::RPC(&RPC_DestroyObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(wPlayerID), 0, 0);
	}
	m_HiddenObjects.insert(objectid);
}

void CPlayerData::ShowObject(WORD objectid, bool sync)
{
	m_HiddenObjects.erase(objectid);
	if (sync)
	{
		CSAMPFunctions::SpawnObjectForPlayer(objectid, wPlayerID);
	}
}

void CPlayerData::HideNewObjects(bool toggle)
{
	m_HideNewObjects = toggle;
}

bool CPlayerData::NewObjectsHidden() const
{
	return m_HideNewObjects;
}

bool CPlayerData::IsObjectHidden(WORD objectid) const
{
	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	if (!pObjectPool->bObjectSlotState[objectid]) return false;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	if (!pObject) return false;

	return m_HiddenObjects.find(objectid) != m_HiddenObjects.end();
}

void CPlayerData::SetBuildingsRemoved(int modelid, const CVector &pos, float range)
{
	m_RemovedBuildings.emplace_back(modelid, pos, range);
}

size_t CPlayerData::GetBuildingsRemoved() const
{
	return m_RemovedBuildings.size();
}

bool CPlayerData::IsBuildingRemoved(int modelid, const CVector &pos, float range, bool any) const
{
	for (auto &building : m_RemovedBuildings)
	{
		if (modelid == -1 || building.ModelId == -1 || modelid == building.ModelId)
		{
			float d = (pos - building.Position).Length();
			if(any)
			{
				if(d <= range + building.Range)
				{
					return true;
				}
			} else {
				if(d + range <= building.Range)
				{
					return true;
				}
			}
		}
	}
	return false;
}
