#ifndef YSF_FUNCTIONS_H
#define YSF_FUNCTIONS_H

#include <sampgdk/sampgdk.h>

#ifdef _WIN32
#include <winsock2.h>

#define STDCALL __stdcall
#define THISCALL __thiscall
#else
typedef int SOCKET;

#define STDCALL
#define THISCALL
#define CDECL
#endif

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

typedef bool (THISCALL *CFilterscripts__LoadFilterscript_t)(void *pFilterscriptPool, char *szName);
typedef bool (THISCALL *CFilterscripts__UnLoadFilterscript_t)(void *pFilterscriptPool, char *szName);

typedef void (THISCALL *CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef DWORD (THISCALL *CPlayerPool__HandleVehicleRespawn_t)(CPlayerPool *pPlayerPool, WORD wVehicleID);

typedef int (THISCALL *Packet_WeaponsUpdate_t)(void *pNetGame, Packet *p);
typedef int (THISCALL *Packet_StatsUpdate_t)(void *pNetGame, Packet *p);
typedef char* (CDECL *format_amxstring_t)(AMX *amx, cell *params, int parm, int &len);

class CHookRakServer
{
public:
	static bool THISCALL Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	static bool THISCALL RPC_2(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
	static Packet* THISCALL Receive(void* ppRakServer);

#ifdef CUSTOM_BANLIST
	static void THISCALL AddToBanList(void* ppRakServer, const char *IP, unsigned int milliseconds=0);
	static void THISCALL RemoveFromBanList(void* ppRakServer, const char *IP);
	static void THISCALL ClearBanList(void* ppRakServer);
#endif
};

typedef bool (THISCALL *RakNet__Send_t)(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
typedef bool (THISCALL *RakNet__RPC_t)(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
typedef Packet* (THISCALL *RakNet__Receive_t)(void* ppRakServer);

#ifdef CUSTOM_BANLIST
typedef void (THISCALL *RakNet__AddToBanList_t)(void* ppRakServer, const char *IP, unsigned int milliseconds);
typedef void (THISCALL *RakNet__RemoveFromBanList_t)(void* ppRakServer, const char *IP);
typedef void (THISCALL *RakNet__ClearBanList_t)(void* ppRakServer);
#endif

class CSAMPFunctions
{
public:
	static void		PreInitialize();
	static void		PostInitialize();
	
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

	static bool		LoadFilterscript(char *szName);
	static bool		UnLoadFilterscript(char *szName);
	
	static void		SpawnPlayer_(int iPlayerId);

	static void		Packet_WeaponsUpdate(Packet *p);
	static void		Packet_StatsUpdate(Packet *p);

	static char*	format_amxstring(AMX *amx, cell *params, int parm, int &len);

	static bool		Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	static bool		RPC(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
	static Packet*	Receive(void* ppRakServer);

	static bool		IsBanned(const char *IP);
#ifdef CUSTOM_BANLIST
	static void		AddToBanList(void* ppRakServer, const char *IP, unsigned int milliseconds=0);
	static void		RemoveFromBanList(void* ppRakServer, const char *IP);
	static void		ClearBanList(void* ppRakServer);
#endif

	static void		RespawnVehicle(CVehicle *pVehicle);

	static char*	GetNPCCommandLine(WORD npcid);
	static int		FindNPCProcessID(WORD npcid);

	// Function
	// Rules
	static CConsole__AddStringVariable_t			pfn__CConsole__AddStringVariable;
	static CConsole__GetStringVariable_t			pfn__CConsole__GetStringVariable;
	static CConsole__SetStringVariable_t			pfn__CConsole__SetStringVariable;
	static CConsole__GetIntVariable_t				pfn__CConsole__GetIntVariable;
	static CConsole__SetIntVariable_t				pfn__CConsole__SetIntVariable;
	static CConsole__GetBoolVariable_t				pfn__CConsole__GetBoolVariable;
	static CConsole__ModifyVariableFlags_t			pfn__CConsole__ModifyVariableFlags;
	static CConsole__FindVariable_t					pfn__CConsole__FindVariable;
	static CConsole__SendRules_t					pfn__CConsole__SendRules;
	static CConsole__Execute_t						pfn__CConsole__Execute;

	// Filterscripts
	static CFilterscripts__LoadFilterscript_t		pfn__CFilterscripts__LoadFilterscript;
	static CFilterscripts__UnLoadFilterscript_t		pfn__CFilterscripts__UnLoadFilterscript;

	// Player
	static CPlayer__SpawnForWorld_t					pfn__CPlayer__SpawnForWorld;
	static CPlayerPool__HandleVehicleRespawn_t		pfn__CPlayerPool__HandleVehicleRespawn;

	// Query
	static Packet_WeaponsUpdate_t					pfn__Packet_WeaponsUpdate;
	static Packet_WeaponsUpdate_t					pfn__Packet_StatsUpdate;
	static format_amxstring_t						pfn__format_amxstring;

	// RakServer
	static RakNet__Send_t							pfn__RakNet__Send;
	static RakNet__RPC_t							pfn__RakNet__RPC;
	static RakNet__Receive_t						pfn__RakNet__Receive;

#ifdef CUSTOM_BANLIST
	static RakNet__AddToBanList_t					pfn__RakNet__AddToBanList;
	static RakNet__RemoveFromBanList_t				pfn__RakNet__RemoveFromBanList;
	static RakNet__ClearBanList_t					pfn__RakNet__ClearBanList;
#endif
};

#endif
