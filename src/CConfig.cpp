#include <cstdio>
#include "Utils.h"
#include "CConfig.h"

CConfig::CConfig()
{
	// Avoid errors when somebody forgotten to copy YSF.cfg to plugins directory
	FILE* fileConfig = fopen("plugins/YSF.cfg", "r");
	if (!fileConfig)
	{
		fileConfig = fopen("plugins/YSF.cfg", "w");

		fprintf(fileConfig, "# Protection against fake pickup ids\n");
		fprintf(fileConfig, "PickupProtection 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Protection against fakekill\n");
		fprintf(fileConfig, "DeathProtection 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Protection against sproofed dialog ids\n");
		fprintf(fileConfig, "DialogProtection 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Use redirected YSF's own RPC for spawning\n");
		fprintf(fileConfig, "UseCustomSpawn 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Set it to 1 when you want to use per player gangzones\n");
		fprintf(fileConfig, "UsePerPlayerGangZones 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Allowing remote RCON connections with banned IPs (its very good to enable when you need to unban yourself)\n");
		fprintf(fileConfig, "AllowRemoteRCONWithBannedIPs 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Use this if you want to use SetMaxPlayers to increase your server slots at runtime\n");
		fprintf(fileConfig, "# DANGER: With enabling this option server will allow to connect 1000 players, doesn't matter what is your \"maxplayers\" value in server.cfg!\n");
		fprintf(fileConfig, "IncreaseRakNetInternalPlayers 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# If the option above isn't enabled this option won't have any effect\n");
		fprintf(fileConfig, "# Change raknet internal threads sleeping time, lowering the value migh result in smoother sync - by default is 5ms\n");
		fprintf(fileConfig, "RakNetInternalSleepTime 5\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Delay im ms - object will be attached to player after this delay passed, lowering this delay might result in crashes \n");
		fprintf(fileConfig, "AttachObjectDelay 2000\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# SA-MP by default doesn't store material info for per-player objects, which made GetPlayerObjectMaterial/MaterialText broken\n");
		fprintf(fileConfig, "# If you just use streamer for objects and you don't wanna use those two natives below, then disable this option\n");
		fprintf(fileConfig, "StorePlayerObjectsMaterial 1\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# With this option you can load YSF on whatever server version, but it can result unwanted behavior\n");
		fprintf(fileConfig, "SkipVersionCheck 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# This option makes newly created player objects prioritize IDs used by other existing player objects.\n");
		fprintf(fileConfig, "GroupPlayerObjects 0\n");

		fclose(fileConfig);
	}

	m_bPickupProtection = static_cast<int>(Utility::CFGLoad("PickupProtection") != 0);
	m_bDeathProtection = static_cast<int>(Utility::CFGLoad("DeathProtection") != 0);
	m_bDialogProtection = static_cast<int>(Utility::CFGLoad("DialogProtection") != 0);
	m_bUseCustomSpawn = static_cast<int>(Utility::CFGLoad("UseCustomSpawn") != 0);
	m_bUsePerPlayerGangZones = static_cast<int>(Utility::CFGLoad("UsePerPlayerGangZones") != 0);
	m_bAllowRemoteRCONWithBannedIPs = static_cast<int>(Utility::CFGLoad("AllowRemoteRCONWithBannedIPs") != 0);
	m_bIncreaseRakNetInternalPlayers = static_cast<int>(Utility::CFGLoad("IncreaseRakNetInternalPlayers") != 0);
	m_iRakNetInternalSleepTime = Utility::CFGLoad("RakNetInternalSleepTime");
	m_iAttachObjectDelay = Utility::CFGLoad("AttachObjectDelay");
	m_bStorePlayerObjectsMaterial = static_cast<int>(Utility::CFGLoad("StorePlayerObjectsMaterial") != 0);
	m_bGroupPlayerObjects = static_cast<int>(Utility::CFGLoad("GroupPlayerObjects") != 0);

	//logprintf("%d, %d, %d, %d", m_bPickupProtection, m_bDeathProtection, m_iRakNetInternalSleepTime, m_iAttachObjectDelay);
}