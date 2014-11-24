
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
			pPlayerData[playerid]->dwLastUpdateTick = GetTickCount();
			pPlayerData[playerid]->bEverUpdated = true;
			/*
			if (packetId == ID_VEHICLE_SYNC)
			{
				RakNet::BitStream bsData(p->data, p->length, false);
				CVehicleSyncData pVehicleSync;

				bsData.SetReadOffset(8);
				bsData.Read((char*)&pVehicleSync, sizeof(pVehicleSync));
				logprintf("keys sync: %d", pVehicleSync.wKeys);
				
				pVehicleSync.wKeys = 0;
				pVehicleSync.wUDAnalog = 0;
				pVehicleSync.wLRAnalog = 0;
			
			}
			*/
			/*
			// Based on JernejL's tutorial - http://forum.sa-mp.com/showthread.php?t=172085
			DWORD dwDrunkNew = pPlayerPool->dwDrunkLevel[playerid];
			logprintf("drunknew: %d", dwDrunkNew);
			if (dwDrunkNew < 100)
			{
			
				RakNet::BitStream bs;
				bs.Write(2000);

				int drunk = 0x23;
				pRakServer->RPC(&drunk, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 2, pRakServer->GetPlayerIDFromIndex(playerid), 0, 0);
			}
			else
			{
				logprintf("elsegeci: %d", pPlayerData[playerid]->dwLastDrunkLevel != dwDrunkNew);
				if (pPlayerData[playerid]->dwLastDrunkLevel != dwDrunkNew)
				{
					DWORD fps = pPlayerData[playerid]->dwLastDrunkLevel - dwDrunkNew;
					logprintf("fps: %d", dwDrunkNew);
					if (fps > 0 && fps < 300)
					{
						pPlayerData[playerid]->dwFPS = fps;
					}

					pPlayerData[playerid]->dwLastDrunkLevel = dwDrunkNew;
				}
			}
			*/
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
		
		if (packetId == ID_WEAPONS_UPDATE)
		{
			CSAMPFunctions::Packet_WeaponsUpdate(p);
			CCallbackManager::OnPlayerStatsAndWeaponsUpdate(playerid);
			return 0xFF;
		}
		
		// Bullet crasher fix
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

//----------------------------------------------------

BOOL	bRconSocketReply = FALSE;

SOCKET	cur_sock = INVALID_SOCKET;
char*	cur_data = NULL;
int		cur_datalen = 0;
sockaddr_in to;

//----------------------------------------------------

void RconSocketReply(char* szMessage);

//----------------------------------------------------

static void HOOK_logprintf(const char *msg, ...)
{
	SubHook::ScopedRemove remove(&logprintf_hook);

	char buffer[1024];
	va_list arguments;
	va_start(arguments, msg);
	vsnprintf(buffer, sizeof(buffer), msg, arguments);
	va_end(arguments);

	CCallbackManager::OnServerMessage(buffer);

	logprintf(buffer);
	if (bRconSocketReply) 
	{
		RconSocketReply(buffer);
	}
}



//----------------------------------------------------
// int CheckQueryFlood()
// returns 1 if this query could flood
// returns 0 otherwise

DWORD dwLastQueryTick = 0;
unsigned int lastBinAddr = 0;

int CheckQueryFlood(unsigned int binaryAddress)
{
	if (!dwLastQueryTick) {
		dwLastQueryTick = (DWORD)GetTickCount();
		lastBinAddr = binaryAddress;
		return 0;
	}
	if (lastBinAddr == binaryAddress) {
		return 0;
	}
	if ((GetTickCount() - dwLastQueryTick) < 25) {
		return 1;
	}
	dwLastQueryTick = GetTickCount();
	lastBinAddr = binaryAddress;
	return 0;
}

//----------------------------------------------------

int HOOK_ProcessQueryPacket(unsigned int binaryAddress, unsigned short port, char* data, int length, SOCKET s)
{
	char* szPassword = NULL;
	WORD wStrLen = 0;
	in_addr in;
	in.s_addr = binaryAddress;

	if ((length > 4) && (*(unsigned long*)(data) == 0x504D4153)) // 0x504D4153 = "SAMP" as hex.
	{
		if (!pNetGame || (pNetGame->iGameState != GAMESTATE_RUNNING)) return 0;

		if (length >= 11)
		{
			to.sin_family = AF_INET;
			to.sin_port = htons(port);
			to.sin_addr.s_addr = binaryAddress;

			switch (data[10])
			{
				case 'p':	// ping
				{
					if (length == 15)
					{
						sendto(s, data, 15, 0, (sockaddr*)&to, sizeof(to));
					}
					break;
				}
				case 'i':	// info
				{
					// We do not process these queries 'query' is 0
					if (!pServer->GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					char* szHostname = pServer->GetStringVariable("hostname");
					DWORD dwHostnameLen = strlen(szHostname);
					if (dwHostnameLen > 50) dwHostnameLen = 50;

					char* szGameMode = pServer->GetStringVariable("gamemodetext");
					DWORD dwGameModeLen = strlen(szGameMode);
					if (dwGameModeLen > 30) dwGameModeLen = 30;

					char* szMapName = pServer->GetStringVariable("mapname");
					DWORD dwMapNameLen = strlen(szMapName);
					if (dwMapNameLen > 30) dwMapNameLen = 30;

					WORD wPlayerCount = pServer->GetPlayerCount();
					CPlayerPool* pPlayerPool = pNetGame->pPlayerPool;

					WORD wMaxPlayers = pServer->GetMaxPlayers();

					BYTE byteIsPassworded = pServer->GetStringVariable("password")[0] != 0;

					DWORD datalen = 28;	// Previous data = 11b
					// IsPassworded = 1b
					// Player count = 2b
					// Max player count = 2b
					// String-length bytes = 12b (3 * sizeof(DWORD))
					datalen += dwHostnameLen;
					datalen += dwGameModeLen;
					datalen += dwMapNameLen;


					char* newdata = (char*)malloc(datalen);
					char* keep_ptr = newdata;
					// Previous Data
					memcpy(newdata, data, 11);
					newdata += 11;

					// IsPassworded
					memcpy(newdata, &byteIsPassworded, sizeof(BYTE));
					newdata += sizeof(BYTE);

					// Player Count
					memcpy(newdata, &wPlayerCount, sizeof(WORD));
					newdata += sizeof(WORD);

					// Max Players
					memcpy(newdata, &wMaxPlayers, sizeof(WORD));
					newdata += sizeof(WORD);

					// Hostname
					memcpy(newdata, &dwHostnameLen, sizeof(DWORD));
					newdata += sizeof(DWORD);
					memcpy(newdata, szHostname, dwHostnameLen);
					newdata += dwHostnameLen;

					// Game Mode
					memcpy(newdata, &dwGameModeLen, sizeof(DWORD));
					newdata += sizeof(DWORD);
					memcpy(newdata, szGameMode, dwGameModeLen);
					newdata += dwGameModeLen;

					// Map Name
					memcpy(newdata, &dwMapNameLen, sizeof(DWORD));
					newdata += sizeof(DWORD);
					memcpy(newdata, szMapName, dwMapNameLen);
					newdata += dwMapNameLen;

					sendto(s, keep_ptr, datalen, 0, (sockaddr*)&to, sizeof(to));
					free(keep_ptr);
					break;
				}
				case 'c':	// players
				{
					// We do not process these queries 'query' is 0
					if (!pServer->GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					WORD wPlayerCount = pServer->GetPlayerCount();
					CPlayerPool* pPlayerPool = pNetGame->pPlayerPool;
					char* newdata = (char*)malloc(13 + (wPlayerCount * (MAX_PLAYER_NAME + 5))); // 5 = 1b String len, and 4b Score
					char* keep_ptr = newdata;
					// Previous Data
					memcpy(newdata, data, 11);
					newdata += 11;

					// Player Count
					memcpy(newdata, &wPlayerCount, sizeof(WORD));
					newdata += sizeof(WORD);

					if (pPlayerPool) 
					{
						char* szName;
						BYTE byteNameLen;
						DWORD dwScore;

						for (WORD r = 0; r<MAX_PLAYERS; r++)
						{
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->bHidden)
							{
								szName = GetPlayerName(r);
								byteNameLen = (BYTE)strlen(szName);
								memcpy(newdata, &byteNameLen, sizeof(BYTE));
								newdata += sizeof(BYTE);
								memcpy(newdata, szName, byteNameLen);
								newdata += byteNameLen;
								dwScore = pPlayerPool->dwScore[r];
								memcpy(newdata, &dwScore, sizeof(DWORD));
								newdata += sizeof(DWORD);

							}
						}
					}

					sendto(s, keep_ptr, (int)(newdata - keep_ptr), 0, (sockaddr*)&to, sizeof(to));
					free(keep_ptr);
					break;
				}
				case 'd':	// detailed player list id.namelength.name.score.ping
				{
					// We do not process these queries 'query' is 0
					if (!pServer->GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					WORD wPlayerCount = pServer->GetPlayerCount();
					CPlayerPool* pPlayerPool = pNetGame->pPlayerPool;
					char* newdata = (char*)malloc(13 + (wPlayerCount * (MAX_PLAYER_NAME + 10))); // 9 = 1b String len, 4b Score, 4b Ping, 1b Playerid
					char* keep_ptr = newdata;
					// Previous Data
					memcpy(newdata, data, 11);
					newdata += 11;

					// Player Count
					memcpy(newdata, &wPlayerCount, sizeof(WORD));
					newdata += sizeof(WORD);

					if (pPlayerPool) 
					{
						char* szName;
						BYTE byteNameLen;
						DWORD dwScore, dwPing;

						for (WORD r = 0; r<MAX_PLAYERS; r++)
						{
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->bHidden)
							{
								memcpy(newdata, &r, sizeof(BYTE));
								newdata += sizeof(BYTE);
								szName = GetPlayerName(r);
								byteNameLen = (BYTE)strlen(szName);
								memcpy(newdata, &byteNameLen, sizeof(BYTE));
								newdata += sizeof(BYTE);
								memcpy(newdata, szName, byteNameLen);
								newdata += byteNameLen;
								dwScore = pPlayerPool->dwScore[r];
								memcpy(newdata, &dwScore, sizeof(DWORD));
								newdata += sizeof(DWORD);
								dwPing = pRakServer->GetLastPing(pRakServer->GetPlayerIDFromIndex(r));
								memcpy(newdata, &dwPing, sizeof(DWORD));
								newdata += sizeof(DWORD);
							}
						}
					}
					sendto(s, keep_ptr, (int)(newdata - keep_ptr), 0, (sockaddr*)&to, sizeof(to));
					free(keep_ptr);
					break;
				}

				case 'r':	// rules
				{
					// We do not process these queries 'query' is 0
					if (!pServer->GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					CSAMPFunctions::SendRules(s, data, (sockaddr_in*)&to, sizeof(to));
					break;
				}
				case 'x':	// rcon
				{
					if (pRakServer && pRakServer->IsBanned(inet_ntoa(in))) return 1;

					// We do not process these queries 'query' is 0
					if (!pServer->GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					cur_sock = s;
					cur_data = data;
					cur_datalen = 11; // How much of the message we send back.
					int tmp_datalen = cur_datalen;

					data += cur_datalen;

					// Check there's enough data for another WORD
					tmp_datalen += sizeof(WORD);
					if (length < tmp_datalen)
						goto cleanup;	// Malformed packet! Haxy bastard!
					// Read the string length for the password
					wStrLen = *(WORD*)data;
					data += sizeof(WORD);
					// Check there's enough data for the password string
					tmp_datalen += wStrLen;
					if (length < tmp_datalen)
						goto cleanup;	// Malformed packet! Haxy bastard!
					// Read the password string
					szPassword = (char*)malloc(wStrLen + 1);
					memcpy(szPassword, data, wStrLen);
					szPassword[wStrLen] = 0;
					data += wStrLen;

					if (strcmp(szPassword, pServer->GetStringVariable("rcon_password")) == 0)
					{
						// Check there's enough data for another WORD
						tmp_datalen += sizeof(WORD);
						if (length < tmp_datalen)
						{
							free(szPassword);
							goto cleanup;	// Malformed packet! Haxy bastard!
						}
						// Read the string length for the command
						wStrLen = *(WORD*)data;
						data += sizeof(WORD);
						tmp_datalen += wStrLen;
						if (length < tmp_datalen)
						{
							free(szPassword);
							goto cleanup;	// Malformed packet! Haxy bastard!
						}

						// Read the command string
						char* szCommand = (char*)malloc(wStrLen + 1);
						memcpy(szCommand, data, wStrLen);
						szCommand[wStrLen] = 0;

						if (pConsole)
						{
							if (CCallbackManager::OnRemoteRCONPacket(binaryAddress, port, szPassword, szCommand))
							{ 
								bRconSocketReply = TRUE;
								// Execute the command
								CSAMPFunctions::Execute(szCommand);
								bRconSocketReply = FALSE;
							}
						}

						free(szCommand);
					}
					else 
					{
						in_addr in;
						in.s_addr = binaryAddress;
						logprintf("BAD RCON ATTEMPT BY: %s", inet_ntoa(in));

						bRconSocketReply = TRUE;
						RconSocketReply("Invalid RCON password.");
						bRconSocketReply = FALSE;

						CCallbackManager::OnRemoteRCONPacket(binaryAddress, port, szPassword, NULL);
					}
					free(szPassword);

				cleanup:
					cur_datalen = 0;
					cur_data = NULL;
					cur_sock = INVALID_SOCKET;
					break;
				}
			}
		}
		return 1;
	}
	else 
	{
		return 0;
	}
}

//----------------------------------------------------

void RconSocketReply(char* szMessage)
{
	// IMPORTANT!
	// Don't use logprintf from here... You'll cause an infinite loop.
	if (bRconSocketReply)
	{
		char* newdata = (char*)malloc(cur_datalen + strlen(szMessage) + sizeof(WORD));
		char* keep_ptr = newdata;
		memcpy(newdata, cur_data, cur_datalen);
		newdata += cur_datalen;
		*(WORD*)newdata = (WORD)strlen(szMessage);
		newdata += sizeof(WORD);
		memcpy(newdata, szMessage, strlen(szMessage));
		newdata += strlen(szMessage);
		sendto(cur_sock, keep_ptr, (int)(newdata - keep_ptr), 0, (sockaddr*)&to, sizeof(to));
		free(keep_ptr);
	}
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
	query_hook.Install((void*)CAddress::FUNC_ProcessQueryPacket, (void*)HOOK_ProcessQueryPacket);
}