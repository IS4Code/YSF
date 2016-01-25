#ifndef YSF_FUNCTIONS_H
#define YSF_FUNCTIONS_H

#include <sampgdk/sampgdk.h>

#ifdef _WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

struct Packet;
struct ConsoleVariable_s;

#ifdef _WIN32

typedef void(__thiscall *CConsole__AddStringVariable_t)(void *pConsole, char *szRule, int flags, char *szString, void *changefunc);
typedef char*(__thiscall *CConsole__GetStringVariable_t)(void *pConsole, char *szRule);
typedef void(__thiscall *CConsole__SetStringVariable_t)(void *pConsole, char *szRule, char *szString);
typedef int(__thiscall *CConsole__GetIntVariable_t)(void *pConsole, char *szRule);
typedef void(__thiscall *CConsole__SetIntVariable_t)(void *pConsole, char *szRule, int value);
typedef bool(__thiscall *CConsole__GetBoolVariable_t)(void *pConsole, char *szRule);
typedef void(__thiscall *CConsole__ModifyVariableFlags_t)(void *pConsole, char *szRule, int value);
typedef ConsoleVariable_s *(__thiscall *CConsole__FindVariable_t)(void *pConsole, char *szRule);
typedef void(__thiscall *CConsole__SendRules_t)(void *pConsole, SOCKET s, char* data, const sockaddr_in* to, int tolen);
typedef void(__thiscall *CConsole__Execute_t)(void *pConsole, char* pExecLine);

typedef bool(__thiscall *CFilterscripts__LoadFilterscript_t)(void *pFilterscriptPool, char *szName);
typedef bool(__thiscall *CFilterscripts__UnLoadFilterscript_t)(void *pFilterscriptPool, char *szName);

typedef void(__thiscall *CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef DWORD(__thiscall *CPlayerPool__HandleVehicleRespawn_t)(CPlayerPool *pPlayerPool, WORD wVehicleID);

typedef int(__thiscall *Packet_WeaponsUpdate_t)(void *pNetGame, Packet *p);
typedef int(__thiscall *Packet_StatsUpdate_t)(void *pNetGame, Packet *p);
typedef char *(__cdecl *format_amxstring_t)(AMX *amx, cell *params, int parm, int &len);

#else
typedef void(*CConsole__AddStringVariable_t)(void *pConsole, char *szRule, int flags, char *szString, void *changefunc);
typedef char*(*CConsole__GetStringVariable_t)(void *pConsole, char *szRule);
typedef void(*CConsole__SetStringVariable_t)(void *pConsole, char *szRule, char *szString);
typedef int(*CConsole__GetIntVariable_t)(void *pConsole, char *szRule);
typedef void(*CConsole__SetIntVariable_t)(void *pConsole, char *szRule, int value);
typedef bool(*CConsole__GetBoolVariable_t)(void *pConsole, char *szRule);
typedef void(*CConsole__ModifyVariableFlags_t)(void *pConsole, char *szRule, int value);
typedef ConsoleVariable_s *(*CConsole__FindVariable_t)(void *pConsole, char *szRule);
typedef void(*CConsole__SendRules_t)(void *pConsole, SOCKET s, char* data, const struct sockaddr_in* to, int tolen);
typedef void(*CConsole__Execute_t)(void *pConsole, char* pExecLine);

typedef bool(*CFilterscripts__LoadFilterscript_t)(void *pFilterscriptPool, char *szName);
typedef bool(*CFilterscripts__UnLoadFilterscript_t)(void *pFilterscriptPool, char *szName);
             
typedef void(*CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef DWORD(*CPlayerPool__HandleVehicleRespawn_t)(CPlayerPool *pPlayerPool, WORD wVehicleID);

typedef int (*Packet_WeaponsUpdate_t)(void *pNetGame, Packet *p);
typedef int (*Packet_StatsUpdate_t)(void *pNetGame, Packet *p);
typedef char *(*format_amxstring_t)(AMX *amx, cell *params, int parm, int &len);

#endif

class CSAMPFunctions
{
public:
	static void		Initialize();
	
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
	static void		RespawnVehicle_(void *pVehicle);

	static void		Packet_WeaponsUpdate(Packet *p);
	static void		Packet_StatsUpdate(Packet *p);

	static char*	format_amxstring(AMX *amx, cell *params, int parm, int &len);

	static void		RespawnVehicle(CVehicle *pVehicle);

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
};

#endif
