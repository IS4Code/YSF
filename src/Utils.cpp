/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

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

bool IsPlayerConnected(int playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return false;

	return pPlayerData[playerid] != NULL && pNetGame->pPlayerPool->pPlayer != NULL;
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
int Utility::CFGLoad(char const * const name, char * const dest, size_t dlen)
{
	FILE * fileConfig = fopen("plugins/YSF.cfg", "r");
	//logprintf("2");

	int
		ret = 1,
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
		return pPlayerData[playerid]->strNameInQuery.c_str();
	}
	else
	{
		// Get the player name pointer from memory.
		return (MAX_PLAYER_NAME + 1) * playerid + (char*)pNetGame->pPlayerPool + 0x2693C;
	}
}

// Y_Less - original YSF
bool Unlock(void *address, size_t len)
{
	#ifdef _WIN32
		DWORD
			oldp;
		// Shut up the warnings :D
		return !!VirtualProtect(address, len, PAGE_EXECUTE_READWRITE, &oldp);
	#else
		size_t
			iPageSize = getpagesize(),
			iAddr = ((reinterpret_cast <uint32_t>(address) / iPageSize) * iPageSize);
		return !mprotect(reinterpret_cast <void*>(iAddr), len, PROT_READ | PROT_WRITE | PROT_EXEC);
	#endif
}

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask)
{
	for(; *mask; ++mask, ++data, ++pattern)
	{
		if(*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}

DWORD FindPattern(char *pattern, char *mask)
{
	DWORD i;
	DWORD size;
	DWORD address;
#ifdef _WIN32
	MODULEINFO info = { 0 };

	address = (DWORD)GetModuleHandle(NULL);
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &info, sizeof(MODULEINFO));
	size = (DWORD)info.SizeOfImage;
#else
	address = 0x804b480; // around the elf base
	size = 0x8128B80 - address;
#endif
	for(i = 0; i < size; ++i)
	{
		if(memory_compare((BYTE *)(address + i), (BYTE *)pattern, mask))
			return (DWORD)(address + i);
	}
	return 0;
}

void InstallJump(unsigned long addr, void *func)
{
#ifdef WIN32
	unsigned long dwOld;
	VirtualProtect((LPVOID)addr, 5, PAGE_EXECUTE_READWRITE, &dwOld);
#else
	int pagesize = sysconf(_SC_PAGE_SIZE);
	void *unpraddr = (void *)(((int)addr + pagesize - 1) & ~(pagesize - 1)) - pagesize;
	mprotect(unpraddr, pagesize, PROT_WRITE);
#endif
	*(unsigned char *)addr = 0xE9;
	*(unsigned long *)((unsigned long)addr + 0x1) = (unsigned long)func - (unsigned long)addr - 5;
#ifdef WIN32
	VirtualProtect((LPVOID)addr, 5, dwOld, &dwOld);
#else
	mprotect(unpraddr, pagesize, PROT_READ | PROT_EXEC);
#endif
}

// From raknet
BYTE GetPacketID(Packet *p)
{
	if (p == 0) return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else return (unsigned char)p->data[0];
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