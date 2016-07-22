#include "main.h"

//#define print_addresses
#ifdef _WIN32
DWORD CAddress::FUNC_Logprintf_03Z = 0x00486CB0;
DWORD CAddress::FUNC_Logprintf_03ZR2_2 = 0x00487310;
DWORD CAddress::FUNC_Logprintf_03ZR3 = 0x00487460;
DWORD CAddress::FUNC_Logprintf_03ZR4 = 0x004875F0;
DWORD CAddress::FUNC_Logprintf_037 = 0x0048A0B0;
DWORD CAddress::FUNC_Logprintf_037_R2_1 = 0x0048C8D0;
#else
DWORD CAddress::FUNC_Logprintf_03Z = 0x080A7440;
DWORD CAddress::FUNC_Logprintf_03ZR2_2 = 0x080A77D0;
DWORD CAddress::FUNC_Logprintf_03ZR3 = 0x080A78E0;
DWORD CAddress::FUNC_Logprintf_03ZR4 = 0x80A7A90;
DWORD CAddress::FUNC_Logprintf_037 = 0x080A9000;
DWORD CAddress::FUNC_Logprintf_037_R2_1 = 0x080A91D0;
#endif

// Pointers
DWORD CAddress::VAR_ppNetGame = NULL;
DWORD CAddress::VAR_ppConsole = NULL;
DWORD CAddress::VAR_ppRakServer = NULL;

// Variables
DWORD CAddress::VAR_pRestartWaitTime = NULL;
DWORD CAddress::VAR_pPosSyncBounds[4];

// Functions
DWORD CAddress::FUNC_CConsole__AddStringVariable = NULL;
DWORD CAddress::FUNC_CConsole__GetStringVariable = NULL;
DWORD CAddress::FUNC_CConsole__SetStringVariable = NULL;
DWORD CAddress::FUNC_CConsole__GetIntVariable = NULL;
DWORD CAddress::FUNC_CConsole__SetIntVariable = NULL;
DWORD CAddress::FUNC_CConsole__GetBoolVariable = NULL;
DWORD CAddress::FUNC_CConsole__ModifyVariableFlags = NULL;
DWORD CAddress::FUNC_CConsole__FindVariable = NULL;
DWORD CAddress::FUNC_CConsole__SendRules = NULL;
DWORD CAddress::FUNC_CConsole__Execute = NULL;

DWORD CAddress::FUNC_CNetGame__SetWeather = NULL;
DWORD CAddress::FUNC_CNetGame__SetGravity = NULL;

DWORD CAddress::FUNC_CFilterscripts__LoadFilterscript = NULL;
DWORD CAddress::FUNC_CFilterscripts__UnLoadFilterscript = NULL;
DWORD CAddress::FUNC_ContainsInvalidChars = NULL;
DWORD CAddress::FUNC_GetPacketID = NULL;

DWORD CAddress::FUNC_CPlayer__SpawnForWorld = NULL;

DWORD CAddress::FUNC_CVehicle__Respawn = NULL;
DWORD CAddress::FUNC_CPlayerPool__HandleVehicleRespawn = NULL;

DWORD CAddress::FUNC_ProcessQueryPacket = NULL;
DWORD CAddress::FUNC_Packet_WeaponsUpdate = NULL;
DWORD CAddress::FUNC_Packet_StatsUpdate = NULL;
DWORD CAddress::FUNC_format_amxstring = NULL;

// Others
DWORD CAddress::ADDR_GetNetworkStats_VerbosityLevel = NULL;
DWORD CAddress::ADDR_GetPlayerNetworkStats_VerbosityLevel = NULL;

DWORD CAddress::ADDR_RecordingDirectory = NULL;

void CAddress::Initialize(eSAMPVersion sampVersion)
{
	// Thx for Whitetiger
	DWORD dwRestartTime, dwPosSyncBounds;
#ifdef _WIN32
	dwRestartTime =								FindPattern("\xD9\x15\x00\x00\x00\x00\xD8\x1D\x00\x00\x00\x00\xDF\xE0\xF6\xC4\x41\x75\x07", "xx????xx????xxxxxxx") + 6;
	VAR_pRestartWaitTime =						 *(DWORD*)(dwRestartTime + 2);

	dwPosSyncBounds =							FindPattern("Shot out of bounds.\0", "xxxxxxxxxxxxxxxxxxxx");
	if(dwPosSyncBounds)
	{
		VAR_pPosSyncBounds[0] = dwPosSyncBounds + 20;
		VAR_pPosSyncBounds[1] = VAR_pPosSyncBounds[0] + 4;
	}
	dwPosSyncBounds =							FindPattern("Offset out of bounds.\0\0\0", "xxxxxxxxxxxxxxxxxxxxxxxx");
	if(dwPosSyncBounds)
	{
		VAR_pPosSyncBounds[2] = dwPosSyncBounds + 24;
	}					
	VAR_pPosSyncBounds[3] =						FindPattern("\x00\x50\x43\x48\x00\x00\xC8\xC2\x00\x00\x00\x3F\xB2\x00\x00\x00\xAD\x00\x00\x00\xAE\x00\x00\x00maxnpc", "????xxxxxxxxxxxxxxxxxxxxxxxxxx");

	FUNC_CConsole__AddStringVariable =			FindPattern("\x53\x56\x57\x8B\x7C\x24\x18\x85\xFF", "xxxxxxxxx");
	FUNC_CConsole__GetStringVariable =			FindPattern("\x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x85\xC0\x74\x0B", "xxxxxx????xxxx");
	FUNC_CConsole__SetStringVariable =			FindPattern("\x8B\x44\x24\x04\x53\x50\xE8\xD5\xFE\xFF\xFF\x8B\xD8\x85\xDB", "xxxxxxx???xxxx");
	FUNC_CConsole__GetIntVariable =				FindPattern("\x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x85\xC0\x74\x0D\x83\x38\x01\x75\x08", "xxxxxx????xxxxxxxxx");
	FUNC_CConsole__SetIntVariable =				FindPattern("\x8B\x44\x24\x04\x50\xE8\xF6\xFD\xFF\xFF\x85\xC0\x74\xE0\x83\x38\x01", "xxxxxx????xx??xxx") + 0x20;
	FUNC_CConsole__GetBoolVariable =			FindPattern("\x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x85\xC0\x74\x0D", "xxxxxx????xxxx") + 0x90;
	FUNC_CConsole__ModifyVariableFlags =		FindPattern("\x8B\x44\x24\x04\x50\xE8\x16\xFF\xFF\xFF\x85\xC0\x74\x07", "xxxxxx????xxxx");
	FUNC_CConsole__FindVariable =				FindPattern("\x8B\x84\x24\x30\x01\x00\x00\x53\x56\x57", "xxxxxxxxxx") - 0x1B;
	FUNC_CConsole__SendRules =					FindPattern("\x81\xEC\x08\x04\x00\x00\x53\x55\x56\x57\x8B\xF9\x8B\x77\x04", "xx????xxxxxxxxx");
	FUNC_CConsole__Execute =					FindPattern("\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x0C\x01\x00\x00", "xxxxxxxxxxxx");

	FUNC_CNetGame__SetWeather =					0x48E190;
	FUNC_CNetGame__SetGravity =					0x48E260;

	FUNC_CFilterscripts__LoadFilterscript =		FindPattern("\x8B\x44\x24\x04\x81\xEC\x04\x01\x00\x00", "xxxxxxxxxx");
	FUNC_CFilterscripts__UnLoadFilterscript =	FindPattern("\xCC\x51\x53\x8B\x5C\x24\x0C\x55\x56\x57\x89", "xxxxxxxxxxx") + 0x1;

	FUNC_ContainsInvalidChars =					FindPattern("\x8B\x4C\x24\x04\x8A\x01\x84\xC0", "xxxxxxxx");
	FUNC_GetPacketID =							FindPattern("\x8B\x44\x24\x04\x85\xC0\x75\x03\x0C\xFF\xC3", "xxxxxxx???x");

	FUNC_CPlayer__SpawnForWorld =				FindPattern("\x56\x8B\xF1\x8B\x86\x3B\x26\x00\x00\x85\xC0\x0F\x84", "xxxxx????xxxx");
	FUNC_CVehicle__Respawn =					FindPattern("\x53\x33\xC0\x56\x8B\xF1\x57\xB9\x10\x00\x00\x00\x8D\x7E\x0C", "xxxxxxxxx???xxx");
	FUNC_CPlayerPool__HandleVehicleRespawn =	FindPattern("\x53\x55\x56\x8B\xF1\x8B\xAE\x68\x09", "xxxxxx?xx");

	FUNC_ProcessQueryPacket =					FindPattern("\x83\xEC\x24\x53\x55\x56\x57\x8B\x7C\x24", "xxxxxxxxxx");
	FUNC_Packet_WeaponsUpdate =					FindPattern("\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x0\x0\x0\x0\x50\x64\x89\x25\x0\x0\x0\x0\x81\xEC\x2C\x01\x00\x00\x55\x56", "xx????xx????xxxx????xxxx??xx");
	FUNC_Packet_StatsUpdate =					FindPattern("\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x20\x01\x00\x00\x56\x57", "xxx????xx????xxxxxxxxxxx???xx");
	FUNC_format_amxstring =						FindPattern("\x8B\x54\x24\x08\x56\x8B\x74\x24\x08\x57\x33\xC0", "xxxxxxxxxxxx");

	ADDR_GetNetworkStats_VerbosityLevel =		FindPattern("\x6A\x01\x8D\x4C\x0C", "xxxxx"); // 0x0047362A
	ADDR_GetPlayerNetworkStats_VerbosityLevel = FindPattern("\x6A\x01\x8D\x44\x04", "xxxxx"); // 0x004730B9;

	ADDR_RecordingDirectory =					FindPattern("\x75\xDF\x8D\x44\x24\x18\x50\x8D\x8C\x24", "xxxxx?xxxx") + 14; // 0x00482265
	#else

	// Thx for Mellnik
	VAR_pRestartWaitTime = 						NULL;
	/*
	dwPosSyncBounds =							FindPattern("\x00\x00\x80\xBF\x00\x40\x9C\x46\x00\x40\x9C\xC6\x00\x50\x43\x48\x00\x00\x7A\xC4\x00\x00\xC8\x42\x00\x00\xC8\xC2\x00\x00\x00\x3F\x00\x00\x00\xC0", "xxxx????????????????xxxxxxxxxxxxxxxx");
	if(dwPosSyncBounds)
	{
		VAR_pPosSyncBounds[0] = dwPosSyncBounds+8;
		VAR_pPosSyncBounds[1] = dwPosSyncBounds+4;
		VAR_pPosSyncBounds[2] = dwPosSyncBounds+20;
		VAR_pPosSyncBounds[3] = dwPosSyncBounds+12;
	}
	*/

	FUNC_CConsole__AddStringVariable = 			FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x00\x8B\x75\x00\x85\xF6\x74\x00\x89\x34\x24", "xxxxxxx?xx?xxx?xxx");
	FUNC_CConsole__GetStringVariable =			FUNC_CConsole__AddStringVariable - 0x760;//0x80A0190;
	FUNC_CConsole__SetStringVariable =			FindPattern("\x55\x89\xE5\x83\xEC\x00\x89\x75\x00\x8B\x45\x00\x89\x7D\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00\x8B\x45\x00", "xxxxx?xx?xx?xx?xx?xx?xxx?xx?");
	FUNC_CConsole__SetIntVariable =				FindPattern("\x83\x38\x00\x74\x00\xC9\xC3\x8B\x50\x00\x8B\x45\x00", "xx?x?xxxx?xx?") - 0x1C;
	FUNC_CConsole__GetIntVariable =				FUNC_CConsole__SetIntVariable + 0x30; // 0x80A0070;
	FUNC_CConsole__GetBoolVariable =			FUNC_CConsole__SetIntVariable - 0x30;//0x80A0010;
	FUNC_CConsole__ModifyVariableFlags =		FindPattern("\x89\x04\x24\xE8\x00\x00\x00\x00\x85\xC0\x89\xC2\x74\x00\x8B\x45\x00", "xxxx????xxxxx?xx?") - 0x10;	
	FUNC_CConsole__FindVariable =				FindPattern("\xB9\xFF\x00\x00\x00\x89\xE5\x81\xEC\x68\x01\x00\x00", "xxxxxxxxxxxxx") - 0x1;
	FUNC_CConsole__SendRules =					FindPattern("\x55\x31\xD2\x89\xE5\x57\x56\x53\x81\xEC\x4C\x04", "xxxxxxxxxxxx");
	FUNC_CConsole__Execute =					FindPattern("\x55\x89\xE5\x57\x56\x53\x81\xEC\x3C\x01\x00\x00\x8B\x45\x0C", "xxxxxxxxxxxxxxx");

	FUNC_CFilterscripts__LoadFilterscript =		FindPattern("\x89\x7D\x00\x8B\x45\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00", "xx?xx?xx?xx?xxx?") - 0x9;
	FUNC_CFilterscripts__UnLoadFilterscript =	FindPattern("\x31\xF6\x53\x83\xEC\x00\x8B\x45\x00\x8B\x7D\x00\x89\xC3", "xxxxx?xx?xx?xx") - 0x5;

	FUNC_ContainsInvalidChars =					FindPattern("\x53\x8B\x5D\x00\x0F\xB6\x0B\x84\xC9\x74\x00\x66\x90", "xxx?xxxxxx?xx") - 0x3;
	FUNC_GetPacketID =							FindPattern("\x55\xB8\x00\x00\x00\x00\x89\xE5\x8B\x55\x00\x85\xD2", "xx????xxxx?xx");

	FUNC_CPlayer__SpawnForWorld =				FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x00\x8B\x75\x00\xA1\x00\x00\x00\x00", "xxxxxxx?xx?x????");
	
	// 
	DWORD addr = FindPattern("\x55\x31\xD2\x89\xE5\x57\xB9\x40\x00\x00\x00", "xxxxxxxxxxx");
	logprintf("addr: %x", addr);
	
	FUNC_ProcessQueryPacket =					FindPattern("\x81\xEC\xA8\x00\x00\x00\x89\x5D\xF4\x8B\x5D\x14\x89\x75\xF8", "xxxxxxxxxxxxxxx") - 0x3;
	FUNC_Packet_WeaponsUpdate =					FindPattern("\x55\x31\xC0\x89\xE5\x81\xEC\x58\x01\x00\x00", "xxxxxxxxxxx");
	FUNC_Packet_StatsUpdate =					FindPattern("\x55\x31\xD2\x89\xE5\x81\xEC\x58\x01\x00\x00\x89\x5D\xF4", "xxxxxxxxxxxxxx"); // 80AD430
	FUNC_format_amxstring = FindPattern("\x55\xB8\x00\x10\x00\x00\x89\xE5\x56\x53\x83\xEC\x20", "xxxxxxxxxxxxx");

	ADDR_GetNetworkStats_VerbosityLevel =		FindPattern("\xB8\x01\x00\x00\x00\x83\xD9\x03", "xxxxxxxx");
	ADDR_GetPlayerNetworkStats_VerbosityLevel = FindPattern("\xBB\x01\x00\x00\x00\x83\xD9\x03", "xxxxxxxx"); // 080DD7FA

	switch(sampVersion)
	{
		case SAMP_VERSION_037:
		{
			VAR_pRestartWaitTime =						0x081A0840;
			break;
		}
		case SAMP_VERSION_037_R2:
		{
			VAR_pRestartWaitTime =						0x0815A528; // 12.0
			VAR_pPosSyncBounds[0] =						0x08150710;
			VAR_pPosSyncBounds[1] =						0x0815070C;
			VAR_pPosSyncBounds[2] =						0x08150718;
			VAR_pPosSyncBounds[3] =						0x08150714;
			ADDR_RecordingDirectory =					0x080CC7D1;
			FUNC_CVehicle__Respawn =					0x814B4C0;
			FUNC_CPlayerPool__HandleVehicleRespawn =	0x80D1480;
			break;
		}
	}
	#endif
	/*
	logprintf("VAR_pRestartWaitTime: %X", VAR_pRestartWaitTime);

	logprintf("FUNC_CConsole__AddStringVariable: %X", FUNC_CConsole__AddStringVariable);
	logprintf("FUNC_CConsole__GetStringVariable: %X", FUNC_CConsole__GetStringVariable);
	logprintf("FUNC_CConsole__SetStringVariable: %X", FUNC_CConsole__SetStringVariable);
	logprintf("FUNC_CConsole__GetIntVariable: %X", FUNC_CConsole__GetIntVariable);
	logprintf("FUNC_CConsole__SetIntVariable: %X", FUNC_CConsole__SetIntVariable);
	logprintf("FUNC_CConsole__GetBoolVariable: %X", FUNC_CConsole__GetBoolVariable);
	logprintf("FUNC_CConsole__FindVariable: %X", FUNC_CConsole__FindVariable);
	logprintf("FUNC_CConsole__SendRules: %X", FUNC_CConsole__SendRules);
	logprintf("FUNC_CConsole__Execute: %X", FUNC_CConsole__Execute);

	logprintf("FUNC_CFilterscripts__LoadFilterscript: %X", FUNC_CFilterscripts__LoadFilterscript);
	logprintf("FUNC_CFilterscripts__UnLoadFilterscript: %X", FUNC_CFilterscripts__UnLoadFilterscript);

	logprintf("FUNC_ContainsInvalidChars: %X", FUNC_ContainsInvalidChars);
	logprintf("FUNC_GetPacketID: %X", FUNC_GetPacketID);

	logprintf("FUNC_CPlayer__SpawnForWorld: %X", FUNC_CPlayer__SpawnForWorld);
	logprintf("FUNC_CVehicle__Respawn: %X", FUNC_CVehicle__Respawn);
	logprintf("FUNC_CPlayerPool__HandleVehicleRespawn: %X", FUNC_CPlayerPool__HandleVehicleRespawn);

	logprintf("FUNC_ProcessQueryPacket: %X", FUNC_ProcessQueryPacket);
	logprintf("FUNC_Packet_WeaponsUpdate: %X", FUNC_Packet_WeaponsUpdate);
	logprintf("FUNC_Packet_StatsUpdate: %X", FUNC_Packet_StatsUpdate);

	logprintf("ADDR_GetNetworkStats_VerbosityLevel: %X", ADDR_GetNetworkStats_VerbosityLevel);
	logprintf("ADDR_GetPlayerNetworkStats_VerbosityLevel: %X", ADDR_GetPlayerNetworkStats_VerbosityLevel);

	logprintf("FUNC_format_amxstring: %X", FUNC_format_amxstring);
	*/
	// Unlock restart wait time
	if (VAR_pRestartWaitTime)
		Unlock((void*)VAR_pRestartWaitTime, 4);
	for(BYTE i = 0; i < 4; i++)
	{
		if(VAR_pPosSyncBounds[i])
		{
			Unlock((void*)VAR_pPosSyncBounds[i], 4);
		}
	}

	// Patch GetNetworkStats to get more advanced stats than default
	if(ADDR_GetNetworkStats_VerbosityLevel)
	{
#ifdef __WIN32
		Unlock((void*)ADDR_GetNetworkStats_VerbosityLevel, 2);
#else
		Unlock((void*)ADDR_GetNetworkStats_VerbosityLevel, 5);
#endif
	}

	// Patch GetPlayerNetworkStats to get more advanced stats than default
	if(ADDR_GetPlayerNetworkStats_VerbosityLevel)
	{
#ifdef __WIN32
		Unlock((void*)ADDR_GetPlayerNetworkStats_VerbosityLevel, 2);
#else
		Unlock((void*)ADDR_GetPlayerNetworkStats_VerbosityLevel, 5);
#endif
	}
}