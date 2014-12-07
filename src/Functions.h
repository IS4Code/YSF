#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sdk/plugin.h>

#ifdef _WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

struct Packet;
struct ConsoleVariable_s;

#ifdef _WIN32

typedef void(__thiscall *CConsole__AddStringVariable_t)(void *pConsole, char *szRule, int flags, char *szString, void *changefunc);
typedef ConsoleVariable_s *(__thiscall *CConsole__FindVariable_t)(void *pConsole, char *szRule);
typedef void(__thiscall *CConsole__SendRules_t)(void *pConsole, SOCKET s, char* data, const sockaddr_in* to, int tolen);
typedef void(__thiscall *CConsole__Execute_t)(void *pConsole, char* pExecLine);

typedef bool(__thiscall *CFilterscripts__LoadFilterscript_t)(void *pFilterscriptPool, char *szName);
typedef bool(__thiscall *CFilterscripts__UnLoadFilterscript_t)(void *pFilterscriptPool, char *szName);

typedef void(__thiscall *CPlayer__SpawnForWorld_t)(void *pPlayer);

typedef int(__thiscall *Packet_WeaponsUpdate_t)(void *pNetGame, Packet *p);
typedef char *(__thiscall *format_amxstring_t)(AMX *amx, cell *params, int parm, int &len);

#else
typedef void(*CConsole__AddStringVariable_t)(void *pConsole, char *szRule, int flags, char *szString, void *changefunc);
typedef ConsoleVariable_s *(*CConsole__FindVariable_t)(void *pConsole, char *szRule);
typedef void(*CConsole__SendRules_t)(void *pConsole, SOCKET s, char* data, const struct sockaddr_in* to, int tolen);
typedef void(*CConsole__Execute_t)(void *pConsole, char* pExecLine);

typedef bool(*CFilterscripts__LoadFilterscript_t)(void *pFilterscriptPool, char *szName);
typedef bool(*CFilterscripts__UnLoadFilterscript_t)(void *pFilterscriptPool, char *szName);
             
typedef void(*CPlayer__SpawnForWorld_t)(void *pPlayer);

typedef int (*Packet_WeaponsUpdate_t)(void *pNetGame, Packet *p);
typedef char *(*format_amxstring_t)(AMX *amx, cell *params, int parm, int &len);
#endif

class CSAMPFunctions
{
public:
	static void		Initialize();
	
	static void		AddStringVariable(char *szRule, int flags, char *szString, void *changefunc);
	static ConsoleVariable_s* FindVariable(char *szRule);
	static void		SendRules(SOCKET s, char* data, const sockaddr_in* to, int tolen);
	static void		Execute(char* pExecLine);

	static bool		LoadFilterscript(char *szName);
	static bool		UnLoadFilterscript(char *szName);
	
	static void		SpawnPlayer_(int iPlayerId);
	static void		Packet_WeaponsUpdate(Packet *p);
	static char*	format_amxstring(AMX *amx, cell *params, int parm, int &len);

	// Function
	// Rules
	static CConsole__AddStringVariable_t			pfn__CConsole__AddStringVariable;
	static CConsole__FindVariable_t					pfn__CConsole__FindVariable;
	static CConsole__SendRules_t					pfn__CConsole__SendRules;
	static CConsole__Execute_t						pfn__CConsole__Execute;

	// Filterscripts
	static CFilterscripts__LoadFilterscript_t		pfn__CFilterscripts__LoadFilterscript;
	static CFilterscripts__UnLoadFilterscript_t		pfn__CFilterscripts__UnLoadFilterscript;
	// Player
	static CPlayer__SpawnForWorld_t					pfn__CPlayer__SpawnForWorld;
	// Query
	static Packet_WeaponsUpdate_t					pfn__Packet_WeaponsUpdate;
	static format_amxstring_t						pfn__format_amxstring;
};

#endif
