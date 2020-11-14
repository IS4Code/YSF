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

	// native GetPlayerDisabledKeysSync(playerid, &keys, &updown, &leftright);
	AMX_DECLARE_NATIVE(GetPlayerDisabledKeysSync)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		CScriptParams::Get()->Add(CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeys, static_cast<short>(CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysUD), static_cast<short>(CServer::Get()->PlayerPool.Extra(playerid).wDisabledKeysLR));
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

	// native GetPlayerSyncWeapon(playerid);
	AMX_DECLARE_NATIVE(GetPlayerSyncWeapon)
	{
		CHECK_PARAMS(1, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		switch(pPlayer->byteState)
		{
			case PLAYER_STATE_ONFOOT:
				return pPlayer->syncData.byteWeapon;
			case PLAYER_STATE_DRIVER:
				return pPlayer->vehicleSyncData.bytePlayerWeapon;
			case PLAYER_STATE_PASSENGER:
				return pPlayer->passengerSyncData.bytePlayerWeapon;
		}
		return 0;
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

	// native SetPlayerSyncPos(playerid, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.vecPosition.fX = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.vecPosition.fY = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.vecPosition.fZ = CScriptParams::Get()->ReadFloat();
		return 1;
	}

	// native SetPlayerSyncVelocity(playerid, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncVelocity)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.vecVelocity.fX = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.vecVelocity.fY = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.vecVelocity.fZ = CScriptParams::Get()->ReadFloat();
		return 1;
	}

	// native SetPlayerSyncRotationQuat(playerid, Float:w, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncRotationQuat)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.fQuaternion[0] = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.fQuaternion[1] = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.fQuaternion[2] = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.fQuaternion[3] = CScriptParams::Get()->ReadFloat();

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

	// native SetPlayerSyncHealth(playerid, Float:health);
	AMX_DECLARE_NATIVE(SetPlayerSyncHealth)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.byteHealth = pPlayer->vehicleSyncData.bytePlayerHealth = pPlayer->passengerSyncData.bytePlayerHealth = static_cast<BYTE>(CScriptParams::Get()->ReadFloat());
		return 1;
	}

	// native SetPlayerSyncArmour(playerid, Float:armour);
	AMX_DECLARE_NATIVE(SetPlayerSyncArmour)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;
		
		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.byteArmour = pPlayer->vehicleSyncData.bytePlayerArmour = pPlayer->passengerSyncData.bytePlayerArmour = static_cast<BYTE>(CScriptParams::Get()->ReadFloat());

		return 1;
	}

	// native SetPlayerSyncPosition(playerid, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncPosition)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->syncData.vecPosition.fX = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.vecPosition.fY = CScriptParams::Get()->ReadFloat();
		pPlayer->syncData.vecPosition.fZ = CScriptParams::Get()->ReadFloat();

		return 1;
	}

	// native SetPlayerSyncVehicleId(playerid, vehicleid);
	AMX_DECLARE_NATIVE(SetPlayerSyncVehicleId)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->vehicleSyncData.wVehicleId = pPlayer->passengerSyncData.wVehicleId = CScriptParams::Get()->ReadInt();
		
		return 1;
	}

	// native SetPlayerSyncVehicleSeat(playerid, seat);
	AMX_DECLARE_NATIVE(SetPlayerSyncVehicleSeat)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->passengerSyncData.byteSeatFlags = CScriptParams::Get()->ReadInt();

		return 1;
	}

	// native SetPlayerSyncVehiclePosition(playerid, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncVehiclePosition)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->vehicleSyncData.vecPosition.fX = pPlayer->passengerSyncData.vecPosition.fX = CScriptParams::Get()->ReadFloat();
		pPlayer->vehicleSyncData.vecPosition.fY = pPlayer->passengerSyncData.vecPosition.fY = CScriptParams::Get()->ReadFloat();
		pPlayer->vehicleSyncData.vecPosition.fZ = pPlayer->passengerSyncData.vecPosition.fZ = CScriptParams::Get()->ReadFloat();
		
		return 1;
	}

	// native SetPlayerSyncVehicleVelocity(playerid, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncVehicleVelocity)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->vehicleSyncData.vecVelocity.fX = CScriptParams::Get()->ReadFloat();
		pPlayer->vehicleSyncData.vecVelocity.fY = CScriptParams::Get()->ReadFloat();
		pPlayer->vehicleSyncData.vecVelocity.fZ = CScriptParams::Get()->ReadFloat();

		return 1;
	}

	// native SetPlayerSyncVehicleRotQuat(playerid, Float:w, Float:x, Float:y, Float:z);
	AMX_DECLARE_NATIVE(SetPlayerSyncVehicleRotQuat)
	{
		CHECK_PARAMS(5, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->vehicleSyncData.fQuaternion[0] = CScriptParams::Get()->ReadFloat();
		pPlayer->vehicleSyncData.fQuaternion[1] = CScriptParams::Get()->ReadFloat();
		pPlayer->vehicleSyncData.fQuaternion[2] = CScriptParams::Get()->ReadFloat();
		pPlayer->vehicleSyncData.fQuaternion[3] = CScriptParams::Get()->ReadFloat();

		return 1;
	}

	// native SetPlayerSyncVehicleHealth(playerid, Float:health);
	AMX_DECLARE_NATIVE(SetPlayerSyncVehicleHealth)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->vehicleSyncData.fHealth = CScriptParams::Get()->ReadFloat();

		return 1;
	}

	// native SetPlayerSyncTrainSpeed(playerid, Float:speed);
	AMX_DECLARE_NATIVE(SetPlayerSyncTrainSpeed)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		pPlayer->vehicleSyncData.fTrainSpeed = CScriptParams::Get()->ReadFloat();

		return 1;
	}

	// native SendPlayerDeath(playerid, forplayerid=-1);
	AMX_DECLARE_NATIVE(SendPlayerDeath)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		int forplayerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(forplayerid) && forplayerid != -1) return 0;

		RakNet::BitStream bs;
		bs.Write((WORD)playerid);
		CSAMPFunctions::RPC(&RPC_DeathBroadcast, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, forplayerid == -1 ? UNASSIGNED_PLAYER_ID : CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), forplayerid == -1, 0);
		return 1;
	}

	// native UpdatePlayerSyncData(playerid, type=-1);
	AMX_DECLARE_NATIVE(UpdatePlayerSyncData)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		int type = CScriptParams::Get()->ReadInt();

		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
		if(type == -1)
		{
			switch(pPlayer->byteState)
			{
				case PLAYER_STATE_ONFOOT:
					pPlayer->iUpdateState = 1;
					break;
				case PLAYER_STATE_DRIVER:
					pPlayer->iUpdateState = 2;
					break;
				case PLAYER_STATE_PASSENGER:
					pPlayer->iUpdateState = 3;
					break;
			}
		}else{
			pPlayer->iUpdateState = type;
		}
		return 1;
	}

	// native SetPlayerConnectMode(playerid, E_PLAYER_CONNECT_MODE:mode);
	AMX_DECLARE_NATIVE(SetPlayerConnectMode)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;
		pNetGame->pPlayerPool->pRemoteSystem[playerid]->connectMode = static_cast<RemoteSystemStruct::ConnectMode>(CScriptParams::Get()->ReadInt());
		return 1;
	}

	// native E_PLAYER_CONNECT_MODE:GetPlayerConnectMode(playerid);
	AMX_DECLARE_NATIVE(GetPlayerConnectMode)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;
		return static_cast<cell>(pNetGame->pPlayerPool->pRemoteSystem[playerid]->connectMode);
	}

	// native SendPlayerClientGameInit(playerid, bool:usecjwalk=bool:-1, bool:limitglobalchat=bool:-1, Float:globalchatradius=Float:-1, Float:nametagdistance=Float:-1, bool:disableenterexits=bool:-1, bool:nametaglos=bool:-1, bool:manualvehengineandlights=bool:-1, spawnsavailable=-1, bool:shownametags=bool:-1, bool:showplayermarkers=bool:-1, onfoot_rate=-1, incar_rate=-1, weapon_rate=-1, lagcompmode=-1, bool:vehiclefriendlyfire=bool:-1, const hostname[]="");
	AMX_DECLARE_NATIVE(SendPlayerClientGameInit)
	{
		CHECK_PARAMS(17, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;
		bool usecjwalk = params[2] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->bUseCJWalk) != 0;
		bool limitglobalchat = params[3] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->bLimitGlobalChatRadius) != 0;
		float globalchatradius = params[4] != -1 ? CScriptParams::Get()->ReadFloat() : pNetGame->fGlobalChatRadius;
		float nametagdistance = params[5] != -1 ? CScriptParams::Get()->ReadFloat() : pNetGame->fNameTagDrawDistance;
		bool disableenterexits = params[6] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->byteDisableEnterExits) != 0;
		bool nametaglos = params[7] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->byteNameTagLOS) != 0;
		bool manualvehengineandlights = params[8] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->bManulVehicleEngineAndLights) != 0;
		int spawnsavailable = params[9] != -1 ? CScriptParams::Get()->ReadInt() : pNetGame->iSpawnsAvailable;
		bool shownametags = params[10] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->byteShowNameTags) != 0;
		bool showplayermarkers = params[11] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->bShowPlayerMarkers) != 0;
		int onfoot_rate = params[12] != -1 ? CScriptParams::Get()->ReadInt() : CSAMPFunctions::GetIntVariable("onfoot_rate");
		int incar_rate = params[13] != -1 ? CScriptParams::Get()->ReadInt() : CSAMPFunctions::GetIntVariable("incar_rate");
		int weapon_rate = params[14] != -1 ? CScriptParams::Get()->ReadInt() : CSAMPFunctions::GetIntVariable("weapon_rate");
		int lagcompmode = params[15] != -1 ? CScriptParams::Get()->ReadInt() : CSAMPFunctions::GetIntVariable("lagcompmode");
		bool vehiclefriendlyfire = params[16] != -1 ? CScriptParams::Get()->ReadBool() : static_cast<int>(pNetGame->bVehicleFriendlyFire) != 0;

		RakNet::BitStream bsSync;
		bsSync.Write((bool)!!pNetGame->byteEnableZoneNames);
		bsSync.Write((bool)usecjwalk);
		bsSync.Write((bool)!!pNetGame->byteAllowWeapons);
		bsSync.Write(limitglobalchat);
		bsSync.Write(globalchatradius);
		bsSync.Write((bool)!!pNetGame->byteStuntBonus);
		bsSync.Write(nametagdistance);
		bsSync.Write(disableenterexits);
		bsSync.Write(nametaglos);
		bsSync.Write(manualvehengineandlights);
		bsSync.Write(pNetGame->iSpawnsAvailable);
		bsSync.Write((WORD)playerid);
		bsSync.Write(shownametags);
		bsSync.Write((int)showplayermarkers);
		bsSync.Write(pNetGame->byteWorldTimeHour);
		bsSync.Write(pNetGame->byteWeather);
		bsSync.Write(pNetGame->fGravity);
		bsSync.Write((bool)!!pNetGame->bLanMode);
		bsSync.Write(pNetGame->iDeathDropMoney);
		bsSync.Write(false);
		bsSync.Write(onfoot_rate);
		bsSync.Write(incar_rate);
		bsSync.Write(weapon_rate);
		bsSync.Write((int)2);
		bsSync.Write(lagcompmode);

		std::string hostname;
		CScriptParams::Get()->Read(hostname);

		const char* szHostName = !hostname.empty() ? hostname.c_str() : CSAMPFunctions::GetStringVariable("hostname");
		if(szHostName)
		{
			size_t len = strlen(szHostName);
			bsSync.Write((BYTE)len);
			bsSync.Write(szHostName, len);
		} else
		{
			bsSync.Write((BYTE)0);
		}
		bsSync.Write((char*)&pNetGame->pVehiclePool, 212); // modelsUsed
		bsSync.Write((DWORD)vehiclefriendlyfire);

		CSAMPFunctions::RPC(&RPC_InitGame, &bsSync, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);

		return 1;
	}

	// native GetPlayerRawIp(playerid);
	AMX_DECLARE_NATIVE(GetPlayerRawIp)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(playerid)) return 0;

		return CSAMPFunctions::GetPlayerIDFromIndex(playerid).binaryAddress;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(SetPlayerAdmin)

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
	AMX_DEFINE_NATIVE(TogglePlayerWidescreen)
	AMX_DEFINE_NATIVE(IsPlayerWidescreenToggled)
	AMX_DEFINE_NATIVE(GetSpawnInfo) // R8
	AMX_DEFINE_NATIVE(GetPlayerSkillLevel) // R3
	AMX_DEFINE_NATIVE(IsPlayerCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerCheckpoint) // R4
	AMX_DEFINE_NATIVE(IsPlayerRaceCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerRaceCheckpoint) // R4
	AMX_DEFINE_NATIVE(IsPlayerInModShop) // R4
	AMX_DEFINE_NATIVE(SendBulletData) // R6
	AMX_DEFINE_NATIVE(ShowPlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(HidePlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(AddPlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(RemovePlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(SetPlayerChatBubbleForPlayer) // R10
	AMX_DEFINE_NATIVE(SetPlayerVersion) // R9
	AMX_DEFINE_NATIVE(IsPlayerSpawned) // R9
	AMX_DEFINE_NATIVE(SpawnForWorld) // R10
	AMX_DEFINE_NATIVE(BroadcastDeath) // R13
	AMX_DEFINE_NATIVE(IsPlayerCameraTargetEnabled) // R13
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

	AMX_DEFINE_NATIVE(GetPlayerGhostMode) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncKeys) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncCameraFrontVector) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncCameraPos) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncCameraMode) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncWeapon) // R20
	AMX_DEFINE_NATIVE(GetPlayerSyncWeapon) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncWeaponState) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncSpecialAction) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncHealth) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncArmour) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncPosition) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncVehicleId) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncVehiclePosition) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncVehicleVelocity) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncVehicleRotQuat) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncVehicleHealth) // R20
	AMX_DEFINE_NATIVE(SendPlayerDeath) // R20
	AMX_DEFINE_NATIVE(UpdatePlayerSyncData) // R20
	AMX_DEFINE_NATIVE(SetPlayerConnectMode) // R20
	AMX_DEFINE_NATIVE(GetPlayerConnectMode) // R20
	AMX_DEFINE_NATIVE(SendPlayerClientGameInit) // R20
	AMX_DEFINE_NATIVE(SetPlayerSyncVehicleSeat)
	AMX_DEFINE_NATIVE(SetPlayerSyncTrainSpeed)
	AMX_DEFINE_NATIVE(SetPlayerSyncPos)
	AMX_DEFINE_NATIVE(SetPlayerSyncVelocity)
	AMX_DEFINE_NATIVE(SetPlayerSyncRotationQuat)
	AMX_DEFINE_NATIVE(GetPlayerRawIp)
};

void PlayersLoadNatives()
{
	RegisterNatives(native_list);
}
