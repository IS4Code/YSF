#include "../main.h"
#include "../Natives.h"

// native TogglePlayerScoresPingsUpdate(playerid, bool:toggle);
AMX_DECLARE_NATIVE(Natives::TogglePlayerScoresPingsUpdate)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const bool toggle = CScriptParams::Get()->ReadBool();

	if (!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->bUpdateScoresPingsDisabled = !toggle;
	return 1;
}

// native TogglePlayerFakePing(playerid, bool:toggle);
AMX_DECLARE_NATIVE(Natives::TogglePlayerFakePing)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	bool toggle = CScriptParams::Get()->ReadBool();

	if (!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->bFakePingToggle = toggle;
	return 1;
}

// native SetPlayerFakePing(playerid, ping);
AMX_DECLARE_NATIVE(Natives::SetPlayerFakePing)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	int fakeping = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->dwFakePingValue = fakeping;
	return 1;
}

// native SetPlayerNameInServerQuery(playerid, const name[]);
AMX_DECLARE_NATIVE(Natives::SetPlayerNameInServerQuery)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	std::string name;
	CScriptParams::Get()->Read(&name);
	if (name.length() >= MAX_PLAYER_NAME) return 0;

	pPlayerData[playerid]->strNameInQuery = std::move(name);
	return 1;
}

// native GetPlayerNameInServerQuery(playerid, name[], len = sizeof(name));
AMX_DECLARE_NATIVE(Natives::GetPlayerNameInServerQuery)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CScriptParams::Get()->Add(pPlayerData[playerid]->strNameInQuery);
	return 1;
}
