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
 *  the Initial Developer. All Rights Reserved.
 *  
 *  Contributor(s):
 *  
 *  Peter Beverloo
 *  Marcus Bauer
 *  MaVe;
 *  Sammy91
 *  Incognito
 *  
 *  Special Thanks to:
 *  
 *  SA:MP Team past, present and future
 */

#include "main.h"
#include <fstream>
#include <iostream>

extern void *pConsole;
extern AddServerRule_t g_pCConsole__AddRule;
extern SetServerRule_t g_pCConsole__SetRule;
//extern RemoveServerRule_t g_pCConsole__RemoveRule;
extern ModifyFlag_t g_pCConsole__MFlag;
extern LoadFS_t  g_pCFilterscript__LoadFS_t;
extern UnLoadFS_t  g_pCFilterscript__UnLoadFS_t;

#define _CONSOLE ((FakeClass *)pConsole)


void AddServerRule(char * rule, char * value, int flags)
{
	// This call (and the others like it) will give a debug assertaion error however the call is safe
	#ifdef WIN32
		(_CONSOLE->*g_pCConsole__AddRule)(rule, flags, value, 0);
	#else
		g_pCConsole__AddRule(_CONSOLE, rule, flags, value, 0);
	#endif
}

void SetServerRule(char * rule, char * value)
{
	#ifdef WIN32
		(_CONSOLE->*g_pCConsole__SetRule)(rule, value);
	#else
		// Linux messes up lowercase
		// But not any more :D
		g_pCConsole__SetRule(_CONSOLE, rule, value);
	#endif
}
/*
void RemoveServerRule(char * rule)
{
	#ifdef WIN32
		(_CONSOLE->*g_pCConsole__RemoveRule)(rule);
	#else
		// Linux messes up lowercase
		// But not any more :D
		g_pCConsole__RemoveRule(_CONSOLE, rule);
	#endif
}
*/
void ModifyFlag(char * rule, int varflag)
{
	#ifdef WIN32
		(_CONSOLE->*g_pCConsole__MFlag)(rule, varflag);
	#else
		// Linux messes up lowercase
		// But not any more :D
		g_pCConsole__MFlag(_CONSOLE, rule, varflag);
	#endif
}

int LoadFilterscript(char * name)
{
	CSAMPFilterScriptPool *pFS = pNetGame->pFilterScriptPool;
	
	#ifdef WIN32
		return ((FakeClass *)pFS->*g_pCFilterscript__LoadFS_t)(name);
	#else
		g_pCFilterscript__LoadFS_t((FakeClass *)pFS, name);
	#endif
   return 1;
}

int UnLoadFilterscript(char * name)
{
	CSAMPFilterScriptPool *pFS = pNetGame->pFilterScriptPool;

	#ifdef WIN32
		return ((FakeClass *)pFS->*g_pCFilterscript__UnLoadFS_t)(name);
	#else
		return g_pCFilterscript__UnLoadFS_t((FakeClass *)pFS, name);
	#endif
}


bool IsPlayerConnected(int playerid)
{
	if(playerid >= MAX_PLAYERS)
		return false;

	return pPlayerData[playerid] != NULL;
}

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max)
{
  cell* dest = (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
  cell* start = dest;
  while (max--&&*source)
    *dest++=(cell)*source++;
  *dest = 0;
  return dest-start;
}

const char* GetWeaponName(BYTE weaponid)
{
	switch(weaponid) 
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
	  case WEAPON_NIGHTVISION: 
         return "Nightvision"; 
	  case WEAPON_INFRARED: 
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

BYTE GetWeaponSlot(BYTE weaponid)
{
	switch(weaponid)
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
		
	case WEAPON_NIGHTVISION: 
	case WEAPON_INFRARED: 
	case WEAPON_PARACHUTE: 
		return 11;

	case WEAPON_BOMB: 
		return 12;
	}
	return 0;
}
/*
char *strdel(char *string, size_t first, size_t len)
{
      char *pos0, *pos1;

      if (string)
      {
            if (first < strlen(string))
            {
                  for (pos0 = pos1 = string + first;
                        *pos1 && len;
                        ++pos1, --len)
                  {
                        ;
                  }
                  strcpy(pos0, pos1);
            }
      }
      return string;
}
*/
