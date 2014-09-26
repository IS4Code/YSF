/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef UTILS_H
#define UTILS_H

// Linux
#ifndef _WIN32

int GetTickCount();
void TickCountLoad();

#endif

float GetDistance3D(CVector *vecPosition, CVector *_vecPosition);
bool IsPlayerConnected(int playerid);
const char* GetWeaponName(BYTE weaponid);
BYTE GetWeaponSlot(BYTE weaponid);

int CFGLoad(char const * const name, char * const dest, size_t dlen);

#define ABGR_RGBA(color) (((DWORD)color & 0xff) << 24) | (((DWORD)(color >> 8) & 0xff) << 16) | (((DWORD)(color >> 16) & 0xff) << 8) | (color >> 24) & 0xff
#define RGBA_ABGR(color) (((color >> 24) & 0x000000FF) | ((color >> 8) & 0x0000FF00) | ((color << 8) & 0x00FF0000) | ((color << 24) & 0xFF000000))

#define ABGR_ARGB(color) (((color >> 24) & 0x000000FF) << 24 | (((DWORD)(color) & 0xFF) << 16) | (((DWORD)(color >> 8) & 0xFF) << 8) | (((DWORD)(color >> 16) & 0xFF)))

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

#ifndef CHECK_PARAMS
#define CHECK_PARAMS(m,n)                                                                                           \
	do                                                                                                              \
	{                                                                                                               \
		if (params[0] != (m * 4))                                                                                   \
		{                                                                                                           \
			logprintf(PROJECT_NAME " Error: Incorrect parameter count on \"" n "\", %d != %d\n", m, params[0] / 4); \
			return 0;                                                                                               \
		}                                                                                                           \
	}                                                                                                               \
	while (0)
#endif

#endif