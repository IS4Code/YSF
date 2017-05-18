#include "../main.h"
#include "../Natives.h"

// native IsValidGangZone(zoneid);
AMX_DECLARE_NATIVE(Natives::IsValidGangZone)
{
	CHECK_PARAMS(1, LOADED);

	const int zoneid = CScriptParams::Get()->ReadInt();
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	return CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid));
}

// native IsGangZoneVisibleForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsGangZoneVisibleForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	return !!(pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid)) != 0xFF);
}

// native GangZoneGetColorForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::GangZoneGetColorForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->dwClientSideZoneColor[id];
	}
	return 0;
}

// native GangZoneGetFlashColorForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::GangZoneGetFlashColorForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->dwClientSideZoneFlashColor[id];
	}
	return 0;
}

// native IsGangZoneFlashingForPlayer(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsGangZoneFlashingForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->bIsGangZoneFlashing[id];
	}
	return 0;
}

// native IsPlayerInGangZone(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerInGangZone)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->bInGangZone[id];
	}
	return 0;
}

// native GangZoneGetPos(zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);
AMX_DECLARE_NATIVE(Natives::GangZoneGetPos)
{
	CHECK_PARAMS(5, LOADED);

	const int zoneid = CScriptParams::Get()->ReadInt();
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!CServer::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid)))  return 0;

	CGangZone *pGangZone = CServer::Get()->pGangZonePool->pGangZone[zoneid];
	CScriptParams::Get()->Add(pGangZone->fGangZone[0], pGangZone->fGangZone[1], pGangZone->fGangZone[2], pGangZone->fGangZone[3]);
	return 1;
}


// native CreatePlayerGangZone(playerid, Float:minx, Float:miny, Float:maxx, Float:maxy);
AMX_DECLARE_NATIVE(Natives::CreatePlayerGangZone)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	float fMinX, fMinY, fMaxX, fMaxY;
	CScriptParams::Get()->Read(&fMinX, &fMinY, &fMaxX, &fMaxY);

	// If coordinates are wrong, then won't create bugged zone!
	if (fMaxX <= fMinX || fMaxY <= fMinY)
	{
		logprintf("CreatePlayerGangZone: MaxX, MaxY must be bigger than MinX, MinY. Not inversely!");
		logprintf("CreatePlayerGangZone: %f, %f, %f, %f", fMinX, fMinY, fMaxX, fMaxY);
		return -1;
	}

	WORD ret = CServer::Get()->pGangZonePool->New(static_cast<WORD>(playerid), fMinX, fMinY, fMaxX, fMaxY);
	if (ret == 0xFFFF) return -1;

	return ret;
}

// native PlayerGangZoneShow(playerid, zoneid, color);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneShow)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	int color = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->ShowForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(color), true);
	return 1;
}

// native PlayerGangZoneHide(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneHide)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
	return 1;
}

// native PlayerGangZoneFlash(playerid, zoneid, color);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneFlash)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	int color = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->FlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(color), true);
	return 1;
}

// native PlayerGangZoneStopFlash(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneStopFlash)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	CServer::Get()->pGangZonePool->StopFlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
	return 1;
}

// native PlayerGangZoneDestroy(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneDestroy)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
	return 1;
}

// native IsValidPlayerGangZone(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsValidPlayerGangZone)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	return pPlayerData[playerid]->pPlayerZone[zoneid] != NULL;
}

// native IsPlayerInPlayerGangZone(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerInPlayerGangZone)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->bInGangZone[id];
	}
	return 0;
}

// native PlayerGangZoneGetPos(playerid, zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneGetPos)
{
	CHECK_PARAMS(6, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	const int zoneid = CScriptParams::Get()->ReadInt();
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if (id != 0xFFFF)
	{
		CGangZone *pGangZone = pPlayerData[playerid]->pPlayerZone[zoneid];
		CScriptParams::Get()->Add(pGangZone->fGangZone[0], pGangZone->fGangZone[1], pGangZone->fGangZone[2], pGangZone->fGangZone[3]);
		return 1;
	}
	return 0;
}

// native IsPlayerGangZoneVisible(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerGangZoneVisible)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	return pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true) != 0xFFFF;
}

// native PlayerGangZoneGetColor(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneGetColor)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->dwClientSideZoneColor[id];
	}
	return 0;
}

// native PlayerGangZoneGetFlashColor(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::PlayerGangZoneGetFlashColor)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->dwClientSideZoneFlashColor[id];
	}
	return 0;
}

// native IsPlayerGangZoneFlashing(playerid, zoneid);
AMX_DECLARE_NATIVE(Natives::IsPlayerGangZoneFlashing)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	if (!pPlayerData[playerid]->pPlayerZone[zoneid]) return 0;

	WORD id = pPlayerData[playerid]->GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
	if (id != 0xFFFF)
	{
		return pPlayerData[playerid]->bIsGangZoneFlashing[id];
	}
	return 0;
}

/* --------------------------- HOOKS --------------------------- */

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneCreate)
{
	CHECK_PARAMS(4, LOADED);

	float fMinX, fMinY, fMaxX, fMaxY;
	CScriptParams::Get()->Read(&fMinX, &fMinY, &fMaxX, &fMaxY);

	// If coordinates are wrong, then won't create bugged zone!
	if (fMaxX <= fMinX || fMaxY <= fMinY)
	{
		//logprintf("GangZoneCreate: MaxX, MaxY must be bigger than MinX, MinY. Not inversely!");
		//logprintf("GangZoneCreate: %f, %f, %f, %f",fMinX, fMinY, fMaxX, fMaxY);
		return -1;
	}

	WORD ret = CServer::Get()->pGangZonePool->New(fMinX, fMinY, fMaxX, fMaxY);

	return (ret == 0xFFFF) ? -1 : ret;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneDestroy)
{
	CHECK_PARAMS(1, LOADED);

	CGangZonePool *pGangZonePool = CServer::Get()->pGangZonePool;
	const int zoneid = CScriptParams::Get()->ReadInt();
	if (!pGangZonePool || !pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

	pGangZonePool->Delete(static_cast<WORD>(zoneid));
	return 1;
}

// native YSF_GangZoneShowForPlayer(playerid, zone, color);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneShowForPlayer)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();

	// For security
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	return CServer::Get()->pGangZonePool->ShowForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(CScriptParams::Get()->ReadInt()));
}

// native YSF_GangZoneHideForPlayer(playerid, zone);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneHideForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();

	// For security
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), false, true);
	return 1;
}

// native YSF_GangZoneShowForAll(zone, color);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneShowForAll)
{
	CHECK_PARAMS(2, LOADED);

	const int zoneid = CScriptParams::Get()->ReadInt();
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->ShowForAll(static_cast<WORD>(zoneid), static_cast<DWORD>(params[2]));
	return 1;
}

// native YSF_GangZoneHideForAll(zone);
AMX_DECLARE_NATIVE(Natives::YSF_GangZoneHideForAll)
{
	CHECK_PARAMS(1, LOADED);

	const int zoneid = CScriptParams::Get()->ReadInt();
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->HideForAll(static_cast<WORD>(zoneid));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneFlashForPlayer)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();

	// For security
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->FlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(CScriptParams::Get()->ReadInt()));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneFlashForAll)
{
	CHECK_PARAMS(2, LOADED);

	const int zoneid = CScriptParams::Get()->ReadInt();
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->FlashForAll(static_cast<WORD>(zoneid), static_cast<DWORD>(params[2]));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneStopFlashForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int zoneid = CScriptParams::Get()->ReadInt();

	// For security
	if (!IsPlayerConnected(playerid)) return 0;
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->StopFlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid));
	return 1;
}

AMX_DECLARE_NATIVE(Natives::YSF_GangZoneStopFlashForAll)
{
	CHECK_PARAMS(1, LOADED);

	const int zoneid = CScriptParams::Get()->ReadInt();
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

	CServer::Get()->pGangZonePool->StopFlashForAll(static_cast<WORD>(zoneid));
	return 1;
}
