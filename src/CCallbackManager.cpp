#include "CCallbackManager.h"
#include "CServer.h"
#include "Structs.h"

#include <sdk/plugin.h>
#include "main.h"
#include "Utils.h"

#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#else

#endif
std::vector<AMX *> CCallbackManager::m_vecAMX;

void CCallbackManager::RegisterAMX(AMX *pAMX)
{
	// Add the amx to the pointers list
	m_vecAMX.push_back(pAMX);
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
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerEnterGangZone", &idx))
		{
			amx_Push(*iter, zoneid);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}

void CCallbackManager::OnPlayerLeaveGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerLeaveGangZone", &idx))
		{
			amx_Push(*iter, zoneid);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}

void CCallbackManager::OnPlayerEnterPlayerGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerEnterPlayerGangZone", &idx))
		{
			amx_Push(*iter, zoneid);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}

void CCallbackManager::OnPlayerLeavePlayerGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerLeavePlayerGangZone", &idx))
		{
			amx_Push(*iter, zoneid);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}

void CCallbackManager::OnPlayerPauseStateChange(WORD playerid, bool pausestate)
{
	int idx = -1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerPauseStateChange", &idx))
		{
			amx_Push(*iter, pausestate);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}

void CCallbackManager::OnPlayerPickedUpPickup(WORD playerid, WORD pickupid)
{
	int idx = -1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerPickUpPickup", &idx))
		{
			amx_Push(*iter, pickupid);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}

void CCallbackManager::OnPlayerPickedUpPlayerPickup(WORD playerid, WORD pickupid)
{
	int idx = -1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerPickUpPlayerPickup", &idx))
		{
			amx_Push(*iter, pickupid);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}

void CCallbackManager::OnServerMessage(char* message)
{
	int idx = -1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnServerMessage", &idx))
		{
			cell amx_addr, *phys_addr;
			amx_PushString(*iter, &amx_addr, &phys_addr, message, 0, 0);

			amx_Exec(*iter, NULL, idx);
			amx_Release(*iter, amx_addr);
		}
	}
}

bool CCallbackManager::OnRemoteRCONPacket(unsigned int binaryAddress, int port, char *password, char* command)
{
	int idx = -1;
	cell ret = 1;
	for(std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnRemoteRCONPacket", &idx))
		{
			cell amx_addr, *phys_addr;
			
			in_addr in;
			in.s_addr = binaryAddress;

			amx_PushString(*iter, &amx_addr, &phys_addr, command, 0, 0);
			amx_PushString(*iter, &amx_addr, &phys_addr, password, 0, 0);
			amx_Push(*iter, port);
			amx_PushString(*iter, &amx_addr, &phys_addr, inet_ntoa(in), 0, 0);
			amx_Exec(*iter, &ret, idx);
			amx_Release(*iter, amx_addr);

			if (!ret) return 0;
		}
	}
	return !!ret;
}

void CCallbackManager::OnPlayerStatsAndWeaponsUpdate(WORD playerid)
{
	int idx = -1;
	for (std::vector<AMX*>::const_iterator iter = m_vecAMX.begin(); iter != m_vecAMX.end(); ++iter)
	{
		if (!amx_FindPublic(*iter, "OnPlayerStatsAndWeaponsUpdate", &idx))
		{
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}
/*
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
{
	if (playerid >= 0 && playerid < MAX_PLAYERS)
	{
		bool ret = pServer->AddPlayer(playerid);

		// Initialize pickups
		if (ret)
			pNetGame->pPickupPool->InitializeForPlayer(playerid);
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	pServer->RemovePlayer(playerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerStreamIn(int playerid, int forplayerid)
{
	pServer->OnPlayerStreamIn(playerid, forplayerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerStreamOut(int playerid, int forplayerid)
{
	pServer->OnPlayerStreamOut(playerid, forplayerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char * inputtext)
{
	if (IsPlayerConnected(playerid))
	{
		//pNetGame->pPlayerPool->pPlayer[playerid]->wDialogID = 0;
	}
	return true;
}
*/