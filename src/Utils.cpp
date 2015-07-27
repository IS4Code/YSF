/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "main.h"

// Linux GetTickCount
#ifndef _WIN32

#include "sys/time.h"
timeval startTime;
timeval currentTime;

void LoadTickCount()
{
	// Get the starting time
	gettimeofday(&startTime, 0);
}

int GetTickCount()
{
	// Get the time elapsed since the start
	gettimeofday(&currentTime, 0);
	return (currentTime.tv_usec - startTime.tv_usec) / 1000 + 1000 * (currentTime.tv_sec - startTime.tv_sec);
}

#endif

float GetDistance3D(CVector *vecPosition, CVector *_vecPosition)
{
	// Get the distance between the two vectors
	float fSX = (_vecPosition->fX - vecPosition->fX) * (_vecPosition->fX - vecPosition->fX);
	float fSY = (_vecPosition->fY - vecPosition->fY) * (_vecPosition->fY - vecPosition->fY);
	float fSZ = (_vecPosition->fZ - vecPosition->fZ) * (_vecPosition->fZ - vecPosition->fZ);
	return ((float)sqrt(fSX + fSY + fSZ));	
}

bool IsPlayerConnectedEx(int playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return false;

	return pPlayerData[playerid] != NULL && pNetGame->pPlayerPool->pPlayer != NULL;
}

const char* CUtils::GetWeaponName_(BYTE weaponid)
{
	switch (weaponid)
	{
	case 0:
		return "Fists";
	case WEAPON_BRASSKNUCKLE:
		return "Brass Knuckles";
	case WEAPON_GOLFCLUB:
		return "Golf Club";
	case WEAPON_NITESTICK:
		return "Nite Stick";
	case WEAPON_KNIFE:
		return "Knife";
	case WEAPON_BAT:
		return "Baseball Bat";
	case WEAPON_SHOVEL:
		return "Shovel";
	case WEAPON_POOLSTICK:
		return "Pool Cue";
	case WEAPON_KATANA:
		return "Katana";
	case WEAPON_CHAINSAW:
		return "Chainsaw";
	case WEAPON_DILDO:
		return "Dildo";
	case WEAPON_DILDO2:
		return "Dildo";
	case WEAPON_VIBRATOR:
		return "Vibrator";
	case WEAPON_VIBRATOR2:
		return "Vibrator";
	case WEAPON_FLOWER:
		return "Flowers";
	case WEAPON_CANE:
		return "Cane";
	case WEAPON_GRENADE:
		return "Grenade";
	case WEAPON_TEARGAS:
		return "Teargas";
	case WEAPON_MOLTOV:
		return "Molotov";
	case 19:
		return "Vehicle Missile";
	case 20:
		return "Hydra Flare";
	case 21:
		return "Jetpack";
	case WEAPON_COLT45:
		return "Colt 45";
	case WEAPON_SILENCED:
		return "Silenced Pistol";
	case WEAPON_DEAGLE:
		return "Desert Eagle";
	case WEAPON_SHOTGUN:
		return "Shotgun";
	case WEAPON_SAWEDOFF:
		return "Sawn-off Shotgun";
	case WEAPON_SHOTGSPA: // wtf? 
		return "Combat Shotgun";
	case WEAPON_UZI:
		return "UZI";
	case WEAPON_MP5:
		return "MP5";
	case WEAPON_AK47:
		return "AK47";
	case WEAPON_M4:
		return "M4";
	case WEAPON_TEC9:
		return "TEC9";
	case WEAPON_RIFLE:
		return "Rifle";
	case WEAPON_SNIPER:
		return "Sniper Rifle";
	case WEAPON_ROCKETLAUNCHER:
		return "Rocket Launcher";
	case WEAPON_HEATSEEKER:
		return "Heat Seaker";
	case WEAPON_FLAMETHROWER:
		return "Flamethrower";
	case WEAPON_MINIGUN:
		return "Minigun";
	case WEAPON_SATCHEL:
		return "Satchel Explosives";
	case WEAPON_BOMB:
		return "Bomb";
	case WEAPON_SPRAYCAN:
		return "Spray Can";
	case WEAPON_FIREEXTINGUISHER:
		return "Fire Extinguisher";
	case WEAPON_CAMERA:
		return "Camera";
	case WEAPON_NIGHTVISION:
		return "Nightvision";
	case WEAPON_INFRARED:
		return "Infrared";
	case WEAPON_PARACHUTE:
		return "Parachute";
	case 47:
		return "Fake Pistol";
	case WEAPON_VEHICLE:
		return "Vehicle";
	case 50:
		return "Helicopter Blades";
	case 51:
		return "Explosion";
	case WEAPON_DROWN:
		return "Drowned";
	case WEAPON_COLLISION:
		return "Splat";
	}
	return "";
}

BYTE CUtils::GetWeaponSlot(BYTE weaponid)
{
	BYTE result; // eax@2

	switch (weaponid)
	{
	case 0:
	case 1:
		result = 0;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		result = 1;
		break;
	case 22:
	case 23:
	case 24:
		result = 2;
		break;
	case 25:
	case 26:
	case 27:
		result = 3;
		break;
	case 28:
	case 29:
	case 32:
		result = 4;
		break;
	case 30:
	case 31:
		result = 5;
		break;
	case 33:
	case 34:
		result = 6;
		break;
	case 35:
	case 36:
	case 37:
	case 38:
		result = 7;
		break;
	case 16:
	case 17:
	case 18:
	case 39:
		result = 8;
		break;
	case 41:
	case 42:
	case 43:
		result = 9;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		result = 10;
		break;
	case 44:
	case 45:
	case 46:
		result = 11;
		break;
	case 40:
		result = 12;
		break;
	default:
		result = 0xFF;
		break;
	}
	return result;
}

char *GetPlayerName_(int playerid)
{
	if (!IsPlayerConnectedEx(playerid)) return NULL;

	// Get the player name pointer from memory.
	return 25 * playerid + (char*)pNetGame->pPlayerPool + 0x2693C;
}

// Created by: https://github.com/Zeex/sampgdk/blob/master/plugins/unlimitedfs/unlimitedfs.cpp
std::string GetServerCfgOption(const std::string &option)
{
	std::string name, value;
	std::string line;
	std::fstream server_cfg("server.cfg");
	if (server_cfg)
	{
		while (std::getline(server_cfg, line))
		{
			std::stringstream ss(line);
			ss >> name;
			if (name == option)
			{
				ss >> value;
				break;
			}
		}
	}
	return "0";
}