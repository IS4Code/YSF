
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

AMX_NATIVE pDestroyPlayerObject = NULL, pCancelEdit = NULL, pTogglePlayerControllable = NULL, pSetPlayerWorldBounds = NULL, pSetPlayerTeam = NULL, pSetPlayerSkin = NULL, pSetPlayerFightingStyle = NULL, pSetPlayerName = NULL, pSetVehicleToRespawn = NULL;

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
			if(!pDestroyPlayerObject && !strcmp(nativelist[i].name, "DestroyPlayerObject"))
				pDestroyPlayerObject = nativelist[i].func;

			if(!pCancelEdit && !strcmp(nativelist[i].name, "CancelEdit"))
				pCancelEdit = nativelist[i].func;

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
			
			if (packetId == ID_VEHICLE_SYNC || packetId == ID_PASSENGER_SYNC || packetId == ID_UNOCCUPIED_SYNC)
			{
				static CVector emptyVector = CVector(0.0f, 0.0f, 0.0f);
				CVector* vecPosition = &emptyVector;
				CVector* vecVelocity = &emptyVector;
				float fTrainSpeed = 0.0f;
				CVehicleSyncData *vd = NULL;
				CUnoccupiedSyncData *ud = NULL;
				CPassengerSyncData *pd = NULL;

				switch (packetId) {
				case ID_VEHICLE_SYNC:
					vd = (CVehicleSyncData*)(&p->data[1]);
					vecPosition = &vd->vecPosition;
					vecVelocity = &vd->vecVelocity;
					fTrainSpeed = vd->fTrainSpeed;
					break;

				case ID_UNOCCUPIED_SYNC:
					ud = (CUnoccupiedSyncData*)(&p->data[1]);
					vecPosition = &ud->vecPosition;
					vecVelocity = &ud->vecVelocity;
					break;

				case ID_PASSENGER_SYNC:
					pd = (CPassengerSyncData*)(&p->data[1]);
					vecPosition = &pd->vecPosition;
					break;

				default:
					break;
				}
				// Fix "player bugger"
				// Causes this screen: http://scrn.sixtytiger.com/sa-mp-026.png
				// Happens when a player is in a car with a cheating player, and that cheating player
				// manipulates the vehicle position to be outside the GTA:SA position range
				if (vecPosition->fX < -20000.0f || vecPosition->fX > 20000.0f ||
					vecPosition->fY < -20000.0f || vecPosition->fY > 20000.0f ||
					vecPosition->fZ < -20000.0f || vecPosition->fZ > 20000.0f ||
					
					// MOVE SPEED

					vecVelocity->fX > 35.0f || vecVelocity->fX < -35.0f ||
					vecVelocity->fY > 35.0f || vecVelocity->fY < -35.0f ||
					vecVelocity->fZ > 35.0f || vecVelocity->fZ < -35.0f ||

					// infinity checks

					!isfinite(vecPosition->fX) || !isfinite(vecPosition->fY) || !isfinite(vecPosition->fZ) ||
					!isfinite(vecVelocity->fX) || !isfinite(vecVelocity->fY) || !isfinite(vecVelocity->fZ)
					)
				{
					logprintf("vehicle crasher");
					return 0xFF;
				}
				/*
				// Fix "bike crash"
				if (fTrainSpeed > 100000.0f || fTrainSpeed < 0.0f)
				{
					return 0xFF;
				}
				*/
			}
			
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
			}

			// Sync 
			for(WORD i = 0; i != MAX_PLAYERS; i++)
			{
				if(IsPlayerConnected(i))
				{
					if(pPlayerData[i]->bCustomPos[playerid])
						pSyncData->vecPosition = *pPlayerData[i]->vecCustomPos[playerid];
				}
			}
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
		
		// Bullet crasher fix
		if (packetId == ID_BULLET_SYNC)
		{
			RakNet::BitStream bsData(p->data, p->length, false);
			BULLET_SYNC_DATA pBulletSync;

			bsData.SetReadOffset(8);
			bsData.Read((char*)&pBulletSync, sizeof(pBulletSync));

			if (pBulletSync.byteHitType == BULLET_HIT_TYPE_PLAYER && ((pBulletSync.vecCenterOfHit.fX > 10.0f || pBulletSync.vecCenterOfHit.fX < -10.0f) || (pBulletSync.vecCenterOfHit.fY > 10.0f || pBulletSync.vecCenterOfHit.fY < -10.0f) || (pBulletSync.vecCenterOfHit.fZ > 10.0f || pBulletSync.vecCenterOfHit.fZ < -10.0f)))
			{
				return 0xFF;
			}

			if (pBulletSync.byteHitType == BULLET_HIT_TYPE_VEHICLE && ((pBulletSync.vecCenterOfHit.fX > 100.0f || pBulletSync.vecCenterOfHit.fX < -100.0f) || (pBulletSync.vecCenterOfHit.fY > 100.0f || pBulletSync.vecCenterOfHit.fY < -100.0f) || (pBulletSync.vecCenterOfHit.fZ > 100.0f || pBulletSync.vecCenterOfHit.fZ < -100.0f)))
			{
				return 0xFF;
			}

			if (pBulletSync.byteHitType == BULLET_HIT_TYPE_OBJECT || pBulletSync.byteHitType == BULLET_HIT_TYPE_PLAYER_OBJECT && ((pBulletSync.vecCenterOfHit.fX > 1000.0 || pBulletSync.vecCenterOfHit.fX < -1000.0) || (pBulletSync.vecCenterOfHit.fY > 1000.0 || pBulletSync.vecCenterOfHit.fY < -1000.0) || (pBulletSync.vecCenterOfHit.fZ > 1000.0 || pBulletSync.vecCenterOfHit.fZ < -1000.0)))
			{
				return 0xFF;
			}
		}

		if (packetId == ID_AIM_SYNC)
		{
			RakNet::BitStream bsData(p->data, p->length, false);

			CAimSyncData *aim = (CAimSyncData*)(&p->data[1]);;
			int weaponid = pNetGame->pPlayerPool->pPlayer[p->playerIndex]->byteCurrentWeapon;

			CVector vecPos = pNetGame->pPlayerPool->pPlayer[p->playerIndex]->vecPosition;
			CVector vecDist = aim->vecPosition - vecPos;

			if ((abs(vecDist.fX) + abs(vecDist.fY) + abs(vecDist.fZ)) > 500.0f)
			{
				return 0xFF;
			}

			if (!isfinite(aim->vecPosition.fX) || !isfinite(aim->vecPosition.fY) || !isfinite(aim->vecPosition.fZ) ||
				!isfinite(aim->vecFront.fX) || !isfinite(aim->vecFront.fY) || !isfinite(aim->vecFront.fZ))
			{
				return 0xFF;
			}

			if (aim->vecFront.fX < -9.9f || aim->vecFront.fX > 9.9f ||
				aim->vecFront.fY < -9.9f || aim->vecFront.fY > 9.9f ||
				aim->vecFront.fZ < -9.9f || aim->vecFront.fZ > 9.9f)
			{
				return 0xFF;
			}
		}
	}
	return packetId;
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
	vsnprintf(buffer, sizeof(buffer), msg, arguments);
	va_end(arguments);

	if(CCallbackManager::OnServerMessage(buffer))
		logprintf(buffer);

	if (bRconSocketReply) 
		RconSocketReply(buffer);

	//SAFE_DELETE(buffer);
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

					char* szMapName = pServer->GetStringVariable("mapname");
					DWORD dwMapNameLen = strlen(szMapName);
					if (dwMapNameLen > 30) dwMapNameLen = 30;

					WORD wPlayerCount = pServer->GetPlayerCount();
					CPlayerPool* pPlayerPool = pNetGame->pPlayerPool;

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
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->bHidden)
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
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->bHidden)
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

						
						CCallbackManager::OnRemoteRCONPacket(binaryAddress, port, (!szPassword[0]) ? ("NULL") : (szPassword), false, "NULL");
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
/*
class CHookVehicle
{
public:
	void* __thiscall HOOK_CVehicle__Respawn(void* pVehicle);
};

void __stdcall HOOK_CVehicle__Respawn(void *pVehicle)
{
	SubHook::ScopedRemove remove(&CVehicle__Respawn_hook);

	logprintf("respawn: %x", pVehicle);
	//CSAMPFunctions::RespawnVehicle_((CVehicle*)pVehicle);
}
*/
void InstallPreHooks()
{
	if (pServer)
	{
		Namecheck_hook.Install((void *)CAddress::FUNC_ContainsInvalidChars, (void *)HOOK_ContainsInvalidChars);
		amx_Register_hook.Install((void*)*(DWORD*)((DWORD)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
		GetPacketID_hook.Install((void*)CAddress::FUNC_GetPacketID, (void*)HOOK_GetPacketID);
		query_hook.Install((void*)CAddress::FUNC_ProcessQueryPacket, (void*)HOOK_ProcessQueryPacket);

		//CVehicle__Respawn_hook.Install((void*)CAddress::FUNC_CVehicle__Respawn, HOOK_CVehicle__Respawn);
	}
	logprintf_hook.Install((void*)ppPluginData[PLUGIN_DATA_LOGPRINTF], (void*)HOOK_logprintf);	
}

