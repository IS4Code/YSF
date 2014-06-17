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
	// Thx for Whitetiger
	#ifdef WIN32
	VAR_pRestartWaitTime =						FindPattern("\x00\x00\xC8\xC2\x00\x00", "xxxxxx") + 0x4;
 
	FUNC_CConsole_AddStringVariable =			FindPattern("\x53\x56\x57\x8B\x7C\x24\x18\x85\xFF", "xxxxxxxxx");
	FUNC_CConsole_SetStringVariable =			FindPattern("\x8B\x44\x24\x04\x53\x50\xE8\xD5\xFE\xFF\xFF\x8B\xD8\x85\xDB", "xxxxxxx???xxxx");
	FUNC_CConsole_ModifyVariableFlags =			FindPattern("\x8B\x44\x24\x04\x50\xE8\x16\xFF\xFF\xFF\x85\xC0\x74\x07", "xxxxxx????xxxx");
 
	FUNC_CFilterscripts_LoadFilterscript =		FindPattern("\x8B\x44\x24\x04\x81\xEC\x04\x01\x00\x00", "xxxxxxxxxx");
	FUNC_CFilterscripts_UnLoadFilterscript =	FindPattern("\xCC\x51\x53\x8B\x5C\x24\x0C\x55\x56\x57\x89", "xxxxxxxxxxx") + 0x1;
 
	#else
 
	switch(sampVersion)
	{
		case SAMP_VERSION_03Z:
		{
			VAR_pRestartWaitTime =						0x8150130;
 
			FUNC_CConsole_AddStringVariable =			0x0809F590;
			FUNC_CConsole_SetStringVariable =			0x0809EDB0;
			FUNC_CConsole_ModifyVariableFlags =			0x0809EE60;
 
			FUNC_CFilterscripts_LoadFilterscript =		0x0809FDB0;
			FUNC_CFilterscripts_UnLoadFilterscript =	0x080A01E0;
			break;
		}
		case SAMP_VERSION_03Z_R2_2:
		{
			VAR_pRestartWaitTime =						0x8150B60;
 
			FUNC_CConsole_AddStringVariable =			0x809F760;
			FUNC_CConsole_SetStringVariable =			0x809F000;
			FUNC_CConsole_ModifyVariableFlags =			0x809F030;
 
			FUNC_CFilterscripts_LoadFilterscript =		0x0809FF80;
			FUNC_CFilterscripts_UnLoadFilterscript =	0x080A03B0;
			break;
		} 
	}
	#endif
}