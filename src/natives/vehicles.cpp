#include "../Natives.h"
#include "../includes/platform.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

namespace Natives
{
	// native GetVehicleSpawnInfo(vehicleid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRot, &color1, &color2);
	AMX_DECLARE_NATIVE(GetVehicleSpawnInfo)
	{
		CHECK_PARAMS(7, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		CVehicle* pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];
		if (!pVehicle)
			return 0;

		CVehicleSpawn spawn;
		bool custom = CServer::Get()->VehiclePool.MapExtra
		(
			vehicleid, [&spawn](CVehicleData &data)
			{
				if (data.customSpawnData)
				{
					spawn.vecPos = data.spawnData.vecPos;
					spawn.fRot = data.spawnData.fRot;
					spawn.iColor1 = data.spawnData.iColor1;
					spawn.iColor2 = data.spawnData.iColor2;
					return true;
				}
				return false;
			}
		);
		if (!custom)
		{
			spawn.vecPos = pVehicle->customSpawn.vecPos;
			spawn.fRot = pVehicle->customSpawn.fRot;
			spawn.iColor1 = pVehicle->customSpawn.iColor1;
			spawn.iColor2 = pVehicle->customSpawn.iColor2;
		}
		CScriptParams::Get()->Add(spawn.vecPos, spawn.fRot, spawn.iColor1, spawn.iColor2);
		return 1;
	}

	// native SetVehicleSpawnInfo(vehicleid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, respawntime = -2, interior = -2);
	AMX_DECLARE_NATIVE(SetVehicleSpawnInfo)
	{
		if (!CAddress::FUNC_CVehicle__Respawn) return 0;
		CHECK_PARAMS(10, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		int modelid = CScriptParams::Get()->ReadInt();
		if (modelid < 400 || modelid > 611) return 0;

		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];
		if (!pVehicle)
			return 0;

		bool bStreamedIn = false;
		CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;

		for (WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
			{
				if (pPlayerPool->pPlayer[i]->byteVehicleStreamedIn[pVehicle->wVehicleID])
				{
					bStreamedIn = true;
					break;
				}
			}
		}

		CVehicleSpawn spawn;
		spawn.iModelID = modelid;
		CScriptParams::Get()->Read(spawn.vecPos, spawn.fRot, spawn.iColor1, spawn.iColor2);
		spawn.iRespawnTime = pVehicle->customSpawn.iRespawnTime;
		spawn.iInterior = pVehicle->customSpawn.iInterior;

		// logprintf("spawndata: %d, %d", spawn.iRespawnTime, spawn.iInterior);

		int respawntime = CScriptParams::Get()->ReadInt();
		if (respawntime >= -1)
		{
			spawn.iRespawnTime = respawntime > 0 ? respawntime * 1000 : respawntime;
		}

		int interior = CScriptParams::Get()->ReadInt();
		if (interior != -2)
		{
			spawn.iInterior = interior;
		}

		CVehicleData &data = CServer::Get()->VehiclePool.Extra(vehicleid);
		data.customSpawnData = true;
		data.spawnData = spawn;

		// logprintf("streamedin: %d, iRespawnTime: %d, interior: %d", bStreamedIn, respawntime, interior);

		if (!bStreamedIn)
		{
			pVehicle->customSpawn.iModelID = spawn.iModelID;
			pVehicle->customSpawn.fRot = spawn.fRot;
			pVehicle->customSpawn.iColor1 = spawn.iColor1;
			pVehicle->customSpawn.iColor2 = spawn.iColor2;
			pVehicle->customSpawn.iRespawnTime = spawn.iRespawnTime;
			pVehicle->customSpawn.iInterior = spawn.iInterior;
		}
		return 1;
	}

	// native GetVehicleModelCount(modelid);
	AMX_DECLARE_NATIVE(GetVehicleModelCount)
	{
		CHECK_PARAMS(1, LOADED);

		const int modelid = CScriptParams::Get()->ReadInt();
		if (modelid < 400 || modelid > 611) return 0;

		return pNetGame->pVehiclePool->byteVehicleModelsUsed[modelid - 400];
	}

	// native GetVehicleModelsUsed();
	AMX_DECLARE_NATIVE(GetVehicleModelsUsed)
	{
		if (!CPlugin::IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		BYTE byteModelsUsed = 0;
		for (BYTE i = 0; i != 212; ++i)
		{
			if (pNetGame->pVehiclePool->byteVehicleModelsUsed[i] != 0)
				byteModelsUsed++;
		}

		return byteModelsUsed;
	}

	// native GetVehicleColor(vehicleid, &color1, &color2);
	AMX_DECLARE_NATIVE(GetVehicleColor)
	{
		CHECK_PARAMS(3, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];
		
		bool changed = CServer::Get()->VehiclePool.MapExtra(vehicleid, [](CVehicleData &data) {return data.bChangedVehicleColor; });
		
		int color1 = changed ? pVehicle->vehModInfo.iColor1 : pVehicle->customSpawn.iColor1;
		int color2 = changed ? pVehicle->vehModInfo.iColor2 : pVehicle->customSpawn.iColor2;

		CScriptParams::Get()->Add(color1, color2);
		return 1;
	}

	// native GetVehiclePaintjob(vehicleid);
	AMX_DECLARE_NATIVE(GetVehiclePaintjob)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehModInfo.bytePaintJob - 1;
	}

	// native GetVehicleInterior(vehicleid);
	AMX_DECLARE_NATIVE(GetVehicleInterior)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iInterior;
	}

	// native GetVehicleNumberPlate(vehicleid, plate[], len = sizeof(plate));
	AMX_DECLARE_NATIVE(GetVehicleNumberPlate)
	{
		CHECK_PARAMS(3, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		char szPlate[32 + 1];
		if (pNetGame->pVehiclePool->pVehicle[vehicleid]->szNumberplate[0])
		{
			strcpy(szPlate, pNetGame->pVehiclePool->pVehicle[vehicleid]->szNumberplate);
		}
		else
		{
			strcpy(szPlate, "XYZSR998");
		}
		CScriptParams::Get()->Add(szPlate);
		return 1;
	}

	// native SetVehicleRespawnDelay(vehicleid, delay);
	AMX_DECLARE_NATIVE(SetVehicleRespawnDelay)
	{
		CHECK_PARAMS(2, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		cell delay = CScriptParams::Get()->ReadInt();

		pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iRespawnTime = delay > 0 ? delay * 1000 : delay;
		return 1;
	}

	// native GetVehicleRespawnDelay(vehicleid);
	AMX_DECLARE_NATIVE(GetVehicleRespawnDelay)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		int delay = pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iRespawnTime;

		return delay > 0 ? delay / 1000 : delay;
	}

	// native SetVehicleOccupiedTick(vehicleid, ticks);
	AMX_DECLARE_NATIVE(SetVehicleOccupiedTick)
	{
		CHECK_PARAMS(2, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		pNetGame->pVehiclePool->pVehicle[vehicleid]->vehOccupiedTick = CScriptParams::Get()->ReadInt();
		return 1;
	}

	// native GetVehicleOccupiedTick(vehicleid);
	AMX_DECLARE_NATIVE(GetVehicleOccupiedTick)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehOccupiedTick;
	}

	// native SetVehicleRespawnTick(vehicleid, ticks);
	AMX_DECLARE_NATIVE(SetVehicleRespawnTick)
	{
		CHECK_PARAMS(2, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		pNetGame->pVehiclePool->pVehicle[vehicleid]->vehRespawnTick = CScriptParams::Get()->ReadInt();
		return 1;
	}

	// native GetVehicleRespawnTick(vehicleid);
	AMX_DECLARE_NATIVE(GetVehicleRespawnTick)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehRespawnTick;
	}

	// native GetVehicleLastDriver(vehicleid);
	AMX_DECLARE_NATIVE(GetVehicleLastDriver)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->wLastDriverID;
	}

	// native GetVehicleCab(vehicleid);
	AMX_DECLARE_NATIVE(GetVehicleCab)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		CVehicle *pVeh;
		for (WORD i = 0; i != MAX_VEHICLES; ++i)
		{
			pVeh = pNetGame->pVehiclePool->pVehicle[i];
			if (!pVeh) continue;

			if (pVeh->wTrailerID != 0 && pVeh->wTrailerID == vehicleid)
				return i;
		}
		return 0;
	}

	// native HasVehicleBeenOccupied(vehicleid);
	AMX_DECLARE_NATIVE(HasVehicleBeenOccupied)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->bOccupied;
	}

	// native SetVehicleBeenOccupied(vehicleid, occupied);
	AMX_DECLARE_NATIVE(SetVehicleBeenOccupied)
	{
		CHECK_PARAMS(2, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		pNetGame->pVehiclePool->pVehicle[vehicleid]->bOccupied = static_cast<BYTE>(params[2]) != 0;
		return 1;
	}

	// native IsVehicleOccupied(vehicleid);
	AMX_DECLARE_NATIVE(IsVehicleOccupied)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		CPlayer *pPlayer;
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			if (!IsPlayerConnected(i)) continue;
			pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (pPlayer->wVehicleId == vehicleid && (pPlayer->byteState == PLAYER_STATE_DRIVER || pPlayer->byteState == PLAYER_STATE_PASSENGER))
				return 1;
		}
		return 0;
	}

	// native IsVehicleDead(vehicleid);
	AMX_DECLARE_NATIVE(IsVehicleDead)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->bDead;
	}

	// native SetVehicleDead(vehicleid, bool:dead);
	AMX_DECLARE_NATIVE(SetVehicleDead)
	{
		CHECK_PARAMS(2, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if(!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		pNetGame->pVehiclePool->pVehicle[vehicleid]->bDead = CScriptParams::Get()->ReadBool();
		return 1;
	}

	// native SetVehicleParamsSirenState(vehicleid, sirenState);
	AMX_DECLARE_NATIVE(SetVehicleParamsSirenState)
	{
		CHECK_PARAMS(2, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		BYTE sirenState = static_cast<BYTE>(params[2]);

		pNetGame->pVehiclePool->pVehicle[vehicleid]->vehParamEx.siren = sirenState;
		return 1;
	}

	// native ToggleVehicleSirenEnabled(vehicleid, enabled);
	AMX_DECLARE_NATIVE(ToggleVehicleSirenEnabled)
	{
		CHECK_PARAMS(2, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		BYTE enabled = static_cast<BYTE>(params[2]);

		pNetGame->pVehiclePool->pVehicle[vehicleid]->byteSirenEnabled = enabled;
		return 1;
	}

	// native IsVehicleSirenEnabled(vehicleid);
	AMX_DECLARE_NATIVE(IsVehicleSirenEnabled)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		return pNetGame->pVehiclePool->pVehicle[vehicleid]->byteSirenEnabled;
	}

	// native GetVehicleMatrix(vehicleid, &Float:rightX, &Float:rightY, &Float:rightZ, &Float:upX, &Float:upY, &Float:upZ, &Float:atX, &Float:atY, &Float:atZ);
	AMX_DECLARE_NATIVE(GetVehicleMatrix)
	{
		CHECK_PARAMS(10, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		MATRIX4X4* matrix = &pNetGame->pVehiclePool->pVehicle[vehicleid]->vehMatrix;

		// IS4 Note for the future - vehicles remotely updated have the "at" vector zeroed, which corrupts results from GetVehicleRotationQuat
		// Might be a reasonable method to replace IsValidQuaternion from i_quat
		CScriptParams::Get()->Add(matrix->right, matrix->up, matrix->at);
		return 1;
	}
}

namespace Original
{
	AMX_NATIVE ChangeVehicleColor;
	AMX_NATIVE DestroyVehicle;
}

namespace Hooks
{
	// native ChangeVehicleColor(vehicleid, color1, color2)
	AMX_DECLARE_NATIVE(ChangeVehicleColor)
	{
		CHECK_PARAMS(3, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (Original::ChangeVehicleColor(amx, params))
		{
			CServer::Get()->VehiclePool.Extra(vehicleid).bChangedVehicleColor = true;
			return 1;
		}
		return 0;
	}

	// native DestroyVehicle(vehicleid);
	AMX_DECLARE_NATIVE(DestroyVehicle)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if (Original::DestroyVehicle(amx, params))
		{
			CServer::Get()->VehiclePool.RemoveExtra(vehicleid);
			return 1;
		}
		return 0;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(GetVehicleSpawnInfo)
	AMX_DEFINE_NATIVE(SetVehicleSpawnInfo) // R16
	AMX_DEFINE_NATIVE(GetVehicleColor)
	AMX_DEFINE_NATIVE(GetVehiclePaintjob)
	AMX_DEFINE_NATIVE(GetVehicleInterior)
	AMX_DEFINE_NATIVE(GetVehicleNumberPlate)
	AMX_DEFINE_NATIVE(SetVehicleRespawnDelay)
	AMX_DEFINE_NATIVE(GetVehicleRespawnDelay)
	AMX_DEFINE_NATIVE(SetVehicleOccupiedTick) // R6
	AMX_DEFINE_NATIVE(GetVehicleOccupiedTick)
	AMX_DEFINE_NATIVE(SetVehicleRespawnTick)
	AMX_DEFINE_NATIVE(GetVehicleRespawnTick)
	AMX_DEFINE_NATIVE(GetVehicleLastDriver)
	AMX_DEFINE_NATIVE(GetVehicleCab) // R9
	AMX_DEFINE_NATIVE(HasVehicleBeenOccupied) // R9
	AMX_DEFINE_NATIVE(SetVehicleBeenOccupied) // R9
	AMX_DEFINE_NATIVE(IsVehicleOccupied) // R9
	AMX_DEFINE_NATIVE(IsVehicleDead) // R9
	AMX_DEFINE_NATIVE(SetVehicleDead) // R20
	AMX_DEFINE_NATIVE(SetVehicleParamsSirenState) // R19
	AMX_DEFINE_NATIVE(ToggleVehicleSirenEnabled) // R19
	AMX_DEFINE_NATIVE(IsVehicleSirenEnabled) // R19
	AMX_DEFINE_NATIVE(GetVehicleMatrix) // R19
	AMX_DEFINE_NATIVE(GetVehicleModelCount) // R17
	AMX_DEFINE_NATIVE(GetVehicleModelsUsed) // R17
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(ChangeVehicleColor)
	AMX_DEFINE_HOOK(DestroyVehicle)
};

void VehiclesLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
