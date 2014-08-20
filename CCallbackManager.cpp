#include <list>
#include "SDK/plugin.h"
#include "CCallbackManager.h"

std::list<AMX *> CCallbackManager::m_listAMX;

void CCallbackManager::RegisterAMX(AMX *pAMX)
{
	// Add the amx to the pointers list
	m_listAMX.push_back(pAMX);
}

void CCallbackManager::UnregisterAMX(AMX *pAMX)
{
	// Remove the amx from the pointers list
	m_listAMX.remove(pAMX);
}

void CCallbackManager::OnPlayerEnterGangZone(WORD playerid, WORD zoneid)
{
	int idx = -1;
	for(std::list<AMX*>::iterator iter = m_listAMX.begin(); iter != m_listAMX.end(); ++iter)
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
	for(std::list<AMX*>::iterator iter = m_listAMX.begin(); iter != m_listAMX.end(); ++iter)
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
	for(std::list<AMX*>::iterator iter = m_listAMX.begin(); iter != m_listAMX.end(); ++iter)
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
	for(std::list<AMX*>::iterator iter = m_listAMX.begin(); iter != m_listAMX.end(); ++iter)
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
	for(std::list<AMX*>::iterator iter = m_listAMX.begin(); iter != m_listAMX.end(); ++iter)
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
	for(std::list<AMX*>::iterator iter = m_listAMX.begin(); iter != m_listAMX.end(); ++iter)
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
	for(std::list<AMX*>::iterator iter = m_listAMX.begin(); iter != m_listAMX.end(); ++iter)
	{
		if(!amx_FindPublic(*iter, "OnPlayerPickUpPlayerPickup", &idx))
		{
			amx_Push(*iter, pickupid);
			amx_Push(*iter, playerid);

			amx_Exec(*iter, NULL, idx);
		}
	}
}