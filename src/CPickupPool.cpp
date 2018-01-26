/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

#include "CPickupPool.h"

#define MAX_PICKUP_DISTANCE	70.0f

#ifdef NEW_PICKUP_SYSTEM
CYSFPickupPool::CYSFPickupPool() : m_bStreamingEnabled(0)
{

}

CYSFPickupPool::~CYSFPickupPool()
{

}

void CYSFPickupPool::InitializeForPlayer(WORD playerid)
{
	int count = 0;
	for (PickupMap::iterator p = m_Pickups.begin(); p != m_Pickups.end(); ++p)
	{
		pPlayerData[playerid]->ClientPlayerPickups.insert(PickupMap::value_type(count, p->second));
		pPlayerData[playerid]->bClientPickupSlots.set(count, 1);
		
		// If streaming is disabled, show pickup for player
		if(!m_bStreamingEnabled) ShowPickup(count, playerid, p->second);
		count++;
	}
}

int CYSFPickupPool::New(int modelid, int type, CVector vecPos, int world)
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
		for(WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			// Skip unconnected players
			if(!IsPlayerConnected(i)) continue;

			int freeslot = -1;
			// Find free pickup slot
			for(int x = 0; x != MAX_PICKUPS; ++x)
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
			if(!m_bStreamingEnabled)
			{
				ShowPickup(freeslot, i, pPickup);
				//pPlayerData[i]->bClientPickupStreamedIn[freeslot] = true;
			}
		}
		return slot;
	}
	return 0xFFFF;
}

int CYSFPickupPool::New(WORD playerid, int modelid, int type, CVector vecPos, int world)
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
	for(int x = 0; x != MAX_PICKUPS; ++x)
	{
		// If pickup slot is free
		if(!pPlayerData[playerid]->bClientPickupSlots[x])
		{
			freeslot = x;
			//logprintf("freeslot player pickup %d - playerid: %d", x, playerid);
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
	if(!m_bStreamingEnabled)
	{
		ShowPickup(freeslot, playerid, pPickup);
		//pPlayerData[playerid]->bClientPickupStreamedIn[freeslot] = true;
	}
	return slot;
}

void CYSFPickupPool::Destroy(int pickupid)
{
	// If valid pickup
	PickupMap::iterator it = m_Pickups.find(pickupid);
	if(it != m_Pickups.end())
	{
		// Send RPC for each player
		for(WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			// Skip unconnected players
			if(!IsPlayerConnected(i)) continue;

			for (PickupMap::iterator p = pPlayerData[i]->ClientPlayerPickups.begin(); p != pPlayerData[i]->ClientPlayerPickups.end(); ++p)
			{
				if(p->second == it->second)
				{
					logprintf("destroy pickup: playerid: %d, pickup %d, clientside: %d - pos: %f, %f, %f", i, pickupid, p->first, p->second->vecPos.fX, p->second->vecPos.fY, p->second->vecPos.fZ);

					HidePickup(static_cast<int>(p->first), i);
					pPlayerData[i]->bClientPickupStreamedIn[p->first] = false;

					pPlayerData[i]->bClientPickupSlots.set(p->first, false);
					pPlayerData[i]->ClientPlayerPickups.erase(p);
					break;
				}
			}
		}

		// And finaly, remove pickup from server pool
		m_bPickupSlots.set(pickupid, false);
		SAFE_DELETE(it->second);
		m_Pickups.erase(it);
	}
}

void CYSFPickupPool::Destroy(WORD playerid, int pickupid)
{
	// If valid pickup
	PickupMap::iterator it = pPlayerData[playerid]->PlayerPickups.find(pickupid);
	if(it != pPlayerData[playerid]->PlayerPickups.end())
	{
		for (PickupMap::iterator p = pPlayerData[playerid]->ClientPlayerPickups.begin(); p != pPlayerData[playerid]->ClientPlayerPickups.end(); ++p)
		{
			if(p->second == it->second)
			{
				HidePickup(static_cast<int>(p->first), playerid);

				pPlayerData[playerid]->bClientPickupSlots.set(p->first, false);
				pPlayerData[playerid]->ClientPlayerPickups.erase(p);

				// And finaly, remove pickup from server pool
				pPlayerData[playerid]->bPlayerPickup.set(pickupid, false);
				SAFE_DELETE(it->second);
				pPlayerData[playerid]->PlayerPickups.erase(it);

				pPlayerData[playerid]->bClientPickupStreamedIn[p->first] = false;
				break;
			}
		}
	}
}

void CYSFPickupPool::ShowPickup(int pickupid, WORD playerid, CPickup *pPickup)
{
	HidePickup(pickupid, playerid);

	RakNet::BitStream bsPickup;
	bsPickup.Write(pickupid);
	bsPickup.Write(pPickup->iModel);
	bsPickup.Write(pPickup->iType);
	bsPickup.Write(pPickup->vecPos.fX);
	bsPickup.Write(pPickup->vecPos.fY);
	bsPickup.Write(pPickup->vecPos.fZ);
	CSAMPFunctions::RPC(&RPC_CreatePickup, &bsPickup, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
}

void CYSFPickupPool::HidePickup(int pickupid, WORD playerid)
{
	RakNet::BitStream bsPickup;
	bsPickup.Write(pickupid);
	CSAMPFunctions::RPC(&RPC_CreatePickup, &bsPickup, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
}

bool CYSFPickupPool::IsStreamed(WORD playerid, CPickup* pPickup)
{
	for(PickupMap::iterator p = pPlayerData[playerid]->ClientPlayerPickups.begin(); p != pPlayerData[playerid]->ClientPlayerPickups.end(); ++p)
	{
		if(p->second == pPickup)
			return pPlayerData[playerid]->bClientPickupStreamedIn[p->first];
	}
	return 0;
}

CPickup* CYSFPickupPool::FindPickup(int pickupid)
{
	PickupMap::iterator p = m_Pickups.find(pickupid);
	if(p != m_Pickups.end())
	{
		return p->second;
	}
	return NULL;
}

CPickup* CYSFPickupPool::FindPickup(WORD playerid, int pickupid)
{
	PickupMap::iterator p = pPlayerData[playerid]->PlayerPickups.find(pickupid);
	if(p != m_Pickups.end())
	{
		return p->second;
	}
	return NULL;
}

int CYSFPickupPool::FindPickup(CPickup *pPickup)
{
	for (PickupMap::iterator p = m_Pickups.begin(); p != m_Pickups.end(); ++p)
	{
		if(p->second == pPickup)
			return p->first;
	}
	return 0xFFFF;
}

int CYSFPickupPool::FindPickup(WORD playerid, CPickup *pPickup)
{
	for (PickupMap::iterator p = pPlayerData[playerid]->PlayerPickups.begin(); p != pPlayerData[playerid]->PlayerPickups.end(); ++p)
	{
		if(p->second == pPickup)
			return p->first;
	}
	return 0xFFFF;
}

void CYSFPickupPool::Process(void)
{
	if (m_bStreamingEnabled)
	{
		for (WORD playerid = 0; playerid != MAX_PLAYERS; ++playerid)
		{
			if (!IsPlayerConnected(playerid)) continue;
			CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[playerid]->vecPosition;

			for (PickupMap::iterator p = pPlayerData[playerid]->ClientPlayerPickups.begin(); p != pPlayerData[playerid]->ClientPlayerPickups.end(); ++p)
			{
				float distance = GetDistance3D(vecPos, &p->second->vecPos);
				if (distance < MAX_PICKUP_DISTANCE && !pPlayerData[playerid]->bClientPickupStreamedIn[p->first])
				{
					if (pNetGame->pPlayerPool->dwVirtualWorld[playerid] == static_cast<DWORD>(p->second->iWorld) || p->second->iWorld == -1)
					{
						logprintf("streamin: %f - %d, world: %d (pickupid: %d)", distance, playerid, p->second->iWorld, p->first);
						pPlayerData[playerid]->bClientPickupStreamedIn.set(p->first, true);

						ShowPickup(static_cast<int>(p->first), playerid, p->second);
					}
				}
				else if((distance >= MAX_PICKUP_DISTANCE || (pNetGame->pPlayerPool->dwVirtualWorld[playerid] != static_cast<DWORD>(p->second->iWorld) && p->second->iWorld != -1)) && pPlayerData[playerid]->bClientPickupStreamedIn[p->first])
				{
					logprintf("streamout: %f - %d (pickupid: %d)", distance, playerid, p->first);
					pPlayerData[playerid]->bClientPickupStreamedIn.set(p->first, false);

					HidePickup(static_cast<int>(p->first), playerid);
				}
			}
		}
	}
}

void CYSFPickupPool::SetStreamingEnabled(bool enabled)
{
	m_bStreamingEnabled = enabled;
}

bool CYSFPickupPool::IsStreamingEnabled(void)
{
	return m_bStreamingEnabled;
}
#endif