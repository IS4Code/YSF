#include "../Natives.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CConfig.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

#define CHECK_PER_PLAYER_ZONES_STATE() \
	if (!CConfig::Get()->m_bUsePerPlayerGangZones) \
	{ \
		logprintf("YSF: Per player gangzones are disabled, you aren't allowed to use \"%s\" function!", __FUNCTION__); \
		return 0; \
	} \

namespace Natives
{
	// native IsValidGangZone(zoneid);
	AMX_DECLARE_NATIVE(IsValidGangZone)
	{
		CHECK_PARAMS(1, LOADED);

		const int zoneid = CScriptParams::Get()->ReadInt();
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if(CConfig::Get()->m_bUsePerPlayerGangZones)
		{
			return CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid));
		}else{
			return pNetGame->pGangZonePool->bSlotState[static_cast<WORD>(zoneid)] != 0;
		}
	}

	// native IsGangZoneVisibleForPlayer(playerid, zoneid);
	AMX_DECLARE_NATIVE(IsGangZoneVisibleForPlayer)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

		return !!(CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid)) != 0xFF);
	}

	// native GangZoneGetColorForPlayer(playerid, zoneid);
	AMX_DECLARE_NATIVE(GangZoneGetColorForPlayer)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).dwClientSideZoneColor[id];
		}
		return 0;
	}

	// native GangZoneGetFlashColorForPlayer(playerid, zoneid);
	AMX_DECLARE_NATIVE(GangZoneGetFlashColorForPlayer)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).dwClientSideZoneFlashColor[id];
		}
		return 0;
	}

	// native IsGangZoneFlashingForPlayer(playerid, zoneid);
	AMX_DECLARE_NATIVE(IsGangZoneFlashingForPlayer)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).bIsGangZoneFlashing[id];
		}
		return 0;
	}

	// native IsPlayerInGangZone(playerid, zoneid);
	AMX_DECLARE_NATIVE(IsPlayerInGangZone)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid));
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).bInGangZone[id];
		}
		return 0;
	}

	// native GangZoneGetPos(zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);
	AMX_DECLARE_NATIVE(GangZoneGetPos)
	{
		CHECK_PARAMS(5, LOADED);

		const int zoneid = CScriptParams::Get()->ReadInt();
		if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
		if(CConfig::Get()->m_bUsePerPlayerGangZones)
		{
			if(!CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

			auto &zone = CPlugin::Get()->pGangZonePool->pGangZone[zoneid]->fGangZone;
			CScriptParams::Get()->Add(zone[0], zone[1], zone[2], zone[3]);
		}else{
			if(!pNetGame->pGangZonePool->bSlotState[static_cast<WORD>(zoneid)]) return 0;

			auto &zone = pNetGame->pGangZonePool->fGangZone[static_cast<WORD>(zoneid)];
			CScriptParams::Get()->Add(zone[0], zone[1], zone[2], zone[3]);
		}
		return 1;
	}


	// native CreatePlayerGangZone(playerid, Float:minx, Float:miny, Float:maxx, Float:maxy);
	AMX_DECLARE_NATIVE(CreatePlayerGangZone)
	{
		CHECK_PARAMS(5, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		float fMinX, fMinY, fMaxX, fMaxY;
		CScriptParams::Get()->Read(fMinX, fMinY, fMaxX, fMaxY);

		// If coordinates are wrong, then won't create bugged zone!
		if (fMaxX <= fMinX || fMaxY <= fMinY)
		{
			logprintf("CreatePlayerGangZone: MaxX, MaxY must be bigger than MinX, MinY. Not inversely!");
			logprintf("CreatePlayerGangZone: %f, %f, %f, %f", fMinX, fMinY, fMaxX, fMaxY);
			return -1;
		}

		WORD ret = CPlugin::Get()->pGangZonePool->New(static_cast<WORD>(playerid), fMinX, fMinY, fMaxX, fMaxY);
		if (ret == 0xFFFF) return -1;

		return ret;
	}

	// native PlayerGangZoneShow(playerid, zoneid, color);
	AMX_DECLARE_NATIVE(PlayerGangZoneShow)
	{
		CHECK_PARAMS(3, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		int color = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		CPlugin::Get()->pGangZonePool->ShowForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(color), true);
		return 1;
	}

	// native PlayerGangZoneHide(playerid, zoneid);
	AMX_DECLARE_NATIVE(PlayerGangZoneHide)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		CPlugin::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
		return 1;
	}

	// native PlayerGangZoneFlash(playerid, zoneid, color);
	AMX_DECLARE_NATIVE(PlayerGangZoneFlash)
	{
		CHECK_PARAMS(3, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		int color = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		CPlugin::Get()->pGangZonePool->FlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(color), true);
		return 1;
	}

	// native PlayerGangZoneStopFlash(playerid, zoneid);
	AMX_DECLARE_NATIVE(PlayerGangZoneStopFlash)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		CPlugin::Get()->pGangZonePool->StopFlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
		return 1;
	}

	// native PlayerGangZoneDestroy(playerid, zoneid);
	AMX_DECLARE_NATIVE(PlayerGangZoneDestroy)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), true);
		return 1;
	}

	// native IsValidPlayerGangZone(playerid, zoneid);
	AMX_DECLARE_NATIVE(IsValidPlayerGangZone)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid] != NULL;
	}

	// native IsPlayerInPlayerGangZone(playerid, zoneid);
	AMX_DECLARE_NATIVE(IsPlayerInPlayerGangZone)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).bInGangZone[id];
		}
		return 0;
	}

	// native PlayerGangZoneGetPos(playerid, zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);
	AMX_DECLARE_NATIVE(PlayerGangZoneGetPos)
	{
		CHECK_PARAMS(6, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		const int zoneid = CScriptParams::Get()->ReadInt();
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
		if (id != 0xFFFF)
		{
			CGangZone *pGangZone = CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid];
			CScriptParams::Get()->Add(pGangZone->fGangZone[0], pGangZone->fGangZone[1], pGangZone->fGangZone[2], pGangZone->fGangZone[3]);
			return 1;
		}
		return 0;
	}

	// native IsPlayerGangZoneVisible(playerid, zoneid);
	AMX_DECLARE_NATIVE(IsPlayerGangZoneVisible)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true) != 0xFFFF;
	}

	// native PlayerGangZoneGetColor(playerid, zoneid);
	AMX_DECLARE_NATIVE(PlayerGangZoneGetColor)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).dwClientSideZoneColor[id];
		}
		return 0;
	}

	// native PlayerGangZoneGetFlashColor(playerid, zoneid);
	AMX_DECLARE_NATIVE(PlayerGangZoneGetFlashColor)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).dwClientSideZoneFlashColor[id];
		}
		return 0;
	}

	// native IsPlayerGangZoneFlashing(playerid, zoneid);
	AMX_DECLARE_NATIVE(IsPlayerGangZoneFlashing)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if (!CServer::Get()->PlayerPool.Extra(playerid).pPlayerZone[zoneid]) return 0;

		WORD id = CServer::Get()->PlayerPool.Extra(playerid).GetGangZoneIDFromClientSide(static_cast<WORD>(zoneid), true);
		if (id != 0xFFFF)
		{
			return CServer::Get()->PlayerPool.Extra(playerid).bIsGangZoneFlashing[id];
		}
		return 0;
	}
}

namespace Original
{
	AMX_NATIVE GangZoneCreate;
	AMX_NATIVE GangZoneDestroy;
	AMX_NATIVE GangZoneShowForPlayer;
	AMX_NATIVE GangZoneHideForPlayer;
	AMX_NATIVE GangZoneShowForAll;
	AMX_NATIVE GangZoneHideForAll;
	AMX_NATIVE GangZoneFlashForPlayer;
	AMX_NATIVE GangZoneFlashForAll;
	AMX_NATIVE GangZoneStopFlashForPlayer;
	AMX_NATIVE GangZoneStopFlashForAll;
}

namespace Hooks
{
	AMX_DECLARE_NATIVE(GangZoneCreate)
	{
		CHECK_PARAMS(4, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		float fMinX, fMinY, fMaxX, fMaxY;
		CScriptParams::Get()->Read(fMinX, fMinY, fMaxX, fMaxY);

		// If coordinates are wrong, then won't create bugged zone!
		if (fMaxX <= fMinX || fMaxY <= fMinY)
		{
			//logprintf("GangZoneCreate: MaxX, MaxY must be bigger than MinX, MinY. Not inversely!");
			//logprintf("GangZoneCreate: %f, %f, %f, %f",fMinX, fMinY, fMaxX, fMaxY);
			return -1;
		}

		WORD ret = CPlugin::Get()->pGangZonePool->New(fMinX, fMinY, fMaxX, fMaxY);

		return (ret == 0xFFFF) ? -1 : ret;
	}

	AMX_DECLARE_NATIVE(GangZoneDestroy)
	{
		CHECK_PARAMS(1, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		CGangZonePool *pGangZonePool = CPlugin::Get()->pGangZonePool;
		const int zoneid = CScriptParams::Get()->ReadInt();
		if (!pGangZonePool || !pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

		pGangZonePool->Delete(static_cast<WORD>(zoneid));
		return 1;
	}

	// native GangZoneShowForPlayer(playerid, zone, color);
	AMX_DECLARE_NATIVE(GangZoneShowForPlayer)
	{
		CHECK_PARAMS(3, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();

		// For security
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		return CPlugin::Get()->pGangZonePool->ShowForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(CScriptParams::Get()->ReadInt()));
	}

	// native GangZoneHideForPlayer(playerid, zone);
	AMX_DECLARE_NATIVE(GangZoneHideForPlayer)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();

		// For security
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->HideForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), false, true);
		return 1;
	}

	// native GangZoneShowForAll(zone, color);
	AMX_DECLARE_NATIVE(GangZoneShowForAll)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int zoneid = CScriptParams::Get()->ReadInt();
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->ShowForAll(static_cast<WORD>(zoneid), static_cast<DWORD>(params[2]));
		return 1;
	}

	// native GangZoneHideForAll(zone);
	AMX_DECLARE_NATIVE(GangZoneHideForAll)
	{
		CHECK_PARAMS(1, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int zoneid = CScriptParams::Get()->ReadInt();
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->HideForAll(static_cast<WORD>(zoneid));
		return 1;
	}

	AMX_DECLARE_NATIVE(GangZoneFlashForPlayer)
	{
		CHECK_PARAMS(3, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();

		// For security
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->FlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid), static_cast<DWORD>(CScriptParams::Get()->ReadInt()));
		return 1;
	}

	AMX_DECLARE_NATIVE(GangZoneFlashForAll)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int zoneid = CScriptParams::Get()->ReadInt();
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->FlashForAll(static_cast<WORD>(zoneid), static_cast<DWORD>(params[2]));
		return 1;
	}

	AMX_DECLARE_NATIVE(GangZoneStopFlashForPlayer)
	{
		CHECK_PARAMS(2, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int playerid = CScriptParams::Get()->ReadInt();
		const int zoneid = CScriptParams::Get()->ReadInt();

		// For security
		if (!IsPlayerConnected(playerid)) return 0;
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->StopFlashForPlayer(static_cast<WORD>(playerid), static_cast<WORD>(zoneid));
		return 1;
	}

	AMX_DECLARE_NATIVE(GangZoneStopFlashForAll)
	{
		CHECK_PARAMS(1, LOADED);
		CHECK_PER_PLAYER_ZONES_STATE()

		const int zoneid = CScriptParams::Get()->ReadInt();
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		CPlugin::Get()->pGangZonePool->StopFlashForAll(static_cast<WORD>(zoneid));
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// Gangzone - Global
	AMX_DEFINE_NATIVE(IsValidGangZone)
	AMX_DEFINE_NATIVE(IsPlayerInGangZone)
	AMX_DEFINE_NATIVE(IsGangZoneVisibleForPlayer)
	AMX_DEFINE_NATIVE(GangZoneGetColorForPlayer)
	AMX_DEFINE_NATIVE(GangZoneGetFlashColorForPlayer)
	AMX_DEFINE_NATIVE(IsGangZoneFlashingForPlayer) // R6
	AMX_DEFINE_NATIVE(GangZoneGetPos)

	// Gangzone - Player
	AMX_DEFINE_NATIVE(CreatePlayerGangZone)
	AMX_DEFINE_NATIVE(PlayerGangZoneDestroy)
	AMX_DEFINE_NATIVE(PlayerGangZoneShow)
	AMX_DEFINE_NATIVE(PlayerGangZoneHide)
	AMX_DEFINE_NATIVE(PlayerGangZoneFlash)
	AMX_DEFINE_NATIVE(PlayerGangZoneStopFlash)
	AMX_DEFINE_NATIVE(IsValidPlayerGangZone)
	AMX_DEFINE_NATIVE(IsPlayerInPlayerGangZone)
	AMX_DEFINE_NATIVE(IsPlayerGangZoneVisible)
	AMX_DEFINE_NATIVE(PlayerGangZoneGetColor)
	AMX_DEFINE_NATIVE(PlayerGangZoneGetFlashColor)
	AMX_DEFINE_NATIVE(IsPlayerGangZoneFlashing) // R6
	AMX_DEFINE_NATIVE(PlayerGangZoneGetPos)
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(GangZoneCreate)
	AMX_DEFINE_HOOK(GangZoneDestroy)
	AMX_DEFINE_HOOK(GangZoneShowForPlayer)
	AMX_DEFINE_HOOK(GangZoneHideForPlayer)
	AMX_DEFINE_HOOK(GangZoneShowForAll)
	AMX_DEFINE_HOOK(GangZoneHideForAll)
	AMX_DEFINE_HOOK(GangZoneFlashForPlayer)
	AMX_DEFINE_HOOK(GangZoneFlashForAll)
	AMX_DEFINE_HOOK(GangZoneStopFlashForPlayer)
	AMX_DEFINE_HOOK(GangZoneStopFlashForAll)
};

void GangZonesLoadNatives()
{
	RegisterNatives(native_list);
	if (CConfig::Get()->m_bUsePerPlayerGangZones)
	{
		RegisterHooks(hook_list);
	}
}
