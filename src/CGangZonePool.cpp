#include "main.h"

CGangZonePool::CGangZonePool()
{
	// Set every gang zone pointer to NULL
	memset(pGangZone, NULL, sizeof(pGangZone));
}

CGangZonePool::~CGangZonePool()
{
	for (WORD i = 0; i != MAX_GANG_ZONES; i++)
	{
		SAFE_DELETE(pGangZone[i]);
	}
}

WORD CGangZonePool::New(float fMinX, float fMinY, float fMaxX, float fMaxY)
{
	WORD wZone = 0;
	while (wZone < MAX_GANG_ZONES)
	{
		if (!pGangZone[wZone]) break;
		wZone++;
	}
	if (wZone == MAX_GANG_ZONES) return 0xFFFF;
	
	// Allocate memory for gangzone
	pGangZone[wZone] = new CGangZone();
	pGangZone[wZone]->fGangZone[0] = fMinX;
	pGangZone[wZone]->fGangZone[1] = fMinY;
	pGangZone[wZone]->fGangZone[2] = fMaxX;
	pGangZone[wZone]->fGangZone[3] = fMaxY;
	return wZone;
}

WORD CGangZonePool::New(WORD playerid, float fMinX, float fMinY, float fMaxX, float fMaxY)
{
	WORD wZone = 0;
	while (wZone < MAX_GANG_ZONES)
	{
		if (!pPlayerData[playerid]->pPlayerZone[wZone]) break;
		wZone++;
	}
	if (wZone == MAX_GANG_ZONES) return 0xFFFF;
		
	// Allocate memory for gangzone
	pPlayerData[playerid]->pPlayerZone[wZone] = new CGangZone();
	pPlayerData[playerid]->pPlayerZone[wZone]->fGangZone[0] = fMinX;
	pPlayerData[playerid]->pPlayerZone[wZone]->fGangZone[1] = fMinY;
	pPlayerData[playerid]->pPlayerZone[wZone]->fGangZone[2] = fMaxX;
	pPlayerData[playerid]->pPlayerZone[wZone]->fGangZone[3] = fMaxY;
	return wZone;
}

void CGangZonePool::Delete(WORD wZone)
{
	for (WORD i = 0; i != MAX_PLAYERS; i++)
	{
		// Skip not connected players
		if (!IsPlayerConnectedEx(i)) continue;

		HideForPlayer(i, wZone);
	}
	
	SAFE_DELETE(pGangZone[wZone]);
}

void CGangZonePool::Delete(WORD playerid, WORD wZone)
{
	HideForPlayer(playerid, wZone, true);

	SAFE_DELETE(pPlayerData[playerid]->pPlayerZone[wZone]);
}

bool CGangZonePool::ShowForPlayer(WORD playerid, WORD wZone, DWORD dwColor, bool bPlayerZone)
{	
	// Find free zone slot on player, client side
	WORD i = 0;;
	CGangZone *pZone = NULL;

	while(i != MAX_GANG_ZONES)
	{
		if(pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 0xFF) break;
		i++;
	}
	if (i == MAX_GANG_ZONES) return 0;

	// Mark client side zone id as used
	if(!bPlayerZone)
	{
		pZone = pGangZone[wZone];
		if(!pZone) return 0;

		// Hide the old one, if showed
		HideForPlayer(playerid, wZone, false);

		pPlayerData[playerid]->byteClientSideZoneIDUsed[i] = 0;
		pPlayerData[playerid]->wClientSideGlobalZoneID[i] = wZone;
		
	}
	else
	{
		pZone = pPlayerData[playerid]->pPlayerZone[wZone];
		if(!pZone) return 0;

		// Hide the old one, if showed
		HideForPlayer(playerid, wZone, true);

		pPlayerData[playerid]->byteClientSideZoneIDUsed[i] = 1;
		pPlayerData[playerid]->wClientSidePlayerZoneID[i] = wZone;
	}
	pPlayerData[playerid]->dwClientSideZoneColor[i] = dwColor;

	//logprintf("CGangZonePool::ShowForPlayer playerid = %d, zoneid = %d, clientsideid = %d, bPlayerZone = %d", playerid, wZone, i, bPlayerZone);

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	bsParams.Write(pZone->fGangZone[0]);
	bsParams.Write(pZone->fGangZone[1]);
	bsParams.Write(pZone->fGangZone[2]);
	bsParams.Write(pZone->fGangZone[3]);
	bsParams.Write(RGBA_ABGR(dwColor));
	pRakServer->RPC(&RPC_ShowGangZone, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

void CGangZonePool::ShowForAll(WORD wZone, DWORD dwColor)
{
	for(WORD playerid = 0; playerid != MAX_PLAYERS; playerid++)
	{
		// Skip not connected players
		if(!IsPlayerConnectedEx(playerid)) continue;

		WORD i = 0;

		while(i != MAX_GANG_ZONES)
		{
			if(pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 0xFF) break;
			i++;
		}
		if (i == MAX_GANG_ZONES) return;

		// Hide the old one, if showed
		HideForPlayer(playerid, wZone);

		// Mark client side zone id as used
		pPlayerData[playerid]->byteClientSideZoneIDUsed[i] = 0;
		pPlayerData[playerid]->wClientSideGlobalZoneID[i] = wZone;
		pPlayerData[playerid]->dwClientSideZoneColor[i] = dwColor;

		RakNet::BitStream bsParams;
		bsParams.Write(i);
		bsParams.Write(pGangZone[wZone]->fGangZone[0]);
		bsParams.Write(pGangZone[wZone]->fGangZone[1]);
		bsParams.Write(pGangZone[wZone]->fGangZone[2]);
		bsParams.Write(pGangZone[wZone]->fGangZone[3]);
		bsParams.Write(RGBA_ABGR(dwColor));
		pRakServer->RPC(&RPC_ShowGangZone, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
	}
}

bool CGangZonePool::HideForPlayer(WORD playerid, WORD wZone, bool bPlayerZone)
{
	WORD i = 0;

	// Mark client side zone id as unused
	if(!bPlayerZone)
	{
		while(i != MAX_GANG_ZONES)
		{
			if(pPlayerData[playerid]->wClientSideGlobalZoneID[i] == wZone && pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 0) break;
			i++;
		}
		if(i == MAX_GANG_ZONES) return 0;

		if (pPlayerData[playerid]->bInGangZone[i])
		{
			CCallbackManager::OnPlayerLeaveGangZone(playerid, pPlayerData[playerid]->wClientSideGlobalZoneID[i]);
		}

		pPlayerData[playerid]->wClientSideGlobalZoneID[i] = 0xFFFF;
	}
	else
	{
		while(i != MAX_GANG_ZONES)
		{
			if(pPlayerData[playerid]->wClientSidePlayerZoneID[i] == wZone && pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 1) break;
			i++;
		}
		if(i == MAX_GANG_ZONES) return 0;
		
		if (pPlayerData[playerid]->bInGangZone[i])
		{
			CCallbackManager::OnPlayerLeavePlayerGangZone(playerid, pPlayerData[playerid]->wClientSidePlayerZoneID[i]);
		}

		pPlayerData[playerid]->wClientSidePlayerZoneID[i] = 0xFFFF;
	}
	if (i == MAX_GANG_ZONES) return 0;

	pPlayerData[playerid]->byteClientSideZoneIDUsed[i] = 0xFF;
	pPlayerData[playerid]->dwClientSideZoneColor[i] = 0;
	pPlayerData[playerid]->bInGangZone[i] = false;
	pPlayerData[playerid]->bIsGangZoneFlashing[i] = false;

	//logprintf("CGangZonePool::HideForPlayer playerid = %d, zoneid = %d, clientsideid = %d, bPlayerZone = %d", playerid, wZone, i, bPlayerZone);

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	pRakServer->RPC(&RPC_HideGangZone, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

void CGangZonePool::HideForAll(WORD wZone)
{
	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		// Skip not connected players
		if(!IsPlayerConnectedEx(i)) continue;

		HideForPlayer(i, wZone);
	}
}

void CGangZonePool::FlashForPlayer(WORD playerid, WORD wZone, DWORD dwColor, bool bPlayerZone)
{
	WORD i = 0;

	// check id
	if(!bPlayerZone)
	{
		while(i != MAX_GANG_ZONES)
		{
			if(pPlayerData[playerid]->wClientSideGlobalZoneID[i] == wZone && pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 0) break;
			i++;
		}
	}
	else
	{
		while(i != MAX_GANG_ZONES)
		{
			if(pPlayerData[playerid]->wClientSidePlayerZoneID[i] == wZone && pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 1) break;
			i++;
		}
	}
	if (i == MAX_GANG_ZONES) return;

	pPlayerData[playerid]->dwClientSideZoneFlashColor[i] = dwColor;
	pPlayerData[playerid]->bIsGangZoneFlashing[i] = true;

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	bsParams.Write(RGBA_ABGR(dwColor));
	pRakServer->RPC(&RPC_FlashGangZone, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
}

void CGangZonePool::FlashForAll(WORD wZone, DWORD dwColor)
{
	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		// Skip not connected players
		if(!IsPlayerConnectedEx(i)) continue;

		FlashForPlayer(i, wZone, dwColor);
	}
}

void CGangZonePool::StopFlashForPlayer(WORD playerid, WORD wZone, bool bPlayerZone)
{
	WORD i = 0;

	// check id
	if(!bPlayerZone)
	{
		while(i != MAX_GANG_ZONES)
		{
			if(pPlayerData[playerid]->wClientSideGlobalZoneID[i] == wZone && pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 0) break;
			i++;
		}
	}
	else
	{
		while(i != MAX_GANG_ZONES)
		{
			if(pPlayerData[playerid]->wClientSidePlayerZoneID[i] == wZone && pPlayerData[playerid]->byteClientSideZoneIDUsed[i] == 1) break;
			i++;
		}
	}
	if (i == MAX_GANG_ZONES) return;

	pPlayerData[playerid]->dwClientSideZoneFlashColor[i] = 0;
	pPlayerData[playerid]->bIsGangZoneFlashing[i] = true;

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	pRakServer->RPC(&RPC_StopFlashGangZone, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(playerid), false, false);
}

void CGangZonePool::StopFlashForAll(WORD wZone)
{
	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		// Skip not connected players
		if(!IsPlayerConnectedEx(i)) continue;

		StopFlashForPlayer(i, wZone);
	}
}
