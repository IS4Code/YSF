/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Utils.h"

#include "CVector.h"
#include "CPlayerData.h"

#include <fstream>
#include <sdk/plugin.h>

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

bool IsPlayerConnected(int playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return false;

	return pPlayerData[playerid] != NULL && pNetGame->pPlayerPool->pPlayer != NULL;
}

const char* CUtils::GetWeaponName_(BYTE weaponid)
{
	switch (weaponid)
	{
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
	case 44:
		return "Nightvision";
	case 45:
		return "Infrared";
	case WEAPON_PARACHUTE:
		return "Parachute";
	case WEAPON_VEHICLE:
		return "Vehicle";
	case WEAPON_DROWN:
		return "Drowned";
	case WEAPON_COLLISION:
		return "Splat";
	}
	return "";
}

BYTE CUtils::GetWeaponSlot(BYTE weaponid)
{
	switch (weaponid)
	{
	case WEAPON_GOLFCLUB:
	case WEAPON_NITESTICK:
	case WEAPON_KNIFE:
	case WEAPON_BAT:
	case WEAPON_SHOVEL:
	case WEAPON_POOLSTICK:
	case WEAPON_KATANA:
	case WEAPON_CHAINSAW:
		return 1;

	case WEAPON_COLT45:
	case WEAPON_SILENCED:
	case WEAPON_DEAGLE:
		return 2;

	case WEAPON_SHOTGUN:
	case WEAPON_SAWEDOFF:
	case WEAPON_SHOTGSPA:
		return 3;

	case WEAPON_UZI:
	case WEAPON_MP5:
	case WEAPON_TEC9:
		return 4;

	case WEAPON_AK47:
	case WEAPON_M4:
		return 5;

	case WEAPON_RIFLE:
	case WEAPON_SNIPER:
		return 6;

	case WEAPON_ROCKETLAUNCHER:
	case WEAPON_HEATSEEKER:
	case WEAPON_FLAMETHROWER:
	case WEAPON_MINIGUN:
		return 7;

	case WEAPON_GRENADE:
	case WEAPON_TEARGAS:
	case WEAPON_MOLTOV:
	case WEAPON_SATCHEL:
		return 8;

	case WEAPON_SPRAYCAN:
	case WEAPON_FIREEXTINGUISHER:
	case WEAPON_CAMERA:
		return 9;

	case WEAPON_DILDO:
	case WEAPON_DILDO2:
	case WEAPON_VIBRATOR:
	case WEAPON_VIBRATOR2:
	case WEAPON_FLOWER:
	case WEAPON_CANE:
		return 10;

	case 44:
	case 45:
	case WEAPON_PARACHUTE:
		return 11;

	case WEAPON_BOMB:
		return 12;
	}
	return 0;
}

// Load an entry from server.cfg.
int CFGLoad(char const * const name, char * const dest, size_t dlen)
{
/*
	std::ifstream
		f("server.cfg");
	int
		ret = 0,
		len = strlen(name);
	if (f.is_open())
	{
		char
			line[256];
		f.clear();
		while (!f.eof())
		{
			f.getline(line, 256);
			if (f.fail())
			{
				goto CFGLoad_close;
			}
			// Does the line START with this text?  Anything other than the
			// first character fails.
			if (!strncmp(line, name, len) && line[len] <= ' ')
			{
				while (line[++len] <= ' ')
				{
					if (line[len] == '\0') goto CFGLoad_close;
				}
				// Skipped leading spaces, save the value.
				if (dest) strncpy(dest, line + len, dlen);
				ret = atoi(line + len);
				goto CFGLoad_close;
			}
		}
	CFGLoad_close:
		// Yes, I used a label!  I needed to escape from a double loop.
		f.close();
	}
	return ret;
	*/
	return 0;
}