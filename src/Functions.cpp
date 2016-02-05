#include "main.h"

CConsole__AddStringVariable_t			CSAMPFunctions::pfn__CConsole__AddStringVariable = NULL;
CConsole__GetStringVariable_t			CSAMPFunctions::pfn__CConsole__GetStringVariable = NULL;
CConsole__SetStringVariable_t			CSAMPFunctions::pfn__CConsole__SetStringVariable = NULL;
CConsole__GetIntVariable_t				CSAMPFunctions::pfn__CConsole__GetIntVariable = NULL;
CConsole__SetIntVariable_t				CSAMPFunctions::pfn__CConsole__SetIntVariable = NULL;
CConsole__GetBoolVariable_t				CSAMPFunctions::pfn__CConsole__GetBoolVariable = NULL;
CConsole__ModifyVariableFlags_t			CSAMPFunctions::pfn__CConsole__ModifyVariableFlags = NULL;
CConsole__FindVariable_t				CSAMPFunctions::pfn__CConsole__FindVariable = NULL;
CConsole__SendRules_t					CSAMPFunctions::pfn__CConsole__SendRules = NULL;
CConsole__Execute_t						CSAMPFunctions::pfn__CConsole__Execute = NULL;

CFilterscripts__LoadFilterscript_t		CSAMPFunctions::pfn__CFilterscripts__LoadFilterscript = NULL;
CFilterscripts__UnLoadFilterscript_t	CSAMPFunctions::pfn__CFilterscripts__UnLoadFilterscript = NULL;

CPlayer__SpawnForWorld_t				CSAMPFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CPlayerPool__HandleVehicleRespawn_t		CSAMPFunctions::pfn__CPlayerPool__HandleVehicleRespawn = NULL;

Packet_WeaponsUpdate_t					CSAMPFunctions::pfn__Packet_WeaponsUpdate = NULL;
Packet_StatsUpdate_t					CSAMPFunctions::pfn__Packet_StatsUpdate = NULL;

format_amxstring_t						CSAMPFunctions::pfn__format_amxstring = NULL;

RakNet__Send_t							CSAMPFunctions::pfn__RakNet__Send = NULL;
RakNet__RPC_t							CSAMPFunctions::pfn__RakNet__RPC = NULL;

void CSAMPFunctions::PreInitialize()
{
	pfn__CConsole__AddStringVariable = (CConsole__AddStringVariable_t)(CAddress::FUNC_CConsole__AddStringVariable);
	pfn__CConsole__GetStringVariable = (CConsole__GetStringVariable_t)(CAddress::FUNC_CConsole__GetStringVariable);
	pfn__CConsole__SetStringVariable = (CConsole__SetStringVariable_t)(CAddress::FUNC_CConsole__SetStringVariable);
	pfn__CConsole__GetIntVariable = (CConsole__GetIntVariable_t)(CAddress::FUNC_CConsole__GetIntVariable);
	pfn__CConsole__SetIntVariable = (CConsole__SetIntVariable_t)(CAddress::FUNC_CConsole__SetIntVariable);
	pfn__CConsole__GetBoolVariable = (CConsole__GetBoolVariable_t)(CAddress::FUNC_CConsole__GetBoolVariable);
	pfn__CConsole__ModifyVariableFlags = (CConsole__ModifyVariableFlags_t)(CAddress::FUNC_CConsole__ModifyVariableFlags);
	pfn__CConsole__FindVariable = (CConsole__FindVariable_t)(CAddress::FUNC_CConsole__FindVariable);
	pfn__CConsole__SendRules = (CConsole__SendRules_t)(CAddress::FUNC_CConsole__SendRules);
	pfn__CConsole__Execute = (CConsole__Execute_t)(CAddress::FUNC_CConsole__Execute);

	pfn__CFilterscripts__LoadFilterscript = (CFilterscripts__LoadFilterscript_t)(CAddress::FUNC_CFilterscripts__LoadFilterscript);
	pfn__CFilterscripts__UnLoadFilterscript = (CFilterscripts__UnLoadFilterscript_t)(CAddress::FUNC_CFilterscripts__UnLoadFilterscript);

	pfn__CPlayer__SpawnForWorld = (CPlayer__SpawnForWorld_t)(CAddress::FUNC_CPlayer__SpawnForWorld);
	pfn__CPlayerPool__HandleVehicleRespawn = (CPlayerPool__HandleVehicleRespawn_t)(CAddress::FUNC_CPlayerPool__HandleVehicleRespawn);

	pfn__Packet_WeaponsUpdate = (Packet_WeaponsUpdate_t)(CAddress::FUNC_Packet_WeaponsUpdate);
	pfn__Packet_StatsUpdate = (Packet_WeaponsUpdate_t)(CAddress::FUNC_Packet_StatsUpdate);

	pfn__format_amxstring = (format_amxstring_t)(CAddress::FUNC_format_amxstring);
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
	pRakServer = (RakServer*)pfn_GetRakServer();

	// Init Send, RPC hook
	int *pRakServer_VTBL = ((int*)(*(void**)pRakServer));
	//logprintf("pRakServer_VTBL: %x", pRakServer_VTBL);

	Unlock((void*)&pRakServer_VTBL[RAKNET_SEND_OFFSET], 4);
	Unlock((void*)&pRakServer_VTBL[RAKNET_RPC_OFFSET], 4);

	pfn__RakNet__Send = (RakNet__Send_t)(pRakServer_VTBL[RAKNET_SEND_OFFSET]);
	pfn__RakNet__RPC = (RakNet__RPC_t)(pRakServer_VTBL[RAKNET_RPC_OFFSET]);

	pRakServer_VTBL[RAKNET_SEND_OFFSET] = (int)CHookRakServer::Send;
	pRakServer_VTBL[RAKNET_RPC_OFFSET] = (int)CHookRakServer::RPC_2;
}

void CSAMPFunctions::AddStringVariable(char *szRule, int flags, char *szString, void *changefunc)
{
	pfn__CConsole__AddStringVariable(pConsole, szRule, flags, szString, changefunc);
}

char* CSAMPFunctions::GetStringVariable(char *szRule)
{
	return pfn__CConsole__GetStringVariable(pConsole, szRule);
}

void CSAMPFunctions::SetStringVariable(char *szRule, char *szString)
{
	pfn__CConsole__SetStringVariable(pConsole, szRule, szString);
}

int CSAMPFunctions::GetIntVariable(char *szRule)
{
	return pfn__CConsole__GetIntVariable(pConsole, szRule);
}

bool CSAMPFunctions::GetBoolVariable(char *szRule)
{
	return pfn__CConsole__GetBoolVariable(pConsole, szRule);
}

void CSAMPFunctions::SetIntVariable(char *szRule, int value)
{
	pfn__CConsole__SetIntVariable(pConsole, szRule, value);
}

void CSAMPFunctions::ModifyVariableFlags(char *szRule, int value)
{
	pfn__CConsole__ModifyVariableFlags(pConsole, szRule, value);
}

ConsoleVariable_s* CSAMPFunctions::FindVariable(char *szRule)
{
	return pfn__CConsole__FindVariable(pConsole, szRule);
}

void CSAMPFunctions::SendRules(SOCKET s, char* data, const sockaddr_in* to, int tolen)
{
	pfn__CConsole__SendRules(pConsole, s, data, to, tolen);
}

void CSAMPFunctions::Execute(char* pExecLine)
{
	pfn__CConsole__Execute(pConsole, pExecLine);
}

bool CSAMPFunctions::LoadFilterscript(char *szName)
{
	return pfn__CFilterscripts__LoadFilterscript(pNetGame->pFilterScriptPool, szName);
}

bool CSAMPFunctions::UnLoadFilterscript(char *szName)
{
	return pfn__CFilterscripts__UnLoadFilterscript(pNetGame->pFilterScriptPool, szName);
}

void CSAMPFunctions::SpawnPlayer_(int playerid)
{
	pfn__CPlayer__SpawnForWorld(pNetGame->pPlayerPool->pPlayer[playerid]);
}

void CSAMPFunctions::Packet_WeaponsUpdate(Packet *p)
{
	pfn__Packet_WeaponsUpdate(pNetGame, p);
}

void CSAMPFunctions::Packet_StatsUpdate(Packet *p)
{
	pfn__Packet_StatsUpdate(pNetGame, p);
}

char* CSAMPFunctions::format_amxstring(AMX *amx, cell *params, int parm, int &len)
{
	return pfn__format_amxstring(amx, params, parm, len);
}

bool CSAMPFunctions::Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast)
{
	return pfn__RakNet__Send(ppRakServer, parameters, priority, reliability, orderingChannel, playerId, broadcast);
}

bool CSAMPFunctions::RPC(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
{
	return pfn__RakNet__RPC(ppRakServer, uniqueID, parameters, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp);
}

void CSAMPFunctions::RespawnVehicle(CVehicle *pVehicle)
{
	memset(&pVehicle->vehMatrix, 0, sizeof(MATRIX4X4));
	memset(&pVehicle->vecVelocity, 0, sizeof(CVector));
	memset(&pVehicle->vecTurnSpeed, 0, sizeof(CVector));
	memset(&pVehicle->vehModInfo, 0, sizeof(CVehicleModInfo));

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
	pVehicle->vehParamEx.alarm = -1;
	pVehicle->vehParamEx.bonnet = -1;
	pVehicle->vehParamEx.boot = -1;
	pVehicle->vehParamEx.doors = -1;
	pVehicle->vehParamEx.engine = -1;
	pVehicle->vehParamEx.lights = -1;
	pVehicle->vehParamEx.objective = -1;
	pVehicle->vehParamEx.siren = -1;
	pVehicle->vehParamEx.door_driver = -1;
	pVehicle->vehParamEx.door_passenger = -1;
	pVehicle->vehParamEx.door_backleft = -1;
	pVehicle->vehParamEx.door_backright = -1;
	pVehicle->vehParamEx.window_driver = -1;
	pVehicle->vehParamEx.window_passenger = -1;
	pVehicle->vehParamEx.window_backleft = -1;
	pVehicle->vehParamEx.window_backright = -1;
	// logprintf("respawned vehicle: %d", pVehicle->wVehicleID);

	// Check if vehicle has custom spawn
	std::map<int, CVehicleSpawn>::iterator v = pServer->vehicleSpawnData.find(pVehicle->wVehicleID);
	if(v == pServer->vehicleSpawnData.end())
	{

	}
	// If yes, then re-create the vehicle at different location
	else 
	{			
		pVehicle->customSpawn.iModelID = v->second.iModelID;
		pVehicle->customSpawn.vecPos = v->second.vecPos;
		pVehicle->customSpawn.fRot = v->second.fRot;
		pVehicle->customSpawn.iColor1 = v->second.iColor1;
		pVehicle->customSpawn.iColor2 = v->second.iColor2;
		pVehicle->customSpawn.iRespawnTime = v->second.iRespawnTime;
		pVehicle->customSpawn.iInterior = v->second.iInterior;

		// logprintf("custom vehicle spawn respawned %d", pVehicle->wVehicleID);
		pServer->vehicleSpawnData.erase(v);
	}

	pVehicle->vehModInfo.iColor1 = pVehicle->customSpawn.iColor1;
	pVehicle->vehModInfo.iColor2 = pVehicle->customSpawn.iColor2;
	pVehicle->vehMatrix.pos = pVehicle->customSpawn.vecPos;
	pVehicle->vecPosition = pVehicle->customSpawn.vecPos;

	if (pNetGame && pNetGame->pPlayerPool)
		pfn__CPlayerPool__HandleVehicleRespawn(pNetGame->pPlayerPool, pVehicle->wVehicleID);

	CCallbackManager::OnVehicleSpawn(pVehicle->wVehicleID);
}