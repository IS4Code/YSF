#include "main.h"
#include "Addresses.h"
#include "Hooks.h"

#ifdef _WIN32
DWORD CAddress::FUNC_Logprintf_03Z = 0x00486CB0;
DWORD CAddress::FUNC_Logprintf_03ZR2_2 = 0x00487310;
DWORD CAddress::FUNC_Logprintf_03ZR3 = 0x00487460;
DWORD CAddress::FUNC_Logprintf_03ZR4 = 0x004875F0;
#else
DWORD CAddress::FUNC_Logprintf_03Z = 0x080A7440;
DWORD CAddress::FUNC_Logprintf_03ZR2_2 = 0x080A77D0;
DWORD CAddress::FUNC_Logprintf_03ZR3 = 0x080A78E0;
DWORD CAddress::FUNC_Logprintf_03ZR4 = 0x80A7A90;
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
DWORD CAddress::FUNC_CConsole_SetIntVariable = NULL;
DWORD CAddress::FUNC_CConsole_ModifyVariableFlags = NULL;

DWORD CAddress::FUNC_CFilterscripts_LoadFilterscript = NULL;
DWORD CAddress::FUNC_CFilterscripts_UnLoadFilterscript = NULL;
DWORD CAddress::FUNC_ContainsInvalidChars = NULL;

// Others
DWORD CAddress::ADDR_CNetGame_GMX_GangZoneDelete = NULL;

// Receive hook
DWORD CAddress::ADDR_RECEIVE_HOOKPOS = NULL;

void CAddress::Initialize(eSAMPVersion sampVersion)
{
	DWORD asd = NULL;
	// Thx for Whitetiger
	#ifdef WIN32
	VAR_pRestartWaitTime =						FindPattern("\x00\x00\xC8\xC2\x00\x00", "xxxxxx") + 0x4;
 
	FUNC_CConsole_AddStringVariable =			FindPattern("\x53\x56\x57\x8B\x7C\x24\x18\x85\xFF", "xxxxxxxxx");
	FUNC_CConsole_SetStringVariable =			FindPattern("\x8B\x44\x24\x04\x53\x50\xE8\xD5\xFE\xFF\xFF\x8B\xD8\x85\xDB", "xxxxxxx???xxxx");
	FUNC_CConsole_SetIntVariable =				FindPattern("\x8B\x44\x24\x04\x50\xE8\xF6\xFD\xFF\xFF\x85\xC0\x74\xE0\x83\x38\x01", "xxxxxx????xx??xxx") + 0x20;
	FUNC_CConsole_ModifyVariableFlags =			FindPattern("\x8B\x44\x24\x04\x50\xE8\x16\xFF\xFF\xFF\x85\xC0\x74\x07", "xxxxxx????xxxx");
 
	FUNC_CFilterscripts_LoadFilterscript =		FindPattern("\x8B\x44\x24\x04\x81\xEC\x04\x01\x00\x00", "xxxxxxxxxx");
	FUNC_CFilterscripts_UnLoadFilterscript =	FindPattern("\xCC\x51\x53\x8B\x5C\x24\x0C\x55\x56\x57\x89", "xxxxxxxxxxx") + 0x1;

	ADDR_CNetGame_GMX_GangZoneDelete =			FindPattern("\x83\xC4\x04\x89\x5E\x24", "xxxxxx") - 0x8;
	FUNC_ContainsInvalidChars =					FindPattern("\x8B\x4C\x24\x04\x8A\x01\x84\xC0", "xxxxxxxx");
	
	ADDR_RECEIVE_HOOKPOS =						FindPattern("\x8B\x4E\x10\x8A\x01\x3C\x16\x74\x10\x83\x7E", "xx?xxxx??xx"); // R2-2: 0x458A20
	asd =										FindPattern("\x75\x53\x8B\x81\x19\x23\x00\x00\x85\xC0", "xxxx????xx");

	logprintf("ADDR_RECEIVE_HOOKPOS: %x", asd);
	switch(sampVersion)
	{
		case SAMP_VERSION_03Z:
		{
			//ADDR_RECEIVE_HOOKPOS =						0x458B80;
			break;
		}
		case SAMP_VERSION_03Z_R4:
		{
			//ADDR_RECEIVE_HOOKPOS =						0x458A20;
			break;
		}
	}
	#else

	// Thx for Mellnik
	FUNC_CConsole_AddStringVariable = 			FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x00\x8B\x75\x00\x85\xF6\x74\x00\x89\x34\x24", "xxxxxxx?xx?xxx?xxx");
	FUNC_CConsole_SetStringVariable =			FindPattern("\x55\x89\xE5\x83\xEC\x00\x89\x75\x00\x8B\x45\x00\x89\x7D\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00\x8B\x45\x00", "xxxxx?xx?xx?xx?xx?xx?xxx?xx?");
	FUNC_CConsole_ModifyVariableFlags =			FindPattern("\x89\x04\x24\xE8\x00\x00\x00\x00\x85\xC0\x89\xC2\x74\x00\x8B\x45\x00", "xxxx????xxxxx?xx?") - 0x10;
	 
	FUNC_CFilterscripts_LoadFilterscript =		FindPattern("\x89\x7D\x00\x8B\x45\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00", "xx?xx?xx?xx?xxx?") - 0x9;
	FUNC_CFilterscripts_UnLoadFilterscript =	FindPattern("\x31\xF6\x53\x83\xEC\x00\x8B\x45\x00\x8B\x7D\x00\x89\xC3", "xxxxx?xx?xx?xx") - 0x5;

	ADDR_CNetGame_GMX_GangZoneDelete =			NULL;
	FUNC_ContainsInvalidChars =					FindPattern("\x53\x8B\x5D\x00\x0F\xB6\x0B\x84\xC9\x74\x00\x66\x90", "xxx?xxxxxx?xx") - 0x3;
	/*
	logprintf("FUNC_CConsole_AddStringVariable: %X", FUNC_CConsole_AddStringVariable);
	logprintf("FUNC_CConsole_SetStringVariable: %X", FUNC_CConsole_SetStringVariable);
	logprintf("FUNC_CConsole_SetIntVariable: %X", FUNC_CConsole_ModifyVariableFlags);
	logprintf("FUNC_CConsole_ModifyVariableFlags: %X", FUNC_CConsole_ModifyVariableFlags);
	
	logprintf("FUNC_CFilterscripts_LoadFilterscript: %X", FUNC_CFilterscripts_LoadFilterscript);
	logprintf("FUNC_CFilterscripts_UnLoadFilterscript: %X", FUNC_CFilterscripts_UnLoadFilterscript);

	logprintf("FUNC_ContainsInvalidChars: %X", FUNC_ContainsInvalidChars);
	*/
	switch(sampVersion)
	{
		case SAMP_VERSION_03Z:
		{
			VAR_pRestartWaitTime =						0x8150130;
			FUNC_CConsole_SetIntVariable =				0x809ECE0;

			ADDR_RECEIVE_HOOKPOS =						0x80ACC0F;							
			break;
		}
		case SAMP_VERSION_03Z_R2_2:
		{
			VAR_pRestartWaitTime =						0x8150B60;
			FUNC_CConsole_SetIntVariable =				0x809EEB0;

			ADDR_RECEIVE_HOOKPOS =						NULL;
			break;
		}
		case SAMP_VERSION_03Z_R3:
		{
			VAR_pRestartWaitTime =						0x81512F0;
			FUNC_CConsole_SetIntVariable =				0x809EFB0;

			ADDR_RECEIVE_HOOKPOS =						0x080AD1FF;
			break;
		}
		case SAMP_VERSION_03Z_R4:
		{
			VAR_pRestartWaitTime =						0x81514E0; // 12.0
			FUNC_CConsole_SetIntVariable =				0x809EFB0; // find for maxplayers
			break;
		}
	}
	#endif

#ifdef WIN32
	// Disable GangZonePool deletion at GMX
	Unlock((void*)ADDR_CNetGame_GMX_GangZoneDelete, 2); // jz      short loc_489DC8 -> change to jnz      short loc_489DC8
	*(BYTE*)(ADDR_CNetGame_GMX_GangZoneDelete) = 0x75;	// jnz

	if(asd)
	{
		Unlock((void*)asd, 2);
		*(BYTE*)(asd) = 0xEB;
	}
#else
	if(asd)
	{
		Unlock((void*)asd, 2);
		*(BYTE*)(asd) = 0xE9;
	}
#endif
}