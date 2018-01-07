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

// Just to keep compatibility with streamer... If I got used to something, I don't like to switch to new one.
namespace Utility
{
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	const char* GetWeaponName(BYTE weaponid);
	BYTE GetWeaponSlot(BYTE weaponid);
	int CFGLoad(char const * const name, char * const dest = 0, size_t dlen = 0);
};

float GetDistance3D(CVector *vecPosition, CVector *_vecPosition);
bool IsPlayerConnected(int playerid);
const char *GetPlayerName(int playerid, bool getForQuery = false);

bool Unlock(void *address, size_t len);

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask);
DWORD FindPattern(char *pattern, char *mask);
void InstallJump(unsigned long addr, void *func);

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