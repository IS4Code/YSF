#ifndef YSF_CALLBACKMANAGER_H
#define YSF_CALLBACKMANAGER_H

// Thx to OrMisicL - from FCNPC
#include <vector>
#include "sdk/plugin.h"

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
	static bool OnServerMessage(char* message);
	static bool OnRemoteRCONPacket(unsigned int binaryAddress, int port, char *password, bool success, char* command);
	static void OnPlayerClientGameInit(WORD playerid, bool* usecjwalk, bool* limitglobalchat, float* globalchatradius, float* nametagdistance, bool* disableenterexits, bool* nametaglos, bool* manualvehengineandlights, int* spawnsavailable, bool* shownametags, bool* showplayermarkers, int* onfoot_rate, int* incar_rate, int* weapon_rate, int* lacgompmode, bool* vehiclefriendlyfire);
	static bool OnOutcomeScmEvent(WORD playerid, WORD issuerid, int eventid, int vehicleid, int arg1, int arg2);
	static bool OnServerQueryInfo(unsigned int binaryAddress, char (&hostname)[51], char (&gameMode)[31], char (&language)[31]);

	// Default SAMP Callbakcs
	static void	OnPlayerDeath(WORD playerid, WORD killerid, BYTE reason);
	static void	OnPlayerSpawn(WORD playerid);
	static void	OnVehicleSpawn(WORD vehicleid);
	static void	OnPlayerPickedUpPickup(WORD playerid, WORD pickupid);
	static void	OnPlayerPickedUpPlayerPickup(WORD playerid, WORD pickupid);


	static std::vector<AMX *>		m_vecAMX;
};

#endif