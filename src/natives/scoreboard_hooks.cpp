#include "../Natives.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

namespace Natives
{
	// native TogglePlayerScoresPingsUpdate(playerid, bool:toggle);
	AMX_DECLARE_NATIVE(TogglePlayerScoresPingsUpdate)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const bool toggle = CScriptParams::Get()->ReadBool();

		if (!IsPlayerConnected(playerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).bUpdateScoresPingsDisabled = !toggle;
		return 1;
	}

	// native TogglePlayerFakePing(playerid, bool:toggle);
	AMX_DECLARE_NATIVE(TogglePlayerFakePing)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		bool toggle = CScriptParams::Get()->ReadBool();

		if (!IsPlayerConnected(playerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).bFakePingToggle = toggle;
		return 1;
	}

	// native SetPlayerFakePing(playerid, ping);
	AMX_DECLARE_NATIVE(SetPlayerFakePing)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		int fakeping = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).dwFakePingValue = fakeping;
		return 1;
	}

	// native SetPlayerNameInServerQuery(playerid, const name[]);
	AMX_DECLARE_NATIVE(SetPlayerNameInServerQuery)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		std::string name;
		CScriptParams::Get()->Read(name);
		if (name.length() >= MAX_PLAYER_NAME) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		data.bCustomNameInQuery = true;
		data.strNameInQuery = std::move(name);
		return 1;
	}

	// native GetPlayerNameInServerQuery(playerid, name[], len = sizeof(name));
	AMX_DECLARE_NATIVE(GetPlayerNameInServerQuery)
	{
		CHECK_PARAMS(3, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		if (data.bCustomNameInQuery)
		{
			CScriptParams::Get()->Add(data.strNameInQuery);
			return 1;
		}
		else {
			return 0;
		}
	}

	// native native ResetPlayerNameInServerQuery(playerid);
	AMX_DECLARE_NATIVE(ResetPlayerNameInServerQuery)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		data.bCustomNameInQuery = false;
		data.strNameInQuery.clear();
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(TogglePlayerScoresPingsUpdate) // R8
	AMX_DEFINE_NATIVE(TogglePlayerFakePing) // R8
	AMX_DEFINE_NATIVE(SetPlayerFakePing) // R8
	AMX_DEFINE_NATIVE(SetPlayerNameInServerQuery) // R11
	AMX_DEFINE_NATIVE(GetPlayerNameInServerQuery) // R11
	AMX_DEFINE_NATIVE(ResetPlayerNameInServerQuery) // R20
};

void ScoreBoardHooksLoadNatives()
{
	RegisterNatives(native_list);
}
