#include "../Natives.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

namespace Natives
{
#ifdef NEW_PICKUP_SYSTEM
	// native IsValidPickup(pickupid);
	AMX_DECLARE_NATIVE(IsValidPickup)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		return CPlugin::Get()->pPickupPool->FindPickup(id) != 0;
	}

	// native IsPickupStreamedIn(playerid, pickupid);
	AMX_DECLARE_NATIVE(IsPickupStreamedIn)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(id);
		if (!pPickup) return 0;

		return CPlugin::Get()->pPickupPool->IsStreamed(playerid, pPickup);
	}

	// native GetPickupPos(pickupid, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(GetPickupPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(id);
		if (!pPickup) return 0;

		CScriptParams::Get()->Add(pPickup->vecPos);
		return 1;
	}

	// native GetPickupModel(pickupid);
	AMX_DECLARE_NATIVE(GetPickupModel)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(id);
		if (!pPickup) return 0;

		return pPickup->iModel;
	}

	// native GetPickupType(pickupid);
	AMX_DECLARE_NATIVE(GetPickupType)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(id);
		if (!pPickup) return 0;

		return pPickup->iType;
	}

	// native GetPickupVirtualWorld(pickupid);
	AMX_DECLARE_NATIVE(GetPickupVirtualWorld)
	{
		CHECK_PARAMS(1, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(id);
		if (!pPickup) return 0;

		return pPickup->iWorld;
	}

	// CreatePlayerPickup(playerid, model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
	AMX_DECLARE_NATIVE(CreatePlayerPickup)
	{
		CHECK_PARAMS(7, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return CPlugin::Get()->pPickupPool->New(playerid, (int)params[2], (int)params[3], CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])), (int)params[7]);
	}

	// native DestroyPlayerPickup(playerid, pickupid);
	AMX_DECLARE_NATIVE(DestroyPlayerPickup)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(playerid, id);
		if (!pPickup) return 0;

		CPlugin::Get()->pPickupPool->Destroy((WORD)playerid, id);
		return 1;
	}

	// native IsValidPlayerPickup(playerid, pickupid);
	AMX_DECLARE_NATIVE(IsValidPlayerPickup)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		return CPlugin::Get()->pPickupPool->FindPickup(playerid, id) != 0;
	}

	// native IsPlayerPickupStreamedIn(playerid, pickupid);
	AMX_DECLARE_NATIVE(IsPlayerPickupStreamedIn)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(playerid, id);
		if (!pPickup) return 0;

		return CPlugin::Get()->pPickupPool->IsStreamed(playerid, pPickup);
	}

	// native GetPlayerPickupPos(playerid, pickupid, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(GetPlayerPickupPos)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(playerid, id);
		if (!pPickup) return 0;

		CScriptParams::Get()->Add(pPickup->vecPos);
		return 1;
	}

	// native GetPlayerPickupModel(playerid, pickupid);
	AMX_DECLARE_NATIVE(GetPlayerPickupModel)
	{
		if (!CPlugin::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(playerid, id);
		if (!pPickup) return 0;

		return pPickup->iModel;
	}

	// native GetPlayerPickupType(playerid, pickupid);
	AMX_DECLARE_NATIVE(GetPlayerPickupType)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(playerid, id);
		if (!pPickup) return 0;

		return pPickup->iType;
	}

	// native GetPlayerPickupVirtualWorld(playerid, pickupid);
	AMX_DECLARE_NATIVE(GetPlayerPickupVirtualWorld)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int id = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;
		if (id < 0 || id >= MAX_PICKUPS) return 0;

		CPickup *pPickup = CPlugin::Get()->pPickupPool->FindPickup(playerid, id);
		if (!pPickup) return 0;

		return pPickup->iWorld;
	}

	// native SetPickupStreamingEnabled(enabled);
	AMX_DECLARE_NATIVE(SetPickupStreamingEnabled)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->pPickupPool->SetStreamingEnabled(!!params[1]);
		return 1;
	}
#else
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
#endif
}

namespace Original
{
	AMX_NATIVE CreatePickup;
	AMX_NATIVE DestroyPickup;
	AMX_NATIVE AddStaticPickup;
}

namespace Hooks
{
#ifdef NEW_PICKUP_SYSTEM
	// native CreatePickup(model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
	AMX_DECLARE_NATIVE(CreatePickup)
	{
		CHECK_PARAMS(6, LOADED);

		return CPlugin::Get()->pPickupPool->New((int)params[1], (int)params[2], CVector(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5])), (int)params[6]);
	}

	constexpr AMX_NATIVE AddStaticPickup = CreatePickup;

	// native DestroyPickup(pickupid);
	AMX_DECLARE_NATIVE(DestroyPickup)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->pPickupPool->Destroy((int)params[1]);
		return 1;
	}
#endif
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
#ifdef NEW_PICKUP_SYSTEM
	// Pickups - Per-player
	AMX_DEFINE_NATIVE(CreatePlayerPickup) // R10
	AMX_DEFINE_NATIVE(DestroyPlayerPickup) // R10
	AMX_DEFINE_NATIVE(IsValidPlayerPickup) // R10
	AMX_DEFINE_NATIVE(IsPlayerPickupStreamedIn) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupPos) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupModel) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupType) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupVirtualWorld) // R10
	AMX_DEFINE_NATIVE(SetPickupStreamingEnabled)
#endif
};

#ifdef NEW_PICKUP_SYSTEM
static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(CreatePickup)
	AMX_DEFINE_HOOK(AddStaticPickup)
	AMX_DEFINE_HOOK(DestroyPickup)
};
#endif

void PickupsLoadNatives()
{
	RegisterNatives(native_list);
#ifdef NEW_PICKUP_SYSTEM
	RegisterHooks(hook_list);
#endif
}
