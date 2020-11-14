#ifndef YSF_UTILS_H
#define YSF_UTILS_H

#include <vector>
#include <cstddef>
#include <algorithm>
#include <string>

#include "raknet/NetworkTypes.h"
#include "includes/types.h"

#include "CVector.h"

#define ARRAY_SIZE(a)	( sizeof((a)) / sizeof(*(a)) )
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }
#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p) = NULL; } }
#define PAD(a, b) char a[b]

#define _OFFSET(p_type, p_member) (size_t)(&((p_type *)NULL)->p_member)

#ifndef PAGESIZE
#define PAGESIZE (4096)
#endif

// Just to keep compatibility with streamer... If I got used to something, I don't like to switch to new one.
namespace Utility
{
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	const char* GetWeaponName(BYTE weaponid);
	BYTE GetWeaponSlot(BYTE weaponid);
	int CFGLoad(char const * const name, int default_val = 1, char * const dest = 0, size_t dlen = 0);
};

float GetDistance3D(CVector *vecPosition, CVector *_vecPosition);
bool IsPlayerConnected(int playerid);
const char *GetPlayerName(int playerid, bool getForQuery = false);

BYTE GetPacketID(Packet *p);
bool IsPlayerUpdatePacket(BYTE packetId);

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

#endif