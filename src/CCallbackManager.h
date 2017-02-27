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
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

#ifndef YSF_CALLBACKMANAGER_H
#define YSF_CALLBACKMANAGER_H

// Thx to OrMisicL - from FCNPC
#include <set>
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
	static void OnPlayerClientGameInit(WORD playerid, bool* usecjwalk, bool* limitglobalchat, float* globalchatradius, float* nametagdistance, bool* disableenterexits, bool* nametaglos, bool* manualvehengineandlights, int* spawnsavailable, bool* shownametags, bool* showplayermarkers, int* onfoot_rate, int* incar_rate, int* weapon_rate, int* lacgompmode, bool* vehiclefriendlyfire);
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

	static std::set<AMX *>		m_setAMX;
};

#endif