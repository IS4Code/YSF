#include "../main.h"
#include "../Natives.h"
#include "../linux_fix.h"

// native SetPlayerAdmin(playerid, bool:admin);
AMX_DECLARE_NATIVE(Natives::SetPlayerAdmin)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	pNetGame->pPlayerPool->bIsAnAdmin[playerid] = static_cast<BOOL>(CScriptParams::Get()->ReadInt());
	return 1;
}

// native IsPlayerPaused(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerPaused)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->bAFKState;
}

// native GetPlayerPausedTime(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerPausedTime)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;
	if (!pPlayerData[playerid]->bAFKState) return 0;

	return static_cast<cell>(std::chrono::duration_cast<std::chrono::milliseconds>(default_clock::now() - pPlayerData[playerid]->LastUpdateTick).count());
}

// native EnableConsoleMSGsForPlayer(playerid, color);
AMX_DECLARE_NATIVE(Natives::EnableConsoleMSGsForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	int playerid, color;
	CScriptParams::Get()->Read(&playerid, &color);
	if (!IsPlayerConnected(playerid)) return 0;

	CServer::Get()->AddConsolePlayer(static_cast<WORD>(playerid), static_cast<DWORD>(color));
	return 1;
}

// native DisableConsoleMSGsForPlayer(playerid);
AMX_DECLARE_NATIVE(Natives::DisableConsoleMSGsForPlayer)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CServer::Get()->RemoveConsolePlayer(static_cast<WORD>(playerid));
	return 1;
}

// native HasPlayerConsoleMessages(playerid, &color = 0);
AMX_DECLARE_NATIVE(Natives::HasPlayerConsoleMessages)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	DWORD color;
	bool ret = CServer::Get()->IsConsolePlayer(static_cast<WORD>(playerid), color);
	CScriptParams::Get()->Add(color);
	return ret;
}

// native SetPlayerGravity(playerid, Float:gravity);
AMX_DECLARE_NATIVE(Natives::SetPlayerGravity)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	// Update stored values
	pPlayerData[playerid]->fGravity = CScriptParams::Get()->ReadFloat();

	RakNet::BitStream bs;
	bs.Write(pPlayerData[playerid]->fGravity);
	CSAMPFunctions::RPC(&RPC_Gravity, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
	return 1;
}

// native Float:GetPlayerGravity(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerGravity)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return amx_ftoc(pPlayerData[playerid]->fGravity);
}

// native SetPlayerTeamForPlayer(playerid, teamplayerid, teamid);
AMX_DECLARE_NATIVE(Natives::SetPlayerTeamForPlayer)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int teamplayerid = CScriptParams::Get()->ReadInt();
	const int team = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;
	if (team < 0 || team > NO_TEAM) return 0;

	pPlayerData[playerid]->SetPlayerTeamForPlayer(static_cast<WORD>(teamplayerid), team);
	return 1;
}

// native GetPlayerTeamForPlayer(playerid, teamplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerTeamForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int teamplayerid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;

	return pPlayerData[playerid]->GetPlayerTeamForPlayer(static_cast<WORD>(teamplayerid));
}

AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerTeam)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (Natives::ORIGINAL_SetPlayerTeam(amx, params))
	{
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerTeam(static_cast<WORD>(playerid));
		}
		return 1;
	}
	return 0;
}

// native SetPlayerSkinForPlayer(playerid, skinplayerid, skin);
AMX_DECLARE_NATIVE(Natives::SetPlayerSkinForPlayer)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int skinplayerid = CScriptParams::Get()->ReadInt();
	const int skin = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;
	if (skin < 0 || skin > 300) return 0;

	pPlayerData[playerid]->SetPlayerSkinForPlayer(static_cast<WORD>(skinplayerid), skin);
	return 1;
}

// native GetPlayerSkinForPlayer(playerid, skinplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSkinForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int skinplayerid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;

	return pPlayerData[playerid]->GetPlayerSkinForPlayer(static_cast<WORD>(skinplayerid));
}

// native SetPlayerNameForPlayer(playerid, nameplayerid, playername[]);
AMX_DECLARE_NATIVE(Natives::SetPlayerNameForPlayer)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int nameplayerid = CScriptParams::Get()->ReadInt();

	std::string name;
	CScriptParams::Get()->Read(&name);

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;

	pPlayerData[playerid]->SetPlayerNameForPlayer(static_cast<WORD>(nameplayerid), name.c_str());
	return 1;
}

// native GetPlayerNameForPlayer(playerid, nameplayerid, playername[], size = sizeof(playername));
AMX_DECLARE_NATIVE(Natives::GetPlayerNameForPlayer)
{
	CHECK_PARAMS(4, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int nameplayerid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;

	CScriptParams::Get()->Add((char*)&pPlayerData[playerid]->GetPlayerNameForPlayer(static_cast<WORD>(nameplayerid))[0]);
	return 1;
}

// native SetPlayerFightStyleForPlayer(playerid, styleplayerid, style);
AMX_DECLARE_NATIVE(Natives::SetPlayerFightStyleForPlayer)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int styleplayerid = CScriptParams::Get()->ReadInt();
	const int style = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;

	pPlayerData[playerid]->SetPlayerFightingStyleForPlayer(static_cast<WORD>(styleplayerid), style);
	return 1;
}

// native GetPlayerFightStyleForPlayer(playerid, skinplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerFightStyleForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int styleplayerid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;

	return pPlayerData[playerid]->GetPlayerFightingStyleForPlayer(static_cast<WORD>(styleplayerid));
}

// native SetPlayerPosForPlayer(playerid, posplayerid, Float:fX, Float:fY, Float:fZ, bool:forcesync = true);
AMX_DECLARE_NATIVE(Natives::SetPlayerPosForPlayer)
{
	CHECK_PARAMS(6, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int posplayerid = CScriptParams::Get()->ReadInt();
	const bool forcesync = static_cast<int>(params[6]) != 0;
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(posplayerid)) return 0;

	if (!forcesync)
	{
		if (pPlayerData[playerid]->customPos.find(posplayerid) != pPlayerData[playerid]->customPos.end())
		{
			pPlayerData[playerid]->customPos.erase(posplayerid);
		}
		return 1;
	}

	CVector vecPos;
	CScriptParams::Get()->Read(&vecPos);

	pPlayerData[playerid]->customPos[posplayerid] = std::make_unique<CVector>(std::move(vecPos));
	return 1;
}

// native SetPlayerRotationQuatForPlayer(playerid, quatplayerid, Float:w, Float:x, Float:y, Float:z, bool:forcesync = true);
AMX_DECLARE_NATIVE(Natives::SetPlayerRotationQuatForPlayer)
{
	CHECK_PARAMS(7, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int posplayerid = CScriptParams::Get()->ReadInt();
	const bool forcesync = static_cast<int>(params[7]) != 0;
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(posplayerid)) return 0;

	if (!forcesync)
	{
		pPlayerData[playerid]->bCustomQuat[posplayerid] = false;
		return 1;
	}

	CPlayer *p = pNetGame->pPlayerPool->pPlayer[playerid];

	CScriptParams::Get()->Read(&pPlayerData[playerid]->fCustomQuat[posplayerid][0], &pPlayerData[playerid]->fCustomQuat[posplayerid][1], &pPlayerData[playerid]->fCustomQuat[posplayerid][2], &pPlayerData[playerid]->fCustomQuat[posplayerid][3]);
	pPlayerData[playerid]->bCustomQuat[posplayerid] = true;
	return 1;
}

// native ApplyAnimationForPlayer(playerid, animplayerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time);
AMX_DECLARE_NATIVE(Natives::ApplyAnimationForPlayer)
{
	CHECK_PARAMS(10, LOADED);

	RakNet::BitStream bsSend;
	char *szAnimLib;
	char *szAnimName;
	BYTE byteAnimLibLen;
	BYTE byteAnimNameLen;
	float fS;
	bool opt1, opt2, opt3, opt4;
	int time;

	const int playerid = CScriptParams::Get()->ReadInt();
	const int animplayerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(animplayerid)) return 0;

	amx_StrParam(amx, params[3], szAnimLib);
	amx_StrParam(amx, params[4], szAnimName);

	if (!szAnimLib || !szAnimName) return 0;

	byteAnimLibLen = static_cast<BYTE>(strlen(szAnimLib));
	byteAnimNameLen = static_cast<BYTE>(strlen(szAnimName));

	fS = amx_ctof(params[5]);
	opt1 = !!params[6];
	opt2 = !!params[7];
	opt3 = !!params[8];
	opt4 = !!params[9];
	time = static_cast<int>(params[10]);

	bsSend.Write((WORD)animplayerid);
	bsSend.Write(byteAnimLibLen);
	bsSend.Write(szAnimLib, byteAnimLibLen);
	bsSend.Write(byteAnimNameLen);
	bsSend.Write(szAnimName, byteAnimNameLen);
	bsSend.Write(fS);
	bsSend.Write(opt1);
	bsSend.Write(opt2);
	bsSend.Write(opt3);
	bsSend.Write(opt4);
	bsSend.Write(time);

	CSAMPFunctions::RPC(&RPC_ScrApplyAnimation, &bsSend, MEDIUM_PRIORITY, UNRELIABLE, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
	return 1;
}

// native GetPlayerWeather(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerWeather)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->byteWeather;
}

// native TogglePlayerWidescreen(playerid, bool:set);
AMX_DECLARE_NATIVE(Natives::TogglePlayerWidescreen)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	BYTE set = static_cast<BYTE>(CScriptParams::Get()->ReadInt()) != 0;
	pPlayerData[playerid]->bWidescreen = !!set;

	RakNet::BitStream bs;
	bs.Write(set);
	CSAMPFunctions::RPC(&RPC_Widescreen, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
	return 1;
}

// native IsPlayerWidescreenToggled(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerWidescreenToggled)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->bWidescreen;
}

// native GetSpawnInfo(playerid, &teamid, &modelid, &Float:spawn_x, &Float:spawn_y, &Float:spawn_z, &Float:z_angle, &weapon1, &weapon1_ammo, &weapon2, &weapon2_ammo,& weapon3, &weapon3_ammo);
AMX_DECLARE_NATIVE(Natives::GetSpawnInfo)
{
	CHECK_PARAMS(13, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CPlayerSpawnInfo *pSpawn = &pNetGame->pPlayerPool->pPlayer[playerid]->spawn;
	CScriptParams::Get()->Add(pSpawn->byteTeam, pSpawn->iSkin, pSpawn->vecPos, pSpawn->fRotation,
		pSpawn->iSpawnWeapons[0], pSpawn->iSpawnWeaponsAmmo[0],
		pSpawn->iSpawnWeapons[1], pSpawn->iSpawnWeaponsAmmo[1],
		pSpawn->iSpawnWeapons[2], pSpawn->iSpawnWeaponsAmmo[2]);
	return 1;
}

// native GetPlayerSkillLevel(playerid, skill);
AMX_DECLARE_NATIVE(Natives::GetPlayerSkillLevel)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int skillid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;
	if (skillid < 0 || skillid > 10) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->wSkillLevel[skillid];
}

// native IsPlayerCheckpointActive(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerCheckpointActive)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bShowCheckpoint;
}

// native GetPlayerCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fSize);
AMX_DECLARE_NATIVE(Natives::GetPlayerCheckpoint)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
	CScriptParams::Get()->Add(pPlayer->vecCPPos, pPlayer->fCPSize);
	return 1;
}

// native IsPlayerRaceCheckpointActive(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerRaceCheckpointActive)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bShowRaceCheckpoint;
}

// native GetPlayerRaceCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fNextX, &Float:fNextY, &fNextZ, &Float:fSize);
AMX_DECLARE_NATIVE(Natives::GetPlayerRaceCheckpoint)
{
	CHECK_PARAMS(8, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
	CScriptParams::Get()->Add(pPlayer->vecRaceCPPos, pPlayer->vecRaceCPNextPos, pPlayer->fRaceCPSize);
	return 1;
}

// native GetPlayerWorldBounds(playerid, &Float:x_max, &Float:x_min, &Float:y_max, &Float:y_min);
AMX_DECLARE_NATIVE(Natives::GetPlayerWorldBounds)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CScriptParams::Get()->Add(pPlayerData[playerid]->fBounds[0], pPlayerData[playerid]->fBounds[1], pPlayerData[playerid]->fBounds[2], pPlayerData[playerid]->fBounds[3]);
	return 1;
}

// native IsPlayerInModShop(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerInModShop)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bIsInModShop;
}

// native GetPlayerSirenState(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSirenState)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.byteSirenState;
}

// native GetPlayerLandingGearState(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerLandingGearState)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.byteGearState;
}

// native GetPlayerHydraReactorAngle(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerHydraReactorAngle)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.wHydraReactorAngle[0];
}

// native Float:GetPlayerTrainSpeed(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerTrainSpeed)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

	return amx_ftoc(pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.fTrainSpeed);
}

// native Float:GetPlayerZAim(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerZAim)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return amx_ftoc(pNetGame->pPlayerPool->pPlayer[playerid]->aimSyncData.fZAim);
}

// native GetPlayerSurfingOffsets(playerid, &Float:fOffsetX, &Float:fOffsetY, &Float:fOffsetZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerSurfingOffsets)
{
	CHECK_PARAMS(4, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CScriptParams::Get()->Add(pNetGame->pPlayerPool->pPlayer[playerid]->syncData.vecSurfing);
	return 1;
}

// native GetPlayerRotationQuat(playerid, &Float:w, &Float:x, &Float:y, &Float:z);
AMX_DECLARE_NATIVE(Natives::GetPlayerRotationQuat)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
	CScriptParams::Get()->Add(pPlayer->fQuaternion[0], pPlayer->fQuaternion[1], pPlayer->fQuaternion[2], pPlayer->fQuaternion[3]);
	return 1;
}

// native GetPlayerDialogID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerDialogID)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->wDialogID;
}

// native GetPlayerSpectateID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSpectateID)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->wSpectateID;
}

// native GetPlayerSpectateType(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSpectateType)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->byteSpectateType;
}

// native GetPlayerLastSyncedVehicleID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerLastSyncedVehicleID)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return static_cast<cell>(pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.wVehicleId);
}

// native GetPlayerLastSyncedTrailerID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerLastSyncedTrailerID)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->trailerSyncData.wTrailerID;
}


// native SendBulletData(senderid, forplayerid = -1, weaponid, hittype, hitid, Float:fHitOriginX, Float:fHitOriginY, Float:fHitOriginZ, Float:fHitTargetX, Float:fHitTargetY, Float:fHitTargetZ, Float:fCenterOfHitX, Float:fCenterOfHitY, Float:fCenterOfHitZ);
AMX_DECLARE_NATIVE(Natives::SendBulletData)
{
	CHECK_PARAMS(14, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int forplayerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (forplayerid != -1)
	{
		if (!IsPlayerConnected(forplayerid)) return 0;
	}

	CBulletSyncData bulletSync;
	CScriptParams::Get()->Read(&bulletSync.byteWeaponID, &bulletSync.byteHitType, &bulletSync.wHitID, &bulletSync.vecHitOrigin, &bulletSync.vecHitTarget, &bulletSync.vecCenterOfHit);

	RakNet::BitStream bs;
	bs.Write((BYTE)ID_BULLET_SYNC);
	bs.Write((WORD)playerid);
	bs.Write((char*)&bulletSync, sizeof(CBulletSyncData));

	if (forplayerid == -1)
	{
		CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
	}
	else
	{
		CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), false);
	}
	return 1;
}

// native ShowPlayerForPlayer(forplayerid, playerid);
AMX_DECLARE_NATIVE(Natives::ShowPlayerForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int forplayerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(forplayerid)) return 0;

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (playerid == forplayerid) return 0;

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	CSAMPFunctions::RPC(&RPC_WorldPlayerAdd, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native HidePlayerForPlayer(forplayerid, playerid);
AMX_DECLARE_NATIVE(Natives::HidePlayerForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int forplayerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(forplayerid)) return 0;

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (playerid == forplayerid) return 0;

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	CSAMPFunctions::RPC(&RPC_WorldPlayerRemove, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native AddPlayerForPlayer(forplayerid, playerid, isnpc = 0);
AMX_DECLARE_NATIVE(Natives::AddPlayerForPlayer)
{
	CHECK_PARAMS(3, LOADED);

	const int forplayerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(forplayerid)) return 0;

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (playerid == forplayerid) return 0;

	const bool npc = CScriptParams::Get()->ReadBool();
	const char* szName = GetPlayerName(playerid);
	BYTE len = static_cast<BYTE>(strlen(szName));

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	bs.Write((DWORD)0);
	bs.Write((BYTE)npc); //  // isNPC
	bs.Write(len);
	bs.Write(szName, len);
	CSAMPFunctions::RPC(&RPC_ServerJoin, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native RemovePlayerForPlayer(forplayerid, playerid);
AMX_DECLARE_NATIVE(Natives::RemovePlayerForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int forplayerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(forplayerid)) return 0;

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	if (playerid == forplayerid) return 0;

	RakNet::BitStream bs;
	bs.Write((WORD)playerid);
	bs.Write((BYTE)0);
	CSAMPFunctions::RPC(&RPC_ServerQuit, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
	return 1;
}

// native SetPlayerChatBubbleForPlayer(forplayerid, playerid, text[], color, Float:drawdistance, expiretime);
AMX_DECLARE_NATIVE(Natives::SetPlayerChatBubbleForPlayer)
{
	CHECK_PARAMS(6, LOADED);

	const int forplayerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(forplayerid)) return 0;

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	std::string text;
	int color;
	float drawdistance;
	int expiretime;
	CScriptParams::Get()->Read(&text, &color, &drawdistance, &expiretime);

	if (!text.empty())
	{
		BYTE len = static_cast<BYTE>(text.length());
		RakNet::BitStream bs;
		bs.Write((WORD)playerid);
		bs.Write(color);
		bs.Write(drawdistance);
		bs.Write(expiretime);
		bs.Write(len);
		bs.Write(text.c_str(), len);
		CSAMPFunctions::RPC(&RPC_ChatBubble, &bs, LOW_PRIORITY, RELIABLE, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
		return 1;
	}
	return 0;
}

// native ResetPlayerMarkerForPlayer(playerid, resetplayerid)
AMX_DECLARE_NATIVE(Natives::ResetPlayerMarkerForPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int resetplayerid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(resetplayerid)) return 0;

	pPlayerData[playerid]->ResetPlayerMarkerForPlayer(static_cast<WORD>(resetplayerid));
	return 1;
}

// native SetPlayerVersion(playerid, version[];
AMX_DECLARE_NATIVE(Natives::SetPlayerVersion)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	std::string version;
	CScriptParams::Get()->Read(&version);

	if (!version.empty() && version.length() < 20)
	{
		pNetGame->pPlayerPool->szVersion[playerid][0] = NULL;
		strcpy(pNetGame->pPlayerPool->szVersion[playerid], version.c_str());
		return 1;
	}
	return 0;
}

// native IsPlayerSpawned(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerSpawned)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	BYTE state = pNetGame->pPlayerPool->pPlayer[playerid]->byteState;
	switch (state)
	{
		case PLAYER_STATE_ONFOOT:
		case PLAYER_STATE_DRIVER:
		case PLAYER_STATE_PASSENGER:
		case PLAYER_STATE_SPAWNED:
		{
			return true;
		}
	}
	return false;
}

// native IsPlayerControllable(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerControllable)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pPlayerData[playerid]->bControllable;
}

// native SpawnForWorld(playerid);
AMX_DECLARE_NATIVE(Natives::SpawnForWorld)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CSAMPFunctions::SpawnPlayer(playerid);
	return 1;
}

// native BroadcastDeath(playerid);
AMX_DECLARE_NATIVE(Natives::BroadcastDeath)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	RakNet::BitStream bsData;
	bsData.Write((WORD)playerid);
	CSAMPFunctions::RPC(&RPC_DeathBroadcast, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), true, false);
	return 1;
}

// native IsPlayerCameraTargetEnabled(playerid);
AMX_DECLARE_NATIVE(Natives::IsPlayerCameraTargetEnabled)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	return pNetGame->pPlayerPool->pPlayer[playerid]->bCameraTarget;
}

// native SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0);
AMX_DECLARE_NATIVE(Natives::SetPlayerDisabledKeysSync)
{
	CHECK_PARAMS(4, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->wDisabledKeys = static_cast<WORD>(CScriptParams::Get()->ReadInt());
	pPlayerData[playerid]->wDisabledKeysUD = static_cast<WORD>(CScriptParams::Get()->ReadInt());
	pPlayerData[playerid]->wDisabledKeysLR = static_cast<WORD>(CScriptParams::Get()->ReadInt());
	return 1;
}

// native GetPlayerDisabledKeysSync(playerid, &keys, &updown, &leftright);
AMX_DECLARE_NATIVE(Natives::GetPlayerDisabledKeysSync)
{
	CHECK_PARAMS(4, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CScriptParams::Get()->Add(pPlayerData[playerid]->wDisabledKeys, static_cast<short>(pPlayerData[playerid]->wDisabledKeysUD), static_cast<short>(pPlayerData[playerid]->wDisabledKeysLR));
	return 1;
}

// native SetExclusiveBroadcast(toggle);
AMX_DECLARE_NATIVE(Natives::SetExclusiveBroadcast)
{
	CHECK_PARAMS(1, LOADED);

	const int toggle = CScriptParams::Get()->ReadInt();
	CServer::Get()->SetExclusiveBroadcast(!!toggle);
	return 1;
}

// native BroadcastToPlayer(playerid, toggle = 1);
AMX_DECLARE_NATIVE(Natives::BroadcastToPlayer)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int toggle = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;

	pPlayerData[playerid]->bBroadcastTo = !!toggle;
	return 1;
}

/* --------------------------- HOOKS --------------------------- */
AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerSkin)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (Natives::ORIGINAL_SetPlayerSkin(amx, params))
	{
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerSkin(static_cast<WORD>(playerid));
		}
		return 1;
	}
	return 0;
}

AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerName)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int ret = Natives::ORIGINAL_SetPlayerName(amx, params);

	if (ret == 1)
	{
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerName(static_cast<WORD>(playerid));
		}
	}
	return ret;
}

AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerFightingStyle)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (Natives::ORIGINAL_SetPlayerFightingStyle(amx, params))
	{
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
				pPlayerData[i]->ResetPlayerFightingStyle(static_cast<WORD>(playerid));
		}
		return 1;
	}
	return 0;
}

// native SetPlayerWeather(playerid, weatherid);
AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerWeather)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();

	if (Natives::ORIGINAL_SetPlayerWeather(amx, params) && IsPlayerConnected(playerid))
	{
		pPlayerData[playerid]->byteWeather = static_cast<BYTE>(CScriptParams::Get()->ReadInt());
		return 1;
	}
	return 0;
}

// native SetPlayerWorldBounds(playerid, Float:x_max, Float:x_min, Float:y_max, Float:y_min)
AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerWorldBounds)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (Natives::ORIGINAL_SetPlayerWorldBounds(amx, params) && IsPlayerConnected(playerid))
	{
		for (BYTE i = 0; i != 4; ++i)
		{
			pPlayerData[playerid]->fBounds[i] = CScriptParams::Get()->ReadFloat();
		}
		return 1;
	}
	return 0;
}

// native TogglePlayerControllable(playerid, bool:toggle)
AMX_DECLARE_NATIVE(Natives::YSF_TogglePlayerControllable)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const bool toggle = CScriptParams::Get()->ReadBool();

	if (Natives::ORIGINAL_TogglePlayerControllable(amx, params) && IsPlayerConnected(playerid))
	{
		pPlayerData[playerid]->bControllable = toggle;
		return 1;
	}
	return 0;
}

// native ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]);
AMX_DECLARE_NATIVE(Natives::YSF_ShowPlayerDialog)
{
	CHECK_PARAMS(7, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int dialogid = CScriptParams::Get()->ReadInt();

	if (Natives::ORIGINAL_ShowPlayerDialog(amx, params) && IsPlayerConnected(playerid))
	{
		pPlayerData[playerid]->wDialogID = dialogid;
		return 1;
	}
	return 0;
}