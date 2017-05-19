#include "../main.h"
#include "../Natives.h"

// native GetVehicleSpawnInfo(vehicleid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRot, &color1, &color2);
AMX_DECLARE_NATIVE(Natives::GetVehicleSpawnInfo)
{
	CHECK_PARAMS(7, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	CVehicle* pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];
	if (!pVehicle)
		return 0;

	CVehicleSpawn spawn;
	auto v = CServer::Get()->vehicleSpawnData.find(vehicleid);
	if (v == CServer::Get()->vehicleSpawnData.end())
	{
		spawn.vecPos = pVehicle->customSpawn.vecPos;
		spawn.fRot = pVehicle->customSpawn.fRot;
		spawn.iColor1 = pVehicle->customSpawn.iColor1;
		spawn.iColor2 = pVehicle->customSpawn.iColor2;
	}
	else
	{
		spawn.vecPos = v->second.vecPos;
		spawn.fRot = v->second.fRot;
		spawn.iColor1 = v->second.iColor1;
		spawn.iColor2 = v->second.iColor2;
	}
	CScriptParams::Get()->Add(spawn.vecPos, spawn.fRot, spawn.iColor1, spawn.iColor2);
	return 1;
}

// native SetVehicleSpawnInfo(vehicleid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, respawntime = -2, interior = -2);
AMX_DECLARE_NATIVE(Natives::SetVehicleSpawnInfo)
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
	CScriptParams::Get()->Read(&spawn.vecPos, &spawn.fRot, &spawn.iColor1, &spawn.iColor2);
	spawn.iRespawnTime = pVehicle->customSpawn.iRespawnTime;
	spawn.iInterior = pVehicle->customSpawn.iInterior;

	// logprintf("spawndata: %d, %d", spawn.iRespawnTime, spawn.iInterior);

	int respawntime = CScriptParams::Get()->ReadInt();
	if (respawntime >= -1)
	{
		spawn.iRespawnTime = respawntime;
	}

	int interior = CScriptParams::Get()->ReadInt();
	if (interior != -2)
	{
		spawn.iInterior = interior;
	}

	CServer::Get()->vehicleSpawnData[vehicleid] = spawn;

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
AMX_DECLARE_NATIVE(Natives::GetVehicleModelCount)
{
	CHECK_PARAMS(1, LOADED);

	const int modelid = CScriptParams::Get()->ReadInt();
	if (modelid < 400 || modelid > 611) return 0;

	return pNetGame->pVehiclePool->byteVehicleModelsUsed[modelid - 400];
}

// native GetVehicleModelsUsed();
AMX_DECLARE_NATIVE(Natives::GetVehicleModelsUsed)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	BYTE byteModelsUsed = 0;
	for (BYTE i = 0; i != 212; ++i)
	{
		if (pNetGame->pVehiclePool->byteVehicleModelsUsed[i] != 0)
			byteModelsUsed++;
	}

	return byteModelsUsed;
}

// native GetVehicleColor(vehicleid, &color1, &color2);
AMX_DECLARE_NATIVE(Natives::GetVehicleColor)
{
	CHECK_PARAMS(3, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];
	const int color1 = CServer::Get()->bChangedVehicleColor[vehicleid] ? pVehicle->vehModInfo.iColor1 : pVehicle->customSpawn.iColor1;
	const int color2 = CServer::Get()->bChangedVehicleColor[vehicleid] ? pVehicle->vehModInfo.iColor2 : pVehicle->customSpawn.iColor2;

	CScriptParams::Get()->Add(color1, color2);
	return 1;
}

// native GetVehiclePaintjob(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehiclePaintjob)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehModInfo.bytePaintJob - 1;
}

// native GetVehicleInterior(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleInterior)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iInterior;
}

// native GetVehicleNumberPlate(vehicleid, plate[], len = sizeof(plate));
AMX_DECLARE_NATIVE(Natives::GetVehicleNumberPlate)
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
	CScriptParams::Get()->Add(&szPlate[0]);
	return 1;
}

// native SetVehicleRespawnDelay(vehicleid, delay);
AMX_DECLARE_NATIVE(Natives::SetVehicleRespawnDelay)
{
	CHECK_PARAMS(2, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iRespawnTime = (CScriptParams::Get()->ReadInt() * 1000);
	return 1;
}

// native GetVehicleRespawnDelay(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleRespawnDelay)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->customSpawn.iRespawnTime / 1000;
}

// native SetVehicleOccupiedTick(vehicleid, ticks);
AMX_DECLARE_NATIVE(Natives::SetVehicleOccupiedTick)
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
AMX_DECLARE_NATIVE(Natives::GetVehicleOccupiedTick)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehOccupiedTick;
}

// native SetVehicleRespawnTick(vehicleid, ticks);
AMX_DECLARE_NATIVE(Natives::SetVehicleRespawnTick)
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
AMX_DECLARE_NATIVE(Natives::GetVehicleRespawnTick)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->vehRespawnTick;
}

// native GetVehicleLastDriver(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleLastDriver)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->wLastDriverID;
}

// native GetVehicleCab(vehicleid);
AMX_DECLARE_NATIVE(Natives::GetVehicleCab)
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
AMX_DECLARE_NATIVE(Natives::HasVehicleBeenOccupied)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->bOccupied;
}

// native SetVehicleBeenOccupied(vehicleid, occupied);
AMX_DECLARE_NATIVE(Natives::SetVehicleBeenOccupied)
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
AMX_DECLARE_NATIVE(Natives::IsVehicleOccupied)
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
AMX_DECLARE_NATIVE(Natives::IsVehicleDead)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->bDead;
}

// native SetVehicleParamsSirenState(vehicleid, sirenState);
AMX_DECLARE_NATIVE(Natives::SetVehicleParamsSirenState)
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
AMX_DECLARE_NATIVE(Natives::ToggleVehicleSirenEnabled)
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
AMX_DECLARE_NATIVE(Natives::IsVehicleSirenEnabled)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

	if (!pNetGame->pVehiclePool->pVehicle[vehicleid])
		return 0;

	return pNetGame->pVehiclePool->pVehicle[vehicleid]->byteSirenEnabled;
}

// native GetVehicleMatrix(vehicleid, &Float:rightX, &Float:rightY, &Float:rightZ, &Float:upX, &Float:upY, &Float:upZ, &Float:atX, &Float:atY, &Float:atZ);
AMX_DECLARE_NATIVE(Natives::GetVehicleMatrix)
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

/* --------------------------- HOOKS --------------------------- */

// native ChangeVehicleColor(vehicleid, color1, color2)
AMX_DECLARE_NATIVE(Natives::YSF_ChangeVehicleColor)
{
	CHECK_PARAMS(3, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (Natives::ORIGINAL_ChangeVehicleColor(amx, params))
	{
		CServer::Get()->bChangedVehicleColor[vehicleid] = true;
		return 1;
	}
	return 0;
}

// native DestroyVehicle(vehicleid);
AMX_DECLARE_NATIVE(Natives::YSF_DestroyVehicle)
{
	CHECK_PARAMS(1, LOADED);

	const int vehicleid = CScriptParams::Get()->ReadInt();
	if (Natives::ORIGINAL_DestroyVehicle(amx, params))
	{
		CServer::Get()->bChangedVehicleColor[vehicleid] = false;
		auto v = CServer::Get()->vehicleSpawnData.find(vehicleid);
		if (v != CServer::Get()->vehicleSpawnData.end())
		{
			CServer::Get()->vehicleSpawnData.erase(v);
		}
		return 1;
	}
	return 0;
}