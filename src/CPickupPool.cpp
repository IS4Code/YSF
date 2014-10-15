#include "CPickupPool.h"

#include "CVector.h"
#include "main.h"
#include <raknet/BitStream.h>
#include "CPlayerData.h"
#include "RPCs.h"
#include "Utils.h"

CPickupPool::CPickupPool() : m_bStreamingEnabled(1)
{

}

CPickupPool::~CPickupPool()
{

}

void CPickupPool::InitializeForPlayer(WORD playerid)
{
	int count = 0;
	for (PickupMap::iterator p = m_Pickups.begin(); p != m_Pickups.end(); p++)
	{
		pPlayerData[playerid]->ClientPlayerPickups.insert(PickupMap::value_type(count, p->second));
		pPlayerData[playerid]->bClientPickupSlots.set(count, 1);
		
		// If streaming is disabled, show pickup for player
		if(!m_bStreamingEnabled) ShowPickup(count, playerid, *p->second);
		count++;
	}
}

int CPickupPool::New(int modelid, int type, CVector vecPos, int world)
{
	if(m_Pickups.size() < MAX_PICKUPS)
	{
		// Find free slot
		WORD slot = 0;
		while(slot != MAX_PICKUPS)
		{
			if(!m_bPickupSlots[slot]) break;
			slot++;
		}
		if(slot == MAX_PICKUPS) return 0xFFFF;

		// Add pickup into map
		CPickup *pPickup = new CPickup(modelid, type, vecPos, world, GLOBAL);

		m_Pickups.insert(PickupMap::value_type(slot, pPickup));
		m_bPickupSlots.set(slot, true);

		//logprintf("createpickup ret: %d", slot);
		// Send RPC for each player
		for(WORD i = 0; i != MAX_PLAYERS; i++)
		{
			// Skip unconnected players
			if(!IsPlayerConnected(i)) continue;

			int freeslot = -1;
			// Find free pickup slot
			for(int x = 0; x != MAX_PICKUPS; x++)
			{
				// If pickup slot is free
				if(!pPlayerData[i]->bClientPickupSlots[x])
				{
					freeslot = x;
					//logprintf("freeslot pickup %d - playerid: %d", x, i);
					break;
				}
			}
			if(freeslot == -1) continue;

			pPlayerData[i]->ClientPlayerPickups.insert(PickupMap::value_type(freeslot, pPickup));
			pPlayerData[i]->bClientPickupSlots.set(freeslot, 1);

			// If streaming is disabled, show pickup for player
			if(!m_bStreamingEnabled) ShowPickup(freeslot, i, *pPickup);
		}
		return slot;
	}
	return 0xFFFF;
}

int CPickupPool::New(WORD playerid, int modelid, int type, CVector vecPos, int world)
{
	// Skip unconnected players
	if(!IsPlayerConnected(playerid)) return 0xFFFF;

	// Find free slot - serverside
	WORD slot = 0;
	while(slot != MAX_PICKUPS)
	{
		if(!pPlayerData[playerid]->bPlayerPickup[slot]) break;
		slot++;
	}
	if(slot == MAX_PICKUPS) return 0xFFFF;

	// Find free pickup slot - client side
	int freeslot = -1;
	for(int x = 0; x != MAX_PICKUPS; x++)
	{
		// If pickup slot is free
		if(!pPlayerData[playerid]->bClientPickupSlots[x])
		{
			freeslot = x;
			logprintf("freeslot player pickup %d - playerid: %d", x, playerid);
			break;
		}
	}
	if(freeslot == -1) return 0xFFFF;

	// Create our pointer to pickup
	CPickup *pPickup = new CPickup(modelid, type, vecPos, world, PLAYER);

	// Add pickup into map - serverside
	pPlayerData[playerid]->PlayerPickups.insert(PickupMap::value_type(slot, pPickup));
	pPlayerData[playerid]->bPlayerPickup.set(slot, true);

	// Store clientside info
	pPlayerData[playerid]->ClientPlayerPickups.insert(PickupMap::value_type(freeslot, pPickup));
	pPlayerData[playerid]->bClientPickupSlots.set(freeslot, true);

	// If streaming is disabled, show pickup for player
	if(!m_bStreamingEnabled) ShowPickup(freeslot, playerid, *pPickup);
	return slot;
}

void CPickupPool::Destroy(int pickupid)
{
	// If valid pickup
	PickupMap::iterator it = m_Pickups.find(pickupid);
	if(it != m_Pickups.end())
	{
		// Send RPC for each player
		for(WORD i = 0; i != MAX_PLAYERS; i++)
		{
			// Skip unconnected players
			if(!IsPlayerConnected(i)) continue;

			for (PickupMap::iterator p = pPlayerData[i]->ClientPlayerPickups.begin(); p != pPlayerData[i]->ClientPlayerPickups.end(); p++)
			{
				if(p->second == it->second)
				{
					if(!m_bStreamingEnabled) HidePickup((int)p->first, i);

					pPlayerData[i]->bClientPickupSlots.set(p->first, false);
					pPlayerData[i]->ClientPlayerPickups.erase(p++);
					break;
				}
			}
		}
	}

	// And finaly, remove pickup from server pool
	m_bPickupSlots.set(pickupid, false);
	 m_Pickups.erase(it);
}

void CPickupPool::Destroy(WORD playerid, int pickupid)
{
	// If valid pickup
	PickupMap::iterator it = pPlayerData[playerid]->PlayerPickups.find(pickupid);
	if(it != pPlayerData[playerid]->PlayerPickups.end())
	{
		for (PickupMap::iterator p = pPlayerData[playerid]->ClientPlayerPickups.begin(); p != pPlayerData[playerid]->ClientPlayerPickups.end(); p++)
		{
			if(p->second == it->second)
			{
				if(!m_bStreamingEnabled) HidePickup((int)p->first, playerid);

				pPlayerData[playerid]->bClientPickupSlots.set(p->first, false);
				pPlayerData[playerid]->ClientPlayerPickups.erase(p++);
				break;
			}
		}
	}

	// And finaly, remove pickup from server pool
	pPlayerData[playerid]->bPlayerPickup.set(pickupid, false);
	pPlayerData[playerid]->PlayerPickups.erase(it);
}

void CPickupPool::ShowPickup(int pickupid, WORD playerid, CPickup pPickup)
{
	RakNet::BitStream bsPickup;
	bsPickup.Write(pickupid);
	bsPickup.Write(pPickup.iModel);
	bsPickup.Write(pPickup.iType);
	bsPickup.Write(pPickup.vecPos.fX);
	bsPickup.Write(pPickup.vecPos.fY);
	bsPickup.Write(pPickup.vecPos.fZ);
	pRakServer->RPC(&RPC_CreatePickup, &bsPickup, HIGH_PRIORITY, RELIABLE, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
}

void CPickupPool::HidePickup(int pickupid, WORD playerid)
{
	RakNet::BitStream bsPickup;
	bsPickup.Write(pickupid);
	pRakServer->RPC(&RPC_CreatePickup, &bsPickup, HIGH_PRIORITY, RELIABLE, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
}

bool CPickupPool::IsStreamed(WORD playerid, CPickup* pPickup)
{
	for(PickupMap::iterator p = pPlayerData[playerid]->ClientPlayerPickups.begin(); p != pPlayerData[playerid]->ClientPlayerPickups.end(); p++)
	{
		if(p->second == pPickup)
			return pPlayerData[playerid]->bClientPickupStreamedIn[p->first];
	}
	return 1;
}

CPickup* CPickupPool::FindPickup(int pickupid)
{
	PickupMap::iterator p = m_Pickups.find(pickupid);
	if(p != m_Pickups.end())
	{
		return p->second;
	}
	return NULL;
}

CPickup* CPickupPool::FindPickup(WORD playerid, int pickupid)
{
	PickupMap::iterator p = pPlayerData[playerid]->PlayerPickups.find(pickupid);
	if(p != m_Pickups.end())
	{
		return p->second;
	}
	return NULL;
}

int CPickupPool::FindPickup(CPickup *pPickup)
{
	for (PickupMap::iterator p = m_Pickups.begin(); p != m_Pickups.end(); p++)
	{
		if(p->second == pPickup)
			return p->first;
	}
	return 0xFFFF;
}

int CPickupPool::FindPickup(WORD playerid, CPickup *pPickup)
{
	for (PickupMap::iterator p = pPlayerData[playerid]->PlayerPickups.begin(); p != pPlayerData[playerid]->PlayerPickups.end(); p++)
	{
		if(p->second == pPickup)
			return p->first;
	}
	return 0xFFFF;
}

void CPickupPool::Process(void)
{
	if (m_bStreamingEnabled)
	{
		for(WORD playerid = 0; playerid != MAX_PLAYERS; playerid++)
		{
			if(!IsPlayerConnected(playerid)) continue;
			CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[playerid]->vecPosition;

			for(PickupMap::iterator p = pPlayerData[playerid]->ClientPlayerPickups.begin(); p != pPlayerData[playerid]->ClientPlayerPickups.end(); p++)
			{
				float distance = GetDistance3D(vecPos, &p->second->vecPos);
				if (distance < 300.0f && !pPlayerData[playerid]->bClientPickupStreamedIn[p->first])
				{
					if (pNetGame->pPlayerPool->dwVirtualWorld[playerid] == p->second->iWorld || p->second->iWorld == -1)
					{
						//logprintf("distance 1: %f - %d, world: %d", distance, playerid, p->second->iWorld);
						pPlayerData[playerid]->bClientPickupStreamedIn.set(p->first, true);

						ShowPickup((int)p->first, playerid, *p->second);
					}
				}
				else if ((distance > 300.0f || (pNetGame->pPlayerPool->dwVirtualWorld[playerid] != p->second->iWorld && p->second->iWorld != -1)) && pPlayerData[playerid]->bClientPickupStreamedIn[p->first])
				{
					//logprintf("distance2: %f - %d", distance, playerid);
					pPlayerData[playerid]->bClientPickupStreamedIn.set(p->first, false);

					HidePickup((int)p->first, playerid);
				}
			}
		}
	}
}

void CPickupPool::SetStreamingEnabled(bool enabled)
{
	m_bStreamingEnabled = enabled;
}

bool CPickupPool::IsStreamingEnabled(void)
{
	return m_bStreamingEnabled;
}