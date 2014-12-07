#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

// Thx to OrMisicL - from FCNPC
#include <vector>
#include "sdk/plugin.h"
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
	static void	OnPlayerStatsAndWeaponsUpdate(WORD playerid);

	// Default SAMP Callbakcs
	static void	OnPlayerDeath(WORD playerid, WORD killerid, BYTE reason);
	static void	OnPlayerSpawn(WORD playerid);
	static void	OnPlayerPickedUpPickup(WORD playerid, WORD pickupid);
	static void	OnPlayerPickedUpPlayerPickup(WORD playerid, WORD pickupid);

	static void OnServerMessage(char* message);
	static bool OnRemoteRCONPacket(unsigned int binaryAddress, int port, char *password, char* command);

	static std::vector<AMX *>		m_vecAMX;
};

#endif