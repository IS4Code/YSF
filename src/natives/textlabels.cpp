#include "../Natives.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"
#include "../Utils.h"
#include "../Globals.h"

namespace Natives
{
	// native IsValid3DTextLabel(Text3D:id);
	AMX_DECLARE_NATIVE(IsValid3DTextLabel)
	{
		CHECK_PARAMS(1, LOADED);

		int id = CScriptParams::Get()->ReadInt();
		if (0 < id || id >= MAX_3DTEXT_GLOBAL) return 0;

		return pNetGame->p3DTextPool->bIsCreated[id];
	}

	// native Is3DTextLabelStreamedIn(playerid, Text3D:id);
	AMX_DECLARE_NATIVE(Is3DTextLabelStreamedIn)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (0 < id || id >= MAX_3DTEXT_PLAYER) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->byte3DTextLabelStreamedIn[id];
	}

	// native Get3DTextLabelText(id, text[], len = sizeof(text));
	AMX_DECLARE_NATIVE(Get3DTextLabelText)
	{
		CHECK_PARAMS(3, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

		if (!pNetGame->p3DTextPool->bIsCreated[id]) return 0;

		const char *szText = pNetGame->p3DTextPool->TextLabels[id].szText;
		return set_amxstring(amx, params[2], szText, params[3]);
	}

	// native Get3DTextLabelColor(id);
	AMX_DECLARE_NATIVE(Get3DTextLabelColor)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

		if (!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
		C3DText &p3DText = pNetGame->p3DTextPool->TextLabels[id];

		return p3DText.dwColor;
	}

	// native Get3DTextLabelPos(id, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(Get3DTextLabelPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

		if (!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
		C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

		CScriptParams::Get()->Add(p3DText.vecPos);
		return 1;
	}

	// native Float:Get3DTextLabelDrawDistance(id);
	AMX_DECLARE_NATIVE(Get3DTextLabelDrawDistance)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

		if (!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
		C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

		return amx_ftoc(p3DText.fDrawDistance);
	}

	// native Get3DTextLabelLOS(id);
	AMX_DECLARE_NATIVE(Get3DTextLabelLOS)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

		if (!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
		C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

		return p3DText.bLineOfSight;
	}

	// native Get3DTextLabelVirtualWorld(id);
	AMX_DECLARE_NATIVE(Get3DTextLabelVirtualWorld)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

		if (!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
		C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

		return p3DText.iWorld;
	}

	// native Get3DTextLabelAttachedData(id, &attached_playerid, &attached_vehicleid);
	AMX_DECLARE_NATIVE(Get3DTextLabelAttachedData)
	{
		CHECK_PARAMS(3, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_3DTEXT_GLOBAL) return 0;

		if (!pNetGame->p3DTextPool->bIsCreated[id]) return 0;
		C3DText p3DText = pNetGame->p3DTextPool->TextLabels[id];

		CScriptParams::Get()->Add(p3DText.wAttachedToPlayerID, p3DText.wAttachedToVehicleID);
		return 1;
	}

	// native IsValidPlayer3DTextLabel(playerid, PlayerText3D:id);
	AMX_DECLARE_NATIVE(IsValidPlayer3DTextLabel)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id];
	}

	// native GetPlayer3DTextLabelText(playerid, PlayerText3D:id, text[], len = sizeof(text));
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelText)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

		const char *szText = (pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id].szText) ? pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id].szText : "";
		return set_amxstring(amx, params[3], szText, params[4]);
	}

	// native GetPlayer3DTextLabelColor(playerid, PlayerText3D:id);
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelColor)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

		C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
		return p3DText.dwColor;
	}

	// native GetPlayer3DTextLabelPos(playerid, PlayerText3D:id, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelPos)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;
		C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];

		CScriptParams::Get()->Add(p3DText.vecPos);
		return 1;
	}

	// native Float:GetPlayer3DTextLabelDrawDist(playerid, PlayerText3D:id);
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelDrawDist)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

		C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
		return amx_ftoc(p3DText.fDrawDistance);
	}

	// native GetPlayer3DTextLabelLOS(playerid, PlayerText3D:id);
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelLOS)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

		C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
		return p3DText.bLineOfSight;
	}

	// native GetPlayer3DTextLabelVirtualW(playerid, PlayerText3D:id);
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelVirtualW)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;

		C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];
		return p3DText.iWorld;
	}

	// native GetPlayer3DTextLabelAttached(playerid, PlayerText3D:id, &attached_playerid, &attached_vehicleid);
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelAttached)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_3DTEXT_PLAYER) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->isCreated[id]) return 0;
		C3DText p3DText = pNetGame->pPlayerPool->pPlayer[playerid]->p3DText->TextLabels[id];

		CScriptParams::Get()->Add(p3DText.wAttachedToPlayerID, p3DText.wAttachedToVehicleID);
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// 3D Text
	AMX_DEFINE_NATIVE(IsValid3DTextLabel) // R4
	AMX_DEFINE_NATIVE(Is3DTextLabelStreamedIn) // R9
	AMX_DEFINE_NATIVE(Get3DTextLabelText)
	AMX_DEFINE_NATIVE(Get3DTextLabelColor)
	AMX_DEFINE_NATIVE(Get3DTextLabelPos)
	AMX_DEFINE_NATIVE(Get3DTextLabelDrawDistance)
	AMX_DEFINE_NATIVE(Get3DTextLabelLOS)
	AMX_DEFINE_NATIVE(Get3DTextLabelVirtualWorld)
	AMX_DEFINE_NATIVE(Get3DTextLabelAttachedData)

	// Per-Player 3D Text
	AMX_DEFINE_NATIVE(IsValidPlayer3DTextLabel) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelText) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelColor) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelPos) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelDrawDist)
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelLOS) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelVirtualW) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelAttached) // R9
};

void TextLabelsLoadNatives()
{
	RegisterNatives(native_list);
}
