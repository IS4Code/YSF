#include "main.h"
#if !defined(_WIN32) && !defined(CUSTOM_BANLIST)
#include <fnmatch.h>
#endif

#ifdef _WIN32
#include <tlhelp32.h>
#include <winternl.h>

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#else
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#endif

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
RakNet__Receive_t						CSAMPFunctions::pfn__RakNet__Receive = NULL;

#ifdef CUSTOM_BANLIST
RakNet__AddToBanList_t					CSAMPFunctions::pfn__RakNet__AddToBanList = NULL;
RakNet__RemoveFromBanList_t				CSAMPFunctions::pfn__RakNet__RemoveFromBanList = NULL;
RakNet__ClearBanList_t					CSAMPFunctions::pfn__RakNet__ClearBanList = NULL;

struct BanRecord
{
	unsigned long ip;
	unsigned long mask;

	BanRecord(const char *addr)
	{
		/*logprintf(addr);
		size_t len = strlen(addr);
		char *str = (char*)alloca(len+1);
		memcpy(str, addr, len);
		str[len] = 0;*/

		ip = 0;
		mask = 0;

		int pos = 0;
		bool next = false;
		size_t len = strlen(addr);
		for(unsigned int i = 0; i < len; i++)
		{
			if(addr[i] == '*')
			{
				mask |= 0xFF << (pos*8);
				next = true;
				pos++;
			}else if(addr[i] == '.')
			{
				next = false;
			}else{
				if(!next)
				{
					ip |= strtol(addr+i, NULL, 10) << (pos*8);
					next = true;
					pos++;
				}
			}
		}
		mask = ~mask;

		logprintf("%x & %x", ip, mask);
		
		/*int pos;
		char *p = strtok(str, ".");
		while(p != NULL)
		{
			if(p[0] != '*')
			{
				ip |= strtol(p, NULL, 10) << (pos*8);
			}else{
				mask |= 0xFF << (pos*8);
			}
			p = strtok(NULL, ".");
			pos++;
		}
		mask = ~mask;*/
	}

	bool operator==(const BanRecord &rec) const
	{
		return (ip == rec.ip && mask == rec.mask);
	}

    /*bool operator <(const BanRecord &rec) const
    {
       return ip < rec.ip;
    }*/
};

std::vector<BanRecord> banList;
#endif

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
	Unlock((void*)&pRakServer_VTBL[RAKNET_RECEIVE_OFFSET], 4);

	pfn__RakNet__Send = (RakNet__Send_t)(pRakServer_VTBL[RAKNET_SEND_OFFSET]);
	pfn__RakNet__RPC = (RakNet__RPC_t)(pRakServer_VTBL[RAKNET_RPC_OFFSET]);
	pfn__RakNet__Receive = (RakNet__Receive_t)(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET]);

	pRakServer_VTBL[RAKNET_SEND_OFFSET] = (int)CHookRakServer::Send;
	pRakServer_VTBL[RAKNET_RPC_OFFSET] = (int)CHookRakServer::RPC_2;
	pRakServer_VTBL[RAKNET_RECEIVE_OFFSET] = (int)CHookRakServer::Receive;

#ifdef CUSTOM_BANLIST
	Unlock((void*)&pRakServer_VTBL[RAKNET_ADD_BAN_OFFSET], 4);
	Unlock((void*)&pRakServer_VTBL[RAKNET_REMOVE_BAN_OFFSET], 4);
	Unlock((void*)&pRakServer_VTBL[RAKNET_CLEAR_BANS_OFFSET], 4);
	
	pfn__RakNet__AddToBanList = (RakNet__AddToBanList_t)(pRakServer_VTBL[RAKNET_ADD_BAN_OFFSET]);
	pfn__RakNet__RemoveFromBanList = (RakNet__RemoveFromBanList_t)(pRakServer_VTBL[RAKNET_REMOVE_BAN_OFFSET]);
	pfn__RakNet__ClearBanList = (RakNet__ClearBanList_t)(pRakServer_VTBL[RAKNET_CLEAR_BANS_OFFSET]);
	
	pRakServer_VTBL[RAKNET_ADD_BAN_OFFSET] = (int)CHookRakServer::AddToBanList;
	pRakServer_VTBL[RAKNET_REMOVE_BAN_OFFSET] = (int)CHookRakServer::RemoveFromBanList;
	pRakServer_VTBL[RAKNET_CLEAR_BANS_OFFSET] = (int)CHookRakServer::ClearBanList;
	
	int bans = pRakServer->GetBanListSize();
	char ***banlist = pRakServer->GetBanList();
	for(int i = 0; i < bans; i++)
	{
		banList.push_back(BanRecord(*banlist[i]));
	}

	logprintf("added %d, count %d", bans, banList.size());
#endif
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

bool CSAMPFunctions::RPC(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
{
	return pfn__RakNet__RPC(ppRakServer, uniqueID, parameters, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp);
}

Packet* CSAMPFunctions::Receive(void* ppRakServer)
{
	return pfn__RakNet__Receive(ppRakServer);
}

bool CSAMPFunctions::IsBanned(const char *IP)
{
#ifdef CUSTOM_BANLIST
	unsigned long addr = inet_addr(IP);
	std::vector<BanRecord>::iterator iter;
	for(iter = banList.begin(); iter != banList.end(); ++iter)
	{
		if((addr & iter->mask) == (iter->ip & iter->mask)) return true;
	}
	return false;
#elif defined(_WIN32)
	return pRakServer->_IsBanned(IP);
#else
	RakBanList *banlist = pRakServer->GetBanList();
	for(unsigned int i = 0; i < banlist->list_size; i++)
	{
		RakBanStruct *ban = banlist->listArray[i];
		if(!fnmatch(ban->IP, IP, 0)) return true;
	}
	return false;
#endif
}

#ifdef CUSTOM_BANLIST

void CSAMPFunctions::AddToBanList(void* ppRakServer, const char *IP, unsigned int milliseconds)
{
	BanRecord rec = BanRecord(IP);

	if(rec.ip != 0x0100007F) //SA-MP prevents banning NPCs
	{
		banList.push_back(rec);
	}

	pfn__RakNet__AddToBanList(ppRakServer, IP, milliseconds);
}

void CSAMPFunctions::RemoveFromBanList(void* ppRakServer, const char *IP)
{
	BanRecord rec = BanRecord(IP);

	banList.erase(std::remove(banList.begin(), banList.end(), rec), banList.end());
	
	pfn__RakNet__RemoveFromBanList(ppRakServer, IP);
}

void CSAMPFunctions::ClearBanList(void* ppRakServer)
{
	banList.clear();

	pfn__RakNet__ClearBanList(ppRakServer);
}
#endif

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

char* CSAMPFunctions::GetNPCCommandLine(WORD npcid)
{
	if(!pPlayerData[npcid]) return NULL;
	int pid = pPlayerData[npcid]->npcPid;
	char *line = NULL;

#ifdef _WIN32
    HRESULT hr = 0;
    IWbemLocator         *WbemLocator  = NULL;
    IWbemServices        *WbemServices = NULL;
    IEnumWbemClassObject *EnumWbem  = NULL;

    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &WbemLocator);
    hr = WbemLocator->ConnectServer(L"ROOT\\CIMV2", NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);

	std::ostringstream query;
	query << "SELECT CommandLine FROM Win32_Process WHERE ProcessId=" << pid;
	hr = WbemServices->ExecQuery(L"WQL", bstr_t(query.str().c_str()), WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);

    if(EnumWbem == NULL) return 0;

    IWbemClassObject *result = NULL;
    ULONG returnedCount = 0;
	
    while((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK)
	{
        VARIANT CommandLine;
        hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);

        line = _com_util::ConvertBSTRToString(CommandLine.bstrVal);

		VariantClear(&CommandLine);
        result->Release();

		break;
    }

    EnumWbem->Release();
    WbemServices->Release();
    WbemLocator->Release();
	
    CoUninitialize();
#else
	char fname[32];
	sprintf(fname, "/proc/%d/cmdline", pid);
						
	FILE *fcmd = fopen(fname, "r");
	if(fcmd != NULL)
	{
		size_t size = 128;
		line = (char*)malloc(size);
		size_t total = 0, read;
		while((read = fread(line+total, 1, size-total, fcmd)) >= size-total)
		{
			total += read;
			size *= 2;
			line = (char*)realloc(line, size);
		}
		fclose(fcmd);
		total += read;
		for(unsigned int i = 0; i < total-1; i++)
		{
			if(!line[i]) line[i] = ' ';
		}
	}
#endif

	return line;
}

int CSAMPFunctions::FindNPCProcessID(WORD npcid)
{
	char *name = pNetGame->pPlayerPool->szName[npcid];
#ifdef _WIN32
	DWORD cpid = GetCurrentProcessId();
	
    HRESULT hr = 0;
    IWbemLocator         *WbemLocator  = NULL;
    IWbemServices        *WbemServices = NULL;
    IEnumWbemClassObject *EnumWbem  = NULL;

    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &WbemLocator);
    hr = WbemLocator->ConnectServer(L"ROOT\\CIMV2", NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);

	std::ostringstream query;
	query << "SELECT ProcessId, CommandLine FROM Win32_Process WHERE ParentProcessId=" << cpid;
	hr = WbemServices->ExecQuery(L"WQL", bstr_t(query.str().c_str()), WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);
	

    if(EnumWbem == NULL) return 0;

    IWbemClassObject *result = NULL;
    ULONG returnedCount = 0;
	
	DWORD pid = 0;
    while((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK)
	{
        VARIANT ProcessId, CommandLine;
        hr = result->Get(L"ProcessId", 0, &ProcessId, 0, 0);
        hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);

        char *line = _com_util::ConvertBSTRToString(CommandLine.bstrVal);
			
		char *iter = line;
		
		bool found = false;
		while(*iter)
		{
			if(*iter == ' ')
			{
				if(*++iter == '-' && *++iter == 'n' && *++iter == ' ')
				{
					if(!strncmp(++iter, name, strlen(name)))
					{
						pid = ProcessId.uintVal;
						found = true;
						break;
					}
				}
			}else{
				iter++;
			}
		}

		free(line);
		VariantClear(&ProcessId);
		VariantClear(&CommandLine);
        result->Release();

		if(found) break;
    }

    EnumWbem->Release();
    WbemServices->Release();
    WbemLocator->Release();

    CoUninitialize();

	return pid;
#else
	pid_t pid, cpid = getpid();

	DIR *dp = opendir("/proc/");
	if(dp != NULL)
	{
		struct dirent *ep;
		
		char line[1024], fname[32];
		while(ep = readdir(dp))
		{
			if(sscanf(ep->d_name, "%d", &pid) == 1)
			{
				sprintf(fname, "/proc/%d/stat", pid);
				
				FILE *fstat = fopen(fname, "r");
				if(fstat != NULL)
				{
					fread(line, 1, sizeof(line), fstat);
					fclose(fstat);

					int gpid;
					if(sscanf(line, "%*d (samp-npc) %*c %*d %d", &gpid) == 1)
					{
						if(gpid == cpid)
						{
							sprintf(fname, "/proc/%d/cmdline", pid);
						
							FILE *fcmd = fopen(fname, "r");
							if(fcmd != NULL)
							{
								size_t total = fread(line, 1, sizeof(line), fcmd);
								fclose(fcmd);
								
								char *iter = line;
								char *end = line+total;

								bool found = false;
								while(iter < end)
								{
									if(*iter == '\0')
									{
										if(*++iter == '-' && *++iter == 'n' && *++iter == '\0')
										{
											if(!strncmp(++iter, name, strlen(name)))
											{
												found = true;
												break;
											}
										}
									}else{
										iter++;
									}
								}
								if(found) break;
							}
						}
					}
				}
			}
			pid = 0;
		}
		closedir(dp);
	}
	return pid;
#endif
}