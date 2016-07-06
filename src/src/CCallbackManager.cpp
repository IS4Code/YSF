#include "main.h"

std::vector<AMX *> CCallbackManager::m_vecAMX;

void CCallbackManager::RegisterAMX(AMX *pAMX)
{
	// Add gamemode to the first position in vector
	if (pNetGame && pNetGame->pGameModePool && &pNetGame->pGameModePool->amx == pAMX)
	{
		std::vector<AMX*>::iterator it = m_vecAMX.begin();
		m_vecAMX.insert(it, pAMX);
	}
	else
	{
		m_vecAMX.push_back(pAMX);
	}
}

void CCallbackManager::UnregisterAMX(AMX *pAMX)
{
	// Remove the amx from the pointers list
	std::vector<AMX *>::iterator p = std::find(m_vecAMX.begin(), m_vecAMX.end(), pAMX);
	if(p != m_vecAMX.end())
	{
		p = m_vecAMX.erase(p);
	}
}

void CCallbackManager::OnPlayerEnterGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerEnterGangZone", &idx))
		{
			amx_Push(*iter, static_cast<cell>(zoneid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerLeaveGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerLeaveGangZone", &idx))
		{
			amx_Push(*iter, static_cast<cell>(zoneid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerEnterPlayerGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerEnterPlayerGangZone", &idx))
		{
			amx_Push(*iter, static_cast<cell>(zoneid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerLeavePlayerGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerLeavePlayerGangZone", &idx))
		{
			amx_Push(*iter, static_cast<cell>(zoneid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerPauseStateChange(WORD playerid, bool pausestate)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerPauseStateChange", &idx))
		{
			amx_Push(*iter, static_cast<cell>(pausestate));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerDeath(WORD playerid, WORD killerid, BYTE reason)
{
	int idx = -1;
	cell ret = 1;
	for (std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnPlayerDeath", &idx))
		{
			amx_Push(*iter, static_cast<cell>(reason));
			amx_Push(*iter, static_cast<cell>(killerid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerSpawn(WORD playerid)
{
	int idx = -1;
	cell ret = 1;
	for (std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnPlayerSpawn", &idx))
		{
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerPickedUpPickup(WORD playerid, WORD pickupid)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerPickUpPickup", &idx))
		{
			amx_Push(*iter, static_cast<cell>(pickupid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

void CCallbackManager::OnPlayerPickedUpPlayerPickup(WORD playerid, WORD pickupid)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerPickUpPlayerPickup", &idx))
		{
			amx_Push(*iter, static_cast<cell>(pickupid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return;
		}
	}
}

bool CCallbackManager::OnServerMessage(char* message)
{
	if (!message) return 0;

	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnServerMessage", &idx))
		{
			cell amx_addr, *phys_addr;
			amx_PushString(*iter, &amx_addr, &phys_addr, message, 0, 0);

			amx_Exec(*iter, &ret, idx);
			amx_Release(*iter, amx_addr);

			if (!ret) return 0;
		}
	}
	return static_cast<int>(ret) != 0;
}

bool CCallbackManager::OnRemoteRCONPacket(unsigned int binaryAddress, int port, char *password, bool success, char* command)
{
	in_addr in;
	in.s_addr = binaryAddress;
	char *addr = inet_ntoa(in);

	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnRemoteRCONPacket", &idx))
		{
			cell amx_addr, *phys_addr;
			

			amx_PushString(*iter, &amx_addr, &phys_addr, command, 0, 0);
			amx_Push(*iter, static_cast<cell>(success));
			amx_PushString(*iter, &amx_addr, &phys_addr, password, 0, 0);
			amx_Push(*iter, static_cast<cell>(port));
			amx_PushString(*iter, &amx_addr, &phys_addr, addr, 0, 0);
			amx_Exec(*iter, &ret, idx);
			amx_Release(*iter, amx_addr);

			if (!ret) return 0;
		}
	}
	return static_cast<int>(ret) != 0;
}

void CCallbackManager::OnPlayerStatsAndWeaponsUpdate(WORD playerid)
{
	int idx = -1;
	cell ret = 1;
	for (std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnPlayerStatsAndWeaponsUpdate", &idx))
		{
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);
		}
	}
}

void CCallbackManager::OnVehicleSpawn(WORD vehicleid)
{
	int idx = -1;
	cell ret = 1;
	for (std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnVehicleSpawn", &idx))
		{
			amx_Push(*iter, vehicleid);

			amx_Exec(*iter, &ret, idx);
		}
	}
}

void CCallbackManager::OnPlayerClientGameInit(WORD playerid, bool* usecjwalk, bool* limitglobalchat, float* globalchatradius, float* nametagdistance, 
	bool* disableenterexits, bool* nametaglos, bool* manualvehengineandlights, int* spawnsavailable, bool* shownametags, bool* showplayermarkers, 
	int* onfoot_rate, int* incar_rate, int* weapon_rate, int* lacgompmode, bool* vehiclefriendlyfire)
{
	int idx = -1;
	cell ret = 1;
	DWORD dwTemp;
	for (std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnPlayerClientGameInit", &idx))
		{
			cell addr = NULL, amx_addr, amx_addr_last = NULL, *phys_ptr, *temp_ptr;

			dwTemp = *vehiclefriendlyfire;
			amx_PushArray(*iter, &amx_addr, &phys_ptr, reinterpret_cast<cell*>(&dwTemp), 1);							// 0
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(lacgompmode), 1);					// 1
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(weapon_rate), 1);					// 2
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(incar_rate), 1);					// 3
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(onfoot_rate), 1);					// 4
			dwTemp = *showplayermarkers;
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 5
			dwTemp = *shownametags;
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 6
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(spawnsavailable), 1);				// 7
			dwTemp = *manualvehengineandlights;
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 8
			dwTemp = *nametaglos;
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 9
			dwTemp = *disableenterexits;
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 10
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, &amx_ftoc(*nametagdistance), 1);							// 11
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, &amx_ftoc(*globalchatradius), 1);							// 12
			dwTemp = *limitglobalchat;
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 13
			dwTemp = *usecjwalk;
			amx_PushArray(*iter, &amx_addr_last, &temp_ptr, reinterpret_cast<cell*>(&dwTemp), 1);						// 14
		
			amx_Push(*iter, static_cast<cell>(playerid));
			amx_Exec(*iter, &ret, idx);
			for(int i = 0; i != ((amx_addr_last - amx_addr) / 4); i++)
			{
				amx_Release(*iter, amx_addr + (i * 4));
			}
			
			*vehiclefriendlyfire = static_cast<int>(phys_ptr[0]) != 0;
			*lacgompmode = static_cast<int>(phys_ptr[1]);
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
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnOutcomeScmEvent", &idx))
		{
			amx_Push(*iter, static_cast<cell>(arg2));
			amx_Push(*iter, static_cast<cell>(arg1));
			amx_Push(*iter, static_cast<cell>(vehicleid));
			amx_Push(*iter, static_cast<cell>(eventid));
			amx_Push(*iter, static_cast<cell>(issuerid));
			amx_Push(*iter, static_cast<cell>(playerid));

			amx_Exec(*iter, &ret, idx);

			if (!ret) return false;
		}
	}
	return static_cast<int>(ret) != 0;
}

bool CCallbackManager::OnServerQueryInfo(unsigned int binaryAddress, char (&hostname)[51], char (&gameMode)[31], char (&language)[31])
{
	in_addr in;
	in.s_addr = binaryAddress;
	char *addr = inet_ntoa(in);

	int idx = -1;
	cell ret = 0;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnServerQueryInfo", &idx))
		{
			cell amx_addr, *phys_addr;
			
			cell *cellArray;

			cell *outputHostname, *outputGameMode, *outputLanguage;

			cellArray = reinterpret_cast<cell*>(alloca(sizeof(cell)*sizeof(language)));
			for(int i = 0; i < sizeof(language); i++)
			{
				cellArray[i] = language[i];
			}
			amx_PushArray(*iter, &amx_addr, &outputLanguage, cellArray, sizeof(language));

			cellArray = reinterpret_cast<cell*>(alloca(sizeof(cell)*sizeof(gameMode)));
			for(int i = 0; i < sizeof(gameMode); i++)
			{
				cellArray[i] = gameMode[i];
			}
			amx_PushArray(*iter, &amx_addr, &outputGameMode, cellArray, sizeof(gameMode));

			cellArray = reinterpret_cast<cell*>(alloca(sizeof(cell)*sizeof(hostname)));
			for(int i = 0; i < sizeof(hostname); i++)
			{
				cellArray[i] = hostname[i];
			}
			amx_PushArray(*iter, &amx_addr, &outputHostname, cellArray, sizeof(hostname));

			amx_PushString(*iter, &amx_addr, &phys_addr, addr, 0, 0);

			amx_Exec(*iter, &ret, idx);
			
			for(int i = 0; i < sizeof(hostname); i++)
			{
				hostname[i] = outputHostname[i];
			}
			for(int i = 0; i < sizeof(gameMode); i++)
			{
				gameMode[i] = outputGameMode[i];
			}
			for(int i = 0; i < sizeof(language); i++)
			{
				language[i] = outputLanguage[i];
			}

			amx_Release(*iter, amx_addr);

			if(ret) return true;
			if(ret != -1) ret = 1;
		}
	}
	return static_cast<int>(ret) == 1;
}