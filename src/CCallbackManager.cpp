#include "CCallbackManager.h"
#include "Globals.h"
#include "includes/platform.h"
#include "includes/net.h"

std::set<AMX *> CCallbackManager::m_setAMX;

void CCallbackManager::RegisterAMX(AMX *pAMX)
{	
	// Add gamemode to the first position in set
	if (pNetGame && pNetGame->pGameModePool && &pNetGame->pGameModePool->amx == pAMX)
	{
		m_setAMX.insert(m_setAMX.begin(), pAMX);
	}
	else
	{
		m_setAMX.insert(m_setAMX.end(), pAMX);
	}
}

void CCallbackManager::UnregisterAMX(AMX *pAMX)
{
	// Remove the amx from the pointers list
	m_setAMX.erase(pAMX);
}

void CCallbackManager::OnPlayerEnterGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnPlayerEnterGangZone", &idx))
		{
			amx_Push(iter, static_cast<cell>(zoneid));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerLeaveGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnPlayerLeaveGangZone", &idx))
		{
			amx_Push(iter, static_cast<cell>(zoneid));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerEnterPlayerGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnPlayerEnterPlayerGangZone", &idx))
		{
			amx_Push(iter, static_cast<cell>(zoneid));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerLeavePlayerGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnPlayerLeavePlayerGangZone", &idx))
		{
			amx_Push(iter, static_cast<cell>(zoneid));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerPauseStateChange(WORD playerid, bool pausestate)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnPlayerPauseStateChange", &idx))
		{
			amx_Push(iter, static_cast<cell>(pausestate));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerDeath(WORD playerid, WORD killerid, BYTE reason)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnPlayerDeath", &idx))
		{
			amx_Push(iter, static_cast<cell>(reason));
			amx_Push(iter, static_cast<cell>(killerid));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerSpawn(WORD playerid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnPlayerSpawn", &idx))
		{
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerPickedUpPickup(WORD playerid, WORD pickupid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnPlayerPickUpPickup", &idx))
		{
			amx_Push(iter, static_cast<cell>(pickupid));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerPickedUpPlayerPickup(WORD playerid, WORD pickupid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnPlayerPickUpPlayerPickup", &idx))
		{
			amx_Push(iter, static_cast<cell>(pickupid));
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);

			if (!ret) return;
		}
	}
}

bool CCallbackManager::OnServerMessage(const char* message)
{
	if (!message) return false;

	// Fix crash caused by % symbol (by default this crash happens in /rcon varlist)
	/*size_t len = strlen(message);
	char* msg = new char[len + 1];
	strncpy(msg, message, len + 1);
	size_t i = 0;
	while (msg[i])
	{
		if (msg[i] == '%')
		{
			msg[i] = '#';
		}
		i++;
	}*/

	int idx;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnServerMessage", &idx))
		{
			cell amx_addr;
			amx_PushString(iter, &amx_addr, nullptr, message, 0, 0);
			amx_Exec(iter, &ret, idx);
			amx_Release(iter, amx_addr);

			if (!ret) return false;
		}
	}

	//delete[] msg;
	return true;
}

bool CCallbackManager::OnRemoteRCONPacket(unsigned int binaryAddress, int port, char *password, bool success, char* command)
{
	in_addr in;
	in.s_addr = binaryAddress;

	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if(!amx_FindPublic(iter, "OnRemoteRCONPacket", &idx))
		{
			cell amx_addr;
			amx_PushString(iter, &amx_addr, nullptr, command, 0, 0);
			amx_Push(iter, static_cast<cell>(success));
			amx_PushString(iter, &amx_addr, nullptr, password, 0, 0);
			amx_Push(iter, static_cast<cell>(port));
			amx_PushString(iter, &amx_addr, nullptr, inet_ntoa(in), 0, 0);
			amx_Exec(iter, &ret, idx);
			amx_Release(iter, amx_addr);

			if (!ret) return 0;
		}
	}
	return static_cast<int>(ret) != 0;
}

void CCallbackManager::OnPlayerStatsAndWeaponsUpdate(WORD playerid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnPlayerStatsAndWeaponsUpdate", &idx))
		{
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);
		}
	}
}

void CCallbackManager::OnVehicleSpawn(WORD vehicleid)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnVehicleSpawn", &idx))
		{
			amx_Push(iter, vehicleid);
			amx_Exec(iter, &ret, idx);
		}
	}
}

void CCallbackManager::OnPlayerClientGameInit(WORD playerid, bool* usecjwalk, bool* limitglobalchat, float* globalchatradius, float* nametagdistance, 
	bool* disableenterexits, bool* nametaglos, bool* manualvehengineandlights, int* spawnsavailable, bool* shownametags, bool* showplayermarkers, 
	int* onfoot_rate, int* incar_rate, int* weapon_rate, int* lagcompmode, bool* vehiclefriendlyfire)
{
	int idx = -1;
	cell ret = 1;
	DWORD dwTemp;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnPlayerClientGameInit", &idx))
		{
			cell amx_addr, amx_addr_last, *phys_ptr, *temp_ptr;

			dwTemp = *vehiclefriendlyfire;
			amx_PushArray(iter, &amx_addr, &phys_ptr, reinterpret_cast<cell*>(&dwTemp), 1);							// 0
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(lagcompmode), 1);					// 1
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(weapon_rate), 1);					// 2
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(incar_rate), 1);					// 3
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(onfoot_rate), 1);					// 4
			dwTemp = *showplayermarkers;
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 5
			dwTemp = *shownametags;
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 6
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(spawnsavailable), 1);				// 7
			dwTemp = *manualvehengineandlights;
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 8
			dwTemp = *nametaglos;
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 9
			dwTemp = *disableenterexits;
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 10
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(amx_ftoc(nametagdistance)), 1);		// 11
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(amx_ftoc(globalchatradius)), 1);	// 12
			dwTemp = *limitglobalchat;
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 13
			dwTemp = *usecjwalk;
			amx_PushArray(iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 14
		
			amx_Push(iter, static_cast<cell>(playerid));
			amx_Exec(iter, &ret, idx);
			/*
			for(int i = 0; i != ((amx_addr_last - amx_addr) / 4); ++i)
			{
				amx_Release(iter, amx_addr + (i * 4));
			}
			*/
			amx_Release(iter, amx_addr);

			*vehiclefriendlyfire = static_cast<int>(phys_ptr[0]) != 0;
			*lagcompmode = static_cast<int>(phys_ptr[1]);
			*weapon_rate = static_cast<int>(phys_ptr[2]);
			*incar_rate = static_cast<int>(phys_ptr[3]);
			*onfoot_rate = static_cast<int>(phys_ptr[4]);
			*showplayermarkers = static_cast<int>(phys_ptr[5]) != 0;
			*shownametags = static_cast<int>(phys_ptr[6]) != 0;
			*spawnsavailable = static_cast<int>(phys_ptr[7]);
			*manualvehengineandlights = static_cast<int>(phys_ptr[8]) != 0;
			*nametaglos = static_cast<int>(phys_ptr[9]) != 0;
			*disableenterexits = static_cast<int>(phys_ptr[10]) != 0;
			*nametagdistance = amx_ctof(phys_ptr[11]);
			*globalchatradius = amx_ctof(phys_ptr[12]);
			*limitglobalchat = static_cast<int>(phys_ptr[13]) != 0;
			*usecjwalk = static_cast<int>(phys_ptr[14]) != 0;
		}
	}
}

bool CCallbackManager::OnOutcomeScmEvent(WORD playerid, WORD issuerid, int eventid, int vehicleid, int arg1, int arg2)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnOutcomeScmEvent", &idx))
		{
			amx_Push(iter, static_cast<cell>(arg2));
			amx_Push(iter, static_cast<cell>(arg1));
			amx_Push(iter, static_cast<cell>(vehicleid));
			amx_Push(iter, static_cast<cell>(eventid));
			amx_Push(iter, static_cast<cell>(issuerid));
			amx_Push(iter, static_cast<cell>(playerid));

			amx_Exec(iter, &ret, idx);

			if (!ret) return false;
		}
	}
	return static_cast<int>(ret) != 0;
}

bool CCallbackManager::OnServerQueryInfo(unsigned int binaryAddress, char(&hostname)[51], char(&gameMode)[31], char(&language)[31])
{
	in_addr in;
	in.s_addr = binaryAddress;

	int idx = -1;
	cell ret = 0;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnServerQueryInfo", &idx))
		{
			cell amx_addr, amx_addr_last;

			// Mirroring characters from char array to cell array
			cell languageCells[sizeof(language)];
			for (int i = 0; i < sizeof(language); i++)
				languageCells[i] = language[i];

			cell gameModeCells[sizeof(gameMode)];
			for (int i = 0; i < sizeof(gameMode); i++)
				gameModeCells[i] = gameMode[i];

			cell hostnameCells[sizeof(hostname)];
			for (int i = 0; i < sizeof(hostname); i++)
				hostnameCells[i] = hostname[i];
			
			cell *outputHostname, *outputGameMode, *outputLanguage;
			amx_PushArray(iter, &amx_addr, &outputLanguage, languageCells, sizeof(language));
			amx_PushArray(iter, &amx_addr_last, &outputGameMode, gameModeCells, sizeof(gameMode));
			amx_PushArray(iter, &amx_addr_last, &outputHostname, hostnameCells, sizeof(hostname));
			amx_PushString(iter, &amx_addr_last, nullptr, inet_ntoa(in), 0, 0);
			amx_Exec(iter, &ret, idx);
			amx_Release(iter, amx_addr);

			// If returned value isn't 0, then write changes back
			if (ret)
			{
				for (int i = 0; i < sizeof(hostname); i++)
					hostname[i] = outputHostname[i];
				
				for (int i = 0; i < sizeof(gameMode); i++)
					gameMode[i] = outputGameMode[i];
				
				for (int i = 0; i < sizeof(language); i++)
					language[i] = outputLanguage[i];
			}
		
			if (ret) 
				return true;
		}
	}
	return static_cast<int>(ret) != 0;
}

void CCallbackManager::OnClientCheckResponse(WORD playerid, BYTE type, DWORD arg, BYTE response)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnClientCheckResponse", &idx)) 
		{
			amx_Push(iter, static_cast<cell>(response));
			amx_Push(iter, static_cast<cell>(arg));
			amx_Push(iter, static_cast<cell>(type));
			amx_Push(iter, static_cast<cell>(playerid));

			amx_Exec(iter, &ret, idx);
			if (ret) break;
		}
	}
}

void CCallbackManager::OnSystemCommandExecute(const char *output, int retval, int index, bool success, int line_current, int line_total)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnSystemCommandExecute", &idx))
		{
			cell amx_addr;
			amx_Push(iter, static_cast<cell>(line_total));
			amx_Push(iter, static_cast<cell>(line_current));
			amx_Push(iter, static_cast<cell>(success));
			amx_Push(iter, static_cast<cell>(index));
			amx_Push(iter, static_cast<cell>(retval));
			amx_PushString(iter, &amx_addr, nullptr, output, 0, 0);
			amx_Exec(iter, &ret, idx);
			amx_Release(iter, amx_addr);
		}
	}
}

void CCallbackManager::OnPlayerSelectObject(WORD playerid, DWORD type, DWORD objectid, DWORD modelid, const CVector &pos)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnPlayerSelectObject", &idx))
		{
			amx_Push(iter, amx_ftoc(pos.fZ));
			amx_Push(iter, amx_ftoc(pos.fY));
			amx_Push(iter, amx_ftoc(pos.fX));
			amx_Push(iter, static_cast<cell>(modelid));
			amx_Push(iter, static_cast<cell>(objectid));
			amx_Push(iter, static_cast<cell>(type));
			amx_Push(iter, static_cast<cell>(playerid));

			amx_Exec(iter, &ret, idx);
			if (ret) break;
		}
	}
}

void CCallbackManager::OnPlayerEditObject(WORD playerid, bool playerobject, DWORD objectid, DWORD response, const CVector &pos, const CVector &rot)
{
	int idx = -1;
	cell ret = 1;
	for (auto iter : m_setAMX)
	{
		if (!amx_FindPublic(iter, "OnPlayerSelectObject", &idx))
		{
			amx_Push(iter, amx_ftoc(rot.fZ));
			amx_Push(iter, amx_ftoc(rot.fY));
			amx_Push(iter, amx_ftoc(rot.fX));
			amx_Push(iter, amx_ftoc(pos.fZ));
			amx_Push(iter, amx_ftoc(pos.fY));
			amx_Push(iter, amx_ftoc(pos.fX));
			amx_Push(iter, static_cast<cell>(response));
			amx_Push(iter, static_cast<cell>(objectid));
			amx_Push(iter, static_cast<cell>(playerobject));
			amx_Push(iter, static_cast<cell>(playerid));

			amx_Exec(iter, &ret, idx);
			if (ret) break;
		}
	}
}

