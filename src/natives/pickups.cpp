#include "../Natives.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

#ifndef NEW_PICKUP_SYSTEM
namespace Natives
{
	// native IsValidPickup(pickupid);
	AMX_DECLARE_NATIVE(IsValidPickup)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS)
			return 0;

		return pNetGame->pPickupPool->bActive[id];
	}

	// native IsPickupStreamedIn(playerid, pickupid);
	AMX_DECLARE_NATIVE(IsPickupStreamedIn)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int pickupid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (pickupid < 0 || pickupid >= MAX_PICKUPS) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->bPickupStreamedIn[pickupid];
	}

	// native GetPickupPos(pickupid, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(GetPickupPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS)
			return 0;

		if (!pNetGame->pPickupPool->bActive[id]) return 0;

		CScriptParams::Get()->Add(pNetGame->pPickupPool->Pickup[id].vecPos);
		return 1;
	}

	// native GetPickupModel(pickupid);
	AMX_DECLARE_NATIVE(GetPickupModel)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS)
			return 0;

		if (!pNetGame->pPickupPool->bActive[id]) return 0;

		return pNetGame->pPickupPool->Pickup[id].iModel;
	}

	// native GetPickupType(pickupid);
	AMX_DECLARE_NATIVE(GetPickupType)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS)
			return 0;

		if (!pNetGame->pPickupPool->bActive[id]) return 0;

		return pNetGame->pPickupPool->Pickup[id].iType;
	}

	// native GetPickupVirtualWorld(pickupid);
	AMX_DECLARE_NATIVE(GetPickupVirtualWorld)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS)
			return 0;

		if (!pNetGame->pPickupPool->bActive[id]) return 0;

		return pNetGame->pPickupPool->iWorld[id];
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// Pickups - Global
	AMX_DEFINE_NATIVE(IsValidPickup) // R10
	AMX_DEFINE_NATIVE(IsPickupStreamedIn) // R10
	AMX_DEFINE_NATIVE(GetPickupPos) // R10
	AMX_DEFINE_NATIVE(GetPickupModel) // R10
	AMX_DEFINE_NATIVE(GetPickupType) // R10
	AMX_DEFINE_NATIVE(GetPickupVirtualWorld) // R10
};

void PickupsLoadNatives()
{
	RegisterNatives(native_list);
}
#else
void PickupsLoadNatives()
{

}
#endif
