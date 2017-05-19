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

//----------------------------------------------------
subhook_t SetWeather_hook;
subhook_t SetGravity_hook;
subhook_t Namecheck_hook;
subhook_t amx_Register_hook;
subhook_t logprintf_hook;
subhook_t query_hook;
subhook_t CVehicle__Respawn_hook;

// Callback hooks instead of using SAMP GDK
subhook_t CGameMode__OnPlayerConnect_hook;
subhook_t CGameMode__OnPlayerDisconnect_hook;
subhook_t CGameMode__OnPlayerSpawn_hook;
subhook_t CGameMode__OnPlayerStreamIn_hook;
subhook_t CGameMode__OnPlayerStreamOut_hook;
subhook_t CGameMode__OnDialogResponse_hook;

//----------------------------------------------------
char gRecordingDataPath[MAX_PATH];

///////////////////////////////////////////////////////////////
// Hooks //
///////////////////////////////////////////////////////////////

typedef void(THISCALL* FUNC_CNetGame__SetWeather)(void *thisptr, BYTE weatherid);
#ifdef _WIN32
void FASTCALL HOOK_CNetGame__SetWeather(void *thisptr, void *padding, BYTE weatherid)
#else
void CDECL HOOK_CNetGame__SetWeather(void *thisptr, BYTE weatherid)
#endif
{
	subhook_remove(SetWeather_hook);

	for (int i = 0; i != MAX_PLAYERS; ++i)
	{
		if (IsPlayerConnected(i))
			pPlayerData[i]->byteWeather = weatherid;
	}

	((FUNC_CNetGame__SetWeather)CAddress::FUNC_CNetGame__SetWeather)(thisptr, weatherid);
	subhook_install(SetWeather_hook);
}

//----------------------------------------------------

typedef void(THISCALL* FUNC_CNetGame__SetGravity)(void *thisptr, float gravity);
#ifdef _WIN32
void FASTCALL HOOK_CNetGame__SetGravity(void *thisptr, void *padding, float gravity)
#else
void CDECL HOOK_CNetGame__SetGravity(void *thisptr, float gravity)
#endif
{
	subhook_remove(SetGravity_hook);

	for (WORD i = 0; i != MAX_PLAYERS; ++i)
	{
		if (IsPlayerConnected(i))
			pPlayerData[i]->fGravity = gravity;
	}

	((FUNC_CNetGame__SetGravity)CAddress::FUNC_CNetGame__SetGravity)(thisptr, gravity);
	subhook_install(SetGravity_hook);
}

//----------------------------------------------------

// Custom name check
bool HOOK_ContainsInvalidChars(char * szString)
{
	return !CServer::Get()->IsValidNick(szString);
}

//----------------------------------------------------

typedef BYTE (*FUNC_amx_Register)(AMX *amx, AMX_NATIVE_INFO *nativelist, int number);
int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	// amx_Register hook for redirect natives
	static bool bNativesHooked = false;

	if (!bNativesHooked && CServer::Get()->IsInitialized())
	{
		int i = 0;
		while (nativelist[i].name)
		{
			//logprintf("native %s", nativelist[i].name);
			int x = 0;
			
			while (redirected_native_list[x].name)
			{
				//logprintf("asdasd %s", RedirectedNatives[x].name);
				if (!strcmp(nativelist[i].name, redirected_native_list[x].name))
				{
					if (!bNativesHooked) bNativesHooked = true;
				
					//logprintf("native: %s, %s", nativelist[i].name, redirected_native_list[x].name);
					if(redirected_native_list[x].originalfunc != NULL)
					{
						*redirected_native_list[x].originalfunc = nativelist[i].func;
					}
					nativelist[i].func = redirected_native_list[x].func;
				}
				x++;
			}
			i++;
		}
	}

	return ((FUNC_amx_Register)subhook_get_trampoline(amx_Register_hook))(amx, nativelist, number);
}

//----------------------------------------------------

bool THISCALL CHookRakServer::Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast)
{
/*	
	BYTE id;
	WORD playerid;
	parameters->Read(id);
	parameters->Read(playerid);

	logprintf("id: %d - playerid: %d, sendto: %d", id, playerid, CSAMPFunctions::GetIndexFromPlayerID(playerId));
*/	
	CServer::Get()->RebuildSyncData(parameters, static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(playerId)));
	return CSAMPFunctions::Send(parameters, priority, reliability, orderingChannel, playerId, broadcast);
}

//----------------------------------------------------

bool THISCALL CHookRakServer::RPC_2(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
{
	if (!CServer::Get()->RebuildRPCData(*uniqueID, parameters, static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(playerId)))) return 1;
	
	if (CServer::Get()->GetExclusiveBroadcast())
	{
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
			if (IsPlayerConnected(i) && pPlayerData[i]->bBroadcastTo)
				CSAMPFunctions::RPC(uniqueID, parameters, priority, reliability, orderingChannel, CSAMPFunctions::GetPlayerIDFromIndex(i), false, shiftTimestamp);

		return 1;
	}

	return CSAMPFunctions::RPC(uniqueID, parameters, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp);
}

//----------------------------------------------------

void THISCALL CHookRakServer::AddToBanList(void* ppRakServer, const char *IP, unsigned int milliseconds)
{
	CServer::Get()->BanIP(IP);
	return CSAMPFunctions::AddToBanList(IP, milliseconds);
}

void THISCALL CHookRakServer::RemoveFromBanList(void* ppRakServer, const char *IP)
{
	CServer::Get()->UnbanIP(IP);
	return CSAMPFunctions::RemoveFromBanList(IP);
}

void THISCALL CHookRakServer::ClearBanList(void* ppRakServer)
{
	CServer::Get()->ClearBans();
	return CSAMPFunctions::ClearBanList();
}

//----------------------------------------------------

Packet* THISCALL CHookRakServer::Receive(void* ppRakServer)
{
	Packet* p = CSAMPFunctions::Receive(ppRakServer);
	BYTE packetId = GetPacketID(p);
	if(packetId == 0xFF) return p;

	WORD playerid = p->playerIndex;

	//logprintf("Receive packet playerid: %d, id: %d", playerid, packetId);
	
	if (IsPlayerConnected(playerid))
	{
		// AFK
		if (IsPlayerUpdatePacket(packetId))
		{
			pPlayerData[playerid]->LastUpdateTick = default_clock::now();
			pPlayerData[playerid]->bEverUpdated = true;
		}

		switch(packetId)
		{
			case ID_PLAYER_SYNC:
			{
				CSyncData *pSyncData = reinterpret_cast<CSyncData*>(&p->data[1]);

				if(CServer::Get()->IsNightVisionFixEnabled())
				{
					// Fix nightvision and infrared sync
					if (pSyncData->byteWeapon == WEAPON_NIGHTVISION || pSyncData->byteWeapon == WEAPON_INFRARED)
					{
						pSyncData->wKeys &= ~(KEY_FIRE | KEY_ACTION);
						pSyncData->byteWeapon = 0;
						pSyncData->_unk_ = 0;
					}
				}

				// Store surfing info because server reset it when player surfing on player object
				pPlayerData[playerid]->wSurfingInfo = pSyncData->wSurfingInfo;
				break;
			}
			case ID_AIM_SYNC:
			{
				CAimSyncData *pAim = reinterpret_cast<CAimSyncData*>(&p->data[1]);

				// Fix up, down aim sync - thx to Slice, from SKY
				switch(pNetGame->pPlayerPool->pPlayer[playerid]->byteCurrentWeapon)
				{
					case WEAPON_SNIPER:
					case WEAPON_ROCKETLAUNCHER:
					case WEAPON_HEATSEEKER:
					case WEAPON_CAMERA:
					{
						pAim->fZAim = -pAim->vecFront.fZ;

						if (pAim->fZAim > 1.0f)
						{
							pAim->fZAim = 1.0f;
						}
						else if (pAim->fZAim < -1.0f)
						{
							pAim->fZAim = -1.0f;
						}
						break;
					}
				}
				break;
			}
			case ID_STATS_UPDATE:
			{
				CSAMPFunctions::Packet_StatsUpdate(p);
				CCallbackManager::OnPlayerStatsAndWeaponsUpdate(playerid);

				p->data[0] = 0xFF;
				break;
			}
			case ID_WEAPONS_UPDATE:
			{
				CSAMPFunctions::Packet_WeaponsUpdate(p);
				CCallbackManager::OnPlayerStatsAndWeaponsUpdate(playerid);

				p->data[0] = 0xFF;
				break;
			}
		}
	}
	return p;
}

//----------------------------------------------------
FILE* g_fLog = NULL;

bool	bRconSocketReply = false;

SOCKET	cur_sock = INVALID_SOCKET;
char*	cur_data = NULL;
int		cur_datalen = 0;
sockaddr_in to;

//----------------------------------------------------

void RconSocketReply(char* szMessage);

//----------------------------------------------------

typedef void (*FUNC_logprintf)(const char *msg, ...);

void HOOK_logprintf(const char *msg, ...)
{
	char buffer[1024];
	va_list arguments;
	va_start(arguments, msg);
	vsnprintf(buffer, sizeof(buffer), msg, arguments);
	va_end(arguments);

	bool bAllow;
	if (CServer::Get()->IsOnServerMessageEnabled())
		bAllow = CCallbackManager::OnServerMessage(buffer);
	else
		bAllow = true;

	if (bAllow)
	{		
#ifdef _WIN32		
		char OEMbuffer[1024];
		CharToOemA(buffer, OEMbuffer);
		puts(OEMbuffer);
#else
		if (CSAMPFunctions::GetBoolVariable("output"))
			puts(buffer);
#endif 
		if (!g_fLog)
			g_fLog = fopen("server_log.txt", "a");

		if (CSAMPFunctions::GetBoolVariable("timestamp"))
		{
			time_t t;
			time(&t);
			char szTimeFormat[256];
			strftime(szTimeFormat, sizeof(szTimeFormat), CSAMPFunctions::GetStringVariable("logtimeformat"), localtime(&t));
			fprintf(g_fLog, "%s %s\n", &szTimeFormat, &buffer);
			fflush(g_fLog);
		}
		else
		{
			fputs(buffer, g_fLog);
			fflush(g_fLog);
		}
	
		if (*(WORD*)CAddress::VAR_wRCONUser != INVALID_PLAYER_ID)
		{
			DWORD len = strlen(buffer);
			RakNet::BitStream bsParams;
			bsParams.Write(0xFFFFFFFF);
			bsParams.Write((DWORD)len);
			bsParams.Write(buffer, len);
			CSAMPFunctions::RPC(&RPC_ClientMessage, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(*(WORD*)CAddress::VAR_wRCONUser), false, false);
		}
		else if (bRconSocketReply)
		{
			RconSocketReply(buffer);
		}

		CServer::Get()->ProcessConsoleMessages(buffer);
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

//----------------------------------------------------
// bool CheckQueryFlood()
// returns 1 if this query could flood
// returns 0 otherwise
bool CheckQueryFlood(unsigned int binaryAddress)
{
	static DWORD dwLastQueryTick = 0;
	static unsigned int lastBinAddr = 0;

	if(!dwLastQueryTick)
	{
		dwLastQueryTick = static_cast<DWORD>(GetTickCount());
		lastBinAddr = binaryAddress;
		return 0;
	}
	if(lastBinAddr != binaryAddress)
	{
		if((static_cast<DWORD>(GetTickCount()) - dwLastQueryTick) < 25)
			return 1;

		dwLastQueryTick = static_cast<DWORD>(GetTickCount());
		lastBinAddr = binaryAddress;
	}
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

			if (CSAMPFunctions::GetBoolVariable("logqueries"))
			{
				logprintf("[query:%c] from %s", data[10], inet_ntoa(in));
			}

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
					if (!CSAMPFunctions::GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					char *temp;

					char szHostname[51];
					temp = CSAMPFunctions::GetStringVariable("hostname");
					size_t dwHostnameLen = strlen(temp);
					if (dwHostnameLen > 50) 
						dwHostnameLen = 50;
					
					memcpy(szHostname, temp, dwHostnameLen);
					szHostname[dwHostnameLen] = 0;

					char szGameMode[31];
					temp = CSAMPFunctions::GetStringVariable("gamemodetext");
					size_t dwGameModeLen = strlen(temp);
					if (dwGameModeLen > 30) 
						dwGameModeLen = 30;
					
					memcpy(szGameMode, temp, dwGameModeLen);
					szGameMode[dwGameModeLen] = 0;

					char szMapName[31];
					temp = CSAMPFunctions::GetStringVariable("language");
					if (!temp[0]) 
						temp = CSAMPFunctions::GetStringVariable("mapname");
					
					size_t dwMapNameLen = strlen(temp);
					if (dwMapNameLen > 30) 
						dwMapNameLen = 30;

					memcpy(szMapName, temp, dwMapNameLen);
					szMapName[dwMapNameLen] = 0;
					/*
					bool stringsChanged = CCallbackManager::OnServerQueryInfo(binaryAddress, szHostname, szGameMode, szMapName);
					if (stringsChanged)
					{
						dwHostnameLen = strlen(szHostname);
						dwGameModeLen = strlen(szGameMode);
						dwMapNameLen = strlen(szMapName);
					}
					*/
					WORD wPlayerCount = CServer::Get()->GetPlayerCount();
					WORD wMaxPlayers = CServer::Get()->GetMaxPlayers();
					BYTE byteIsPassworded = CSAMPFunctions::GetStringVariable("password")[0] != 0;

					size_t datalen = 28;	// Previous data = 11b
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
					if (!CSAMPFunctions::GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					WORD wPlayerCount = CServer::Get()->GetPlayerCount();
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

						for (WORD r = 0; r != MAX_PLAYERS; ++r)
						{
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->strNameInQuery.empty())
							{
								szName = (char*)GetPlayerName(r, true);
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
					if (!CSAMPFunctions::GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					WORD wPlayerCount = CServer::Get()->GetPlayerCount();
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

						for (WORD r = 0; r != MAX_PLAYERS; ++r)
						{
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pPlayerData[r]->strNameInQuery.empty())
							{
								memcpy(newdata, &r, sizeof(BYTE));
								newdata += sizeof(BYTE);
								szName = (char*)GetPlayerName(r, true);
								byteNameLen = (BYTE)strlen(szName);
								memcpy(newdata, &byteNameLen, sizeof(BYTE));
								newdata += sizeof(BYTE);
								memcpy(newdata, szName, byteNameLen);
								newdata += byteNameLen;
								dwScore = pPlayerPool->dwScore[r];
								memcpy(newdata, &dwScore, sizeof(DWORD));
								newdata += sizeof(DWORD);
								dwPing = CSAMPFunctions::GetLastPing(CSAMPFunctions::GetPlayerIDFromIndex(r));
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
					if (!CSAMPFunctions::GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					CSAMPFunctions::SendRules(s, data, (sockaddr_in*)&to, sizeof(to));
					break;
				}
				case 'x':	// rcon
				{
					if (CServer::Get()->IsBanned(inet_ntoa(in)) && !CServer::Get()->m_bAllowRemoteRCONWithBannedIPs) return 1;
					
					// We do not process these queries 'query' is 0
					if (!CSAMPFunctions::GetBoolVariable("query") || !CSAMPFunctions::GetBoolVariable("rcon")) return 1;
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
					
					if (!strcmp(szPassword, CSAMPFunctions::GetStringVariable("rcon_password")))
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

//----------------------------------------------------

#ifdef _WIN32
void FASTCALL HOOK_CVehicle__Respawn(CVehicle *thisptr, void *padding)
#else
void CDECL HOOK_CVehicle__Respawn(CVehicle *thisptr)
#endif
{
	CSAMPFunctions::RespawnVehicle(thisptr);
}
//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerConnect)(CGameMode *thisptr, cell playerid);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerConnect(CGameMode *thisptr, void *padding, cell playerid)
#else
int CDECL HOOK_CGameMode__OnPlayerConnect(CGameMode *thisptr, cell playerid)
#endif
{
	subhook_remove(CGameMode__OnPlayerConnect_hook);

#ifndef NEW_PICKUP_SYSTEM
	CServer::Get()->AddPlayer(playerid);
#else
	// Initialize pickups
	if (CServer::Get()->AddPlayer(playerid))
		CServer::Get()->pPickupPool->InitializeForPlayer(playerid);
#endif

	int ret = ((FUNC_CGameMode__OnPlayerConnect)CAddress::FUNC_CGameMode__OnPlayerConnect)(thisptr, playerid);
	subhook_install(CGameMode__OnPlayerConnect_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerDisconnect)(CGameMode *thisptr, cell playerid, cell reason);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerDisconnect(CGameMode *thisptr, void *padding, cell playerid, cell reason)
#else
int CDECL HOOK_CGameMode__OnPlayerDisconnect(CGameMode *thisptr, cell playerid, cell reason)
#endif
{
	subhook_remove(CGameMode__OnPlayerDisconnect_hook);

	CServer::Get()->RemovePlayer(playerid);
	
	int ret = ((FUNC_CGameMode__OnPlayerDisconnect)CAddress::FUNC_CGameMode__OnPlayerDisconnect)(thisptr, playerid, reason);
	subhook_install(CGameMode__OnPlayerDisconnect_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerSpawn)(CGameMode *thisptr, cell playerid);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerSpawn(CGameMode *thisptr, void *padding, cell playerid)
#else
int CDECL HOOK_CGameMode__OnPlayerSpawn(CGameMode *thisptr, cell playerid)
#endif
{
	subhook_remove(CGameMode__OnPlayerSpawn_hook);

	if (IsPlayerConnected(playerid))
		pPlayerData[playerid]->bControllable = true;

	int ret = ((FUNC_CGameMode__OnPlayerSpawn)CAddress::FUNC_CGameMode__OnPlayerSpawn)(thisptr, playerid);
	subhook_install(CGameMode__OnPlayerSpawn_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerStreamIn)(CGameMode *thisptr, cell playerid, cell forplayerid);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerStreamIn(CGameMode *thisptr, void *padding, cell playerid, cell forplayerid)
#else
int CDECL HOOK_CGameMode__OnPlayerStreamIn(CGameMode *thisptr, cell playerid, cell forplayerid)
#endif
{
	subhook_remove(CGameMode__OnPlayerStreamIn_hook);

	CServer::Get()->OnPlayerStreamIn(static_cast<WORD>(playerid), static_cast<WORD>(forplayerid));

	int ret = ((FUNC_CGameMode__OnPlayerStreamIn)CAddress::FUNC_CGameMode__OnPlayerStreamIn)(thisptr, playerid, forplayerid);
	subhook_install(CGameMode__OnPlayerStreamIn_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerStreamOut)(CGameMode *thisptr, cell playerid, cell forplayerid);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerStreamOut(CGameMode *thisptr, void *padding, cell playerid, cell forplayerid)
#else
int CDECL HOOK_CGameMode__OnPlayerStreamOut(CGameMode *thisptr, cell playerid, cell forplayerid)
#endif
{
	subhook_remove(CGameMode__OnPlayerStreamOut_hook);

	CServer::Get()->OnPlayerStreamOut(static_cast<WORD>(playerid), static_cast<WORD>(forplayerid));
	
	int ret = ((FUNC_CGameMode__OnPlayerStreamOut)CAddress::FUNC_CGameMode__OnPlayerStreamOut)(thisptr, playerid, forplayerid);
	subhook_install(CGameMode__OnPlayerStreamOut_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnDialogResponse)(CGameMode *thisptr, cell playerid, cell dialogid, cell response, cell listitem, char *szInputtext);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnDialogResponse(CGameMode *thisptr, void *padding, cell playerid, cell dialogid, cell response, cell listitem, char *szInputtext)
#else
int CDECL HOOK_CGameMode__OnDialogResponse(CGameMode *thisptr, cell playerid, cell dialogid, cell response, cell listitem, char *szInputtext)
#endif
{
	subhook_remove(CGameMode__OnDialogResponse_hook);

	int ret = -1;
	if (IsPlayerConnected(playerid))
	{
		if (CServer::Get()->m_bDialogProtection && pPlayerData[playerid]->wDialogID != dialogid)
		{
			logprintf("YSF: Might dialog hack has been detected for player %s(%d) - which should be: %d, dialogid: %d", GetPlayerName(playerid), playerid, pPlayerData[playerid]->wDialogID, dialogid);
			ret = 1;
		}
		pPlayerData[playerid]->wDialogID = -1;
	}

	if(ret == -1)
		ret = ((FUNC_CGameMode__OnDialogResponse)CAddress::FUNC_CGameMode__OnDialogResponse)(thisptr, playerid, dialogid, response, listitem, szInputtext);
	
	subhook_install(CGameMode__OnDialogResponse_hook);
	return ret;
}

//----------------------------------------------------

// Things that needs to be hooked before netgame initialied
void InstallPreHooks()
{
	SetWeather_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CNetGame__SetWeather), reinterpret_cast<void*>(HOOK_CNetGame__SetWeather), static_cast<subhook_options_t>(NULL));
	subhook_install(SetWeather_hook);
	
	SetGravity_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CNetGame__SetGravity), reinterpret_cast<void*>(HOOK_CNetGame__SetGravity), static_cast<subhook_options_t>(NULL));
	subhook_install(SetGravity_hook);

	Namecheck_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_ContainsInvalidChars), reinterpret_cast<void*>(HOOK_ContainsInvalidChars), static_cast<subhook_options_t>(NULL));
	subhook_install(Namecheck_hook);

	amx_Register_hook = subhook_new(reinterpret_cast<void*>(*(DWORD*)((DWORD)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4))), reinterpret_cast<void*>(HOOK_amx_Register), static_cast<subhook_options_t>(NULL));
	subhook_install(amx_Register_hook);
	
	query_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_ProcessQueryPacket), reinterpret_cast<void*>(HOOK_ProcessQueryPacket), static_cast<subhook_options_t>(NULL));
	subhook_install(query_hook);

	CVehicle__Respawn_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CVehicle__Respawn), reinterpret_cast<void*>(HOOK_CVehicle__Respawn), static_cast<subhook_options_t>(NULL));
	subhook_install(CVehicle__Respawn_hook);

	// Callback hooks
	CGameMode__OnPlayerConnect_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerConnect), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerConnect), static_cast<subhook_options_t>(NULL));
	subhook_install(CGameMode__OnPlayerConnect_hook);
	
	CGameMode__OnPlayerDisconnect_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerDisconnect), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerDisconnect), static_cast<subhook_options_t>(NULL));
	subhook_install(CGameMode__OnPlayerDisconnect_hook);

	CGameMode__OnPlayerSpawn_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerSpawn), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerSpawn), static_cast<subhook_options_t>(NULL));
	subhook_install(CGameMode__OnPlayerSpawn_hook);

	CGameMode__OnPlayerStreamIn_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerStreamIn), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerStreamIn), static_cast<subhook_options_t>(NULL));
	subhook_install(CGameMode__OnPlayerStreamIn_hook);

	CGameMode__OnPlayerStreamOut_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerStreamOut), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerStreamOut), static_cast<subhook_options_t>(NULL));
	subhook_install(CGameMode__OnPlayerStreamOut_hook);

	CGameMode__OnDialogResponse_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnDialogResponse), reinterpret_cast<void*>(HOOK_CGameMode__OnDialogResponse), static_cast<subhook_options_t>(NULL));
	subhook_install(CGameMode__OnDialogResponse_hook);

	if(CAddress::ADDR_RecordingDirectory)
	{
		strcpy(gRecordingDataPath, "scriptfiles/%s.rec");
		Unlock((void*)CAddress::ADDR_RecordingDirectory, 5);
		*(DWORD*)(CAddress::ADDR_RecordingDirectory + 1) = (DWORD)&gRecordingDataPath;
	}
}

// Things that needs to be hooked after netgame initialied
void InstallPostHooks()
{
	CSAMPFunctions::PostInitialize();

	if (CServer::Get()->m_bIncreaseRakNetInternalPlayers)
		CSAMPFunctions::Start(MAX_PLAYERS, 0, CServer::Get()->m_iRakNetInternalSleepTime, static_cast<unsigned short>(CSAMPFunctions::GetIntVariable("port")), CSAMPFunctions::GetStringVariable("bind"));

	// Recreate pools
	CServer::Get()->pGangZonePool = new CGangZonePool();

#ifdef NEW_PICKUP_SYSTEM
	// Recreate Pickup pool
	CServer::Get()->pPickupPool = new CYSFPickupPool();
#endif
	// Re-init a few RPCs
	InitRPCs();

	logprintf_hook = subhook_new(reinterpret_cast<void*>(ppPluginData[PLUGIN_DATA_LOGPRINTF]), reinterpret_cast<void*>(HOOK_logprintf), static_cast<subhook_options_t>(NULL));
	subhook_install(logprintf_hook);

	// logprintf("YSF - pNetGame: 0x%X, pConsole: 0x%X, pRakServer: 0x%X", pNetGame, pConsole, pRakServer);
}

void UninstallHooks()
{
	SUBHOOK_REMOVE(SetWeather_hook);
	SUBHOOK_REMOVE(SetGravity_hook);
	SUBHOOK_REMOVE(Namecheck_hook);
	SUBHOOK_REMOVE(amx_Register_hook);
	SUBHOOK_REMOVE(query_hook);
	SUBHOOK_REMOVE(logprintf_hook);
	SUBHOOK_REMOVE(CVehicle__Respawn_hook);
	SUBHOOK_REMOVE(CGameMode__OnPlayerConnect_hook);
	SUBHOOK_REMOVE(CGameMode__OnPlayerDisconnect_hook);
	SUBHOOK_REMOVE(CGameMode__OnPlayerSpawn_hook);
	SUBHOOK_REMOVE(CGameMode__OnPlayerStreamIn_hook);
	SUBHOOK_REMOVE(CGameMode__OnPlayerStreamOut_hook);
	SUBHOOK_REMOVE(CGameMode__OnDialogResponse_hook);

	// Close log file
	if (g_fLog)
	{
		fclose(g_fLog);
		g_fLog = NULL;
	}
}
