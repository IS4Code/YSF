#include "../main.h"
#include "../Natives.h"

#ifdef NEW_PICKUP_SYSTEM

// native IsValidPickup(pickupid);
AMX_DECLARE_NATIVE(Natives::IsValidPickup)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	return CServer::Get()->pPickupPool->FindPickup(id) != 0;
}

// native IsPickupStreamedIn(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsPickupStreamedIn)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if (!pPickup) return 0;

	return CServer::Get()->pPickupPool->IsStreamed(playerid, pPickup);
}

// native GetPickupPos(pickupid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPickupPos)
{
	CHECK_PARAMS(4, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if (!pPickup) return 0;

	CScriptParams::Get()->Add(pPickup->vecPos);
	return 1;
}

// native GetPickupModel(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupModel)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if (!pPickup) return 0;

	return pPickup->iModel;
}

// native GetPickupType(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupType)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if (!pPickup) return 0;

	return pPickup->iType;
}

// native GetPickupVirtualWorld(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupVirtualWorld)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(id);
	if (!pPickup) return 0;

	return pPickup->iWorld;
}

// CreatePlayerPickup(playerid, model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
AMX_DECLARE_NATIVE(Natives::CreatePlayerPickup)
{
	CHECK_PARAMS(7, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return CServer::Get()->pPickupPool->New(playerid, (int)params[2], (int)params[3], CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])), (int)params[7]);
}

// native DestroyPlayerPickup(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::DestroyPlayerPickup)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if (!pPickup) return 0;

	CServer::Get()->pPickupPool->Destroy((WORD)playerid, id);
	return 1;
}

// native IsValidPlayerPickup(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsValidPlayerPickup)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	return CServer::Get()->pPickupPool->FindPickup(playerid, id) != 0;
}

// native IsPlayerPickupStreamedIn(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsPlayerPickupStreamedIn)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if (!pPickup) return 0;

	return CServer::Get()->pPickupPool->IsStreamed(playerid, pPickup);
}

// native GetPlayerPickupPos(playerid, pickupid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupPos)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if (!pPickup) return 0;

	CScriptParams::Get()->Add(pPickup->vecPos);
	return 1;
}

// native GetPlayerPickupModel(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupModel)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if (!pPickup) return 0;

	return pPickup->iModel;
}

// native GetPlayerPickupType(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupType)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if (!pPickup) return 0;

	return pPickup->iType;
}

// native GetPlayerPickupVirtualWorld(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPickupVirtualWorld)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int id = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (id < 0 || id >= MAX_PICKUPS) return 0;

	CPickup *pPickup = CServer::Get()->pPickupPool->FindPickup(playerid, id);
	if (!pPickup) return 0;

	return pPickup->iWorld;
}

#else

// native IsValidPickup(pickupid);
AMX_DECLARE_NATIVE(Natives::IsValidPickup)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	return pNetGame->pPickupPool->bActive[id];
}

// native IsPickupStreamedIn(playerid, pickupid);
AMX_DECLARE_NATIVE(Natives::IsPickupStreamedIn)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int pickupid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (pickupid < 0 || pickupid >= MAX_PICKUPS) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bPickupStreamedIn[pickupid];
}

// native GetPickupPos(pickupid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPickupPos)
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
AMX_DECLARE_NATIVE(Natives::GetPickupModel)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	if (!pNetGame->pPickupPool->bActive[id]) return 0;

	return pNetGame->pPickupPool->Pickup[id].iModel;
}

// native GetPickupType(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupType)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	if (!pNetGame->pPickupPool->bActive[id]) return 0;

	return pNetGame->pPickupPool->Pickup[id].iType;
}

// native GetPickupVirtualWorld(pickupid);
AMX_DECLARE_NATIVE(Natives::GetPickupVirtualWorld)
{
	CHECK_PARAMS(1, LOADED);

	const int id = CScriptParams::Get()->ReadInt();
	if (id < 0 || id >= MAX_PICKUPS)
		return 0;

	if (!pNetGame->pPickupPool->bActive[id]) return 0;

	return pNetGame->pPickupPool->iWorld[id];
}

#endif

/* --------------------------- HOOKS --------------------------- */

#ifdef NEW_PICKUP_SYSTEM
// native CreatePickup(model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
AMX_DECLARE_NATIVE(Natives::CreatePickup)
{
	CHECK_PARAMS(6, LOADED);

	return CServer::Get()->pPickupPool->New((int)params[1], (int)params[2], CVector(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5])), (int)params[6]);
}

// native DestroyPickup(pickupid);
AMX_DECLARE_NATIVE(Natives::DestroyPickup)
{
	CHECK_PARAMS(1, LOADED);

	CServer::Get()->pPickupPool->Destroy((int)params[1]);
	return 1;
}

// native SetPickupStreamingEnabled(enabled);
AMX_DECLARE_NATIVE(Natives::SetPickupStreamingEnabled)
{
	CHECK_PARAMS(1, LOADED);

	CServer::Get()->pPickupPool->SetStreamingEnabled(!!params[1]);
	return 1;
}
#endif
