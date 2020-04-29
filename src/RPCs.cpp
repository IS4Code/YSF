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

#include "RPCs.h"
#include "CPlugin.h"
#include "CServer.h"
#include "CConfig.h"
#include "CFunctions.h"
#include "CCallbackManager.h"
#include "Utils.h"
#include "Globals.h"

//#define testspawn

BYTE RPCID::tmp;

//----------------------------------------------------

void RedirectRPC(BYTE rpcid, void(*functionPointer) (RPCParameters *rpcParams))
{
	int _rpcid = static_cast<int>(rpcid);
	CSAMPFunctions::UnregisterAsRemoteProcedureCall(&_rpcid);
	CSAMPFunctions::RegisterAsRemoteProcedureCall(&_rpcid, functionPointer);
}

//----------------------------------------------------

void InitRPCs()
{
	// Allow manipulation scoreboard
	RedirectRPC(RPC_UpdateScoresPingsIPs, [](RPCParameters* rpcParams)
	{
		RakNet::BitStream bsUpdate;
		auto &pool = CServer::Get()->PlayerPool;
		for (WORD i = 0; i < MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
			{
				bsUpdate.Write(i);
				auto &data = pool.Extra(i);

				if (!data.bUpdateScoresPingsDisabled)
				{
					bsUpdate.Write(pNetGame->pPlayerPool->dwScore[i]);

					if (data.bFakePingToggle)
						bsUpdate.Write(data.dwFakePingValue);
					else
						bsUpdate.Write(CSAMPFunctions::GetLastPing(CSAMPFunctions::GetPlayerIDFromIndex(i)));
				}
				else
				{
					bsUpdate.Write(0);
					bsUpdate.Write(0);
				}
			}
		}

		CSAMPFunctions::RPC(&RPC_UpdateScoresPingsIPs, &bsUpdate, LOW_PRIORITY, UNRELIABLE, 0, rpcParams->sender, false, false);
	});

	//----------------------------------------------------
	// Spawning
	if (CConfig::Get()->m_bUseCustomSpawn)
	{
		RedirectRPC(RPC_Spawn, [](RPCParameters* rpcParams)
		{
			RakNet::BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

			if (pNetGame->iGameState != GAMESTATE_RUNNING) return;

			WORD playerid = static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(rpcParams->sender));
			if (!IsPlayerConnected(playerid)) return;
			CPlayer	*pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];

			// Sanity checks
			if (!pPlayer->bHasSpawnInfo) return;
			int iSpawnClass = pPlayer->spawn.iSkin;
			if (iSpawnClass < 0 || iSpawnClass > 319) return;

			// Call OnPlayerSpawn
			CCallbackManager::OnPlayerSpawn(playerid);

			// Reset all their sync attributes.
			pPlayer->syncData.vecPosition = pPlayer->spawn.vecPos;
			pPlayer->syncData.fQuaternion[4] = pPlayer->spawn.fRotation;
			pPlayer->vecPosition = pPlayer->spawn.vecPos;
			pPlayer->wVehicleId = 0;
			CServer::Get()->PlayerPool.Extra(playerid).bControllable = true;

			CSAMPFunctions::SpawnPlayer(playerid);
		});
	}
	
	//----------------------------------------------------
	// Protection against fakekill
	if (CConfig::Get()->m_bDeathProtection)
	{
		RedirectRPC(RPC_Death, [](RPCParameters* rpcParams)
		{
			RakNet::BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

			WORD playerid = static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(rpcParams->sender)),
				killerid;
			BYTE reasonid;

			bsData.Read(reasonid);
			bsData.Read(killerid);

			if (!IsPlayerConnected(playerid))
				return;

			CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];

			// If another player killed
			if (IsPlayerConnected(killerid))
			{
				CPlayer *pKiller = pNetGame->pPlayerPool->pPlayer[killerid];

				// If they aren't streamed for each other, then won't call OnPlayerDeath
				if (!pKiller->byteStreamedIn[playerid] || !pPlayer->byteStreamedIn[killerid])
					return;

				/*if (pKiller->syncData.byteWeapon != reasonid && pKiller->byteState != PLAYER_STATE_DRIVER)
				{
					if (reasonid <= 46 && reasonid != WEAPON_ROCKETLAUNCHER && reasonid != WEAPON_HEATSEEKER)// 46 = parachute
					{
						//			logprintf("onplayerdeath error 1, synced weapon: %d, reason: %d", pKiller->syncData.byteWeapon, reasonid);
						return;
					}
				}*/

				if (CSAMPFunctions::GetIntVariable("chatlogging"))
					logprintf("[kill] %s killed %s %s", GetPlayerName(killerid), GetPlayerName(playerid), Utility::GetWeaponName(reasonid));
			}
			else
			{
				if (CSAMPFunctions::GetIntVariable("chatlogging"))
					logprintf("[death] %s died %d", GetPlayerName(playerid), reasonid);
			}

			bsData.Reset();
			bsData.Write((WORD)playerid);
			CSAMPFunctions::RPC(&RPC_DeathBroadcast, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), true, false);

			pPlayer->byteState = PLAYER_STATE_WASTED;

			CCallbackManager::OnPlayerDeath(playerid, killerid, reasonid);
		});
	}

	//----------------------------------------------------
	// Add distance protection to OnPlayerPickupPickup against fake pickup ids
	if (CConfig::Get()->m_bPickupProtection)
	{
		RedirectRPC(RPC_PickedUpPickup, [](RPCParameters* rpcParams)
		{
			RakNet::BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

			WORD playerid = static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(rpcParams->sender));
			int pickupid;

			// Just for security..
			if (!IsPlayerConnected(playerid)) return;

			bsData.Read(pickupid);

#ifdef NEW_PICKUP_SYSTEM
			logprintf("pickup playerid %d, pickupid: %d", playerid, pickupid);

			// Find pickup in player client side pickuppool
			PickupMap::iterator p = pPlayerData[playerid]->ClientPlayerPickups.find(pickupid);
			if (p != pPlayerData[playerid]->ClientPlayerPickups.end())
			{
				logprintf("pos: %f, %f, %f", p->second->vecPos.fX, p->second->vecPos.fY, p->second->vecPos.fZ);
				// 99% - fake pickup RPC
				if (GetDistance3D(&pNetGame->pPlayerPool->pPlayer[playerid]->vecPosition, &p->second->vecPos) > 15.0)
				{
					logprintf("fakepickup %d", pickupid);
					return;
				}

				// If global pickup
				if (p->second->type == GLOBAL)
				{
					// Find global pickup ID by player pickup pointer
					WORD pickupid = CPlugin::Get()->pPickupPool->FindPickup(p->second);
					if (pickupid != 0xFFFF)
					{
						CCallbackManager::OnPlayerPickedUpPickup(playerid, pickupid);
					}
				}
				else
				{
					for (PickupMap::iterator p2 = pPlayerData[playerid]->PlayerPickups.begin(); p2 != pPlayerData[playerid]->PlayerPickups.end(); ++p2)
					{
						if (p2->second == p->second)
						{
							CCallbackManager::OnPlayerPickedUpPlayerPickup(playerid, (WORD)p2->first);
							break;
						}
					}
				}
			}
#else
			if (pickupid >= 0 && pickupid < MAX_PICKUPS)
			{
				if (pNetGame->pPickupPool->bActive[pickupid])
				{
					if (GetDistance3D(&pNetGame->pPlayerPool->pPlayer[playerid]->vecPosition, &pNetGame->pPickupPool->Pickup[pickupid].vecPos) > 15.0)
					{
						logprintf("fakepickup %d", pickupid);
						return;
					}

					CCallbackManager::OnPlayerPickedUpPickup(playerid, static_cast<WORD>(pickupid));
				}
			}
#endif
		});
	}

	//----------------------------------------------------
	// Made OnClientCheckResponse work in gamemode
	RedirectRPC(RPC_ClientCheck, [](RPCParameters* rpcParams)
	{
		WORD playerid = static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(rpcParams->sender));
		DWORD arg;
		BYTE type, response;

		RakNet::BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);
		bsData.Read(type);
		bsData.Read(arg);
		bsData.Read(response);

		CCallbackManager::OnClientCheckResponse(playerid, type, arg, response);
	});
}
