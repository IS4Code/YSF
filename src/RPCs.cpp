#include "main.h"

//#define testspawn

int RPC_Gravity = 0x92;
int RPC_Weather = 0x98;

int RPC_CreatePickup = 95;
int RPC_DestroyPickup = 63;

int RPC_SetPlayerTeam = 45;
int RPC_CreateObject = 0x2C;

int RPC_DestroyObject = 0x2F;
int RPC_AttachObject = 0x4B;
int RPC_Widescreen = 111;
int RPC_ShowGangZone = 0x6C;
int RPC_HideGangZone = 0x78;
int RPC_FlashGangZone = 0x79;
int RPC_StopFlashGangZone = 0x55;
int RPC_RemovePlayerAttachedObject = 0x71;
int RPC_WorldPlayerAdd = 32;
int RPC_WorldPlayerRemove = 163;
int RPC_ChatBubble = 0x3B;
int RPC_SetPlayerSkin = 0x99;
int RPC_SetPlayerName = 0x0B;
int RPC_SetFightingStyle = 0x59;
int RPC_ScrApplyAnimation = 0x56;
int RPC_ClientMessage = 0x5D;
int RPC_ScrDisplayGameText = 0x49;
int RPC_Chat = 0x65;

int RPC_UpdateScoresPingsIPs = 0x9B;
int RPC_PickedUpPickup = 0x83;
int RPC_Spawn = 0x34;
int RPC_Death = 0x35;
int RPC_DeathBroadcast = 0xA6;

void UpdateScoresPingsIPs(RPCParameters *rpcParams)
{
	RakNet::BitStream bsUpdate;
	for(WORD i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnectedEx(i))
		{
			bsUpdate.Write(i);

			if(!pPlayerData[i]->bUpdateScoresPingsDisabled)
			{
				bsUpdate.Write(pNetGame->pPlayerPool->dwScore[i]);

				if(pPlayerData[i]->bFakePingToggle)
					bsUpdate.Write(pPlayerData[i]->dwFakePingValue);
				else
					bsUpdate.Write(pRakServer->GetLastPing(pRakServer->GetPlayerIDFromIndex(i)));
			}
			else
			{
				bsUpdate.Write(0);
				bsUpdate.Write(0);
			}
		}
	}

	pRakServer->RPC(&RPC_UpdateScoresPingsIPs, &bsUpdate, MEDIUM_PRIORITY, RELIABLE, 0, rpcParams->sender, false, false);
}

#ifdef testspawn
void Spawn(RPCParameters *rpcParams)
{
	RakNet::BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

	if (pNetGame->iGameState != GAMESTATE_RUNNING) return;

	WORD playerid = pRakServer->GetIndexFromPlayerID(rpcParams->sender);
	if (!IsPlayerConnectedEx(playerid)) return;
	CPlayer	*pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
	
	// Sanity checks
	if (!pPlayer->bHasSpawnInfo) return;
	int iSpawnClass = pPlayer->spawn.iSkin;
	if (iSpawnClass < 0 || iSpawnClass > 319) return;

	// Call OnPlayerSpawn
	CCallbackManager::OnPlayerSpawn(playerid);

	// Reset all their sync attributes.
	pPlayer->syncData.vecPosition = pPlayer->spawn.vecPos;
	pPlayer->syncData.fQuaternion[4] =  pPlayer->spawn.fRotation;
	pPlayer->vecPosition = pPlayer->spawn.vecPos;
	pPlayer->wVehicleId = 0;

	CSAMPFunctions::SpawnPlayer_(playerid);
}
#endif

void Death(RPCParameters* rpcParams)
{
	RakNet::BitStream bsData( rpcParams->input, rpcParams->numberOfBitsOfData / 8, false );

	WORD playerid = static_cast<WORD>(pRakServer->GetIndexFromPlayerID(rpcParams->sender)),
		killerid;
	BYTE reasonid;

	bsData.Read(reasonid);
	bsData.Read(killerid);

	if (!IsPlayerConnectedEx(playerid))
		return;

	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];

	// If another player killed
	if(IsPlayerConnectedEx(killerid))
	{
		CPlayer *pKiller = pNetGame->pPlayerPool->pPlayer[killerid];

		// If they aren't streamed for each other, then won't call OnPlayerDeath
		//logprintf("streamed: %d, %d", pKiller->byteStreamedIn[playerid], pPlayer->byteStreamedIn[killerid]);
		if(!pKiller->byteStreamedIn[playerid] || !pPlayer->byteStreamedIn[killerid])
			return;

		//logprintf("syncdata: %d, reason: %d, health: %f", pKiller->syncData.byteWeapon, reasonid, pPlayer->fHealth);
		if( pKiller->syncData.byteWeapon != reasonid && reasonid <= 46 )// 46 = parachute
			return;
		else if( ( reasonid == 48 || reasonid == 49 ) && pKiller->byteState != PLAYER_STATE_DRIVER ) // 48 - carkill // 49 - helikill
			return;

		if (pServer->GetIntVariable("chatlogging"))
			logprintf("[kill] %s killed %s %s", GetPlayerName_(killerid), GetPlayerName_(playerid), CUtils::GetWeaponName_(reasonid));
	}
	else
	{
		if (pServer->GetIntVariable("chatlogging"))
			logprintf("[death] %s died %d", GetPlayerName_(playerid), reasonid);
	}
	
	bsData.Reset();
	bsData.Write((WORD)playerid);
	pRakServer->RPC(&RPC_DeathBroadcast, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(playerid), true, false);
	
	pPlayer->byteState = PLAYER_STATE_WASTED;

	CCallbackManager::OnPlayerDeath(playerid, killerid, reasonid);
}

void PickedUpPickup(RPCParameters* rpcParams)
{
	RakNet::BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

	WORD playerid = static_cast<WORD>(pRakServer->GetIndexFromPlayerID(rpcParams->sender));
	int pickupid;

	// Just for security..
	if (!IsPlayerConnectedEx(playerid)) return;

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
			WORD pickupid = pNetGame->pPickupPool->FindPickup(p->second);
			if (pickupid != 0xFFFF)
			{
				CCallbackManager::OnPlayerPickedUpPickup(playerid, pickupid);
			}
		}
		else
		{
			for (PickupMap::iterator p2 = pPlayerData[playerid]->PlayerPickups.begin(); p2 != pPlayerData[playerid]->PlayerPickups.end(); p2++)
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
		if (pNetGame->pPickupPool->m_bActive[pickupid])
		{
			if (GetDistance3D(&pNetGame->pPlayerPool->pPlayer[playerid]->vecPosition, &pNetGame->pPickupPool->m_Pickup[pickupid].vecPos) > 15.0)
			{
				logprintf("fakepickup %d", pickupid);
				return;
			}

			CCallbackManager::OnPlayerPickedUpPickup(playerid, static_cast<WORD>(pickupid));
		}
	}
#endif
}

void InitRPCs()
{
	pRakServer->UnregisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs);
	pRakServer->RegisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs, UpdateScoresPingsIPs);
#ifdef testspawn
	pRakServer->UnregisterAsRemoteProcedureCall(&RPC_Spawn);
	pRakServer->RegisterAsRemoteProcedureCall(&RPC_Spawn, Spawn);
#endif
	pRakServer->UnregisterAsRemoteProcedureCall(&RPC_Death);
	pRakServer->RegisterAsRemoteProcedureCall(&RPC_Death, Death);

	pRakServer->UnregisterAsRemoteProcedureCall(&RPC_PickedUpPickup);
	pRakServer->RegisterAsRemoteProcedureCall(&RPC_PickedUpPickup, PickedUpPickup);
}
