#ifndef YSF_CALLBACKMANAGER_H
#define YSF_CALLBACKMANAGER_H

// Thx to OrMisicL - from FCNPC
#include <set>
#include "Structs.h"
#include "includes/types.h"
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
	static bool OnServerMessage(const char* message);
	static bool OnRemoteRCONPacket(unsigned int binaryAddress, int port, char *password, bool success, char* command);
	static void OnPlayerClientGameInit(WORD playerid, bool* usecjwalk, bool* limitglobalchat, float* globalchatradius, float* nametagdistance, bool* disableenterexits, bool* nametaglos, bool* manualvehengineandlights, int* spawnsavailable, bool* shownametags, bool* showplayermarkers, int* onfoot_rate, int* incar_rate, int* weapon_rate, int* lagcompmode, bool* vehiclefriendlyfire);
	static bool OnOutcomeScmEvent(WORD playerid, WORD issuerid, int eventid, int vehicleid, int arg1, int arg2);
	static bool OnServerQueryInfo(unsigned int binaryAddress, char(&hostname)[51], char(&gameMode)[31], char(&language)[31]);
	static void OnSystemCommandExecute(const char *output, int retval, int index, bool success, int line_current, int line_total);

	// Default SAMP Callbakcs
	static void	OnPlayerDeath(WORD playerid, WORD killerid, BYTE reason);
	static void	OnPlayerSpawn(WORD playerid);
	static void	OnVehicleSpawn(WORD vehicleid);
	static void	OnPlayerPickedUpPickup(WORD playerid, WORD pickupid);
	static void	OnPlayerPickedUpPlayerPickup(WORD playerid, WORD pickupid);
	static void	OnClientCheckResponse(WORD playerid, BYTE type, DWORD arg, BYTE response);
	static void	OnPlayerSelectObject(WORD playerid, DWORD type, DWORD objectid, DWORD modelid, const CVector &pos);
	static void	OnPlayerEditObject(WORD playerid, bool playerobject, DWORD objectid, DWORD response, const CVector &pos, const CVector &rot);

	static std::set<AMX *>		m_setAMX;
};

#endif