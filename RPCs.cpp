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
int RPC_UpdateScoresPingsIPs = 0x9B;
int RPC_RemovePlayerAttachedObject = 0x71;

void UpdateScoresPingsIPs(RPCParameters *rpcParams);

void UpdateScoresPingsIPs(RPCParameters *rpcParams)
{
	RakNet::BitStream bsUpdate;

	for(PLAYERID i = 0; i < MAX_PLAYERS; i++)
	{
		if(pNetGame->pPlayerPool->bIsPlayerConnected[i] && pPlayerData[i])
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

void InitRPCs()
{
	pRakServer->UnregisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs);
	pRakServer->RegisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs, UpdateScoresPingsIPs);
}
