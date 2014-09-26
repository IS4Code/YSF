#include "main.h"
#include "Functions.h"

CConsole__AddStringVariable_t			CSAMPFunctions::pfn__CConsole__AddStringVariable = NULL;
CConsole__SetStringVariable_t			CSAMPFunctions::pfn__CConsole__SetStringVariable = NULL;
CConsole__SetIntVariable_t				CSAMPFunctions::pfn__CConsole__SetIntVariable = NULL;
CConsole__ModifyVariableFlags_t			CSAMPFunctions::pfn__CConsole__ModifyVariableFlags = NULL;

CFilterscripts__LoadFilterscript_t		CSAMPFunctions::pfn__CFilterscripts__LoadFilterscript = NULL;
CFilterscripts__UnLoadFilterscript_t	CSAMPFunctions::pfn__CFilterscripts__UnLoadFilterscript = NULL;

CPlayer__SpawnForWorld_t				CSAMPFunctions::pfn__CPlayer__SpawnForWorld = NULL;

ProcessQueryPacket_t					CSAMPFunctions::pfn__ProcessQueryPacket = NULL;

void CSAMPFunctions::Initialize()
{
	pfn__CConsole__AddStringVariable = (CConsole__AddStringVariable_t)(CAddress::FUNC_CConsole__AddStringVariable);
	pfn__CConsole__SetStringVariable = (CConsole__SetStringVariable_t)(CAddress::FUNC_CConsole__SetStringVariable);
	pfn__CConsole__SetIntVariable = (CConsole__SetIntVariable_t)(CAddress::FUNC_CConsole__SetIntVariable);
	pfn__CConsole__ModifyVariableFlags = (CConsole__ModifyVariableFlags_t)(CAddress::FUNC_CConsole__ModifyVariableFlags);

	pfn__CFilterscripts__LoadFilterscript = (CFilterscripts__LoadFilterscript_t)(CAddress::FUNC_CFilterscripts__LoadFilterscript);
	pfn__CFilterscripts__UnLoadFilterscript = (CFilterscripts__UnLoadFilterscript_t)(CAddress::FUNC_CFilterscripts__UnLoadFilterscript);

	pfn__CPlayer__SpawnForWorld = (CPlayer__SpawnForWorld_t)(CAddress::FUNC_CPlayer__SpawnForWorld);

	pfn__ProcessQueryPacket = (ProcessQueryPacket_t)(CAddress::FUNC_ProcessQueryPacket);
}

void CSAMPFunctions::AddStringVariable(char *szRule, int flags, char *szString, void *changefunc)
{
	pfn__CConsole__AddStringVariable(pConsole, szRule, flags, szString, changefunc);
}

void CSAMPFunctions::SetStringVariable(char *szRule, char *szString)
{
	pfn__CConsole__SetStringVariable(pConsole, szRule, szString);
}

void CSAMPFunctions::SetIntVariable(char *szRule, int value)
{
	pfn__CConsole__SetIntVariable(pConsole, szRule, value);
}

void CSAMPFunctions::ModifyVariableFlags(char *szRule, int value)
{
	pfn__CConsole__ModifyVariableFlags(pConsole, szRule, value);
}

bool CSAMPFunctions::LoadFilterscript(char *szName)
{
	return pfn__CFilterscripts__LoadFilterscript(pNetGame->pFilterScriptPool, szName);
}

bool CSAMPFunctions::UnLoadFilterscript(char *szName)
{
	return pfn__CFilterscripts__UnLoadFilterscript(pNetGame->pFilterScriptPool, szName);
}

void CSAMPFunctions::SpawnPlayer(int playerid)
{
	pfn__CPlayer__SpawnForWorld(pNetGame->pPlayerPool->pPlayer[playerid]);
}

int CSAMPFunctions::ProcessQueryPacket(unsigned int binaryAddress, unsigned short port, char* data, int length, SOCKET s)
{
	return pfn__ProcessQueryPacket(binaryAddress, port, data, length, s);
}