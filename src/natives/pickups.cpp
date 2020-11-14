#include "../Natives.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"
#include "../RPCs.h"

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

	static void HidePickup(int pickupid, int world)
	{
		RakNet::BitStream bsPickup;
		bsPickup.Write(pickupid);

		for(WORD playerid = 0; playerid != MAX_PLAYERS; ++playerid)
		{
			if(!IsPlayerConnected(playerid)) continue;

			if(pNetGame->pPlayerPool->dwVirtualWorld[playerid] == static_cast<DWORD>(world))
			{
				CSAMPFunctions::RPC(&RPC_CreatePickup, &bsPickup, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
			}
		}
	}

	static void ShowPickup(int pickupid, int world, tPickup &pickup)
	{
		RakNet::BitStream bsPickup;
		bsPickup.Write(pickupid);
		bsPickup.Write(pickup.iModel);
		bsPickup.Write(pickup.iType);
		bsPickup.Write(pickup.vecPos.fX);
		bsPickup.Write(pickup.vecPos.fY);
		bsPickup.Write(pickup.vecPos.fZ);

		for(WORD playerid = 0; playerid != MAX_PLAYERS; ++playerid)
		{
			if(!IsPlayerConnected(playerid)) continue;

			if(pNetGame->pPlayerPool->dwVirtualWorld[playerid] == static_cast<DWORD>(world))
			{
				CSAMPFunctions::RPC(&RPC_CreatePickup, &bsPickup, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
			}
		}
	}

	// native SetPickupPos(pickupid, Float:X, Float:Y, Float:Z);
	AMX_DECLARE_NATIVE(SetPickupPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if(id < 0 || id >= MAX_PICKUPS)
			return 0;

		if(!pNetGame->pPickupPool->bActive[id]) return 0;

		auto &pickup = pNetGame->pPickupPool->Pickup[id];
		CScriptParams::Get()->Read(pickup.vecPos);

		ShowPickup(id, pNetGame->pPickupPool->iWorld[id], pickup);

		return 1;
	}

	// native SetPickupModel(pickupid, model);
	AMX_DECLARE_NATIVE(SetPickupModel)
	{
		CHECK_PARAMS(2, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if(id < 0 || id >= MAX_PICKUPS)
			return 0;

		if(!pNetGame->pPickupPool->bActive[id]) return 0;

		auto &pickup = pNetGame->pPickupPool->Pickup[id];
		CScriptParams::Get()->Read(pickup.iModel);

		ShowPickup(id, pNetGame->pPickupPool->iWorld[id], pickup);

		return 1;
	}

	// native SetPickupType(pickupid, type);
	AMX_DECLARE_NATIVE(SetPickupType)
	{
		CHECK_PARAMS(2, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if(id < 0 || id >= MAX_PICKUPS)
			return 0;

		if(!pNetGame->pPickupPool->bActive[id]) return 0;

		auto &pickup = pNetGame->pPickupPool->Pickup[id];
		CScriptParams::Get()->Read(pickup.iType);

		ShowPickup(id, pNetGame->pPickupPool->iWorld[id], pickup);

		return 1;
	}

	// native SetPickupVirtualWorld(pickupid, virtualworld);
	AMX_DECLARE_NATIVE(SetPickupVirtualWorld)
	{
		CHECK_PARAMS(2, LOADED);

		const int id = CScriptParams::Get()->ReadInt();
		if(id < 0 || id >= MAX_PICKUPS)
			return 0;

		if(!pNetGame->pPickupPool->bActive[id]) return 0;

		auto &world = pNetGame->pPickupPool->iWorld[id];
		HidePickup(id, world);
		CScriptParams::Get()->Read(world);
		ShowPickup(id, world, pNetGame->pPickupPool->Pickup[id]);

		return 1;
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

	AMX_DEFINE_NATIVE(SetPickupPos)
	AMX_DEFINE_NATIVE(SetPickupModel)
	AMX_DEFINE_NATIVE(SetPickupType)
	AMX_DEFINE_NATIVE(SetPickupVirtualWorld)
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
