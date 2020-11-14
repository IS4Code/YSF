#include <string>
#include <sstream>

#include "includes/platform.h"
#include "Utils.h"
#include "CServer.h"
#include "Globals.h"
#include "RPCs.h"

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

	return pNetGame->pPlayerPool->pPlayer[playerid] != NULL;
}

std::vector<std::string> &Utility::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) 
	{
		if (item.length() > 0) 
		{
			elems.push_back(item);
		}
	}
	return elems;
}


const char* Utility::GetWeaponName(BYTE weaponid)
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

BYTE Utility::GetWeaponSlot(BYTE weaponid)
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

// Load an entry from server.cfg - Y_Less
int Utility::CFGLoad(char const * const name, int default_val, char * const dest, size_t dlen)
{
	FILE * fileConfig = fopen("plugins/YSF.cfg", "r");
	//logprintf("2");

	int
		ret = default_val,
		len = strlen(name);
	//logprintf("3");

	if (fileConfig)
	{
		//logprintf("4");

		char line[256];
		while (!feof(fileConfig))
		{
			//logprintf("5");

			if (!fgets(line, 256, fileConfig))
			{
				goto CFGLoad_close;
			}
			//logprintf("6");

			// Does the line START with this text?  Anything other than the
			// first character fails.
			if (!strncmp(line, name, len) && line[len] <= ' ')
			{
				//logprintf("7");

				while (line[++len] <= ' ')
				{
					if (line[len] == '\0') goto CFGLoad_close;
				}
				//logprintf("8");

				// Skipped leading spaces, save the value.
				if (dest) strncpy(dest, line + len, dlen);
				ret = atoi(line + len);
				goto CFGLoad_close;
			}
		}
	CFGLoad_close:
		//logprintf("9");

		// Yes, I used a label!  I needed to escape from a double loop.
		fclose(fileConfig);
		//logprintf("10");

	}
	//logprintf("11");

	return ret;
}

const char *GetPlayerName(int playerid, bool getForQuery)
{
	if (!IsPlayerConnected(playerid)) return NULL;

	if (getForQuery)
	{
		auto &data = CServer::Get()->PlayerPool.Extra(playerid);
		if (data.bCustomNameInQuery)
		{
			return data.strNameInQuery.c_str();
		}
	}
	// Get the player name pointer from memory.
	return pNetGame->pPlayerPool->szName[playerid];
}

// From raknet
BYTE GetPacketID(Packet *p)
{
	if (!p || !p->data || p->length == 0) return 0xFF;

	if (p->data[0] == ID_TIMESTAMP)
	{
		auto ofs = sizeof(unsigned char) + sizeof(unsigned long);
		if(p->length <= ofs) return 0xFF;
		return p->data[ofs];
	}
	else return p->data[0];
}

bool IsPlayerUpdatePacket(BYTE packetId)
{
	switch (packetId)
	{
		case ID_PLAYER_SYNC:
		case ID_VEHICLE_SYNC:
		case ID_PASSENGER_SYNC:
		case ID_SPECTATOR_SYNC:
		case ID_AIM_SYNC:
		case ID_TRAILER_SYNC:
			return true;
		default:
			return false;
	}
	return false;
}