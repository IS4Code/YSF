
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

#ifndef PAGESIZE
	#define PAGESIZE (4096)
#endif

extern void *pAMXFunctions;

static SubHook Namecheck_hook;
static SubHook amx_Register_hook;
static SubHook GetPacketID_hook;
static SubHook logprintf_hook;
static SubHook query_hook;
static SubHook CVehicle__Respawn_hook;

// RakServer::Send hook - Thanks to Gamer_Z
#ifdef _WIN32
class CHookRakServer
{
public:
	static bool __thiscall Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	static bool __thiscall RPC(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
};
#else
class CHookRakServer
{
public:
	static bool Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	static bool RPC(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
};
#endif

#ifdef _WIN32
typedef bool (__thiscall *RakNet__Send_t)(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
typedef bool (__thiscall *RakNet__RPC_t)(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
#else
typedef bool (*RakNet__Send_t)(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
typedef bool (*RakNet__RPC_t)(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
#endif

RakNet__Send_t RakNetOriginalSend;
RakNet__RPC_t RakNetOriginalRPC;

AMX_NATIVE pDestroyPlayerObject = NULL, pTogglePlayerControllable = NULL, pSetPlayerWorldBounds = NULL, pSetPlayerTeam = NULL, pSetPlayerSkin = NULL, pSetPlayerFightingStyle = NULL, pSetPlayerName = NULL, pSetVehicleToRespawn = NULL, pChangeVehicleColor = NULL, pDestroyVehicle = NULL;

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
///////////////////////////////////////////////////////////////
// Hooks //
///////////////////////////////////////////////////////////////

//----------------------------------------------------
// Custom name check

bool HOOK_ContainsInvalidChars(char * szString)
{
	SubHook::ScopedRemove remove(&Namecheck_hook);

	return !pServer->IsValidNick(szString);
}

//----------------------------------------------------

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
			if(!pDestroyPlayerObject && !strcmp(nativelist[i].name, "DestroyPlayerObject"))
				pDestroyPlayerObject = nativelist[i].func;

			if(!pTogglePlayerControllable && !strcmp(nativelist[i].name, "TogglePlayerControllable"))
				pTogglePlayerControllable = nativelist[i].func;

			if(!pSetPlayerWorldBounds && !strcmp(nativelist[i].name, "SetPlayerWorldBounds"))
				pSetPlayerWorldBounds = nativelist[i].func;
			
			if(!pSetPlayerTeam && !strcmp(nativelist[i].name, "SetPlayerTeam"))
				pSetPlayerTeam = nativelist[i].func;
			
			if(!pSetPlayerSkin && !strcmp(nativelist[i].name, "SetPlayerSkin"))
				pSetPlayerSkin = nativelist[i].func;
			
			if(!pSetPlayerFightingStyle && !strcmp(nativelist[i].name, "SetPlayerFightingStyle"))
				pSetPlayerFightingStyle = nativelist[i].func;
			
			if(!pSetPlayerName && !strcmp(nativelist[i].name, "SetPlayerName"))
				pSetPlayerName = nativelist[i].func;
			
			if(!pSetVehicleToRespawn && !strcmp(nativelist[i].name, "SetVehicleToRespawn"))
				pSetVehicleToRespawn = nativelist[i].func;

			if(!pChangeVehicleColor && !strcmp(nativelist[i].name, "ChangeVehicleColor"))
				pChangeVehicleColor = nativelist[i].func;

			if(!pDestroyVehicle && !strcmp(nativelist[i].name, "DestroyVehicle"))
				pDestroyVehicle = nativelist[i].func;

			//logprintf("native %s", nativelist[i].name);
			int x = 0;
			
			while (RedirectedNatives[x].name)
			{
				//logprintf("asdasd %s", RedirecedtNatives[x].name);
				if (!strcmp(nativelist[i].name, RedirectedNatives[x].name))
				{
					if (!g_bNativesHooked) g_bNativesHooked = true;
				
					//logprintf("native: %s, %s", nativelist[i].name, RedirecedtNatives[x].name);
					nativelist[i].func = RedirectedNatives[x].func;
				}
				x++;
			}
			i++;
		}
	}

	return amx_Register(amx, nativelist, number);
}

//----------------------------------------------------

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

	if (IsPlayerConnectedEx(playerid))
	{
		// AFK
		if (IsPlayerUpdatePacket(packetId))
		{
			pPlayerData[playerid]->dwLastUpdateTick = GetTickCount();
			pPlayerData[playerid]->bEverUpdated = true;
						
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

		if (packetId == ID_PLAYER_SYNC)
		{
			CSyncData *pSyncData = (CSyncData*)(&p->data[1]);

			// Fix nightvision and infrared sync
			if (pSyncData->byteWeapon == 44 || pSyncData->byteWeapon == 45)
			{
				pSyncData->wKeys &= ~4;
				pSyncData->byteWeapon = 0;
			}

			// Store surfing info because server reset it when player surfing on player object
			pPlayerData[playerid]->wSurfingInfo = pSyncData->wSurfingInfo;
		}

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
	}
	return packetId;
}

//----------------------------------------------------

#ifdef _WIN32
bool __thiscall CHookRakServer::Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast)
#else
bool CHookRakServer::Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast)
#endif
{
/*
	BYTE id;
	WORD playerid;
	parameters->Read(id);
	parameters->Read(playerid);

	logprintf("id: %d - playerid: %d, sendto. %d", id, playerid, pRakServer->GetIndexFromPlayerID(playerId));
*/
	RebuildSyncData(parameters, static_cast<WORD>(pRakServer->GetIndexFromPlayerID(playerId)));

	return RakNetOriginalSend(ppRakServer, parameters, priority, reliability, orderingChannel, playerId, broadcast);
}

//----------------------------------------------------

#ifdef _WIN32
bool __thiscall CHookRakServer::RPC(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
#else
bool CHookRakServer::RPC(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
#endif
{
	//logprintf("outgoing rpc: %d", *uniqueID);
	return RakNetOriginalRPC(ppRakServer, uniqueID, parameters, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp);
}

//----------------------------------------------------

bool	bRconSocketReply = false;

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

	static char buffer[1024];
	va_list arguments;
	va_start(arguments, msg);
	if(!msg[0]) return;

	vsnprintf(buffer, sizeof(buffer), msg, arguments);
	va_end(arguments);
	if(!buffer[0]) return;

	if(CCallbackManager::OnServerMessage(buffer))
		logprintf(buffer);

	if (bRconSocketReply) 
		RconSocketReply(buffer);
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

//----------------------------------------------------
// bool CheckQueryFlood()
// returns 1 if this query could flood
// returns 0 otherwise
bool CheckQueryFlood(unsigned int binaryAddress)
{
	static DWORD dwLastQueryTick = 0;
	static unsigned int lastBinAddr = 0;

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

char* RemoveHexColorFromString(char *szMsg) // Thx to P3ti
{
/*
	char szNonColorEmbeddedMsg[257];
	int iNonColorEmbeddedMsgLen = 0;

	for (size_t pos = 0; pos < strlen(szMsg) && szMsg[pos] != '\0'; pos++)
	{
		if (!((*(unsigned char*)(&szMsg[pos]) - 32) >= 0 && (*(unsigned char*)(&szMsg[pos]) - 32) < 224))
			continue;

		if (pos + 7 < strlen(szMsg))
		{
			if (szMsg[pos] == '{' && szMsg[pos + 7] == '}')
			{
				pos += 7;
				continue;
			}
		}

		szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = szMsg[pos];
		iNonColorEmbeddedMsgLen++;
	}
	szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = 0;
	return &szNonColorEmbeddedMsg[0];
	*/
	return szMsg;
}


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

					char* szLanguage = pServer->GetStringVariable("language");
					char* szMapName = (!szLanguage[0]) ? pServer->GetStringVariable("mapname") : szLanguage;

					DWORD dwMapNameLen = strlen(szMapName);
					if (dwMapNameLen > 30) dwMapNameLen = 30;

					WORD wPlayerCount = pServer->GetPlayerCount();
//					CPlayerPool* pPlayerPool = pNetGame->pPlayerPool;

					WORD wMaxPlayers = pServer->GetMaxPlayers_();

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

						for (WORD r = 0; r < MAX_PLAYERS; r++)
						{
							if (IsPlayerConnectedEx(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->bHidden)
							{
								szName = RemoveHexColorFromString(GetPlayerName_(r));
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

						for (WORD r = 0; r < MAX_PLAYERS; r++)
						{
							if (IsPlayerConnectedEx(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->bHidden)
							{
								memcpy(newdata, &r, sizeof(BYTE));
								newdata += sizeof(BYTE);
								szName = GetPlayerName_(r);
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
					if (!pServer->GetBoolVariable("query") || !pServer->GetBoolVariable("rcon")) return 1;
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

					if (!strcmp(szPassword, pServer->GetStringVariable("rcon_password")))
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
							if (CCallbackManager::OnRemoteRCONPacket(binaryAddress, port, szPassword, true, szCommand))
							{ 
								bRconSocketReply = true;
								// Execute the command
								CSAMPFunctions::Execute(szCommand);
								bRconSocketReply = false;
							}
						}

						free(szCommand);
					}
					else 
					{
						in_addr in;
						in.s_addr = binaryAddress;
						logprintf("BAD RCON ATTEMPT BY: %s", inet_ntoa(in));

						bRconSocketReply = true;
						RconSocketReply("Invalid RCON password.");
						bRconSocketReply = false;

						CCallbackManager::OnRemoteRCONPacket(binaryAddress, port, szPassword, false, "NULL");
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

#ifdef WIN32

CVehicle *_pVehicle;

void _declspec(naked) HOOK_CVehicle__Respawn()
{
	_asm mov _pVehicle, ecx
	_asm pushad

	CSAMPFunctions::RespawnVehicle(_pVehicle);

	_asm popad
	_asm retn
}

#endif

// Things that needs to be hooked before netgame initialied
void InstallPreHooks()
{
	if (pServer)
	{
		Namecheck_hook.Install((void *)CAddress::FUNC_ContainsInvalidChars, (void *)HOOK_ContainsInvalidChars);
		amx_Register_hook.Install((void*)*(DWORD*)((DWORD)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
		GetPacketID_hook.Install((void*)CAddress::FUNC_GetPacketID, (void*)HOOK_GetPacketID);
		query_hook.Install((void*)CAddress::FUNC_ProcessQueryPacket, (void*)HOOK_ProcessQueryPacket);
/*
#ifdef WIN32
		InstallJump(CAddress::FUNC_CVehicle__Respawn, (void*)HOOK_CVehicle__Respawn);
#else
		InstallJump(CAddress::FUNC_CVehicle__Respawn, (void*)CSAMPFunctions::RespawnVehicle);
#endif
*/
	}	
}

// Things that needs to be hooked after netgame initialied
void InstallPostHooks()
{
	// Get pNetGame
	int (*pfn_GetNetGame)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_NETGAME];
	pNetGame = (CNetGame*)pfn_GetNetGame();

	// Get pConsole
	int (*pfn_GetConsole)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_CONSOLE];
	pConsole = (void*)pfn_GetConsole();

	// Get pRakServer
	int (*pfn_GetRakServer)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_RAKSERVER];
	pRakServer = (RakServer*)pfn_GetRakServer();

	// SetMaxPlayers() fix
	pRakServer->Start(MAX_PLAYERS, 0, 5, static_cast<unsigned short>(pServer->GetIntVariable("port")), pServer->GetStringVariable("bind"));
#ifdef _WIN32
	logprintf_hook.Install((void*)ppPluginData[PLUGIN_DATA_LOGPRINTF], (void*)HOOK_logprintf);
#endif
	// Recreate GangZone pool
	pNetGame->pGangZonePool = new CGangZonePool();

#ifdef NEW_PICKUP_SYSTEM
	// Recreate Pickup pool
	pNetGame->pPickupPool = new CPickupPool();
#endif
	// Re-init some RPCs
	InitRPCs();

	//logprintf("YSF - pNetGame: 0x%X, pConsole: 0x%X, pRakServer: 0x%X", pNetGame, pConsole, pRakServer);

	// RakServer::Send hook - Thanks to Gamer_Z
	int SendFunc = ((int*)(*(void**)pRakServer))[RAKNET_SEND_OFFSET];
	RakNetOriginalSend = reinterpret_cast<RakNet__Send_t>(SendFunc);
	Unlock((void*)&((int*)(*(void**)pRakServer))[RAKNET_SEND_OFFSET], 4);
	((int*)(*(void**)pRakServer))[RAKNET_SEND_OFFSET] = (int)CHookRakServer::Send;
/*
	// RakServer::RPC hook - Thanks to Gamer_Z
	int RPCFunc = ((int*)(*(void**)pRakServer))[RAKNET_RPC_OFFSET];
	RakNetOriginalRPC = reinterpret_cast<RakNet__RPC_t>(RPCFunc);
	Unlock((void*)&((int*)(*(void**)pRakServer))[RAKNET_RPC_OFFSET], 4);
	((int*)(*(void**)pRakServer))[RAKNET_RPC_OFFSET] = (int)CHookRakServer::RPC;
*/
}

