#include <stdarg.h>
#include "includes/net.h"
#include "includes/platform.h"

#include "Hooks.h"
#include "CPlugin.h"
#include "CServer.h"
#include "CConfig.h"
#include "CFunctions.h"
#include "CCallbackManager.h"
#include "Memory.h"
#include "Natives.h"
#include "Globals.h"
#include "Utils.h"
#include "RPCs.h"
#include "subhook/subhook.h"

extern "C" {
#include "subhook/subhook_private.h"
}

//----------------------------------------------------
template <class Func>
class FuncHook
{
	subhook_t hook;

	class hook_restorer
	{
		subhook_t hook;

	public:
		hook_restorer(subhook_t hook) : hook(hook)
		{
			subhook_remove(hook);
		}

		~hook_restorer()
		{
			subhook_install(hook);
		}
	};

	class hook_restorer_dst
	{
		subhook_t hook;
		void *dst;
		void *olddst;

	public:
		hook_restorer_dst(subhook_t hook, void *dst, void *olddst) : hook(hook), dst(dst), olddst(olddst)
		{
			hook->dst = dst;
			subhook_remove(hook);
		}

		~hook_restorer_dst()
		{
			subhook_install(hook);
			hook->dst = olddst;
		}
	};

public:
	FuncHook() = default;

	FuncHook(const ADDR<Func> &addr, Func dst) : hook(
		subhook_new(reinterpret_cast<void*>(addr.get()), reinterpret_cast<void*>(dst), {})
	)
	{
		subhook_install(hook);
	}

	FuncHook(Func addr, Func dst) : hook(
		subhook_new(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(dst), {})
	)
	{
		subhook_install(hook);
	}

	template <class... Args>
	auto operator()(Args&&... args) -> decltype(static_cast<Func>(nullptr)(std::forward<Args>(args)...))
	{
		auto tptr = subhook_get_trampoline(hook);
		if(!tptr)
		{
			auto src = subhook_get_src(hook);
			auto dst = subhook_read_dst(src);
			auto olddst = subhook_get_dst(hook);
			if(dst != olddst)
			{
				hook_restorer_dst restorer(hook, dst, olddst);
				return reinterpret_cast<Func>(src)(std::forward<Args>(args)...);
			}else if(!dst)
			{
				return reinterpret_cast<Func>(src)(std::forward<Args>(args)...);
			}else{
				hook_restorer restorer(hook);
				return reinterpret_cast<Func>(src)(std::forward<Args>(args)...);
			}
		}
		return reinterpret_cast<Func>(tptr)(std::forward<Args>(args)...);
	}

	FuncHook<Func> &operator=(const FuncHook<Func> &obj) = default;

	void free()
	{
		subhook_remove(hook);
		subhook_free(hook);
		hook = {};
	}
};

#define DEFINE_HOOK(name) static FuncHook<decltype(CAddress::FUNC_##name.get())> name##_hook;

DEFINE_HOOK(CNetGame__SetWeather);
DEFINE_HOOK(CNetGame__SetGravity);
DEFINE_HOOK(ContainsInvalidChars);
static FuncHook<decltype(&amx_Register)> amx_Register_hook;
subhook_t logprintf_hook;
DEFINE_HOOK(ProcessQueryPacket);
DEFINE_HOOK(CVehicle__Respawn);
DEFINE_HOOK(ReplaceBadChars);

// Callback hooks instead of using SAMP GDK
DEFINE_HOOK(CGameMode__OnPlayerConnect);
DEFINE_HOOK(CGameMode__OnPlayerDisconnect);
DEFINE_HOOK(CGameMode__OnPlayerSpawn);
DEFINE_HOOK(CGameMode__OnPlayerStreamIn);
DEFINE_HOOK(CGameMode__OnPlayerStreamOut);
DEFINE_HOOK(CGameMode__OnDialogResponse);

#ifdef SAMP_03DL
DEFINE_HOOK(ClientJoin);
DEFINE_HOOK(AddSimpleModel);
#endif

//----------------------------------------------------
char gRecordingDataPath[MAX_PATH];

///////////////////////////////////////////////////////////////
// Hooks //
///////////////////////////////////////////////////////////////

#ifdef _WIN32
#define HOOK_THISCALL(NAME, ARG1, ...) FASTCALL NAME(ARG1, void *_padding, __VA_ARGS__)
#else
#define HOOK_THISCALL(NAME, ARG1, ...) CDECL NAME(ARG1, ## __VA_ARGS__)
#endif

void HOOK_THISCALL(HOOK_CNetGame__SetWeather, void *thisptr, BYTE weatherid)
{
	auto &pool = CServer::Get()->PlayerPool;
	for (int i = 0; i != MAX_PLAYERS; ++i)
	{
		if (IsPlayerConnected(i))
			pool.Extra(i).byteWeather = weatherid;
	}

	CNetGame__SetWeather_hook(thisptr, weatherid);
}

//----------------------------------------------------

void HOOK_THISCALL(HOOK_CNetGame__SetGravity, void *thisptr, float gravity)
{
	auto &pool = CServer::Get()->PlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
	{
		if (IsPlayerConnected(i))
			pool.Extra(i).fGravity = gravity;
	}

	CNetGame__SetGravity_hook(thisptr, gravity);
}

//----------------------------------------------------

// Custom name check
bool HOOK_ContainsInvalidChars(char * szString)
{
	return !CPlugin::Get()->IsValidNick(szString);
}

//----------------------------------------------------

int AMXAPI HOOK_amx_Register(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
	// amx_Register hook for redirect natives
	static bool bNativesHooked = false;

	if (!bNativesHooked && CPlugin::IsInitialized())
	{
		for(int i = 0; nativelist[i].name; i++)
		{
			if (ApplyHooks(const_cast<AMX_NATIVE_INFO&>(nativelist[i]))) bNativesHooked = true;

			if (i == number - 1) break;
		}
	}
	
	return amx_Register_hook(amx, nativelist, number);
}

//----------------------------------------------------

bool THISCALL CHookRakServer::Send(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast)
{
	RakNet::BitStream *newParams = CPlugin::Get()->BuildSyncData(parameters, static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(playerId)));
	if(newParams)
	{
		if(newParams->GetNumberOfBitsUsed() == 0)
		{
			// Ignore packet
			delete newParams;
			return false;
		}
		bool ret = CSAMPFunctions::Send(newParams, priority, reliability, orderingChannel, playerId, broadcast);
		delete newParams;
		return ret;
	} else {
		return CSAMPFunctions::Send(parameters, priority, reliability, orderingChannel, playerId, broadcast);
	}
}

//----------------------------------------------------

bool THISCALL CHookRakServer::RPC_2(void* ppRakServer, BYTE* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp)
{
	if (!CPlugin::Get()->RebuildRPCData(*uniqueID, parameters, static_cast<WORD>(CSAMPFunctions::GetIndexFromPlayerID(playerId)))) return 1;

	auto &pool = CServer::Get()->PlayerPool;
	int broadcastStatus = CPlugin::Get()->GetExclusiveBroadcast();
	if(broadcastStatus >= 1)
	{
		if(broadcastStatus > 1)
		{
			CPlugin::Get()->SetExclusiveBroadcast(broadcastStatus - 2, false);
		}
		for(WORD i = 0; i != MAX_PLAYERS; ++i)
		{
			if(IsPlayerConnected(i) && pool.Extra(i).bBroadcastTo)
			{
				CSAMPFunctions::RPC(uniqueID, parameters, priority, reliability, orderingChannel, CSAMPFunctions::GetPlayerIDFromIndex(i), false, shiftTimestamp);
			}
		}

		return 1;
	}else{
		if(broadcastStatus < 0)
		{
			CPlugin::Get()->SetExclusiveBroadcast(broadcastStatus + 2, false);
		}
		return CSAMPFunctions::RPC(uniqueID, parameters, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp);
	}
}

//----------------------------------------------------

void THISCALL CHookRakServer::AddToBanList(void* ppRakServer, const char *IP, unsigned int milliseconds)
{
	CPlugin::Get()->BanIP(IP);
	return CSAMPFunctions::AddToBanList(IP, milliseconds);
}

void THISCALL CHookRakServer::RemoveFromBanList(void* ppRakServer, const char *IP)
{
	CPlugin::Get()->UnbanIP(IP);
	return CSAMPFunctions::RemoveFromBanList(IP);
}

void THISCALL CHookRakServer::ClearBanList(void* ppRakServer)
{
	CPlugin::Get()->ClearBans();
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
		auto &data = CServer::Get()->PlayerPool.Extra(playerid);

		// AFK
		if (IsPlayerUpdatePacket(packetId))
		{
			data.LastUpdateTick = default_clock::now();
			data.bEverUpdated = true;
		}

		switch(packetId)
		{
			case ID_PLAYER_SYNC:
			{
				if(p->data && p->length >= sizeof(CSyncData) + 1)
				{
					CSyncData *pSyncData = reinterpret_cast<CSyncData*>(&p->data[1]);

					if(CPlugin::Get()->IsNightVisionFixEnabled())
					{
						// Fix nightvision and infrared sync
						if(pSyncData->byteWeapon == WEAPON_NIGHTVISION || pSyncData->byteWeapon == WEAPON_INFRARED)
						{
							pSyncData->wKeys &= ~(KEY_FIRE | KEY_ACTION);
							pSyncData->byteWeapon = 0;
							pSyncData->_unk_ = 0;
						}
					}

					// Store surfing info because server reset it when player surfing on player object
					data.wSurfingInfo = pSyncData->wSurfingInfo;
				}
				break;
			}
			case ID_AIM_SYNC:
			{
				if(p->data && p->length >= sizeof(CAimSyncData) + 1)
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
				}
				break;
			}
			case ID_STATS_UPDATE:
			{
				CSAMPFunctions::Packet_StatsUpdate(p);
				CCallbackManager::OnPlayerStatsUpdate(playerid);

				if(p->data && p->length >= 1)
				{
					p->data[0] = 0xFF;
				}
				break;
			}
			case ID_WEAPONS_UPDATE:
			{
				CSAMPFunctions::Packet_WeaponsUpdate(p);
				CCallbackManager::OnPlayerWeaponsUpdate(playerid);
				
				if(p->data && p->length >= 1)
				{
					p->data[0] = 0xFF;
				}
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

void custom_logprintf(const char *msg, ...)
{
	char buffer[1024];
	va_list arguments;
	va_start(arguments, msg);
	vsnprintf(buffer, sizeof(buffer), msg, arguments);
	va_end(arguments);

	if(CCallbackManager::OnServerMessage(buffer))
	{
		reinterpret_cast<FUNC_logprintf>(subhook_get_trampoline(logprintf_hook))("%s", buffer);
	}
}

void *logprintf_indirection()
{
	// If server messages aren't handled, the hook will jump to the trampoline
	if(CPlugin::Get()->IsOnServerMessageEnabled() && CPlugin::Get()->IsMainThread())
	{
		return reinterpret_cast<void*>(&custom_logprintf);
	}else{
		return subhook_get_trampoline(logprintf_hook);
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
// bool ProcessQueryFlood()
// returns 1 if this query could flood
// returns 0 otherwise
bool ProcessQueryFlood(unsigned int binaryAddress, char type, DWORD limit)
{
	static struct {
		DWORD dwLastQueryTick = 0;
		unsigned int lastBinAddr = 0;
	} packetTypeFloodData['z' - 'a' + 1];

	auto &data = packetTypeFloodData[type - 'a'];

	if(limit == -1)
	{
		if(data.lastBinAddr == binaryAddress)
		{
			data.dwLastQueryTick = 0;
		}
		return 0;
	}

	auto tick = GetTickCount();
	if(!data.dwLastQueryTick)
	{
		data.dwLastQueryTick = tick;
		data.lastBinAddr = binaryAddress;
		return 0;
	}
	if(data.lastBinAddr != binaryAddress)
	{
		data.lastBinAddr = binaryAddress;
	}else if(tick - data.dwLastQueryTick < limit)
	{
		data.dwLastQueryTick = tick;
		return 1;
	}
	data.dwLastQueryTick = tick;
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

			char type = data[10];
			switch (type)
			{
				case 'p':	// ping
				{
					if (length == 15)
					{
						if(ProcessQueryFlood(binaryAddress, type, 250)) return 1;
						sendto(s, data, 15, 0, (sockaddr*)&to, sizeof(to));
					}
					break;
				}
				case 'i':	// info
				{
					// We do not process these queries 'query' is 0
					if (!CSAMPFunctions::GetBoolVariable("query")) return 1;
					if (ProcessQueryFlood(binaryAddress, type, 25)) return 1;

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
					WORD wPlayerCount = CPlugin::Get()->GetPlayerCount();
					WORD wMaxPlayers = CPlugin::Get()->GetMaxPlayers();
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
					if (ProcessQueryFlood(binaryAddress, type, 100)) return 1;

					WORD wPlayerCount = CPlugin::Get()->GetPlayerCount();
					if (wPlayerCount > 100) return 1;

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

						auto &pool = CServer::Get()->PlayerPool;
						for (WORD r = 0; r != MAX_PLAYERS; ++r)
						{
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pool.Extra(r).HiddenInQuery())
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
					if (ProcessQueryFlood(binaryAddress, type, 200)) return 1;

					WORD wPlayerCount = CPlugin::Get()->GetPlayerCount();
					if (wPlayerCount > 100) return 1;

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

						auto &pool = CServer::Get()->PlayerPool;
						for (WORD r = 0; r != MAX_PLAYERS; ++r)
						{
							if (IsPlayerConnected(r) && !pPlayerPool->bIsNPC[r] && !pool.Extra(r).HiddenInQuery())
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
					if (ProcessQueryFlood(binaryAddress, type, 50)) return 1;

					CSAMPFunctions::SendRules(s, data, (sockaddr_in*)&to, sizeof(to));
					break;
				}
				case 'x':	// rcon
				{
					if (CPlugin::Get()->IsBanned(inet_ntoa(in)) && !CConfig::Get()->m_bAllowRemoteRCONWithBannedIPs) return 1;
					
					// We do not process these queries 'query' is 0
					if (!CSAMPFunctions::GetBoolVariable("query") || !CSAMPFunctions::GetBoolVariable("rcon")) return 1;
					if (ProcessQueryFlood(binaryAddress, type, 25)) return 1;
					
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

						ProcessQueryFlood(binaryAddress, type, -1);

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

void HOOK_THISCALL(HOOK_CVehicle__Respawn, CVehicle *thisptr)
{
	CSAMPFunctions::RespawnVehicle(thisptr);
}
//----------------------------------------------------

int HOOK_THISCALL(HOOK_CGameMode__OnPlayerConnect, CGameMode *thisptr, cell playerid)
{
#ifndef NEW_PICKUP_SYSTEM
	CPlugin::Get()->AddPlayer(playerid);
#else
	// Initialize pickups
	if (CPlugin::Get()->AddPlayer(playerid))
		CPlugin::Get()->pPickupPool->InitializeForPlayer(playerid);
#endif

	return CGameMode__OnPlayerConnect_hook(thisptr, playerid);
}

//----------------------------------------------------

int HOOK_THISCALL(HOOK_CGameMode__OnPlayerDisconnect, CGameMode *thisptr, cell playerid, cell reason)
{
	int result = CGameMode__OnPlayerDisconnect_hook(thisptr, playerid, reason);
	CPlugin::Get()->RemovePlayer(playerid);
	return result;
}

//----------------------------------------------------

int HOOK_THISCALL(HOOK_CGameMode__OnPlayerSpawn, CGameMode *thisptr, cell playerid)
{
	if (IsPlayerConnected(playerid))
		CServer::Get()->PlayerPool.Extra(playerid).bControllable = true;

	return CGameMode__OnPlayerSpawn_hook(thisptr, playerid);
}

//----------------------------------------------------

int HOOK_THISCALL(HOOK_CGameMode__OnPlayerStreamIn, CGameMode *thisptr, cell playerid, cell forplayerid)
{
	CPlugin::Get()->OnPlayerStreamIn(static_cast<WORD>(playerid), static_cast<WORD>(forplayerid));

	return CGameMode__OnPlayerStreamIn_hook(thisptr, playerid, forplayerid);
}

//----------------------------------------------------

int HOOK_THISCALL(HOOK_CGameMode__OnPlayerStreamOut, CGameMode *thisptr, cell playerid, cell forplayerid)
{
	CPlugin::Get()->OnPlayerStreamOut(static_cast<WORD>(playerid), static_cast<WORD>(forplayerid));
	
	return CGameMode__OnPlayerStreamOut_hook(thisptr, playerid, forplayerid);
}

//----------------------------------------------------

int HOOK_THISCALL(HOOK_CGameMode__OnDialogResponse, CGameMode *thisptr, cell playerid, cell dialogid, cell response, cell listitem, char *szInputtext)
{
	int ret = -1;
	if (IsPlayerConnected(playerid))
	{
		auto &data = CServer::Get()->PlayerPool.Extra(playerid);
		if (CConfig::Get()->m_bDialogProtection && data.wDialogID != dialogid)
		{
			logprintf("YSF: Might dialog hack has been detected for player %s(%d) - which should be: %d, dialogid: %d", GetPlayerName(playerid), playerid, data.wDialogID, dialogid);
			ret = 1;
		}
		data.wDialogID = -1;
	}

	if(ret == -1)
		ret = CGameMode__OnDialogResponse_hook(thisptr, playerid, dialogid, response, listitem, szInputtext);
	
	return ret;
}

//----------------------------------------------------

void HOOK_ReplaceBadChars(char *szString)
{
	if(CPlugin::Get()->ChatTextReplacementToggled())
	{
		ReplaceBadChars_hook(szString);
	}
}

//----------------------------------------------------

#ifdef SAMP_03DL
void CDECL HOOK_ClientJoin(RPCParameters *rpcParams)
{
	//4057 - 0.3.7-R2, 4062 - 0.3.DL-R1

	/*int *ver = (int*)rpcParams->input;
	logprintf("Client joined with version %d.", *ver);

	unsigned char namelen = rpcParams->input[5];

	unsigned int *resp = (unsigned int*)rpcParams->input+6+namelen;
	logprintf("Resp %d.", *resp);
	*resp = *resp ^ *ver ^ 4062;
	*ver = 4062;*/

	ClientJoin_hook(rpcParams);
}

int HOOK_THISCALL(HOOK_AddSimpleModel, CArtInfo *pArtInfo, MODEL_TYPE type, int virtualworld, int baseid, int newid, char *dffname, char *txdname, char timeon, char timeoff)
{
	CPlugin &plugin = *CPlugin::Get();

	CModelInfo *model = plugin.FindCachedModelInfo(dffname, txdname);

	int result;
	if (model == nullptr)
	{
		result = AddSimpleModel_hook(pArtInfo, type, virtualworld, baseid, newid, dffname, txdname, timeon, timeoff);

		if (result > 0 || (result == 0 && pArtInfo->artList.dwCapacity > 0 && pArtInfo->artList.pModelList[0]->dwNewId == newid))
		{
			model = pArtInfo->artList.pModelList[result];
			plugin.CacheModelInfo(model);
		}
	} else {
		// server will never delete this
		CModelInfo *newModel = new CModelInfo(*model);
		newModel->bType = type;
		newModel->dwVirtualWorld = virtualworld;
		newModel->dwBaseId = baseid;
		newModel->dwNewId = newid;
		newModel->bTimeOn = timeon;
		newModel->bTimeOff = timeoff;
		result = CAddress::FUNC_DynamicListInsert(&pArtInfo->artList, newModel);
	}

	return result;
}
#endif

//----------------------------------------------------

void dummy_hook_function()
{
	static bool printed = false;
	if(!printed)
	{
		printed = true;
		logprintf("Trampoline function for a non-existent hook was called.");
	}
}

template <class TFunc, class THook>
FuncHook<TFunc> Hook(const char *name, ADDR<TFunc> &func, THook hook)
{
	//logprintf("Hooking %x", (DWORD)func);
	FuncHook<TFunc> var;
	if(!func)
	{
		logprintf("Address for %s was not found. Some functions may not be working correctly.", name);
		var = FuncHook<TFunc>(reinterpret_cast<TFunc>(dummy_hook_function), reinterpret_cast<TFunc>(hook));
	}else{
		var = FuncHook<TFunc>(func, reinterpret_cast<TFunc>(hook));
	}
	return var;
}

#define HOOK(name) Hook(#name, CAddress::FUNC_##name, HOOK_##name)

// Things that needs to be hooked before netgame initialied
void InstallPreHooks()
{
	if(CConfig::Get()->m_bPassiveMode) return;

	CNetGame__SetWeather_hook = HOOK(CNetGame__SetWeather);
	CNetGame__SetGravity_hook = HOOK(CNetGame__SetGravity);
	ContainsInvalidChars_hook = HOOK(ContainsInvalidChars);
	amx_Register_hook = FuncHook<decltype(&amx_Register)>(((decltype(&amx_Register)*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Register], HOOK_amx_Register);
	
	ProcessQueryPacket_hook = HOOK(ProcessQueryPacket);
	CVehicle__Respawn_hook = HOOK(CVehicle__Respawn);
	ReplaceBadChars_hook = HOOK(ReplaceBadChars);
	
#ifdef SAMP_03DL
	//ClientJoin_hook = HOOK(ClientJoin);
	//AddSimpleModel_hook = HOOK(AddSimpleModel);
#endif
	
	// Callback hooks
	CGameMode__OnPlayerConnect_hook = HOOK(CGameMode__OnPlayerConnect);
	CGameMode__OnPlayerDisconnect_hook = HOOK(CGameMode__OnPlayerDisconnect);
	CGameMode__OnPlayerSpawn_hook = HOOK(CGameMode__OnPlayerSpawn);
	CGameMode__OnPlayerStreamIn_hook = HOOK(CGameMode__OnPlayerStreamIn);
	CGameMode__OnPlayerStreamOut_hook = HOOK(CGameMode__OnPlayerStreamOut);
	CGameMode__OnDialogResponse_hook = HOOK(CGameMode__OnDialogResponse);
	
	if(CAddress::ADDR_RecordingDirectory)
	{
		strcpy(gRecordingDataPath, "scriptfiles/%s.rec");
		CAddress::ADDR_RecordingDirectory.unlock();
		*CAddress::ADDR_RecordingDirectory = gRecordingDataPath;
	}
}

// Things that needs to be hooked after netgame initialied
void InstallPostHooks()
{
	CSAMPFunctions::PostInitialize(CConfig::Get()->m_bPassiveMode);

	if (CConfig::Get()->m_bIncreaseRakNetInternalPlayers)
		CSAMPFunctions::Start(MAX_PLAYERS, 0, CConfig::Get()->m_iRakNetInternalSleepTime, static_cast<unsigned short>(CSAMPFunctions::GetIntVariable("port")), CSAMPFunctions::GetStringVariable("bind"));

	if(CConfig::Get()->m_bPassiveMode) return;

	// Recreate pools

	if (CConfig::Get()->m_bUsePerPlayerGangZones)
	{
		CPlugin::Get()->pGangZonePool = new CGangZonePool();
	}

#ifdef NEW_PICKUP_SYSTEM
	// Recreate Pickup pool
	CPlugin::Get()->pPickupPool = new CYSFPickupPool();
#endif
	// Re-init a few RPCs
	InitRPCs();

	static unsigned char HOOK_logprintf[7] = {0xE8, 0xCC, 0xCC, 0xCC, 0xCC, 0xFF, 0xE0}; //call rel, jmp eax
	*reinterpret_cast<uintptr_t*>(HOOK_logprintf + 1) = reinterpret_cast<uintptr_t>(&logprintf_indirection) - reinterpret_cast<uintptr_t>(HOOK_logprintf + 5);
	subhook_unprotect(HOOK_logprintf, sizeof(HOOK_logprintf));

	logprintf_hook = subhook_new(reinterpret_cast<void*>(ppPluginData[PLUGIN_DATA_LOGPRINTF]), HOOK_logprintf, {});
	subhook_install(logprintf_hook);
	if(!subhook_get_trampoline(logprintf_hook))
	{
		subhook_remove(logprintf_hook);

		logprintf("Trampoline function for logprintf could not be generated. OnServerMessage will not be usable.");
	}

	// logprintf("YSF - pNetGame: 0x%X, pConsole: 0x%X, pRakServer: 0x%X", pNetGame, pConsole, pRakServer);
}

void UninstallHooks()
{
	if(CConfig::Get()->m_bPassiveMode) return;

	CNetGame__SetWeather_hook.free();
	CNetGame__SetGravity_hook.free();
	ContainsInvalidChars_hook.free();
	amx_Register_hook.free();
	ProcessQueryPacket_hook.free();
	subhook_remove(logprintf_hook);
	subhook_free(logprintf_hook);
	CVehicle__Respawn_hook.free();
	CGameMode__OnPlayerConnect_hook.free();
	CGameMode__OnPlayerDisconnect_hook.free();
	CGameMode__OnPlayerSpawn_hook.free();
	CGameMode__OnPlayerStreamIn_hook.free();
	CGameMode__OnPlayerStreamOut_hook.free();
	CGameMode__OnDialogResponse_hook.free();
}
