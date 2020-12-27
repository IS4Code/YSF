#include "../Natives.h"
#include "../includes/platform.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"
#include "../RPCs.h"
#include "../linux_fix.h"

namespace Natives
{
	// native IsPlayerPaused(playerid);
	AMX_DECLARE_NATIVE(IsPlayerPaused)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).bAFKState;
	}

	// native GetPlayerPausedTime(playerid);
	AMX_DECLARE_NATIVE(GetPlayerPausedTime)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();

		if(!IsPlayerConnected(playerid)) return 0;
		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		if(!data.bAFKState) return 0;

		return static_cast<cell>(std::chrono::duration_cast<std::chrono::milliseconds>(default_clock::now() - data.LastUpdateTick).count());
	}

	// native EnableConsoleMSGsForPlayer(playerid, color);
	AMX_DECLARE_NATIVE(EnableConsoleMSGsForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid, color;
		CScriptParams::Get()->Read(playerid, color);
		if(!IsPlayerConnected(playerid)) return 0;

		CPlugin::Get()->AddConsolePlayer(static_cast<WORD>(playerid), static_cast<DWORD>(color));
		return 1;
	}

	// native DisableConsoleMSGsForPlayer(playerid);
	AMX_DECLARE_NATIVE(DisableConsoleMSGsForPlayer)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlugin::Get()->RemoveConsolePlayer(static_cast<WORD>(playerid));
		return 1;
	}

	// native HasPlayerConsoleMessages(playerid, &color = 0);
	AMX_DECLARE_NATIVE(HasPlayerConsoleMessages)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		DWORD color;
		bool ret = CPlugin::Get()->IsConsolePlayer(static_cast<WORD>(playerid), color);
		CScriptParams::Get()->Add(color);
		return ret;
	}

	// native GetPlayerWeather(playerid);
	AMX_DECLARE_NATIVE(GetPlayerWeather)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).byteWeather;
	}

	// native GetPlayerWorldBounds(playerid, &Float:x_max, &Float:x_min, &Float:y_max, &Float:y_min);
	AMX_DECLARE_NATIVE(GetPlayerWorldBounds)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CScriptParams::Get()->Add(CServer::Get()->PlayerPool.Extra(playerid).fBounds[0], CServer::Get()->PlayerPool.Extra(playerid).fBounds[1], CServer::Get()->PlayerPool.Extra(playerid).fBounds[2], CServer::Get()->PlayerPool.Extra(playerid).fBounds[3]);
		return 1;
	}

	// native IsPlayerControllable(playerid);
	AMX_DECLARE_NATIVE(IsPlayerControllable)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).bControllable;
	}

	// native SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0);
	AMX_DECLARE_NATIVE(SetPlayerDisabledKeysSync)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeys = static_cast<WORD>(CScriptParams::Get()->ReadInt());
		CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysUD = static_cast<WORD>(CScriptParams::Get()->ReadInt());
		CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysLR = static_cast<WORD>(CScriptParams::Get()->ReadInt());
		return 1;
	}

	// native SetExclusiveBroadcast(toggle);
	AMX_DECLARE_NATIVE(SetExclusiveBroadcast)
	{
		CHECK_PARAMS(1, LOADED);

		const int toggle = CScriptParams::Get()->ReadInt();
		CPlugin::Get()->SetExclusiveBroadcast(!!toggle);
		return 1;
	}

	// native BroadcastToPlayer(playerid, toggle = 1);
	AMX_DECLARE_NATIVE(BroadcastToPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int toggle = CScriptParams::Get()->ReadInt();

		if(!IsPlayerConnected(playerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).bBroadcastTo = !!toggle;
		return 1;
	}

	// native GetPlayerBuildingsRemoved(playerid)
	AMX_DECLARE_NATIVE(GetPlayerBuildingsRemoved)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();

		if(!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).GetBuildingsRemoved();
	}

	// native IsBuildingRemovedForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
	AMX_DECLARE_NATIVE(IsBuildingRemovedForPlayer)
	{
		CHECK_PARAMS(6, LOADED);

		int playerid, modelid;
		CVector pos;
		float range;

		CScriptParams::Get()->Read(playerid, modelid, pos, range);

		if(!IsPlayerConnected(playerid)) return 0;

		return static_cast<cell>(CServer::Get()->PlayerPool.Extra(playerid).IsBuildingRemoved(modelid, pos, range, false));
	}

	// native TogglePlayerGhostMode(playerid, bool:toggle);
	AMX_DECLARE_NATIVE(TogglePlayerGhostMode)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		data.ghostMode = CScriptParams::Get()->ReadBool();
		return 1;
	}
}

namespace Original
{
	AMX_NATIVE SetPlayerTeam;
	AMX_NATIVE SetPlayerSkin;
	AMX_NATIVE SetPlayerName;
	AMX_NATIVE SetPlayerWeather;
	AMX_NATIVE SetPlayerFightingStyle;
	AMX_NATIVE SetPlayerWorldBounds;
	AMX_NATIVE TogglePlayerControllable;
	AMX_NATIVE ShowPlayerDialog;
	AMX_NATIVE RemoveBuildingForPlayer;
}

namespace Hooks
{
	AMX_DECLARE_NATIVE(SetPlayerTeam)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(Original::SetPlayerTeam(amx, params))
		{
			auto &pool = CServer::Get()->PlayerPool;
			for(WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if(IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerTeam(static_cast<WORD>(playerid));
			}
			return 1;
		}
		return 0;
	}

	AMX_DECLARE_NATIVE(SetPlayerSkin)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(Original::SetPlayerSkin(amx, params))
		{
			auto &pool = CServer::Get()->PlayerPool;
			for(WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if(IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerSkin(static_cast<WORD>(playerid));
			}
			return 1;
		}
		return 0;
	}

	AMX_DECLARE_NATIVE(SetPlayerName)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int ret = Original::SetPlayerName(amx, params);

		if(ret == 1)
		{
			auto &pool = CServer::Get()->PlayerPool;
			for(WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if(IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerName(static_cast<WORD>(playerid));
			}
		}
		return ret;
	}

	AMX_DECLARE_NATIVE(SetPlayerFightingStyle)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(Original::SetPlayerFightingStyle(amx, params))
		{
			auto &pool = CServer::Get()->PlayerPool;
			for(WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if(IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerFightingStyle(static_cast<WORD>(playerid));
			}
			return 1;
		}
		return 0;
	}

	// native SetPlayerWeather(playerid, weatherid);
	AMX_DECLARE_NATIVE(SetPlayerWeather)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();

		if(Original::SetPlayerWeather(amx, params) && IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).byteWeather = static_cast<BYTE>(CScriptParams::Get()->ReadInt());
			return 1;
		}
		return 0;
	}

	// native SetPlayerWorldBounds(playerid, Float:x_max, Float:x_min, Float:y_max, Float:y_min)
	AMX_DECLARE_NATIVE(SetPlayerWorldBounds)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(Original::SetPlayerWorldBounds(amx, params) && IsPlayerConnected(playerid))
		{
			for(BYTE i = 0; i != 4; ++i)
			{
				CServer::Get()->PlayerPool.Extra(playerid).fBounds[i] = CScriptParams::Get()->ReadFloat();
			}
			return 1;
		}
		return 0;
	}

	// native TogglePlayerControllable(playerid, bool:toggle)
	AMX_DECLARE_NATIVE(TogglePlayerControllable)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const bool toggle = CScriptParams::Get()->ReadBool();

		if(Original::TogglePlayerControllable(amx, params) && IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).bControllable = toggle;
			return 1;
		}
		return 0;
	}

	// native ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]);
	AMX_DECLARE_NATIVE(ShowPlayerDialog)
	{
		CHECK_PARAMS(7, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int dialogid = CScriptParams::Get()->ReadInt();

		if(Original::ShowPlayerDialog(amx, params) && IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).wDialogID = dialogid;
			return 1;
		}
		return 0;
	}

	// native RemoveBuildingForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
	AMX_DECLARE_NATIVE(RemoveBuildingForPlayer)
	{
		CHECK_PARAMS(6, LOADED);

		int playerid, modelid;
		CVector pos;
		float range;

		CScriptParams::Get()->Read(playerid, modelid, pos, range);
		if(IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).SetBuildingsRemoved(modelid, pos, range);
		}
		return Original::RemoveBuildingForPlayer(amx, params);
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(EnableConsoleMSGsForPlayer) // R18
	AMX_DEFINE_NATIVE(DisableConsoleMSGsForPlayer) // R18
	AMX_DEFINE_NATIVE(HasPlayerConsoleMessages) // R18 

	// Exclusive RPC broadcast
	AMX_DEFINE_NATIVE(SetExclusiveBroadcast)
	AMX_DEFINE_NATIVE(BroadcastToPlayer)

	AMX_DEFINE_NATIVE(GetPlayerWeather)
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds)
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds) // R5
	AMX_DEFINE_NATIVE(IsPlayerControllable) // R11

	// AFK
	AMX_DEFINE_NATIVE(IsPlayerPaused)
	AMX_DEFINE_NATIVE(GetPlayerPausedTime)

	AMX_DEFINE_NATIVE(GetPlayerBuildingsRemoved) // R20
	AMX_DEFINE_NATIVE(IsBuildingRemovedForPlayer) // R20
	AMX_DEFINE_NATIVE(TogglePlayerGhostMode) // R20
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(SetPlayerTeam)
	AMX_DEFINE_HOOK(SetPlayerSkin)
	AMX_DEFINE_HOOK(SetPlayerName)
	AMX_DEFINE_HOOK(SetPlayerFightingStyle)
	AMX_DEFINE_HOOK(SetPlayerWeather)
	AMX_DEFINE_HOOK(SetPlayerWorldBounds)
	AMX_DEFINE_HOOK(TogglePlayerControllable)
	AMX_DEFINE_HOOK(ShowPlayerDialog)
	AMX_DEFINE_HOOK(RemoveBuildingForPlayer)
};

void PlayersHooksLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
