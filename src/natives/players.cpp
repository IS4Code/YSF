#include "../Natives.h"
#include "../includes/platform.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"
#include "../RPCs.h"
#include "../linux_fix.h"

namespace Natives
{
	// native SetPlayerAdmin(playerid, bool:admin);
	AMX_DECLARE_NATIVE(SetPlayerAdmin)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		pNetGame->pPlayerPool->bIsAnAdmin[playerid] = static_cast<BOOL>(CScriptParams::Get()->ReadInt());
		return 1;
	}

	// native IsPlayerPaused(playerid);
	AMX_DECLARE_NATIVE(IsPlayerPaused)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).bAFKState;
	}

	// native GetPlayerPausedTime(playerid);
	AMX_DECLARE_NATIVE(GetPlayerPausedTime)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		if (!data.bAFKState) return 0;

		return static_cast<cell>(std::chrono::duration_cast<std::chrono::milliseconds>(default_clock::now() - data.LastUpdateTick).count());
	}

	// native EnableConsoleMSGsForPlayer(playerid, color);
	AMX_DECLARE_NATIVE(EnableConsoleMSGsForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid, color;
		CScriptParams::Get()->Read(playerid, color);
		if (!IsPlayerConnected(playerid)) return 0;

		CPlugin::Get()->AddConsolePlayer(static_cast<WORD>(playerid), static_cast<DWORD>(color));
		return 1;
	}

	// native DisableConsoleMSGsForPlayer(playerid);
	AMX_DECLARE_NATIVE(DisableConsoleMSGsForPlayer)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CPlugin::Get()->RemoveConsolePlayer(static_cast<WORD>(playerid));
		return 1;
	}

	// native HasPlayerConsoleMessages(playerid, &color = 0);
	AMX_DECLARE_NATIVE(HasPlayerConsoleMessages)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		DWORD color;
		bool ret = CPlugin::Get()->IsConsolePlayer(static_cast<WORD>(playerid), color);
		CScriptParams::Get()->Add(color);
		return ret;
	}

	// native SetPlayerGravity(playerid, Float:gravity);
	AMX_DECLARE_NATIVE(SetPlayerGravity)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		// Update stored values
		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		data.fGravity = CScriptParams::Get()->ReadFloat();

		RakNet::BitStream bs;
		bs.Write(data.fGravity);
		CSAMPFunctions::RPC(&RPC_SetGravity, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
		return 1;
	}

	// native Float:GetPlayerGravity(playerid);
	AMX_DECLARE_NATIVE(GetPlayerGravity)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return amx_ftoc(CServer::Get()->PlayerPool.Extra(playerid).fGravity);
	}

	// native SetPlayerTeamForPlayer(playerid, teamplayerid, teamid);
	AMX_DECLARE_NATIVE(SetPlayerTeamForPlayer)
	{
		CHECK_PARAMS(3, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int teamplayerid = CScriptParams::Get()->ReadInt();
		const int team = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;
		if (team < 0 || team > NO_TEAM) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).SetPlayerTeamForPlayer(static_cast<WORD>(teamplayerid), team);
		return 1;
	}

	// native GetPlayerTeamForPlayer(playerid, teamplayerid);
	AMX_DECLARE_NATIVE(GetPlayerTeamForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int teamplayerid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(teamplayerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).GetPlayerTeamForPlayer(static_cast<WORD>(teamplayerid));
	}

	// native SetPlayerSkinForPlayer(playerid, skinplayerid, skin);
	AMX_DECLARE_NATIVE(SetPlayerSkinForPlayer)
	{
		CHECK_PARAMS(3, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int skinplayerid = CScriptParams::Get()->ReadInt();
		const int skin = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;
		if (skin < 0 || skin > 300) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).SetPlayerSkinForPlayer(static_cast<WORD>(skinplayerid), skin);
		return 1;
	}

	// native GetPlayerSkinForPlayer(playerid, skinplayerid);
	AMX_DECLARE_NATIVE(GetPlayerSkinForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int skinplayerid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(skinplayerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).GetPlayerSkinForPlayer(static_cast<WORD>(skinplayerid));
	}

	// native SetPlayerNameForPlayer(playerid, nameplayerid, playername[]);
	AMX_DECLARE_NATIVE(SetPlayerNameForPlayer)
	{
		CHECK_PARAMS(3, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int nameplayerid = CScriptParams::Get()->ReadInt();

		std::string name;
		CScriptParams::Get()->Read(name);

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).SetPlayerNameForPlayer(static_cast<WORD>(nameplayerid), name.c_str());
		return 1;
	}

	// native GetPlayerNameForPlayer(playerid, nameplayerid, playername[], size = sizeof(playername));
	AMX_DECLARE_NATIVE(GetPlayerNameForPlayer)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int nameplayerid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(nameplayerid)) return 0;

		CScriptParams::Get()->Add(CServer::Get()->PlayerPool.Extra(playerid).GetPlayerNameForPlayer(static_cast<WORD>(nameplayerid)));
		return 1;
	}

	// native SetPlayerFightStyleForPlayer(playerid, styleplayerid, style);
	AMX_DECLARE_NATIVE(SetPlayerFightStyleForPlayer)
	{
		CHECK_PARAMS(3, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int styleplayerid = CScriptParams::Get()->ReadInt();
		const int style = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).SetPlayerFightingStyleForPlayer(static_cast<WORD>(styleplayerid), style);
		return 1;
	}

	// native GetPlayerFightStyleForPlayer(playerid, skinplayerid);
	AMX_DECLARE_NATIVE(GetPlayerFightStyleForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int styleplayerid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(styleplayerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).GetPlayerFightingStyleForPlayer(static_cast<WORD>(styleplayerid));
	}

	// native SetPlayerPosForPlayer(playerid, posplayerid, Float:fX, Float:fY, Float:fZ, bool:forcesync = true);
	AMX_DECLARE_NATIVE(SetPlayerPosForPlayer)
	{
		CHECK_PARAMS(6, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int posplayerid = CScriptParams::Get()->ReadInt();
		const bool forcesync = static_cast<int>(params[6]) != 0;
		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(posplayerid)) return 0;

		if (!forcesync)
		{
			if (CServer::Get()->PlayerPool.Extra(playerid).customPos.find(posplayerid) != CServer::Get()->PlayerPool.Extra(playerid).customPos.end())
			{
				CServer::Get()->PlayerPool.Extra(playerid).customPos.erase(posplayerid);
			}
			return 1;
		}

		CVector vecPos;
		CScriptParams::Get()->Read(vecPos);

		CServer::Get()->PlayerPool.Extra(playerid).customPos[posplayerid] = std::move(vecPos);
		return 1;
	}

	// native SetPlayerRotationQuatForPlayer(playerid, quatplayerid, Float:w, Float:x, Float:y, Float:z, bool:forcesync = true);
	AMX_DECLARE_NATIVE(SetPlayerRotationQuatForPlayer)
	{
		CHECK_PARAMS(7, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int posplayerid = CScriptParams::Get()->ReadInt();
		const bool forcesync = static_cast<int>(params[7]) != 0;
		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(posplayerid)) return 0;

		if (!forcesync)
		{
			CServer::Get()->PlayerPool.Extra(playerid).bCustomQuat[posplayerid] = false;
			return 1;
		}

		CPlayer *p = pNetGame->pPlayerPool->pPlayer[playerid];

		CScriptParams::Get()->Read(CServer::Get()->PlayerPool.Extra(playerid).fCustomQuat[posplayerid][0], CServer::Get()->PlayerPool.Extra(playerid).fCustomQuat[posplayerid][1], CServer::Get()->PlayerPool.Extra(playerid).fCustomQuat[posplayerid][2], CServer::Get()->PlayerPool.Extra(playerid).fCustomQuat[posplayerid][3]);
		CServer::Get()->PlayerPool.Extra(playerid).bCustomQuat[posplayerid] = true;
		return 1;
	}

	// native ApplyAnimationForPlayer(playerid, animplayerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time);
	AMX_DECLARE_NATIVE(ApplyAnimationForPlayer)
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

		CSAMPFunctions::RPC(&RPC_ApplyAnimation, &bsSend, MEDIUM_PRIORITY, UNRELIABLE, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
		return 1;
	}

	// native GetPlayerWeather(playerid);
	AMX_DECLARE_NATIVE(GetPlayerWeather)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).byteWeather;
	}

	// native TogglePlayerWidescreen(playerid, bool:set);
	AMX_DECLARE_NATIVE(TogglePlayerWidescreen)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		BYTE set = static_cast<BYTE>(CScriptParams::Get()->ReadInt()) != 0;
		CServer::Get()->PlayerPool.Extra(playerid).bWidescreen = !!set;

		RakNet::BitStream bs;
		bs.Write(set);
		CSAMPFunctions::RPC(&RPC_Widescreen, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
		return 1;
	}

	// native IsPlayerWidescreenToggled(playerid);
	AMX_DECLARE_NATIVE(IsPlayerWidescreenToggled)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).bWidescreen;
	}

	// native GetSpawnInfo(playerid, &teamid, &modelid, &Float:spawn_x, &Float:spawn_y, &Float:spawn_z, &Float:z_angle, &weapon1, &weapon1_ammo, &weapon2, &weapon2_ammo,& weapon3, &weapon3_ammo);
	AMX_DECLARE_NATIVE(GetSpawnInfo)
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
	AMX_DECLARE_NATIVE(GetPlayerSkillLevel)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int skillid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (skillid < 0 || skillid > 10) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->wSkillLevel[skillid];
	}

	// native IsPlayerCheckpointActive(playerid);
	AMX_DECLARE_NATIVE(IsPlayerCheckpointActive)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->bShowCheckpoint;
	}

	// native GetPlayerCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fSize);
	AMX_DECLARE_NATIVE(GetPlayerCheckpoint)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		CScriptParams::Get()->Add(pPlayer->vecCPPos, pPlayer->fCPSize);
		return 1;
	}

	// native IsPlayerRaceCheckpointActive(playerid);
	AMX_DECLARE_NATIVE(IsPlayerRaceCheckpointActive)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->bShowRaceCheckpoint;
	}

	// native GetPlayerRaceCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fNextX, &Float:fNextY, &fNextZ, &Float:fSize);
	AMX_DECLARE_NATIVE(GetPlayerRaceCheckpoint)
	{
		CHECK_PARAMS(8, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		CScriptParams::Get()->Add(pPlayer->vecRaceCPPos, pPlayer->vecRaceCPNextPos, pPlayer->fRaceCPSize);
		return 1;
	}

	// native GetPlayerWorldBounds(playerid, &Float:x_max, &Float:x_min, &Float:y_max, &Float:y_min);
	AMX_DECLARE_NATIVE(GetPlayerWorldBounds)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CScriptParams::Get()->Add(CServer::Get()->PlayerPool.Extra(playerid).fBounds[0], CServer::Get()->PlayerPool.Extra(playerid).fBounds[1], CServer::Get()->PlayerPool.Extra(playerid).fBounds[2], CServer::Get()->PlayerPool.Extra(playerid).fBounds[3]);
		return 1;
	}

	// native IsPlayerInModShop(playerid);
	AMX_DECLARE_NATIVE(IsPlayerInModShop)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->bIsInModShop;
	}

	// native GetPlayerSirenState(playerid);
	AMX_DECLARE_NATIVE(GetPlayerSirenState)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.byteSirenState;
	}

	// native GetPlayerLandingGearState(playerid);
	AMX_DECLARE_NATIVE(GetPlayerLandingGearState)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.byteGearState;
	}

	// native GetPlayerHydraReactorAngle(playerid);
	AMX_DECLARE_NATIVE(GetPlayerHydraReactorAngle)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.wHydraReactorAngle[0];
	}

	// native Float:GetPlayerTrainSpeed(playerid);
	AMX_DECLARE_NATIVE(GetPlayerTrainSpeed)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		if (!pNetGame->pPlayerPool->pPlayer[playerid]->wVehicleId) return 0;

		return amx_ftoc(pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.fTrainSpeed);
	}

	// native Float:GetPlayerZAim(playerid);
	AMX_DECLARE_NATIVE(GetPlayerZAim)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return amx_ftoc(pNetGame->pPlayerPool->pPlayer[playerid]->aimSyncData.fZAim);
	}

	// native GetPlayerSurfingOffsets(playerid, &Float:fOffsetX, &Float:fOffsetY, &Float:fOffsetZ);
	AMX_DECLARE_NATIVE(GetPlayerSurfingOffsets)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CScriptParams::Get()->Add(pNetGame->pPlayerPool->pPlayer[playerid]->syncData.vecSurfing);
		return 1;
	}

	// native GetPlayerRotationQuat(playerid, &Float:w, &Float:x, &Float:y, &Float:z);
	AMX_DECLARE_NATIVE(GetPlayerRotationQuat)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		CScriptParams::Get()->Add(pPlayer->fQuaternion[0], pPlayer->fQuaternion[1], pPlayer->fQuaternion[2], pPlayer->fQuaternion[3]);
		return 1;
	}

	// native GetPlayerDialogID(playerid);
	AMX_DECLARE_NATIVE(GetPlayerDialogID)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->wDialogID;
	}

	// native GetPlayerSpectateID(playerid);
	AMX_DECLARE_NATIVE(GetPlayerSpectateID)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->wSpectateID;
	}

	// native GetPlayerSpectateType(playerid);
	AMX_DECLARE_NATIVE(GetPlayerSpectateType)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->byteSpectateType;
	}

	// native GetPlayerLastSyncedVehicleID(playerid);
	AMX_DECLARE_NATIVE(GetPlayerLastSyncedVehicleID)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return static_cast<cell>(pNetGame->pPlayerPool->pPlayer[playerid]->vehicleSyncData.wVehicleId);
	}

	// native GetPlayerLastSyncedTrailerID(playerid);
	AMX_DECLARE_NATIVE(GetPlayerLastSyncedTrailerID)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->trailerSyncData.wTrailerID;
	}


	// native SendBulletData(senderid, forplayerid = -1, weaponid, hittype, hitid, Float:fHitOriginX, Float:fHitOriginY, Float:fHitOriginZ, Float:fHitTargetX, Float:fHitTargetY, Float:fHitTargetZ, Float:fCenterOfHitX, Float:fCenterOfHitY, Float:fCenterOfHitZ);
	AMX_DECLARE_NATIVE(SendBulletData)
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
		CScriptParams::Get()->Read(bulletSync.byteWeaponID, bulletSync.byteHitType, bulletSync.wHitID, bulletSync.vecHitOrigin, bulletSync.vecHitTarget, bulletSync.vecCenterOfHit);

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

	// native ShowPlayerForPlayer(forplayerid, playerid, bool:setskin=false);
	AMX_DECLARE_NATIVE(ShowPlayerForPlayer)
	{
		CHECK_PARAMS(2, MORE_PARAMETER_ALLOWED);

		const int forplayerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(forplayerid)) return 0;

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		if (playerid == forplayerid) return 0;

		RakNet::BitStream bs;
		bs.Write((WORD)playerid);
		CSAMPFunctions::RPC(&RPC_WorldPlayerAdd, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);

		if(params[0] / sizeof(cell) >= 3 && CScriptParams::Get()->ReadBool())
		{
			RakNet::BitStream bs;
			bs.Write((int)playerid);
			bs.Write((int)pNetGame->pPlayerPool->pPlayer[playerid]->spawn.iSkin);
			CSAMPFunctions::RPC(&RPC_SetPlayerSkin, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0);
		}
		return 1;
	}

	// native HidePlayerForPlayer(forplayerid, playerid);
	AMX_DECLARE_NATIVE(HidePlayerForPlayer)
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
	AMX_DECLARE_NATIVE(AddPlayerForPlayer)
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
	AMX_DECLARE_NATIVE(RemovePlayerForPlayer)
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
	AMX_DECLARE_NATIVE(SetPlayerChatBubbleForPlayer)
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
		CScriptParams::Get()->Read(text, color, drawdistance, expiretime);

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
	AMX_DECLARE_NATIVE(ResetPlayerMarkerForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int resetplayerid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid) || !IsPlayerConnected(resetplayerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).ResetPlayerMarkerForPlayer(static_cast<WORD>(resetplayerid));
		return 1;
	}

	// native SetPlayerVersion(playerid, version[];
	AMX_DECLARE_NATIVE(SetPlayerVersion)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		std::string version;
		CScriptParams::Get()->Read(version);

		if (!version.empty() && version.length() < 20)
		{
			pNetGame->pPlayerPool->szVersion[playerid][0] = NULL;
			strcpy(pNetGame->pPlayerPool->szVersion[playerid], version.c_str());
			return 1;
		}
		return 0;
	}

	// native IsPlayerSpawned(playerid);
	AMX_DECLARE_NATIVE(IsPlayerSpawned)
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
	AMX_DECLARE_NATIVE(IsPlayerControllable)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).bControllable;
	}

	// native SpawnForWorld(playerid);
	AMX_DECLARE_NATIVE(SpawnForWorld)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CSAMPFunctions::SpawnPlayer(playerid);
		return 1;
	}

	// native BroadcastDeath(playerid);
	AMX_DECLARE_NATIVE(BroadcastDeath)
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
	AMX_DECLARE_NATIVE(IsPlayerCameraTargetEnabled)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->bCameraTarget;
	}

	// native SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0);
	AMX_DECLARE_NATIVE(SetPlayerDisabledKeysSync)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeys = static_cast<WORD>(CScriptParams::Get()->ReadInt());
		CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysUD = static_cast<WORD>(CScriptParams::Get()->ReadInt());
		CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysLR = static_cast<WORD>(CScriptParams::Get()->ReadInt());
		return 1;
	}

	// native GetPlayerDisabledKeysSync(playerid, &keys, &updown, &leftright);
	AMX_DECLARE_NATIVE(GetPlayerDisabledKeysSync)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CScriptParams::Get()->Add(CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeys, static_cast<short>(CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysUD), static_cast<short>(CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysLR));
		return 1;
	}

	// native SetExclusiveBroadcast(toggle);
	AMX_DECLARE_NATIVE(SetExclusiveBroadcast)
	{
		CHECK_PARAMS(1, LOADED);

		const int toggle = CScriptParams::Get()->ReadInt();
		CPlugin::Get()->SetExclusiveBroadcast(!!toggle);
		return 1;
	}

	// native BroadcastToPlayer(playerid, toggle = 1);
	AMX_DECLARE_NATIVE(BroadcastToPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int toggle = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;

		CServer::Get()->PlayerPool.Extra(playerid).bBroadcastTo = !!toggle;
		return 1;
	}

	// native GetPlayerBuildingsRemoved(playerid)
	AMX_DECLARE_NATIVE(GetPlayerBuildingsRemoved)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;

		return CServer::Get()->PlayerPool.Extra(playerid).GetBuildingsRemoved();
	}

	// native IsBuildingRemovedForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
	AMX_DECLARE_NATIVE(IsBuildingRemovedForPlayer)
	{
		CHECK_PARAMS(6, LOADED);

		int playerid, modelid;
		CVector pos;
		float range;

		CScriptParams::Get()->Read(playerid, modelid, pos, range);

		if (!IsPlayerConnected(playerid)) return 0;

		return static_cast<cell>(CServer::Get()->PlayerPool.Extra(playerid).IsBuildingRemoved(modelid, pos, range));
	}

	// native TogglePlayerGhostMode(playerid, bool:toggle);
	AMX_DECLARE_NATIVE(TogglePlayerGhostMode)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		data.ghostMode = CScriptParams::Get()->ReadBool();
		return 1;
	}

	// native GetPlayerGhostMode(playerid);
	AMX_DECLARE_NATIVE(GetPlayerGhostMode)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
		return static_cast<cell>(data.ghostMode);
	}

	// native SetPlayerSyncKeys(playerid, keys, updown, leftright);
	AMX_DECLARE_NATIVE(SetPlayerSyncKeys)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.wKeys = pPlayer->vehicleSyncData.wKeys = pPlayer->passengerSyncData.wKeys = CScriptParams::Get()->ReadInt();
		pPlayer->syncData.wUDAnalog = pPlayer->vehicleSyncData.wUDAnalog = pPlayer->passengerSyncData.wUDAnalog = CScriptParams::Get()->ReadInt();
		pPlayer->syncData.wLRAnalog = pPlayer->vehicleSyncData.wLRAnalog = pPlayer->passengerSyncData.wLRAnalog = CScriptParams::Get()->ReadInt();
		return 1;
	}

	// native SetPlayerSyncCameraFrontVector(playerid, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncCameraFrontVector)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->aimSyncData.vecFront.fX = CScriptParams::Get()->ReadFloat();
		pPlayer->aimSyncData.vecFront.fY = CScriptParams::Get()->ReadFloat();
		pPlayer->aimSyncData.vecFront.fZ = CScriptParams::Get()->ReadFloat();
		pPlayer->bHasAimSync = true;
		return 1;
	}

	// native SetPlayerSyncCameraPos(playerid, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncCameraPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->aimSyncData.vecPosition.fX = CScriptParams::Get()->ReadFloat();
		pPlayer->aimSyncData.vecPosition.fY = CScriptParams::Get()->ReadFloat();
		pPlayer->aimSyncData.vecPosition.fZ = CScriptParams::Get()->ReadFloat();
		pPlayer->bHasAimSync = true;
		return 1;
	}

	// native SetPlayerSyncCameraMode(playerid, mode);
	AMX_DECLARE_NATIVE(SetPlayerSyncCameraMode)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->aimSyncData.byteCameraMode = CScriptParams::Get()->ReadInt();
		pPlayer->bHasAimSync = true;
		return 1;
	}

	// native SetPlayerSyncWeapon(playerid, weaponid);
	AMX_DECLARE_NATIVE(SetPlayerSyncWeapon)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.byteWeapon = pPlayer->vehicleSyncData.bytePlayerWeapon = pPlayer->passengerSyncData.bytePlayerWeapon = CScriptParams::Get()->ReadInt() & 0x3F;
		return 1;
	}

	// native SetPlayerSyncWeaponState(playerid, weaponstate);
	AMX_DECLARE_NATIVE(SetPlayerSyncWeaponState)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->aimSyncData.byteWeaponState = CScriptParams::Get()->ReadInt() & 0x3;
		pPlayer->bHasAimSync = true;
		return 1;
	}

	// native SetPlayerSyncSpecialAction(playerid, actionid);
	AMX_DECLARE_NATIVE(SetPlayerSyncSpecialAction)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.byteSpecialAction = CScriptParams::Get()->ReadInt();
		return 1;
	}
}

namespace Original
{
	AMX_NATIVE SetPlayerTeam;
	AMX_NATIVE SetPlayerSkin;
	AMX_NATIVE SetPlayerName;
	AMX_NATIVE SetPlayerWeather;
	AMX_NATIVE SetPlayerFightingStyle;
	AMX_NATIVE SetPlayerWorldBounds;
	AMX_NATIVE TogglePlayerControllable;
	AMX_NATIVE ShowPlayerDialog;
	AMX_NATIVE RemoveBuildingForPlayer;
}

namespace Hooks
{
	AMX_DECLARE_NATIVE(SetPlayerTeam)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (Original::SetPlayerTeam(amx, params))
		{
			auto &pool = CServer::Get()->PlayerPool;
			for (WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerTeam(static_cast<WORD>(playerid));
			}
			return 1;
		}
		return 0;
	}

	AMX_DECLARE_NATIVE(SetPlayerSkin)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (Original::SetPlayerSkin(amx, params))
		{
			auto &pool = CServer::Get()->PlayerPool;
			for (WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerSkin(static_cast<WORD>(playerid));
			}
			return 1;
		}
		return 0;
	}

	AMX_DECLARE_NATIVE(SetPlayerName)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int ret = Original::SetPlayerName(amx, params);

		if (ret == 1)
		{
			auto &pool = CServer::Get()->PlayerPool;
			for (WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerName(static_cast<WORD>(playerid));
			}
		}
		return ret;
	}

	AMX_DECLARE_NATIVE(SetPlayerFightingStyle)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (Original::SetPlayerFightingStyle(amx, params))
		{
			auto &pool = CServer::Get()->PlayerPool;
			for (WORD i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
					pool.Extra(i).ResetPlayerFightingStyle(static_cast<WORD>(playerid));
			}
			return 1;
		}
		return 0;
	}

	// native SetPlayerWeather(playerid, weatherid);
	AMX_DECLARE_NATIVE(SetPlayerWeather)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();

		if (Original::SetPlayerWeather(amx, params) && IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).byteWeather = static_cast<BYTE>(CScriptParams::Get()->ReadInt());
			return 1;
		}
		return 0;
	}

	// native SetPlayerWorldBounds(playerid, Float:x_max, Float:x_min, Float:y_max, Float:y_min)
	AMX_DECLARE_NATIVE(SetPlayerWorldBounds)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (Original::SetPlayerWorldBounds(amx, params) && IsPlayerConnected(playerid))
		{
			for (BYTE i = 0; i != 4; ++i)
			{
				CServer::Get()->PlayerPool.Extra(playerid).fBounds[i] = CScriptParams::Get()->ReadFloat();
			}
			return 1;
		}
		return 0;
	}

	// native TogglePlayerControllable(playerid, bool:toggle)
	AMX_DECLARE_NATIVE(TogglePlayerControllable)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const bool toggle = CScriptParams::Get()->ReadBool();

		if (Original::TogglePlayerControllable(amx, params) && IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).bControllable = toggle;
			return 1;
		}
		return 0;
	}

	// native ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]);
	AMX_DECLARE_NATIVE(ShowPlayerDialog)
	{
		CHECK_PARAMS(7, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int dialogid = CScriptParams::Get()->ReadInt();

		if (Original::ShowPlayerDialog(amx, params) && IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).wDialogID = dialogid;
			return 1;
		}
		return 0;
	}

	// native RemoveBuildingForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
	AMX_DECLARE_NATIVE(RemoveBuildingForPlayer)
	{
		CHECK_PARAMS(6, LOADED);

		int playerid, modelid;
		CVector pos;
		float range;

		CScriptParams::Get()->Read(playerid, modelid, pos, range);
		if (IsPlayerConnected(playerid))
		{
			CServer::Get()->PlayerPool.Extra(playerid).SetBuildingsRemoved(modelid, pos, range);
		}
		return Original::RemoveBuildingForPlayer(amx, params);
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(SetPlayerAdmin)

	AMX_DEFINE_NATIVE(EnableConsoleMSGsForPlayer) // R18
	AMX_DEFINE_NATIVE(DisableConsoleMSGsForPlayer) // R18
	AMX_DEFINE_NATIVE(HasPlayerConsoleMessages) // R18 

	// Exclusive RPC broadcast
	AMX_DEFINE_NATIVE(SetExclusiveBroadcast)
	AMX_DEFINE_NATIVE(BroadcastToPlayer)

	// Special
	AMX_DEFINE_NATIVE(SetPlayerGravity)
	AMX_DEFINE_NATIVE(GetPlayerGravity)

	AMX_DEFINE_NATIVE(SetPlayerTeamForPlayer) // R5
	AMX_DEFINE_NATIVE(GetPlayerTeamForPlayer) // R5
	AMX_DEFINE_NATIVE(SetPlayerSkinForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerSkinForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerNameForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerNameForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerFightStyleForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerFightStyleForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerPosForPlayer) // R12
	AMX_DEFINE_NATIVE(SetPlayerRotationQuatForPlayer) // R12
	AMX_DEFINE_NATIVE(ApplyAnimationForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerWeather)
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds)
	AMX_DEFINE_NATIVE(TogglePlayerWidescreen)
	AMX_DEFINE_NATIVE(IsPlayerWidescreenToggled)
	AMX_DEFINE_NATIVE(GetSpawnInfo) // R8
	AMX_DEFINE_NATIVE(GetPlayerSkillLevel) // R3
	AMX_DEFINE_NATIVE(IsPlayerCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerCheckpoint) // R4
	AMX_DEFINE_NATIVE(IsPlayerRaceCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerRaceCheckpoint) // R4
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds) // R5
	AMX_DEFINE_NATIVE(IsPlayerInModShop) // R4
	AMX_DEFINE_NATIVE(SendBulletData) // R6
	AMX_DEFINE_NATIVE(ShowPlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(HidePlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(AddPlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(RemovePlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(SetPlayerChatBubbleForPlayer) // R10
	AMX_DEFINE_NATIVE(SetPlayerVersion) // R9
	AMX_DEFINE_NATIVE(IsPlayerSpawned) // R9
	AMX_DEFINE_NATIVE(IsPlayerControllable) // R11
	AMX_DEFINE_NATIVE(SpawnForWorld) // R10
	AMX_DEFINE_NATIVE(BroadcastDeath) // R13
	AMX_DEFINE_NATIVE(IsPlayerCameraTargetEnabled) // R13
	AMX_DEFINE_NATIVE(SetPlayerDisabledKeysSync) // R16
	AMX_DEFINE_NATIVE(GetPlayerDisabledKeysSync) // R16

	// Special things from syncdata
	AMX_DEFINE_NATIVE(GetPlayerSirenState)
	AMX_DEFINE_NATIVE(GetPlayerLandingGearState)
	AMX_DEFINE_NATIVE(GetPlayerHydraReactorAngle)
	AMX_DEFINE_NATIVE(GetPlayerTrainSpeed)
	AMX_DEFINE_NATIVE(GetPlayerZAim)
	AMX_DEFINE_NATIVE(GetPlayerSurfingOffsets)
	AMX_DEFINE_NATIVE(GetPlayerRotationQuat) // R3
	AMX_DEFINE_NATIVE(GetPlayerDialogID) // R8
	AMX_DEFINE_NATIVE(GetPlayerSpectateID) // R8
	AMX_DEFINE_NATIVE(GetPlayerSpectateType) // R8
	AMX_DEFINE_NATIVE(GetPlayerLastSyncedVehicleID) // R10
	AMX_DEFINE_NATIVE(GetPlayerLastSyncedTrailerID) // R10

	// AFK
	AMX_DEFINE_NATIVE(IsPlayerPaused)
	AMX_DEFINE_NATIVE(GetPlayerPausedTime)

	AMX_DEFINE_NATIVE(GetPlayerBuildingsRemoved) // R20
	AMX_DEFINE_NATIVE(IsBuildingRemovedForPlayer) // R20
	AMX_DEFINE_NATIVE(TogglePlayerGhostMode) // R20
	AMX_DEFINE_NATIVE(GetPlayerGhostMode) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncKeys) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncCameraFrontVector) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncCameraPos) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncCameraMode) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncWeapon) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncWeaponState) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncSpecialAction) // R20
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(SetPlayerTeam)
	AMX_DEFINE_HOOK(SetPlayerSkin)
	AMX_DEFINE_HOOK(SetPlayerName)
	AMX_DEFINE_HOOK(SetPlayerFightingStyle)
	AMX_DEFINE_HOOK(SetPlayerWeather)
	AMX_DEFINE_HOOK(SetPlayerWorldBounds)
	AMX_DEFINE_HOOK(TogglePlayerControllable)
	AMX_DEFINE_HOOK(ShowPlayerDialog)
	AMX_DEFINE_HOOK(RemoveBuildingForPlayer)
};

void PlayersLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
