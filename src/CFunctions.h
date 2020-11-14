#ifndef YSF_FUNCTIONS_H
#define YSF_FUNCTIONS_H

#include <sdk/amx/amx.h>
#include <raknet/NetworkTypes.h>

#define DEFINE_FUNCTION_POINTER(name) \
	static name ## _t		pfn__ ## name

#define POINT_TO_MEMBER(name, address) \
	pfn__ ## name = (name ## _t)(address) 

#define INIT_FPTR(name) \
	pfn__ ## name = (name ## _t)(CAddress::FUNC_ ## name) 

#include "includes/net.h"

#ifdef _WIN32
#define STDCALL __stdcall
#define THISCALL __thiscall
#define FASTCALL __fastcall
#else
#define STDCALL
#define THISCALL
#define FASTCALL
#define CDECL
#endif

#include "Structs.h"

struct Packet;
struct ConsoleVariable_s;

typedef void (THISCALL *CConsole__AddStringVariable_t)(void *pConsole, char *szRule, int flags, char *szString, void *changefunc);
typedef char* (THISCALL *CConsole__GetStringVariable_t)(void *pConsole, char *szRule);
typedef void (THISCALL *CConsole__SetStringVariable_t)(void *pConsole, char *szRule, char *szString);
typedef int (THISCALL *CConsole__GetIntVariable_t)(void *pConsole, char *szRule);
typedef void (THISCALL *CConsole__SetIntVariable_t)(void *pConsole, char *szRule, int value);
typedef bool (THISCALL *CConsole__GetBoolVariable_t)(void *pConsole, char *szRule);
typedef void (THISCALL *CConsole__ModifyVariableFlags_t)(void *pConsole, char *szRule, int value);
typedef ConsoleVariable_s* (THISCALL *CConsole__FindVariable_t)(void *pConsole, char *szRule);
typedef void (THISCALL *CConsole__SendRules_t)(void *pConsole, SOCKET s, char* data, const sockaddr_in* to, int tolen);
typedef void (THISCALL *CConsole__Execute_t)(void *pConsole, char* pExecLine);

typedef void (THISCALL *CNetGame__SetWeather_t)(void *pNetGame, BYTE weatherid);
typedef void (THISCALL *CNetGame__SetGravity_t)(void *pNetGame, float gravity);

typedef bool (THISCALL *CFilterscripts__LoadFilterscript_t)(void *pFilterscriptPool, const char *szName);
typedef bool (THISCALL *CFilterscripts__UnLoadFilterscript_t)(void *pFilterscriptPool, const char *szName);
typedef bool (*ContainsInvalidChars_t)(char *szString);

typedef void (THISCALL *CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef void (THISCALL *CVehicle__Respawn_t)(CVehicle *pVehicle, void *padding);
typedef DWORD (THISCALL *CPlayerPool__HandleVehicleRespawn_t)(CPlayerPool *pPlayerPool, WORD wVehicleID);
typedef void (THISCALL *CObject__SpawnForPlayer_t)(void *pObject, WORD playerID);

typedef int (*ProcessQueryPacket_t)(unsigned int binaryAddress, unsigned short port, char* data, int length, SOCKET s);
typedef int (THISCALL *Packet_WeaponsUpdate_t)(void *pNetGame, Packet *p);
typedef int (THISCALL *Packet_StatsUpdate_t)(void *pNetGame, Packet *p);
typedef char* (CDECL *format_amxstring_t)(AMX *amx, cell *params, int parm, int &len);

typedef void (CDECL *ReplaceBadChars_t)(char *szString);

#ifdef SAMP_03DL
typedef void (*ClientJoin_t)(RPCParameters *rpcParams);
typedef int (THISCALL *AddSimpleModel_t)(CArtInfo *pArtInfo, MODEL_TYPE type, int virtualworld, int baseid, int newid, char *dffname, char *txdname, char timeon, char timeoff);
typedef int (THISCALL *DynamicListInsert_t)(CArtList *pArtList, CModelInfo *pModelInfo);
#endif

typedef int (THISCALL *CGameMode__OnPlayerConnect_t)(CGameMode *pGameMode, cell playerid);
typedef int (THISCALL *CGameMode__OnPlayerDisconnect_t)(CGameMode *pGameMode, cell playerid, cell reason);
typedef int (THISCALL *CGameMode__OnPlayerSpawn_t)(CGameMode *pGameMode, cell playerid);
typedef int (THISCALL *CGameMode__OnPlayerStreamIn_t)(CGameMode *pGameMode, cell playerid, cell forplayerid);
typedef int (THISCALL *CGameMode__OnPlayerStreamOut_t)(CGameMode *pGameMode, cell playerid, cell forplayerid);
typedef int (THISCALL *CGameMode__OnDialogResponse_t)(CGameMode *pGameMode, cell playerid, cell dialogid, cell response, cell listitem, char *szInputtext);

typedef bool (THISCALL *RakNet__Start_t)(void* ppRakServer, unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress);
typedef bool (THISCALL *RakNet__Send_t)(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
typedef Packet* (THISCALL *RakNet__Receive_t)(void* ppRakServer);
typedef void (THISCALL *RakNet__SetAllowedPlayers_t)(void* ppRakServer, unsigned short numberAllowed);
typedef int (THISCALL *RakNet__GetLastPing_t)(void* ppRakServer, const PlayerID playerId); 
typedef void (THISCALL *RakNet__RegisterAsRemoteProcedureCall_t)(void* ppRakServer, int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms));	
typedef void (THISCALL *RakNet__UnregisterAsRemoteProcedureCall_t)(void* ppRakServer, int* uniqueID);
typedef bool (THISCALL *RakNet__RPC_t)(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
typedef const char* (THISCALL *RakNet__GetLocalIP_t)(void* ppRakServer, unsigned int index);
typedef int (THISCALL *RakNet__GetIndexFromPlayerID_t)(void* ppRakServer, PlayerID playerId);
typedef PlayerID(THISCALL *RakNet__GetPlayerIDFromIndex_t)(void* ppRakServer, int index); 
typedef void (THISCALL *RakNet__SetTimeoutTime_t)(void* ppRakServer, RakNetTime timeMS, const PlayerID target);
typedef void (THISCALL *RakNet__AddToBanList_t)(void* ppRakServer, const char *IP, unsigned int milliseconds);
typedef void (THISCALL *RakNet__RemoveFromBanList_t)(void* ppRakServer, const char *IP);
typedef void (THISCALL *RakNet__ClearBanList_t)(void* ppRakServer);

class CHookRakServer
{
public:
	static bool THISCALL Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	static bool THISCALL RPC_2(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
	static Packet* THISCALL Receive(void* ppRakServer);
	static void THISCALL AddToBanList(void* ppRakServer, const char *IP, unsigned int milliseconds = 0);
	static void THISCALL RemoveFromBanList(void* ppRakServer, const char *IP);
	static void THISCALL ClearBanList(void* ppRakServer);
};

class CSAMPFunctions
{
public:
	static void		PreInitialize();
	static void		PostInitialize(bool passive);
	
	static void		AddStringVariable(char *szRule, int flags, char *szString, void *changefunc);
	static char*	GetStringVariable(char *szRule);
	static void		SetStringVariable(char *szRule, char *szString);
	static int		GetIntVariable(char *szRule);
	static void		SetIntVariable(char *szRule, int value);
	static bool		GetBoolVariable(char *szRule);
	static void		ModifyVariableFlags(char *szRule, int value);
	static ConsoleVariable_s* FindVariable(char *szRule);
	static void		SendRules(SOCKET s, char* data, const sockaddr_in* to, int tolen);
	static void		Execute(char* pExecLine);

	static bool		LoadFilterscript(const char *szName);
	static bool		UnLoadFilterscript(const char *szName);
	
	static void		SpawnPlayer(int iPlayerId);

	static void		SpawnObjectForPlayer(CObject *pObjectId, WORD wPlayerID);
	static void		SpawnObjectForPlayer(int iObjectId, WORD wPlayerID);

	static void		Packet_WeaponsUpdate(Packet *p);
	static void		Packet_StatsUpdate(Packet *p);

	static char*	format_amxstring(AMX *amx, cell *params, int parm, int &len);

	static bool		Start(unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress = 0);
	static bool		Send(RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	static Packet*	Receive(void* ppRakServer);
	static void		SetAllowedPlayers(unsigned short numberAllowed);
	static int		GetLastPing(const PlayerID playerId);
	static void		RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms));
	static void		UnregisterAsRemoteProcedureCall(int* uniqueID);
	static bool		RPC(BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
	static const char* GetLocalIP(unsigned int index);
	static int		GetIndexFromPlayerID(PlayerID playerId);
	static PlayerID GetPlayerIDFromIndex(int index);
	static void		SetTimeoutTime(RakNetTime timeMS, const PlayerID target);
	static void		AddToBanList(const char *IP, unsigned int milliseconds);
	static void		RemoveFromBanList(const char *IP);
	static void		ClearBanList(void);

	static void		RespawnVehicle(CVehicle *pVehicle);

	// Function
	// Rules
	/*DEFINE_FUNCTION_POINTER(CConsole__AddStringVariable);
	DEFINE_FUNCTION_POINTER(CConsole__GetStringVariable);
	DEFINE_FUNCTION_POINTER(CConsole__SetStringVariable);
	DEFINE_FUNCTION_POINTER(CConsole__GetIntVariable);
	DEFINE_FUNCTION_POINTER(CConsole__SetIntVariable);
	DEFINE_FUNCTION_POINTER(CConsole__GetBoolVariable);
	DEFINE_FUNCTION_POINTER(CConsole__ModifyVariableFlags);
	DEFINE_FUNCTION_POINTER(CConsole__FindVariable);
	DEFINE_FUNCTION_POINTER(CConsole__SendRules);
	DEFINE_FUNCTION_POINTER(CConsole__Execute);

	// Filterscripts
	DEFINE_FUNCTION_POINTER(CFilterscripts__LoadFilterscript);
	DEFINE_FUNCTION_POINTER(CFilterscripts__UnLoadFilterscript);

	// Player
	DEFINE_FUNCTION_POINTER(CPlayer__SpawnForWorld);
	DEFINE_FUNCTION_POINTER(CPlayerPool__HandleVehicleRespawn);

	// Object
	DEFINE_FUNCTION_POINTER(CObject__SpawnForPlayer);

	// Query
	DEFINE_FUNCTION_POINTER(Packet_WeaponsUpdate);
	DEFINE_FUNCTION_POINTER(Packet_StatsUpdate);
	DEFINE_FUNCTION_POINTER(format_amxstring);*/

	// RakServer
	DEFINE_FUNCTION_POINTER(RakNet__Start);							
	DEFINE_FUNCTION_POINTER(RakNet__Send);							
	DEFINE_FUNCTION_POINTER(RakNet__Receive);						
	DEFINE_FUNCTION_POINTER(RakNet__SetAllowedPlayers);				
	DEFINE_FUNCTION_POINTER(RakNet__GetLastPing);					
	DEFINE_FUNCTION_POINTER(RakNet__RegisterAsRemoteProcedureCall);	
	DEFINE_FUNCTION_POINTER(RakNet__UnregisterAsRemoteProcedureCall);
	DEFINE_FUNCTION_POINTER(RakNet__RPC);							
	DEFINE_FUNCTION_POINTER(RakNet__GetLocalIP);					
	DEFINE_FUNCTION_POINTER(RakNet__GetIndexFromPlayerID);			
	DEFINE_FUNCTION_POINTER(RakNet__GetPlayerIDFromIndex);			
	DEFINE_FUNCTION_POINTER(RakNet__SetTimeoutTime);				
	DEFINE_FUNCTION_POINTER(RakNet__AddToBanList);
	DEFINE_FUNCTION_POINTER(RakNet__RemoveFromBanList);				
	DEFINE_FUNCTION_POINTER(RakNet__ClearBanList);					
};

#endif
