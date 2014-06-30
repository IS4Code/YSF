/*
Leaked by ZYRONIX.net.
*/

#ifndef SAMPSRV_TIMERS_H
#define SAMPSRV_TIMERS_H

#include "CTypes.h"
#include <iostream>
#include <map>

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

struct ScriptTimer_s
{
	char szScriptFunc[255];
	int iTotalTime;
	int iRemainingTime;
	BOOL bRepeating;
	//BOOL bFilterscript;
	BOOL bKilled;
	AMX* pAMX;
	int iParamCount;
	//cell cellParams[16];
	//char szParamMap[16];
	void* cellParams;
	//void* szParamMap;
};

typedef std::map<DWORD, ScriptTimer_s*> DwordTimerMap;

//----------------------------------------------------------------------------------

class CScriptTimers
{
private:
	DwordTimerMap m_Timers;
	DWORD m_dwTimerCount;

	ScriptTimer_s*	FindTimer(DWORD dwTimerID);
public:
	CScriptTimers();
	~CScriptTimers();
	
	void DeleteForMode(AMX* pEndedAMX);
	DWORD New(char* szScriptFunc, int iInterval, BOOL bRepeating, AMX* pAMX);
	DWORD NewEx(char* szScriptFunc, int iInterval, BOOL bRepeating, cell *params, AMX* pAMX);
	void Delete(DWORD dwTimerId);
	void Kill(DWORD dwTimerId);
	void Process(int iElapsedTime);
	void FreeMem(ScriptTimer_s* Timer);

	DWORD GetTimerCount() { return m_dwTimerCount; }
	bool IsTimerActive(DWORD dwTimerID);
	char* GetTimerCallback(DWORD dwTimerID);

	int GetTimerTimeInterval(DWORD dwTimerID);
	bool SetTimerTimeInterval(DWORD dwTimerID, int time);

	int GetTimerRemainingTime(DWORD dwTimerID);

	bool IsTimerRepeating(DWORD dwTimerID);
};


#endif


//----------------------------------------------------------------------------------
// EOF
