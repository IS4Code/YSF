#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

// Thx to OrMisicL - from FCNPC
#include <list>
#include "SDK/plugin.h"
#include "CTypes.h"

class CCallbackManager
{
public:
	static void	RegisterAMX(AMX *pAMX);
	static void	UnregisterAMX(AMX *pAMX);

	// Callbacks
	static void	OnPlayerEnterGangZone(WORD playerid, WORD zoneid);
	static void	OnPlayerLeaveGangZone(WORD playerid, WORD zoneid);
	static void	OnPlayerEnterPlayerGangZone(WORD playerid, WORD zoneid);
	static void	OnPlayerLeavePlayerGangZone(WORD playerid, WORD zoneid);
	static void	OnPlayerPauseStateChange(WORD playerid, bool pausestate);

	// Default SAMP Callbakcs
	static void	OnPlayerPickedUpPickup(WORD playerid, WORD pickupid);
	static void	OnPlayerPickedUpPlayerPickup(WORD playerid, WORD pickupid);

	static std::list<AMX *>		m_listAMX;
};

#endif