#include "Addresses.h"

#include "Hooks.h"
#include "main.h"

#include <sdk/plugin.h>

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
DWORD CAddress::FUNC_CConsole__AddStringVariable = NULL;
DWORD CAddress::FUNC_CConsole__SetStringVariable = NULL;
DWORD CAddress::FUNC_CConsole__SetIntVariable = NULL;
DWORD CAddress::FUNC_CConsole__ModifyVariableFlags = NULL;

DWORD CAddress::FUNC_CFilterscripts__LoadFilterscript = NULL;
DWORD CAddress::FUNC_CFilterscripts__UnLoadFilterscript = NULL;
DWORD CAddress::FUNC_ContainsInvalidChars = NULL;
DWORD CAddress::FUNC_GetPacketID = NULL;

DWORD CAddress::FUNC_CPlayer__SpawnForWorld = NULL;
DWORD CAddress::FUNC_ProcessQueryPacket = NULL;
DWORD CAddress::FUNC_format_amxstring = NULL;

// Others
DWORD CAddress::ADDR_CNetGame_GMX_GangZoneDelete = NULL;
DWORD CAddress::ADDR_CNetGame_GMX_PckupDelete = NULL;

void CAddress::Initialize(eSAMPVersion sampVersion)
{
	// Thx for Whitetiger
	#ifdef _WIN32
	VAR_pRestartWaitTime =						FindPattern("\x00\x00\xC8\xC2\x00\x00", "xxxxxx") + 0x4;
 
	FUNC_CConsole__AddStringVariable =			FindPattern("\x53\x56\x57\x8B\x7C\x24\x18\x85\xFF", "xxxxxxxxx");
	FUNC_CConsole__SetStringVariable =			FindPattern("\x8B\x44\x24\x04\x53\x50\xE8\xD5\xFE\xFF\xFF\x8B\xD8\x85\xDB", "xxxxxxx???xxxx");
	FUNC_CConsole__SetIntVariable =				FindPattern("\x8B\x44\x24\x04\x50\xE8\xF6\xFD\xFF\xFF\x85\xC0\x74\xE0\x83\x38\x01", "xxxxxx????xx??xxx") + 0x20;
	FUNC_CConsole__ModifyVariableFlags =		FindPattern("\x8B\x44\x24\x04\x50\xE8\x16\xFF\xFF\xFF\x85\xC0\x74\x07", "xxxxxx????xxxx");
 
	FUNC_CFilterscripts__LoadFilterscript =		FindPattern("\x8B\x44\x24\x04\x81\xEC\x04\x01\x00\x00", "xxxxxxxxxx");
	FUNC_CFilterscripts__UnLoadFilterscript =	FindPattern("\xCC\x51\x53\x8B\x5C\x24\x0C\x55\x56\x57\x89", "xxxxxxxxxxx") + 0x1;

	FUNC_ContainsInvalidChars =					FindPattern("\x8B\x4C\x24\x04\x8A\x01\x84\xC0", "xxxxxxxx");
	FUNC_GetPacketID =							FindPattern("\x8B\x44\x24\x04\x85\xC0\x75\x03\x0C\xFF\xC3", "xxxxxxx???x");
	
	FUNC_CPlayer__SpawnForWorld =				FindPattern("\x56\x8B\xF1\x8B\x86\x3B\x26\x00\x00\x85\xC0\x0F\x84", "xxxxx????xxxx");
	FUNC_ProcessQueryPacket =					FindPattern("\x83\xEC\x24\x53\x55\x56\x57\x8B\x7C\x24", "xxxxxxxxxx");

	ADDR_CNetGame_GMX_GangZoneDelete =			FindPattern("\x83\xC4\x04\x89\x5E\x24", "xxxxxx") - 0x8;
	ADDR_CNetGame_GMX_PckupDelete =				FindPattern("\x83\xC4\x04\x89\x5E\x10", "xxxxxx") - 0x8;
	//0x00488820

	FUNC_format_amxstring = 0x0046ED90;


	logprintf("ADDR_CNetGame_GMX_GangZoneDelete: %x", ADDR_CNetGame_GMX_GangZoneDelete); // 00492660
	logprintf("ADDR_CNetGame_GMX_PckupDelete: %x", ADDR_CNetGame_GMX_PckupDelete); // 0048A059
	logprintf("VAR_pRestartWaitTime: %X", VAR_pRestartWaitTime);

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
	VAR_pRestartWaitTime = 						FindPattern("\x00\x00\x40\x41\xFF\xFF\xFF\xFF", "xxxxxxxx");

	FUNC_CConsole__AddStringVariable = 			FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x00\x8B\x75\x00\x85\xF6\x74\x00\x89\x34\x24", "xxxxxxx?xx?xxx?xxx");
	FUNC_CConsole__SetStringVariable =			FindPattern("\x55\x89\xE5\x83\xEC\x00\x89\x75\x00\x8B\x45\x00\x89\x7D\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00\x8B\x45\x00", "xxxxx?xx?xx?xx?xx?xx?xxx?xx?");
	FUNC_CConsole__SetIntVariable =				FindPattern("\x83\x38\x00\x74\x00\xC9\xC3\x8B\x50\x00\x8B\x45\x00", "xx?x?xxxx?xx?") - 0x1C;
	FUNC_CConsole__ModifyVariableFlags =		FindPattern("\x89\x04\x24\xE8\x00\x00\x00\x00\x85\xC0\x89\xC2\x74\x00\x8B\x45\x00", "xxxx????xxxxx?xx?") - 0x10;
	 
	FUNC_CFilterscripts__LoadFilterscript =		FindPattern("\x89\x7D\x00\x8B\x45\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00", "xx?xx?xx?xx?xxx?") - 0x9;
	FUNC_CFilterscripts__UnLoadFilterscript =	FindPattern("\x31\xF6\x53\x83\xEC\x00\x8B\x45\x00\x8B\x7D\x00\x89\xC3", "xxxxx?xx?xx?xx") - 0x5;

	FUNC_ContainsInvalidChars =					FindPattern("\x53\x8B\x5D\x00\x0F\xB6\x0B\x84\xC9\x74\x00\x66\x90", "xxx?xxxxxx?xx") - 0x3;
	FUNC_GetPacketID =							FindPattern("\x55\xB8\x00\x00\x00\x00\x89\xE5\x8B\x55\x00\x85\xD2", "xx????xxxx?xx");

	FUNC_CPlayer__SpawnForWorld =				FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x00\x8B\x75\x00\xA1\x00\x00\x00\x00", "xxxxxxx?xx?x????");
	FUNC_ProcessQueryPacket =					NULL;

	ADDR_CNetGame_GMX_GangZoneDelete =			NULL;
	ADDR_CNetGame_GMX_PckupDelete =				NULL;

	logprintf("VAR_pRestartWaitTime: %X", VAR_pRestartWaitTime);
	logprintf("FUNC_CConsole__AddStringVariable: %X", FUNC_CConsole__AddStringVariable);
	logprintf("FUNC_CConsole__SetStringVariable: %X", FUNC_CConsole__SetStringVariable);
	logprintf("FUNC_CConsole__SetIntVariable: %X", FUNC_CConsole__ModifyVariableFlags);
	logprintf("FUNC_CConsole__ModifyVariableFlags: %X", FUNC_CConsole__ModifyVariableFlags);
	
	logprintf("FUNC_CFilterscripts__LoadFilterscript: %X", FUNC_CFilterscripts__LoadFilterscript);
	logprintf("FUNC_CFilterscripts__UnLoadFilterscript: %X", FUNC_CFilterscripts__UnLoadFilterscript);

	logprintf("FUNC_ContainsInvalidChars: %X", FUNC_ContainsInvalidChars);
	logprintf("FUNC_GetPacketID: %X", FUNC_GetPacketID);

	logprintf("FUNC_CPlayer__SpawnForWorld: %X", FUNC_CPlayer__SpawnForWorld);

	switch(sampVersion)
	{
		case SAMP_VERSION_03Z:
		{
			VAR_pRestartWaitTime =						0x8150130;
			FUNC_CConsole__SetIntVariable =				0x809ECE0;
			break;
		}
		case SAMP_VERSION_03Z_R2_2:
		{
			VAR_pRestartWaitTime =						0x8150B60;
			FUNC_CConsole__SetIntVariable =				0x809EEB0;
			break;
		}
		case SAMP_VERSION_03Z_R3:
		{
			VAR_pRestartWaitTime =						0x81512F0;
			FUNC_CConsole__SetIntVariable =				0x809EFB0;
			break;
		}
		case SAMP_VERSION_03Z_R4:
		{
			VAR_pRestartWaitTime =						0x81514E0; // 12.0
			FUNC_CConsole__SetIntVariable =				0x809EFB0; // find for maxplayers
			FUNC_CPlayer__SpawnForWorld = 				0x080CB160; // find for OnPlayerSpawn
			FUNC_GetPacketID =							0x080A9610;
			//FUNC_ProcessQueryPacket =					NULL;
			break;
		}
	}
	#endif

	// Unlock restart wait time
	Unlock((void*)CAddress::VAR_pRestartWaitTime, 4);

#ifdef _WIN32
	// Disable GangZonePool deletion at GMX
	Unlock((void*)ADDR_CNetGame_GMX_GangZoneDelete, 2); // jz      short loc_489DC8 -> change to jnz      short loc_489DC8
	*(BYTE*)(ADDR_CNetGame_GMX_GangZoneDelete) = 0x75;	// jnz

	// Disable PickupPool deletion at GMX
	//Unlock((void*)ADDR_CNetGame_GMX_PckupDelete, 2); // jz      short loc_489DC8 -> change to jnz      short loc_489DC8
	//*(BYTE*)(ADDR_CNetGame_GMX_PckupDelete) = 0x75;	// jnz
#endif
}