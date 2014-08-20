#include "CPickupPool.h"
#include "CVector.h"
#include "main.h"
#include "BitStream.h"
#include "CPlayerData.h"

void CPickupPool::InitializeForPlayer(WORD playerid)
{
	/*
	int count = 0;
	RakNet::BitStream bs;
	for (PickupMap::iterator p = m_Pickups.begin(); p != m_Pickups.end(); p++)
	{
		pPlayerData[playerid]->ClientPlayerPickups.insert(PickupMap::value_type(count, p->second));
		pPlayerData[playerid]->bClientPickupSlots.set(count, 1);
		
		bs.Write(count);
		bs.Write((PCHAR)&p->second, sizeof (CPickup));
		pRakServer->RPC(&RPC_CreatePickup, &bs, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true, false);
		
		bs.Reset();
		if(count++ == 150) break;
	}
	*/
}

int CPickupPool::New(int modelid, int type, CVector vecPos)
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
		CPickup *pPickup = new CPickup;
		pPickup->iModel  = modelid;
		pPickup->iType = type;
		pPickup->vecPos = vecPos;
		pPickup->byteType = 0;

		m_Pickups.insert(PickupMap::value_type(slot, pPickup));
		m_bPickupSlots.set(slot, true);

		logprintf("createpickup ret: %d", slot);
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
					logprintf("freeslot pickup %d - playerid: %d", x, i);
					break;
				}
			}
			if(freeslot == -1) continue;

			pPlayerData[i]->ClientPlayerPickups.insert(PickupMap::value_type(freeslot, pPickup));
			pPlayerData[i]->bClientPickupSlots.set(freeslot, 1);

			// And finaly, send rpc
			RakNet::BitStream bsPickup;
			bsPickup.Write(freeslot);
			bsPickup.Write(pPickup->iModel);
			bsPickup.Write(pPickup->iType);
			bsPickup.Write(pPickup->vecPos.fX);
			bsPickup.Write(pPickup->vecPos.fY);
			bsPickup.Write(pPickup->vecPos.fZ);
			pRakServer->RPC(&RPC_CreatePickup, &bsPickup, HIGH_PRIORITY, RELIABLE, 0, pRakServer->GetPlayerIDFromIndex(i), false, false);
		}
		return slot;
	}
	return 0xFFFF;
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
					RakNet::BitStream bsPickup;
					bsPickup.Write(p->first);
					pRakServer->RPC(&RPC_DestroyPickup, &bsPickup, HIGH_PRIORITY, RELIABLE, 0, pRakServer->GetPlayerIDFromIndex(i), false, false);

					pPlayerData[i]->bClientPickupSlots.set(p->first, false);
					p = pPlayerData[i]->ClientPlayerPickups.erase(p);
					break;
				}
			}
		}
	}

	// And finaly, remove pickup from server pool
	m_bPickupSlots.set(pickupid, false);
	it = m_Pickups.erase(it);
}

int CPickupPool::New(WORD playerid, int modelid, int type, CVector vecPos)
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
	CPickup *pPickup = new CPickup;
	pPickup->iModel  = modelid;
	pPickup->iType = type;
	pPickup->vecPos = vecPos;
	pPickup->byteType = 1;

	// Add pickup into map - serverside
	pPlayerData[playerid]->PlayerPickups.insert(PickupMap::value_type(slot, pPickup));
	pPlayerData[playerid]->bPlayerPickup.set(slot, true);

	// Store clientside info
	pPlayerData[playerid]->ClientPlayerPickups.insert(PickupMap::value_type(freeslot, pPickup));
	pPlayerData[playerid]->bClientPickupSlots.set(freeslot, true);

	// And finaly, send rpc
	RakNet::BitStream bsPickup;
	bsPickup.Write(freeslot);
	bsPickup.Write(pPickup->iModel);
	bsPickup.Write(pPickup->iType);
	bsPickup.Write(pPickup->vecPos.fX);
	bsPickup.Write(pPickup->vecPos.fY);
	bsPickup.Write(pPickup->vecPos.fZ);
	pRakServer->RPC(&RPC_CreatePickup, &bsPickup, HIGH_PRIORITY, RELIABLE, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
	return slot;
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
				RakNet::BitStream bsPickup;
				bsPickup.Write(p->first);
				pRakServer->RPC(&RPC_DestroyPickup, &bsPickup, HIGH_PRIORITY, RELIABLE, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);

				pPlayerData[playerid]->bClientPickupSlots.set(p->first, false);
				p = pPlayerData[playerid]->ClientPlayerPickups.erase(p);
				break;
			}
		}
	}

	// And finaly, remove pickup from server pool
	pPlayerData[playerid]->bPlayerPickup.set(pickupid, false);
	it = pPlayerData[playerid]->PlayerPickups.erase(it);
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