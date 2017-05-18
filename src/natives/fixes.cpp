#include "../main.h"
#include "../Natives.h"

// native GetWeaponSlot(weaponid);
AMX_DECLARE_NATIVE(Natives::GetWeaponSlot)
{
	CHECK_PARAMS(1, NO_FLAGS);

	return Utility::GetWeaponSlot(CScriptParams::Get()->ReadInt());
}

// native GetWeaponName(weaponid, weaponname[], len = sizeof(weaponname));
AMX_DECLARE_NATIVE(Natives::FIXED_GetWeaponName)
{
	CHECK_PARAMS(3, NO_FLAGS);

	return set_amxstring(amx, params[2], Utility::GetWeaponName(static_cast<BYTE>(params[1])), params[3]);
}

// native IsPlayerConnected(playerid);
AMX_DECLARE_NATIVE(Natives::FIXED_IsPlayerConnected)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid] != NULL;
}
