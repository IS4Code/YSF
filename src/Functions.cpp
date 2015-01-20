#include "main.h"

CConsole__AddStringVariable_t			CSAMPFunctions::pfn__CConsole__AddStringVariable = NULL;
CConsole__FindVariable_t				CSAMPFunctions::pfn__CConsole__FindVariable = NULL;
CConsole__SendRules_t					CSAMPFunctions::pfn__CConsole__SendRules = NULL;
CConsole__Execute_t						CSAMPFunctions::pfn__CConsole__Execute = NULL;

CFilterscripts__LoadFilterscript_t		CSAMPFunctions::pfn__CFilterscripts__LoadFilterscript = NULL;
CFilterscripts__UnLoadFilterscript_t	CSAMPFunctions::pfn__CFilterscripts__UnLoadFilterscript = NULL;

CPlayer__SpawnForWorld_t				CSAMPFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CVehicle__Respawn_t						CSAMPFunctions::pfn__CVehicle__Respawn = NULL;

Packet_WeaponsUpdate_t					CSAMPFunctions::pfn__Packet_WeaponsUpdate = NULL;

format_amxstring_t						CSAMPFunctions::pfn__format_amxstring = NULL;

void CSAMPFunctions::Initialize()
{
	pfn__CConsole__AddStringVariable = (CConsole__AddStringVariable_t)(CAddress::FUNC_CConsole__AddStringVariable);
	pfn__CConsole__FindVariable = (CConsole__FindVariable_t)(CAddress::FUNC_CConsole__FindVariable);
	pfn__CConsole__SendRules = (CConsole__SendRules_t)(CAddress::FUNC_CConsole__SendRules);
	pfn__CConsole__Execute = (CConsole__Execute_t)(CAddress::FUNC_CConsole__Execute);

	pfn__CFilterscripts__LoadFilterscript = (CFilterscripts__LoadFilterscript_t)(CAddress::FUNC_CFilterscripts__LoadFilterscript);
	pfn__CFilterscripts__UnLoadFilterscript = (CFilterscripts__UnLoadFilterscript_t)(CAddress::FUNC_CFilterscripts__UnLoadFilterscript);

	pfn__CPlayer__SpawnForWorld = (CPlayer__SpawnForWorld_t)(CAddress::FUNC_CPlayer__SpawnForWorld);

	pfn__Packet_WeaponsUpdate = (Packet_WeaponsUpdate_t)(CAddress::FUNC_Packet_WeaponsUpdate);

	pfn__format_amxstring = (format_amxstring_t)(CAddress::FUNC_format_amxstring);
}

void CSAMPFunctions::AddStringVariable(char *szRule, int flags, char *szString, void *changefunc)
{
	pfn__CConsole__AddStringVariable(pConsole, szRule, flags, szString, changefunc);
}

ConsoleVariable_s* CSAMPFunctions::FindVariable(char *szRule)
{
	if (!CAddress::FUNC_CConsole__SendRules)
		return NULL;

	return pfn__CConsole__FindVariable(pConsole, szRule);
}

void CSAMPFunctions::SendRules(SOCKET s, char* data, const sockaddr_in* to, int tolen)
{
	if (CAddress::FUNC_CConsole__SendRules)
		pfn__CConsole__SendRules(pConsole, s, data, to, tolen);
}

void CSAMPFunctions::Execute(char* pExecLine)
{
	if (CAddress::FUNC_CConsole__Execute)
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

void CSAMPFunctions::RespawnVehicle_(void *pVehicle)
{
	pfn__CVehicle__Respawn(pVehicle);
}

void CSAMPFunctions::Packet_WeaponsUpdate(Packet *p)
{
	if (CAddress::FUNC_Packet_WeaponsUpdate)
		pfn__Packet_WeaponsUpdate(pNetGame, p);
}

char* CSAMPFunctions::format_amxstring(AMX *amx, cell *params, int parm, int &len)
{
	return pfn__format_amxstring(amx, params, parm, len);
}