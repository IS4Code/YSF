/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef YSF_UTILS_H
#define YSF_UTILS_H

#include "CVector.h"
#include <vector>
#include <cstddef>
#include <algorithm>
#include <string>

// Linux
#ifndef _WIN32

#undef GetTickCount
int GetTickCount();
void LoadTickCount();

#endif
namespace CUtils
{
	const char* GetWeaponName_(BYTE weaponid);
	BYTE GetWeaponSlot(BYTE weaponid);
};

float GetDistance3D(CVector *vecPosition, CVector *_vecPosition);
bool IsPlayerConnectedEx(int playerid);
char *GetPlayerName_(int playerid);
std::string GetServerCfgOption(const std::string &option);

bool Unlock(void *address, size_t len);
//void AssemblySwap(char * addr, char * dat, int len);
//void AssemblyRedirect(void * from, void * to, char * ret);

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask);
DWORD FindPattern(char *pattern, char *mask);

bool IsPlayerUpdatePacket(unsigned char packetId);
#define ABGR_RGBA(color) (((DWORD)color & 0xff) << 24) | (((DWORD)(color >> 8) & 0xff) << 16) | (((DWORD)(color >> 16) & 0xff) << 8) | (color >> 24) & 0xff
#define RGBA_ABGR(color) (((color >> 24) & 0x000000FF) | ((color >> 8) & 0x0000FF00) | ((color << 8) & 0x00FF0000) | ((color << 24) & 0xFF000000))

#define ABGR_ARGB(color) (((color >> 24) & 0x000000FF) << 24 | (((DWORD)(color) & 0xFF) << 16) | (((DWORD)(color >> 8) & 0xFF) << 8) | (((DWORD)(color >> 16) & 0xFF)))

#define GAMESTATE_STOPPED	 0
#define GAMESTATE_RUNNING	 1
#define GAMESTATE_RESTARTING 2

enum
{
	BS_BOOL,
	BS_CHAR,
	BS_UNSIGNEDCHAR,
	BS_SHORT,
	BS_UNSIGNEDSHORT,
	BS_INT,
	BS_UNSIGNEDINT,
	BS_FLOAT,
	BS_STRING
};

template <typename T>
const bool Contains(std::vector<T>& Vec, const T& Element)
{
	if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
		return true;

	return false;
}
#endif