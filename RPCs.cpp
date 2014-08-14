#include "main.h"
#include "Structs.h"

int RPC_Gravity = 0x92;
int RPC_Weather = 0x98;
int RPC_DeathMessage = 0x37;
int RPC_Explosion = 0x4F;
int RPC_CreatePickup = 95;
int RPC_DestroyPickup = 63;
int RPC_SetPlayerTeam = 45;
int RPC_CreateObject = 0x2C;
int RPC_DestroyObject = 0x2F;
int RPC_AttachObject = 0x4B;
//int RPC_ShowGangZone = 0x6C;
//int RPC_DestroyGangZone = 199;
int RPC_Widescreen = 111;
int RPC_WorldBounds = 0x11;
int RPC_ShowGangZone = 0x6C;
int RPC_HideGangZone = 0x78;
int RPC_FlashGangZone = 0x79;
int RPC_StopFlashGangZone = 0x55;
int RPC_RemovePlayerAttachedObject = 0x71;
int RPC_WorldPlayerAdd = 32;
int RPC_WorldPlayerRemove = 163;
int RPC_ChatBubble = 0x3B;

int RPC_UpdateScoresPingsIPs = 0x9B;
int RPC_PickedUpPickup = 0x83;
int RPC_Death = 0x35;

void UpdateScoresPingsIPs(RPCParameters *rpcParams)
{
	RakNet::BitStream bsUpdate;
	for(PLAYERID i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
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

	pRakServer->RPC(&RPC_UpdateScoresPingsIPs, &bsUpdate, HIGH_PRIORITY, RELIABLE, 0, rpcParams->sender, false, false);
}


void Death(RPCParameters* rpcParams)
{
	RakNet::BitStream bsData( rpcParams->input, rpcParams->numberOfBitsOfData / 8, false );

	WORD playerid = pRakServer->GetIndexFromPlayerID(rpcParams->sender),
		killerid;
	BYTE reasonid;

	bsData.Read(reasonid);
	bsData.Read(killerid);

	if(playerid < 0 || playerid >= MAX_PLAYERS) 
		return;

	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
	
	// If another player killed
	if(IsPlayerConnected(killerid))
	{
		CPlayer *pKiller = pNetGame->pPlayerPool->pPlayer[killerid];

		// If they doesn't streamed for each other, then won't call OnPlayerDeath
		logprintf("streamed: %d, %d", pKiller->byteStreamedIn[playerid], pPlayer->byteStreamedIn[killerid]);
		if(!pKiller->byteStreamedIn[playerid] || !pPlayer->byteStreamedIn[killerid])
			return;

		//logprintf("syncdata: %d, reason: %d, health: %f", pKiller->syncData.byteWeapon, reasonid, pPlayer->fHealth);
		if( pKiller->syncData.byteWeapon != reasonid && reasonid <= 46 )// 46 = parachute
			return;
		else if( ( reasonid == 48 || reasonid == 49 ) && pKiller->byteState != 2 ) // 48 - carkill // 49 - helikill
			return;

		//logprintf("[kill] %s killed %s %s", pNetGame->pPlayerPool->szName[killerid], pNetGame->pPlayerPool->szName[playerid], GetWeaponName(reasonid));
	}
	else
	{
		//logprintf("[death] %s died %d", pNetGame->pPlayerPool->szName[playerid], reasonid);
	}
	/*
	// Call OnPlayerDeath
	int idx = -1;
	for(std::list<AMX*>::iterator second = pAMXList.begin(); second != pAMXList.end(); ++second)
	{
		if(!amx_FindPublic(*second, "OnPlayerDeath", &idx))
		{
			amx_Push(*second, reasonid);
			amx_Push(*second, killerid);
			amx_Push(*second, playerid);

			amx_Exec(*second, NULL, idx);
		}
	}
	*/
}

void InitRPCs()
{
	pRakServer->UnregisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs);
	pRakServer->RegisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs, UpdateScoresPingsIPs);

//	pRakServer->UnregisterAsRemoteProcedureCall(&RPC_Death);
//	pRakServer->RegisterAsRemoteProcedureCall(&RPC_Death, Death);
}
