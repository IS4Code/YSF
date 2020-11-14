#include "CGangZonePool.h"
#include "includes/platform.h"
#include "CServer.h"
#include "CFunctions.h"
#include "CCallbackManager.h"
#include "Utils.h"
#include "Globals.h"
#include "RPCs.h"

CGangZonePool::CGangZonePool()
{
	// Set every gang zone pointer to NULL
	memset(pGangZone, NULL, sizeof(pGangZone));
}

CGangZonePool::~CGangZonePool()
{
	for (WORD i = 0; i != MAX_GANG_ZONES; ++i)
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
	auto &data = CServer::Get()->PlayerPool.Extra(playerid);
	while (wZone < MAX_GANG_ZONES)
	{
		if (!data.pPlayerZone[wZone]) break;
		wZone++;
	}
	if (wZone == MAX_GANG_ZONES) return 0xFFFF;
		
	// Allocate memory for gangzone
	data.pPlayerZone[wZone] = new CGangZone();
	data.pPlayerZone[wZone]->fGangZone[0] = fMinX;
	data.pPlayerZone[wZone]->fGangZone[1] = fMinY;
	data.pPlayerZone[wZone]->fGangZone[2] = fMaxX;
	data.pPlayerZone[wZone]->fGangZone[3] = fMaxY;
	return wZone;
}

void CGangZonePool::Delete(WORD wZone)
{
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
	{
		// Skip not connected players
		if (!IsPlayerConnected(i)) continue;

		HideForPlayer(i, wZone);
	}
	
	SAFE_DELETE(pGangZone[wZone]);
}

void CGangZonePool::Delete(WORD playerid, WORD wZone)
{
	HideForPlayer(playerid, wZone, true);

	SAFE_DELETE(CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[wZone]);
}

bool CGangZonePool::ShowForPlayer(WORD playerid, WORD wZone, DWORD dwColor, bool bPlayerZone)
{	
	// Find free zone slot on player, client side
	WORD i = 0;
	CGangZone *pZone = NULL;
	auto &data = CServer::Get()->PlayerPool.Extra(playerid);
	while(i != MAX_GANG_ZONES)
	{
		if(data.byteClientSideZoneIDUsed[i] == 0xFF) break;
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

		data.byteClientSideZoneIDUsed[i] = 0;
		data.wClientSideGlobalZoneID[i] = wZone;
		
	}
	else
	{
		pZone = data.pPlayerZone[wZone];
		if(!pZone) return 0;

		// Hide the old one, if showed
		HideForPlayer(playerid, wZone, true);

		data.byteClientSideZoneIDUsed[i] = 1;
		data.wClientSidePlayerZoneID[i] = wZone;
	}
	data.dwClientSideZoneColor[i] = dwColor;

	//logprintf("CGangZonePool::ShowForPlayer playerid = %d, zoneid = %d, clientsideid = %d, bPlayerZone = %d", playerid, wZone, i, bPlayerZone);

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	bsParams.Write(pZone->fGangZone[0]);
	bsParams.Write(pZone->fGangZone[1]);
	bsParams.Write(pZone->fGangZone[2]);
	bsParams.Write(pZone->fGangZone[3]);
	bsParams.Write(RGBA_ABGR(dwColor));
	CSAMPFunctions::RPC(&RPC_ShowGangZone, &bsParams, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

void CGangZonePool::ShowForAll(WORD wZone, DWORD dwColor)
{
	auto &pool = CServer::Get()->PlayerPool;
	for(WORD playerid = 0; playerid != MAX_PLAYERS; ++playerid)
	{
		// Skip not connected players
		if(!IsPlayerConnected(playerid)) continue;

		WORD i = 0;
		auto &data = pool.Extra(playerid);

		while(i != MAX_GANG_ZONES)
		{
			if(data.byteClientSideZoneIDUsed[i] == 0xFF) break;
			i++;
		}
		if (i == MAX_GANG_ZONES) return;

		// Hide the old one, if showed
		if(data.byteClientSideZoneIDUsed[i] != 0xFF)
			HideForPlayer(playerid, wZone, false, false);

		// Mark client side zone id as used
		data.byteClientSideZoneIDUsed[i] = 0;
		data.wClientSideGlobalZoneID[i] = wZone;
		data.dwClientSideZoneColor[i] = dwColor;

		RakNet::BitStream bsParams;
		bsParams.Write(i);
		bsParams.Write(pGangZone[wZone]->fGangZone[0]);
		bsParams.Write(pGangZone[wZone]->fGangZone[1]);
		bsParams.Write(pGangZone[wZone]->fGangZone[2]);
		bsParams.Write(pGangZone[wZone]->fGangZone[3]);
		bsParams.Write(RGBA_ABGR(dwColor));
		CSAMPFunctions::RPC(&RPC_ShowGangZone, &bsParams, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	}
}

bool CGangZonePool::HideForPlayer(WORD playerid, WORD wZone, bool bPlayerZone, bool bCallCallback)
{
	WORD i = 0;
	auto &data = CServer::Get()->PlayerPool.Extra(playerid);

	// Mark client side zone id as unused
	if(!bPlayerZone)
	{
		while(i != MAX_GANG_ZONES)
		{
			if(data.wClientSideGlobalZoneID[i] == wZone && data.byteClientSideZoneIDUsed[i] == 0) break;
			i++;
		}
		if(i == MAX_GANG_ZONES) return 0;

		if (data.bInGangZone[i] && bCallCallback)
		{
			CCallbackManager::OnPlayerLeaveGangZone(playerid, data.wClientSideGlobalZoneID[i]);
		}

		data.wClientSideGlobalZoneID[i] = 0xFFFF;
	}
	else
	{
		while(i != MAX_GANG_ZONES)
		{
			if(data.wClientSidePlayerZoneID[i] == wZone && data.byteClientSideZoneIDUsed[i] == 1) break;
			i++;
		}
		if(i == MAX_GANG_ZONES) return 0;
		
		if (data.bInGangZone[i] && bCallCallback)
		{
			CCallbackManager::OnPlayerLeavePlayerGangZone(playerid, data.wClientSidePlayerZoneID[i]);
		}

		data.wClientSidePlayerZoneID[i] = 0xFFFF;
	}
	if (i == MAX_GANG_ZONES) return 0;

	data.byteClientSideZoneIDUsed[i] = 0xFF;
	data.dwClientSideZoneColor[i] = 0;
	data.bInGangZone[i] = false;
	data.bIsGangZoneFlashing[i] = false;

	//logprintf("CGangZonePool::HideForPlayer playerid = %d, zoneid = %d, clientsideid = %d, bPlayerZone = %d", playerid, wZone, i, bPlayerZone);

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	CSAMPFunctions::RPC(&RPC_HideGangZone, &bsParams, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

void CGangZonePool::HideForAll(WORD wZone)
{
	for(WORD i = 0; i != MAX_PLAYERS; ++i)
	{
		// Skip not connected players
		if(!IsPlayerConnected(i)) continue;

		HideForPlayer(i, wZone, false, true);
	}
}

void CGangZonePool::FlashForPlayer(WORD playerid, WORD wZone, DWORD dwColor, bool bPlayerZone)
{
	WORD i = 0;
	auto &data = CServer::Get()->PlayerPool.Extra(playerid);

	// check id
	if(!bPlayerZone)
	{
		while(i != MAX_GANG_ZONES)
		{
			if(data.wClientSideGlobalZoneID[i] == wZone && data.byteClientSideZoneIDUsed[i] == 0) break;
			i++;
		}
	}
	else
	{
		while(i != MAX_GANG_ZONES)
		{
			if(data.wClientSidePlayerZoneID[i] == wZone && data.byteClientSideZoneIDUsed[i] == 1) break;
			i++;
		}
	}
	if (i == MAX_GANG_ZONES) return;

	data.dwClientSideZoneFlashColor[i] = dwColor;
	data.bIsGangZoneFlashing[i] = true;

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	bsParams.Write(RGBA_ABGR(dwColor));
	CSAMPFunctions::RPC(&RPC_FlashGangZone, &bsParams, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
}

void CGangZonePool::FlashForAll(WORD wZone, DWORD dwColor)
{
	for(WORD i = 0; i != MAX_PLAYERS; ++i)
	{
		// Skip not connected players
		if(!IsPlayerConnected(i)) continue;

		FlashForPlayer(i, wZone, dwColor);
	}
}

void CGangZonePool::StopFlashForPlayer(WORD playerid, WORD wZone, bool bPlayerZone)
{
	WORD i = 0;
	auto &data = CServer::Get()->PlayerPool.Extra(playerid);

	// check id
	if(!bPlayerZone)
	{
		while(i != MAX_GANG_ZONES)
		{
			if(data.wClientSideGlobalZoneID[i] == wZone && data.byteClientSideZoneIDUsed[i] == 0) break;
			i++;
		}
	}
	else
	{
		while(i != MAX_GANG_ZONES)
		{
			if(data.wClientSidePlayerZoneID[i] == wZone && data.byteClientSideZoneIDUsed[i] == 1) break;
			i++;
		}
	}
	if (i == MAX_GANG_ZONES) return;

	data.dwClientSideZoneFlashColor[i] = 0;
	data.bIsGangZoneFlashing[i] = true;

	RakNet::BitStream bsParams;
	bsParams.Write(i);
	CSAMPFunctions::RPC(&RPC_StopFlashGangZone, &bsParams, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
}

void CGangZonePool::StopFlashForAll(WORD wZone)
{
	for(WORD i = 0; i != MAX_PLAYERS; ++i)
	{
		// Skip not connected players
		if(!IsPlayerConnected(i)) continue;

		StopFlashForPlayer(i, wZone);
	}
}
