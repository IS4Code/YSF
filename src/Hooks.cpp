
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

extern void *pAMXFunctions;

subhook_t SetWeather_hook;
subhook_t SetGravity_hook;
subhook_t Namecheck_hook;
subhook_t amx_Register_hook;
subhook_t logprintf_hook;
subhook_t query_hook;

AMX_NATIVE pDestroyObject = NULL, pDestroyPlayerObject = NULL, pTogglePlayerControllable = NULL, pSetPlayerWorldBounds = NULL,
	pSetPlayerTeam = NULL, pSetPlayerSkin = NULL, pSetPlayerFightingStyle = NULL, pSetPlayerName = NULL, pSetVehicleToRespawn = NULL, 
	pChangeVehicleColor = NULL, pDestroyVehicle = NULL, pAttachObjectToPlayer = NULL;

char gRecordingDataPath[MAX_PATH];

///////////////////////////////////////////////////////////////
// Hooks //
///////////////////////////////////////////////////////////////
/* I was unable to hook this function :(
This needed in future to fix /rcon weather id do not update GetPlayerWeather
class CHookedNetgame
{
	static void __thiscall HOOK_CNetGame__SetWeather(void *pNetGame_, BYTE byteWeather);
	static void __thiscall HOOK_CNetGame__SetGravity(void *pNetGame_, float fGravity);

};

//----------------------------------------------------
typedef void (__thiscall *FUNC_CNetGame__SetWeather)(void *pNetGame_, BYTE byteWeather);
void __thiscall HOOK_CNetGame__SetWeather(void *pNetGame_, BYTE byteWeather)
{
	subhook_remove(SetWeather_hook);

	((FUNC_CNetGame__SetWeather)CAddress::FUNC_CNetGame__SetWeather)(pNetGame, byteWeather);

	for (WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if (IsPlayerConnectedEx(i))
			pPlayerData[i]->byteWeather = byteWeather;
	}

	subhook_install(SetWeather_hook);
}


//typedef BYTE (*FUNC_GetPacketID)(Packet *p);
//----------------------------------------------------
typedef void (__thiscall *FUNC_CNetGame__SetGravity)(void *pNetGame_, float fGravity);
void HOOK_CNetGame__SetGravity(void *pNetGame_, float fGravity)
{
	((FUNC_CNetGame__SetGravity)CAddress::FUNC_CNetGame__SetWeather)(pNetGame_, fGravity);

	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if(IsPlayerConnectedEx(i))
			pPlayerData[i]->fGravity = fGravity; 
	}
}
*/
//----------------------------------------------------
// Custom name check
bool HOOK_ContainsInvalidChars(char * szString)
{
	return !pServer->IsValidNick(szString);
}

//----------------------------------------------------

typedef BYTE (*FUNC_amx_Register)(AMX *amx, AMX_NATIVE_INFO *nativelist, int number);
int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	// amx_Register hook for redirect natives
	static bool g_bNativesHooked = false;

	if (!g_bNativesHooked && pServer)
	{
		int i = 0;
		while (nativelist[i].name)
		{
			if(!pDestroyPlayerObject && !strcmp(nativelist[i].name, "DestroyObject"))
				pDestroyObject = nativelist[i].func;

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

			if(!pAttachObjectToPlayer && !strcmp(nativelist[i].name, "AttachObjectToPlayer"))
				pAttachObjectToPlayer = nativelist[i].func;

			//logprintf("native %s", nativelist[i].name);
			int x = 0;
			
			while (RedirectedNatives[x].name)
			{
				//logprintf("asdasd %s", RedirectedNatives[x].name);
				if (!strcmp(nativelist[i].name, RedirectedNatives[x].name))
				{
					if (!g_bNativesHooked) g_bNativesHooked = true;
				
					//logprintf("native: %s, %s", nativelist[i].name, RedirectedNatives[x].name);
					nativelist[i].func = RedirectedNatives[x].func;
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

	logprintf("id: %d - playerid: %d, sendto. %d", id, playerid, pRakServer->GetIndexFromPlayerID(playerId));
	*/
	RebuildSyncData(parameters, static_cast<WORD>(pRakServer->GetIndexFromPlayerID(playerId)));

	return CSAMPFunctions::Send(ppRakServer, parameters, priority, reliability, orderingChannel, playerId, broadcast);
}

//----------------------------------------------------

bool THISCALL CHookRakServer::RPC_2(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
{
	RebuildRPCData(*uniqueID, parameters, static_cast<WORD>(pRakServer->GetIndexFromPlayerID(playerId)));

	return CSAMPFunctions::RPC(ppRakServer, uniqueID, parameters, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp);
}

//----------------------------------------------------

Packet* THISCALL CHookRakServer::Receive(void* ppRakServer)
{
	Packet* p = CSAMPFunctions::Receive(ppRakServer);
	BYTE packetId = GetPacketID(p);
	if(packetId == 0xFF) return p;

	WORD playerid = p->playerIndex;

	//logprintf("Receive packet playerid: %d, id: %d", playerid, packetId);
	
	if (IsPlayerConnectedEx(playerid))
	{
		// AFK
		if (IsPlayerUpdatePacket(packetId))
		{
			pPlayerData[playerid]->dwLastUpdateTick = GetTickCount();
			pPlayerData[playerid]->bEverUpdated = true;
		}

		switch(packetId)
		{
			case ID_PLAYER_SYNC:
			{
				CSyncData *pSyncData = reinterpret_cast<CSyncData*>(&p->data[1]);

				if(pServer->IsNightVisionFixEnabled())
				{
					// Fix nightvision and infrared sync
					if (pSyncData->byteWeapon == WEAPON_NIGHTVISION || pSyncData->byteWeapon == WEAPON_INFRARED)
					{
						pSyncData->wKeys &= ~4;
						pSyncData->byteWeapon = 0;
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
	subhook_remove(logprintf_hook);

	char buffer[1024];
	va_list arguments;
	va_start(arguments, msg);
	vsnprintf(buffer, sizeof(buffer), msg, arguments);
	va_end(arguments);
	// CCallbackManager::OnServerMessage(buffer)
	if(true)
	{
		// Fix crash caused by % symbol (by default this crash at /rcon varlist)
		int i = 0;
		bool bDoNotPrint = false;
		while(buffer[i])
		{
			if(buffer[i++] == '%')
			{
				bDoNotPrint = true;
 				break;
			}
		}

		if(!bDoNotPrint)
			logprintf(buffer);
	
		if (bRconSocketReply) 
			RconSocketReply(buffer);
	}

	subhook_install(logprintf_hook);
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

					char* szHostname = CSAMPFunctions::GetStringVariable("hostname");
					size_t dwHostnameLen = strlen(szHostname);
					if (dwHostnameLen > 50) dwHostnameLen = 50;

					char* szGameMode = CSAMPFunctions::GetStringVariable("gamemodetext");
					size_t dwGameModeLen = strlen(szGameMode);
					if (dwGameModeLen > 30) dwGameModeLen = 30;

					char* szLanguage = CSAMPFunctions::GetStringVariable("language");
					char* szMapName = (!szLanguage[0]) ? CSAMPFunctions::GetStringVariable("mapname") : szLanguage;

					size_t dwMapNameLen = strlen(szMapName);
					if (dwMapNameLen > 30) dwMapNameLen = 30;

					WORD wPlayerCount = pServer->GetPlayerCount();
//					CPlayerPool* pPlayerPool = pNetGame->pPlayerPool;

					WORD wMaxPlayers = pServer->GetMaxPlayers_();

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
								szName = GetPlayerName_(r);
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
					if (!CSAMPFunctions::GetBoolVariable("query")) return 1;
					if (CheckQueryFlood(binaryAddress)) return 1;

					CSAMPFunctions::SendRules(s, data, (sockaddr_in*)&to, sizeof(to));
					break;
				}
				case 'x':	// rcon
				{
					if (pRakServer/* && pRakServer->IsBanned(inet_ntoa(in))*/) return 1;
					
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
							//logprintf("onremoterconpacket %d, %d, %s - %s", binaryAddress, port, szPassword, szCommand);
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

#ifdef _WIN32

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
	/*
		SetWeather_hook = subhook_new((void *)CAddress::FUNC_CNetGame__SetWeather, (void *)CHookedNetgame::HOOK_CNetGame__SetWeather);
		subhook_install(SetWeather_hook);
		
		SetGravity_hook = subhook_new((void *)CAddress::FUNC_CNetGame__SetGravity, (void *)HOOK_CNetGame__SetGravity);
		subhook_install(SetGravity_hook);
		*/
		Namecheck_hook = subhook_new((void*)CAddress::FUNC_ContainsInvalidChars, (void *)HOOK_ContainsInvalidChars);
		subhook_install(Namecheck_hook);

		amx_Register_hook = subhook_new((void*)*(DWORD*)((DWORD)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
		subhook_install(amx_Register_hook);

		query_hook = subhook_new((void*)CAddress::FUNC_ProcessQueryPacket, (void*)HOOK_ProcessQueryPacket);
		subhook_install(query_hook);

		logprintf_hook = subhook_new((void*)ppPluginData[PLUGIN_DATA_LOGPRINTF], (void*)HOOK_logprintf);
		subhook_install(logprintf_hook);

		if(CAddress::FUNC_CVehicle__Respawn)
		{
#ifdef WIN32
			InstallJump(CAddress::FUNC_CVehicle__Respawn, (void*)HOOK_CVehicle__Respawn);
#else
			InstallJump(CAddress::FUNC_CVehicle__Respawn, (void*)CSAMPFunctions::RespawnVehicle);
#endif
		}

		if(CAddress::ADDR_RecordingDirectory)
		{
			strcpy(gRecordingDataPath, "scriptfiles/%s.rec");
			Unlock((void*)CAddress::ADDR_RecordingDirectory, 5);
			*(DWORD*)(CAddress::ADDR_RecordingDirectory + 1) = (DWORD)&gRecordingDataPath;
		}
	}	
}

// Things that needs to be hooked after netgame initialied
void InstallPostHooks()
{
	CSAMPFunctions::PostInitialize();

	// SetMaxPlayers() fix
	//pRakServer->Start(MAX_PLAYERS, 0, 5, static_cast<unsigned short>(pServer->GetIntVariable("port")), pServer->GetStringVariable("bind"));
		
	// Recreate pools
	pServer->pGangZonePool = new CGangZonePool();

#ifdef NEW_PICKUP_SYSTEM
	// Recreate Pickup pool
	pNetGame->pPickupPool = new CPickupPool();
#endif
	// Re-init some RPCs
	InitRPCs();

	//logprintf("YSF - pNetGame: 0x%X, pConsole: 0x%X, pRakServer: 0x%X", pNetGame, pConsole, pRakServer);
}

void UninstallHooks()
{
	if(SetWeather_hook)
	{
		subhook_remove(SetWeather_hook);
		subhook_free(SetWeather_hook);
	}
	
	if(SetGravity_hook)
	{
		subhook_remove(SetGravity_hook);
		subhook_free(SetGravity_hook);
	}
	
	if(Namecheck_hook)
	{
		subhook_remove(Namecheck_hook);
		subhook_free(Namecheck_hook);
	}

	if(amx_Register_hook)
	{
		subhook_remove(amx_Register_hook);
		subhook_free(amx_Register_hook);
	}

	if(query_hook)
	{
		subhook_remove(query_hook);
		subhook_free(query_hook);
	}

	if(logprintf_hook)
	{
		subhook_remove(logprintf_hook);
		subhook_free(logprintf_hook);
	}
}
