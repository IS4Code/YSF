#include "main.h"
#include "Addresses.h"

#ifdef _WIN32
DWORD CAddress::FUNC_Logprintf_03Z = 0x00486CB0;
DWORD CAddress::FUNC_Logprintf_03ZR2_2 = 0x00487310;
#else
DWORD CAddress::FUNC_Logprintf_03Z = 0x080A7440;
DWORD CAddress::FUNC_Logprintf_03ZR2_2 = 0x080A77D0;
#endif

// Pointers
DWORD CAddress::VAR_ppNetGame = NULL;
DWORD CAddress::VAR_ppConsole = NULL;
DWORD CAddress::VAR_ppRakServer = NULL;

// Variables
DWORD CAddress::VAR_pRestartWaitTime = NULL;

// Functions
DWORD CAddress::FUNC_CConsole_AddStringVariable = NULL;
DWORD CAddress::FUNC_CConsole_SetStringVariable = NULL;
DWORD CAddress::FUNC_CConsole_ModifyVariableFlags = NULL;

DWORD CAddress::FUNC_CFilterscripts_LoadFilterscript = NULL;
DWORD CAddress::FUNC_CFilterscripts_UnLoadFilterscript = NULL;

void CAddress::Initialize(eSAMPVersion sampVersion)
{
	switch(sampVersion)
	{
	case SAMP_VERSION_03Z:
		{
#ifdef _WIN32

			//VAR_ppNetGame =								0x4F234C;
			//VAR_ppConsole =								0x4F2350;
			//VAR_ppRakServer =							0x4F24A4;

			VAR_pRestartWaitTime =						0x4B2414;

			FUNC_CConsole_AddStringVariable =			0x004869B0;
			FUNC_CConsole_SetStringVariable =			0x00485930;
			FUNC_CConsole_ModifyVariableFlags =			0x004858F0;

			FUNC_CFilterscripts_LoadFilterscript =		0x00469F90;
			FUNC_CFilterscripts_UnLoadFilterscript =	0x0046C2B0;
#else

			//VAR_ppNetGame =								0x0819A568;
			//VAR_ppConsole =								0x0819A564;
			//VAR_ppRakServer =							0x0819A6A8;

			VAR_pRestartWaitTime =						0x8150130;

			FUNC_CConsole_AddStringVariable =			0x0809F590;
			FUNC_CConsole_SetStringVariable =			0x0809EDB0;
			FUNC_CConsole_ModifyVariableFlags =			0x0809EE60;

			FUNC_CFilterscripts_LoadFilterscript =		0x0809FDB0;
			FUNC_CFilterscripts_UnLoadFilterscript =	0x080A01E0;
#endif
			break;
		}
	case SAMP_VERSION_03Z_R2_2:
		{
#ifdef _WIN32
			VAR_pRestartWaitTime =						0x4B25C4;

			FUNC_CConsole_AddStringVariable =			0x486FE0;
			FUNC_CConsole_SetStringVariable =			0x485F60;
			FUNC_CConsole_ModifyVariableFlags =			0x485F20;

			FUNC_CFilterscripts_LoadFilterscript =		0x46A100;
			FUNC_CFilterscripts_UnLoadFilterscript =	0x46C4E0;
#else
			VAR_pRestartWaitTime =						0x8150B60;

			FUNC_CConsole_AddStringVariable =			0x809F760;
			FUNC_CConsole_SetStringVariable =			0x809F000;
			FUNC_CConsole_ModifyVariableFlags =			0x809F030;

			FUNC_CFilterscripts_LoadFilterscript =		0x0809FF80;
			FUNC_CFilterscripts_UnLoadFilterscript =	0x080A03B0;
#endif
			break;
		} 
	}
}