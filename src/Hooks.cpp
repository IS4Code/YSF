
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
#include "Hooks.h"

#include "Addresses.h"
#include "RPCs.h"

#include "main.h"
#include "Utils.h"
#include "CCallbackManager.h"
#include "Scripting.h"
#include "Structs.h"
#include "CPlayerData.h"
#include "Functions.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	//#define VC_EXTRALEAN
	#include <Windows.h>
	#include <Psapi.h>
#else
	#include <stdio.h>
	#include <sys/mman.h>
	#include <limits.h>
	#include <string.h>
	#include <algorithm>
	#include <unistd.h>
	#include <cstdarg>
#endif

#include "subhook/subhook.h"
#include <sdk/plugin.h>

#ifndef PAGESIZE
	#define PAGESIZE (4096)
#endif

extern void *pAMXFunctions;

static SubHook Namecheck_hook;
static SubHook amx_Register_hook;
static SubHook GetPacketID_hook;
static SubHook logprintf_hook;
static SubHook query_hook;

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
/*
// Y_Less - fixes2
void AssemblySwap(char * addr, char * dat, int len)
{
	char
		swp;
	while (len--)
	{
		swp = addr[len];
		addr[len] = dat[len];
		dat[len] = swp;
	}
}

void AssemblyRedirect(void * from, void * to, char * ret)
{
	#ifdef LINUX
		size_t
			iPageSize = getpagesize(),
			iAddr = ((reinterpret_cast <uint32_t>(from) / iPageSize) * iPageSize),
			iCount = (5 / iPageSize) * iPageSize + iPageSize * 2;
		mprotect(reinterpret_cast <void*>(iAddr), iCount, PROT_READ | PROT_WRITE | PROT_EXEC);
		//mprotect(from, 5, PROT_READ | PROT_WRITE | PROT_EXEC);
	#else
		DWORD
			old;
		VirtualProtect(from, 5, PAGE_EXECUTE_READWRITE, &old);
	#endif
	*((unsigned char *)ret) = 0xE9;
	*((char **)(ret + 1)) = (char *)((char *)to - (char *)from) - 5;
	AssemblySwap((char *)from, ret, 5);
}
*/
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

///////////////////////////////////////////////////////////////
// Hooks //
///////////////////////////////////////////////////////////////

// Custom name check

bool HOOK_ContainsInvalidChars(char * szString)
{
	SubHook::ScopedRemove remove(&Namecheck_hook);

	return !pServer->IsValidNick(szString);
}

// amx_Register hook for redirect natives
bool g_bNativesHooked = false;  

int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	SubHook::ScopedRemove remove(&amx_Register_hook);

	if (!g_bNativesHooked && pServer)
	{
		int i = 0;
		while (nativelist[i].name)
		{
			//logprintf("native %s", nativelist[i].name);
			int x = 0;
			
			while (RedirecedtNatives[x].name)
			{
				//logprintf("asdasd %s", RedirecedtNatives[x].name);
				if (!strcmp(nativelist[i].name, RedirecedtNatives[x].name))
				{
					if (!g_bNativesHooked) g_bNativesHooked = true;
				
					//logprintf("native: %s, %s", nativelist[i].name, RedirecedtNatives[x].name);
					nativelist[i].func = RedirecedtNatives[x].func;
				}
				x++;
			}
			i++;
		}
	}

	return amx_Register(amx, nativelist, number);
}

// GetPacketID hook
BYTE GetPacketID(Packet *p)
{
	if (p == 0) return 255;

	if ((unsigned char)p->data[0] == 36) 
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else return (unsigned char)p->data[0];
}

bool IsPlayerUpdatePacket(unsigned char packetId)
{
	return (packetId == ID_PLAYER_SYNC || packetId == ID_VEHICLE_SYNC || packetId == ID_PASSENGER_SYNC || packetId == ID_SPECTATOR_SYNC);
}

static BYTE HOOK_GetPacketID(Packet *p)
{
	SubHook::ScopedRemove remove(&GetPacketID_hook);

	BYTE packetId = GetPacketID(p);
	WORD playerid = p->playerIndex;

	//logprintf("packetid: %d, playeird: %d", packetId, playerid);

	if (IsPlayerConnected(playerid))
	{
		// AFK
		if (IsPlayerUpdatePacket(packetId))
		{
			if (pNetGame->pPlayerPool->pPlayer[playerid]->byteState != 0 && pNetGame->pPlayerPool->pPlayer[playerid]->byteState != 7)
			{
				pPlayerData[playerid]->dwLastUpdateTick = GetTickCount();
				pPlayerData[playerid]->bEverUpdated = true;
			}
		}

		/* Doesn't work - tested :(
		if (packetId == ID_PLAYER_SYNC)
		{
			//logprintf("ID_PLAYER_SYNC");
			RakNet::BitStream bsData(p->data, p->length, false);
			CSyncData pSyncData;

			bsData.SetReadOffset(8);
			bsData.Read((char*)&pSyncData, sizeof(pSyncData));

			//logprintf("health: %d, weapon: %d, specialaction: %d", pSyncData.byteHealth, pSyncData.byteWeapon, pSyncData.byteSpecialAction);

			if (pSyncData.byteWeapon == 44 || pSyncData.byteWeapon == 45)
			{
				pSyncData.byteWeapon = 0;
				//logprintf("nightvision");
			}
		}
		*/

		// Stats and weapons update
		if (packetId == ID_STATS_UPDATE)
		{
			pServer->Packet_StatsUpdate(p);
			return 0xFF;
		}
		/*
		if (packetId == ID_WEAPONS_UPDATE)
		{
			pServer->Packet_WeaponsUpdate(p);
			return 0xFF;
		}
		*/
		if (packetId == ID_BULLET_SYNC)
		{
			RakNet::BitStream bsData(p->data, p->length, false);
			BULLET_SYNC_DATA pBulletSync;

			bsData.SetReadOffset(8);
			bsData.Read((char*)&pBulletSync, sizeof(pBulletSync));

			if (pBulletSync.vecCenterOfHit.fX < -20000.0 || pBulletSync.vecCenterOfHit.fX > 20000.0 ||
				pBulletSync.vecCenterOfHit.fY < -20000.0 || pBulletSync.vecCenterOfHit.fY > 20000.0 ||
				pBulletSync.vecCenterOfHit.fZ < -20000.0 || pBulletSync.vecCenterOfHit.fZ > 20000.0)
			{
				//logprintf("bullet crasher detected. id = %d", playerid);
				return 0xFF;
			}
		}
	}
	return packetId;
}

static void HOOK_logprintf(const char *msg, ...)
{
	SubHook::ScopedRemove remove(&logprintf_hook);

	char fmat[1024];
	va_list arguments;
	va_start(arguments, msg);
	vsnprintf(fmat, sizeof(fmat), msg, arguments);
	va_end(arguments);

	CCallbackManager::OnServerMessage(fmat);

	logprintf(fmat);
}

int HOOK_ProcessQueryPacket(unsigned int binaryAddress, unsigned short port, char *data, int length, unsigned int s)
{
	SubHook::ScopedRemove remove(&query_hook);

	//CCallbackManager::OnRemoteRCONLogin(binaryAddress, port, "asdad");

	logprintf("binaddr: %d, port: %d, len: %d, socket: %d", binaryAddress, port, length, s);
	/*
	int i = 0;
	while (data[i])
	{
		logprintf("char: %c", data[i]);
		i++;
	}
	*/

	return CSAMPFunctions::ProcessQueryPacket(binaryAddress, port, data, length, s);
}

void InstallPreHooks()
{
	if (pServer)
	{
		Namecheck_hook.Install((void *)CAddress::FUNC_ContainsInvalidChars, (void *)HOOK_ContainsInvalidChars);
		amx_Register_hook.Install((void*)*(DWORD*)((DWORD)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
		GetPacketID_hook.Install((void*)CAddress::FUNC_GetPacketID, (void*)HOOK_GetPacketID);
	}
	logprintf_hook.Install((void*)logprintf, (void*)HOOK_logprintf);	
	//query_hook.Install((void*)0x00492660, (void*)HOOK_ProcessQueryPacket);
}