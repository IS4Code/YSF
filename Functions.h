#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#ifdef _WIN32

typedef void(__thiscall *CConsole__AddStringVariable_t)(void *pConsole, char *szRule, int flags, char *szString, void *changefunc);
typedef void(__thiscall *CConsole__SetStringVariable_t)(void *pConsole, char *szRule, char *szString);
typedef void(__thiscall *CConsole__SetIntVariable_t)(void *pConsole, char *szRule, int value);
typedef void(__thiscall *CConsole__ModifyVariableFlags_t)(void *pConsole, char *szRule, int value);

typedef bool(__thiscall *CFilterscripts__LoadFilterscript_t)(void *pFilterscriptPool, char *szName);
typedef bool(__thiscall *CFilterscripts__UnLoadFilterscript_t)(void *pFilterscriptPool, char *szName);

typedef void(__thiscall *CPlayer__SpawnForWorld_t)(void *pPlayer);
#else
typedef void(*CConsole__AddStringVariable_t)(void *pConsole, char * rule, int flags, char *szString, void *changefunc);

typedef void(*CPlayer__SpawnForWorld_t)(void *pPlayer);
#endif

class CSAMPFunctions
{
public:
	static void		Initialize();
	
	static void		AddStringVariable(char *szRule, int flags, char *szString, void *changefunc);
	static void		SetStringVariable(char *szRule, char *szString);
	static void		SetIntVariable(char *szRule, int value);
	static void		ModifyVariableFlags(char *szRule, int value);
	
	static bool		LoadFilterscript(char *szName);
	static bool		UnLoadFilterscript(char *szName);
	
	static void		SpawnPlayer(int iPlayerId);

	// Function
	// Rules
	static CConsole__AddStringVariable_t			pfn__CConsole__AddStringVariable;
	static CConsole__SetStringVariable_t			pfn__CConsole__SetStringVariable;
	static CConsole__SetIntVariable_t				pfn__CConsole__SetIntVariable;
	static CConsole__ModifyVariableFlags_t			pfn__CConsole__ModifyVariableFlags;
	// Filterscripts
	static CFilterscripts__LoadFilterscript_t		pfn__CFilterscripts__LoadFilterscript;
	static CFilterscripts__UnLoadFilterscript_t		pfn__CFilterscripts__UnLoadFilterscript;
	// Player
	static CPlayer__SpawnForWorld_t					pfn__CPlayer__SpawnForWorld;
};

#endif
