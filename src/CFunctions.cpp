/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

#include <unordered_map>
#include "sdk/plugincommon.h"
#include "includes/platform.h"

#include "CPlugin.h"
#include "CServer.h"
#include "CFunctions.h"
#include "CAddresses.h"
#include "CCallbackManager.h"
#include "Globals.h"
#include "Memory.h"

/*CConsole__AddStringVariable_t				CSAMPFunctions::pfn__CConsole__AddStringVariable = NULL;
CConsole__GetStringVariable_t				CSAMPFunctions::pfn__CConsole__GetStringVariable = NULL;
CConsole__SetStringVariable_t				CSAMPFunctions::pfn__CConsole__SetStringVariable = NULL;
CConsole__GetIntVariable_t					CSAMPFunctions::pfn__CConsole__GetIntVariable = NULL;
CConsole__SetIntVariable_t					CSAMPFunctions::pfn__CConsole__SetIntVariable = NULL;
CConsole__GetBoolVariable_t					CSAMPFunctions::pfn__CConsole__GetBoolVariable = NULL;
CConsole__ModifyVariableFlags_t				CSAMPFunctions::pfn__CConsole__ModifyVariableFlags = NULL;
CConsole__FindVariable_t					CSAMPFunctions::pfn__CConsole__FindVariable = NULL;
CConsole__SendRules_t						CSAMPFunctions::pfn__CConsole__SendRules = NULL;
CConsole__Execute_t							CSAMPFunctions::pfn__CConsole__Execute = NULL;

CFilterscripts__LoadFilterscript_t			CSAMPFunctions::pfn__CFilterscripts__LoadFilterscript = NULL;
CFilterscripts__UnLoadFilterscript_t		CSAMPFunctions::pfn__CFilterscripts__UnLoadFilterscript = NULL;

CPlayer__SpawnForWorld_t					CSAMPFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CPlayerPool__HandleVehicleRespawn_t			CSAMPFunctions::pfn__CPlayerPool__HandleVehicleRespawn = NULL;
CObject__SpawnForPlayer_t					CSAMPFunctions::pfn__CObject__SpawnForPlayer = NULL;

Packet_WeaponsUpdate_t						CSAMPFunctions::pfn__Packet_WeaponsUpdate = NULL;
Packet_StatsUpdate_t						CSAMPFunctions::pfn__Packet_StatsUpdate = NULL;

format_amxstring_t							CSAMPFunctions::pfn__format_amxstring = NULL;*/

RakNet__Start_t								CSAMPFunctions::pfn__RakNet__Start = NULL;
RakNet__Send_t								CSAMPFunctions::pfn__RakNet__Send = NULL;
RakNet__Receive_t							CSAMPFunctions::pfn__RakNet__Receive = NULL;
RakNet__SetAllowedPlayers_t					CSAMPFunctions::pfn__RakNet__SetAllowedPlayers = NULL;
RakNet__GetLastPing_t						CSAMPFunctions::pfn__RakNet__GetLastPing = NULL;
RakNet__RegisterAsRemoteProcedureCall_t		CSAMPFunctions::pfn__RakNet__RegisterAsRemoteProcedureCall = NULL;
RakNet__UnregisterAsRemoteProcedureCall_t	CSAMPFunctions::pfn__RakNet__UnregisterAsRemoteProcedureCall = NULL;
RakNet__RPC_t								CSAMPFunctions::pfn__RakNet__RPC = NULL;
RakNet__GetLocalIP_t						CSAMPFunctions::pfn__RakNet__GetLocalIP = NULL;
RakNet__GetIndexFromPlayerID_t				CSAMPFunctions::pfn__RakNet__GetIndexFromPlayerID = NULL;
RakNet__GetPlayerIDFromIndex_t				CSAMPFunctions::pfn__RakNet__GetPlayerIDFromIndex = NULL;
RakNet__SetTimeoutTime_t					CSAMPFunctions::pfn__RakNet__SetTimeoutTime = NULL;
RakNet__AddToBanList_t						CSAMPFunctions::pfn__RakNet__AddToBanList = NULL;
RakNet__RemoveFromBanList_t					CSAMPFunctions::pfn__RakNet__RemoveFromBanList = NULL;
RakNet__ClearBanList_t						CSAMPFunctions::pfn__RakNet__ClearBanList = NULL;

void CSAMPFunctions::PreInitialize()
{
	/*INIT_FPTR(CConsole__AddStringVariable);
	INIT_FPTR(CConsole__GetStringVariable);
	INIT_FPTR(CConsole__SetStringVariable);
	INIT_FPTR(CConsole__GetIntVariable);
	INIT_FPTR(CConsole__SetIntVariable);
	INIT_FPTR(CConsole__GetBoolVariable);
	INIT_FPTR(CConsole__ModifyVariableFlags);
	INIT_FPTR(CConsole__FindVariable);
	INIT_FPTR(CConsole__SendRules);
	INIT_FPTR(CConsole__Execute);

	INIT_FPTR(CFilterscripts__LoadFilterscript);
	INIT_FPTR(CFilterscripts__UnLoadFilterscript);

	INIT_FPTR(CPlayer__SpawnForWorld);
	INIT_FPTR(CPlayerPool__HandleVehicleRespawn);
	INIT_FPTR(CObject__SpawnForPlayer);

	INIT_FPTR(Packet_WeaponsUpdate);
	INIT_FPTR(Packet_StatsUpdate);

	INIT_FPTR(format_amxstring);*/
}

void CSAMPFunctions::PostInitialize()
{
	// Get pNetGame
	int (*pfn_GetNetGame)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_NETGAME];
	pNetGame = (CNetGame*)pfn_GetNetGame();

	// Get pConsole
	int (*pfn_GetConsole)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_CONSOLE];
	pConsole = (void*)pfn_GetConsole();

	// Get pRakServer
	int (*pfn_GetRakServer)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_RAKSERVER];
	pRakServer = (void*)pfn_GetRakServer();

	CServer::Init(*pNetGame);

	// Init RakServer functions & hooks
	int *pRakServer_VTBL = ((int*)(*(void**)pRakServer));
	//logprintf("pRakServer_VTBL: %x", pRakServer_VTBL);

	POINT_TO_MEMBER(RakNet__Start, pRakServer_VTBL[RAKNET_START_OFFSET]);
	POINT_TO_MEMBER(RakNet__Send, pRakServer_VTBL[RAKNET_SEND_OFFSET]);
	POINT_TO_MEMBER(RakNet__Receive, pRakServer_VTBL[RAKNET_RECEIVE_OFFSET]);
	POINT_TO_MEMBER(RakNet__SetAllowedPlayers, pRakServer_VTBL[RAKNET_SET_ALLOWED_PLAYERS_OFFSET]);
	POINT_TO_MEMBER(RakNet__GetLastPing, pRakServer_VTBL[RAKNET_GET_LAST_PING_OFFSET]);
	POINT_TO_MEMBER(RakNet__RegisterAsRemoteProcedureCall, pRakServer_VTBL[RAKNET_REGISTER_RPC_OFFSET]);
	POINT_TO_MEMBER(RakNet__UnregisterAsRemoteProcedureCall, pRakServer_VTBL[RAKNET_UNREGISTER_RPC_OFFSET]);
	POINT_TO_MEMBER(RakNet__RPC, pRakServer_VTBL[RAKNET_RPC_OFFSET]);
	POINT_TO_MEMBER(RakNet__GetLocalIP, pRakServer_VTBL[RAKNET_GET_LOCAL_IP_OFFSET]);
	POINT_TO_MEMBER(RakNet__GetIndexFromPlayerID, pRakServer_VTBL[RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET]);
	POINT_TO_MEMBER(RakNet__GetPlayerIDFromIndex, pRakServer_VTBL[RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET]);
	POINT_TO_MEMBER(RakNet__SetTimeoutTime, pRakServer_VTBL[RAKNET_SET_TIMEOUT_OFFSET]);
	POINT_TO_MEMBER(RakNet__AddToBanList, pRakServer_VTBL[RAKNET_ADD_BAN_OFFSET]);
	POINT_TO_MEMBER(RakNet__RemoveFromBanList, pRakServer_VTBL[RAKNET_REMOVE_BAN_OFFSET]);
	POINT_TO_MEMBER(RakNet__ClearBanList, pRakServer_VTBL[RAKNET_CLEAR_BAN_OFFSET]);
	
	Unlock((void*)&pRakServer_VTBL[RAKNET_SEND_OFFSET], 4); pRakServer_VTBL[RAKNET_SEND_OFFSET] = reinterpret_cast<int>(CHookRakServer::Send);
	Unlock((void*)&pRakServer_VTBL[RAKNET_RPC_OFFSET], 4); pRakServer_VTBL[RAKNET_RPC_OFFSET] = reinterpret_cast<int>(CHookRakServer::RPC_2);
	Unlock((void*)&pRakServer_VTBL[RAKNET_RECEIVE_OFFSET], 4); pRakServer_VTBL[RAKNET_RECEIVE_OFFSET] = reinterpret_cast<int>(CHookRakServer::Receive);
	Unlock((void*)&pRakServer_VTBL[RAKNET_ADD_BAN_OFFSET], 4); pRakServer_VTBL[RAKNET_ADD_BAN_OFFSET] = reinterpret_cast<int>(CHookRakServer::AddToBanList);
	Unlock((void*)&pRakServer_VTBL[RAKNET_REMOVE_BAN_OFFSET], 4); pRakServer_VTBL[RAKNET_REMOVE_BAN_OFFSET] = reinterpret_cast<int>(CHookRakServer::RemoveFromBanList);
	Unlock((void*)&pRakServer_VTBL[RAKNET_CLEAR_BAN_OFFSET], 4); pRakServer_VTBL[RAKNET_CLEAR_BAN_OFFSET] = reinterpret_cast<int>(CHookRakServer::ClearBanList);
}

void CSAMPFunctions::AddStringVariable(char *szRule, int flags, char *szString, void *changefunc)
{
	CAddress::FUNC_CConsole__AddStringVariable(pConsole, szRule, flags, szString, changefunc);
}

char* CSAMPFunctions::GetStringVariable(char *szRule)
{
	return CAddress::FUNC_CConsole__GetStringVariable(pConsole, szRule);
}

void CSAMPFunctions::SetStringVariable(char *szRule, char *szString)
{
	CAddress::FUNC_CConsole__SetStringVariable(pConsole, szRule, szString);
}

int CSAMPFunctions::GetIntVariable(char *szRule)
{
	return CAddress::FUNC_CConsole__GetIntVariable(pConsole, szRule);
}

bool CSAMPFunctions::GetBoolVariable(char *szRule)
{
	return CAddress::FUNC_CConsole__GetBoolVariable(pConsole, szRule);
}

void CSAMPFunctions::SetIntVariable(char *szRule, int value)
{
	CAddress::FUNC_CConsole__SetIntVariable(pConsole, szRule, value);
}

void CSAMPFunctions::ModifyVariableFlags(char *szRule, int value)
{
	CAddress::FUNC_CConsole__ModifyVariableFlags(pConsole, szRule, value);
}

ConsoleVariable_s* CSAMPFunctions::FindVariable(char *szRule)
{
	return CAddress::FUNC_CConsole__FindVariable(pConsole, szRule);
}

void CSAMPFunctions::SendRules(SOCKET s, char* data, const sockaddr_in* to, int tolen)
{
	CAddress::FUNC_CConsole__SendRules(pConsole, s, data, to, tolen);
}

void CSAMPFunctions::Execute(char* pExecLine)
{
	CAddress::FUNC_CConsole__Execute(pConsole, pExecLine);
}

bool CSAMPFunctions::LoadFilterscript(const char *szName)
{
	return CAddress::FUNC_CFilterscripts__LoadFilterscript(pNetGame->pFilterScriptPool, szName);
}

bool CSAMPFunctions::UnLoadFilterscript(const char *szName)
{
	return CAddress::FUNC_CFilterscripts__UnLoadFilterscript(pNetGame->pFilterScriptPool, szName);
}

void CSAMPFunctions::SpawnPlayer(int playerid)
{
	CAddress::FUNC_CPlayer__SpawnForWorld(pNetGame->pPlayerPool->pPlayer[playerid]);
}

void CSAMPFunctions::SpawnObjectForPlayer(CObject *pObject, WORD wPlayerID)
{
	CAddress::FUNC_CObject__SpawnForPlayer(pObject, wPlayerID);
}

void CSAMPFunctions::SpawnObjectForPlayer(int iObjectId, WORD wPlayerID)
{
	SpawnObjectForPlayer(pNetGame->pObjectPool->pObjects[iObjectId], wPlayerID);
}

void CSAMPFunctions::Packet_WeaponsUpdate(Packet *p)
{
	CAddress::FUNC_Packet_WeaponsUpdate(pNetGame, p);
}

void CSAMPFunctions::Packet_StatsUpdate(Packet *p)
{
	CAddress::FUNC_Packet_StatsUpdate(pNetGame, p);
}

char* CSAMPFunctions::format_amxstring(AMX *amx, cell *params, int parm, int &len)
{
	return CAddress::FUNC_format_amxstring(amx, params, parm, len);
}

bool CSAMPFunctions::Start(unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress)
{
	return pfn__RakNet__Start(pRakServer, AllowedPlayers, depreciated, threadSleepTimer, port, forceHostAddress);
}

bool CSAMPFunctions::Send(RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast)
{
	return pfn__RakNet__Send(pRakServer, parameters, priority, reliability, orderingChannel, playerId, broadcast);
}

Packet* CSAMPFunctions::Receive(void* ppRakServer)
{
	return pfn__RakNet__Receive(ppRakServer);
}

void CSAMPFunctions::SetAllowedPlayers(unsigned short numberAllowed)
{
	pfn__RakNet__SetAllowedPlayers(pRakServer, numberAllowed);
}

int CSAMPFunctions::GetLastPing(const PlayerID playerId)
{
	return pfn__RakNet__GetLastPing(pRakServer, playerId);
}

void CSAMPFunctions::RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms))
{
	pfn__RakNet__RegisterAsRemoteProcedureCall(pRakServer, uniqueID, functionPointer);
}

void CSAMPFunctions::UnregisterAsRemoteProcedureCall(int* uniqueID)
{
	pfn__RakNet__UnregisterAsRemoteProcedureCall(pRakServer, uniqueID);
}

bool CSAMPFunctions::RPC(BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
{
	return pfn__RakNet__RPC(pRakServer, uniqueID, parameters, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp);
}

const char* CSAMPFunctions::GetLocalIP(unsigned int index)
{
	return pfn__RakNet__GetLocalIP(pRakServer, index);
}

int CSAMPFunctions::GetIndexFromPlayerID(PlayerID playerId)
{
	return pfn__RakNet__GetIndexFromPlayerID(pRakServer, playerId);
}

PlayerID CSAMPFunctions::GetPlayerIDFromIndex(int index)
{
	return pfn__RakNet__GetPlayerIDFromIndex(pRakServer, index);
}

void CSAMPFunctions::SetTimeoutTime(RakNetTime timeMS, const PlayerID target)
{
	pfn__RakNet__SetTimeoutTime(pRakServer, timeMS, target);
}

void CSAMPFunctions::AddToBanList(const char *IP, unsigned int milliseconds)
{
	pfn__RakNet__AddToBanList(pRakServer, IP, milliseconds);
}

void CSAMPFunctions::RemoveFromBanList(const char *IP)
{
	pfn__RakNet__RemoveFromBanList(pRakServer, IP);
}

void CSAMPFunctions::ClearBanList(void)
{
	pfn__RakNet__ClearBanList(pRakServer);
}

void CSAMPFunctions::RespawnVehicle(CVehicle *pVehicle)
{
	pVehicle->vehMatrix = {};
	pVehicle->vecVelocity = {};
	pVehicle->vecTurnSpeed = {};
	pVehicle->vehModInfo = {};

	pVehicle->fHealth = 1000.0f;
	pVehicle->vehDoorStatus = 0;
	pVehicle->vehPanelStatus = 0;
	pVehicle->vehLightStatus = 0;
	pVehicle->vehTireStatus = 0;
	pVehicle->bDead = 0;
	pVehicle->bDeathNotification = 0;
	pVehicle->bOccupied = 0;
	pVehicle->vehRespawnTick = GetTickCount();
	pVehicle->vehOccupiedTick = GetTickCount();

	memset(&pVehicle->vehParamEx, -1, sizeof(pVehicle->vehParamEx));
	// logprintf("respawned vehicle: %d", pVehicle->wVehicleID);

	// Check if vehicle has custom spawn
	CServer::Get()->VehiclePool.MapExtra(
		pVehicle->wVehicleID, [pVehicle](CVehicleData &data)
		{
			// If yes, then re-create the vehicle at different location
			if (data.customSpawnData)
			{
				pVehicle->customSpawn.iModelID = data.spawnData.iModelID;
				pVehicle->customSpawn.vecPos = data.spawnData.vecPos;
				pVehicle->customSpawn.fRot = data.spawnData.fRot;
				pVehicle->customSpawn.iColor1 = data.spawnData.iColor1;
				pVehicle->customSpawn.iColor2 = data.spawnData.iColor2;
				pVehicle->customSpawn.iRespawnTime = data.spawnData.iRespawnTime;
				pVehicle->customSpawn.iInterior = data.spawnData.iInterior;
				// logprintf("custom vehicle spawn respawned %d", pVehicle->wVehicleID);
				data.customSpawnData = false;
			}
		}
	);

	pVehicle->vehModInfo.iColor1 = pVehicle->customSpawn.iColor1;
	pVehicle->vehModInfo.iColor2 = pVehicle->customSpawn.iColor2;
	pVehicle->vehMatrix.pos = pVehicle->customSpawn.vecPos;
	pVehicle->vecPosition = pVehicle->customSpawn.vecPos;

	if (pNetGame && pNetGame->pPlayerPool)
		CAddress::FUNC_CPlayerPool__HandleVehicleRespawn(pNetGame->pPlayerPool, pVehicle->wVehicleID);

	CCallbackManager::OnVehicleSpawn(pVehicle->wVehicleID);
}