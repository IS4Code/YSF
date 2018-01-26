#include "../Natives.h"
#include "../Utils.h"
#include "../CScriptParams.h"
#include "../Structs.h"
#include "../Globals.h"

namespace Natives
{
	// native GetWeaponSlot(weaponid);
	AMX_DECLARE_NATIVE(GetWeaponSlot)
	{
		CHECK_PARAMS(1, NO_FLAGS);

		return Utility::GetWeaponSlot(CScriptParams::Get()->ReadInt());
	}
}

namespace Original
{
	AMX_NATIVE GetWeaponName;
	AMX_NATIVE IsPlayerConnected;
}

namespace Hooks
{
	// native GetWeaponName(weaponid, weaponname[], len = sizeof(weaponname));
	AMX_DECLARE_NATIVE(GetWeaponName)
	{
		CHECK_PARAMS(3, NO_FLAGS);

		return set_amxstring(amx, params[2], Utility::GetWeaponName(static_cast<BYTE>(params[1])), params[3]);
	}

	// native IsPlayerConnected(playerid);
	AMX_DECLARE_NATIVE(IsPlayerConnected)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid] != NULL;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(GetWeaponSlot)
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(GetWeaponName)
	AMX_DEFINE_HOOK(IsPlayerConnected)
};

void FixesLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
