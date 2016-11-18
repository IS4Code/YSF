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

#ifndef YSF_CSERVER_H
#define YSF_CSERVER_H

struct CNetGame;
struct Packet;
class CVehicleSpawn;
class CGangZonePool;
class CYSFPickupPool;

#include "CSingleton.h"
#include "Addresses.h"
#include "CGangZonePool.h"

#include <vector>
#include <set>
#include <bitset>
#include <unordered_map>
#include <unordered_set>

class CServer : public CSingleton<CServer>
{
	friend class CSingleton<CServer>;
public:
	CServer() : m_bInitialized(false)
	{

	}
	~CServer();

	void Initialize(eSAMPVersion version);
	bool inline IsInitialized(void) { return m_bInitialized; }

	bool AddPlayer(int playerid);
	bool RemovePlayer(int playerid);

	void Process();

	bool OnPlayerStreamIn(WORD playerid, WORD forplayerid);
	bool OnPlayerStreamOut(WORD playerid, WORD forplayerid);

	void SetGravity_(float fGravity);
	float GetGravity_(void);

	void SetWeather_(BYTE byteWeather);
	BYTE GetWeather_(void);

	eSAMPVersion GetVersion() { return m_Version; }
	
	void AllowNickNameCharacter(char character, bool enable);
	bool IsNickNameCharacterAllowed(char character);
	bool IsValidNick(char *szName);

	inline void BanIP(const char* ip)
	{
		bannedIPs.insert(ip);
	}

	inline void UnbanIP(const char* ip)
	{
		bannedIPs.erase(ip);
	}

	inline void ClearBans()
	{
		bannedIPs.clear();
	}

	inline bool IsBanned(char* ip)
	{
		return bannedIPs.find(ip) != bannedIPs.end();
	}

	void inline SetTickRate(int rate) { m_iTickRate = rate; }
	int inline GetTickRate(void) { return m_iTickRate; }

	void inline EnableNightVisionFix(bool enable) { m_bNightVisionFix = enable; }
	bool inline IsNightVisionFixEnabled(void) { return m_bNightVisionFix; }

	void SetExtendedNetStatsEnabled(bool enable);
	bool IsExtendedNetStatsEnabled(void);

	void inline SetAFKAccuracy(DWORD time_ms) { m_dwAFKAccuracy = time_ms; }
	DWORD inline GetAFKAccuracy(void) { return m_dwAFKAccuracy; }

	WORD GetMaxPlayers_();
	WORD GetPlayerCount();
	WORD GetNPCCount();

	CGangZonePool *pGangZonePool;
	CYSFPickupPool *pPickupPool;

	std::unordered_map<int, CVehicleSpawn> vehicleSpawnData;
	std::bitset<MAX_VEHICLES> bChangedVehicleColor;
	WORD COBJECT_AttachedObjectPlayer[MAX_OBJECTS];

private:
	eSAMPVersion m_Version;
	int m_iTicks;
	int m_iTickRate;
	bool m_bInitialized = 0;
	bool m_bNightVisionFix : 1;
	bool m_bExtendedNetStats : 1;
	DWORD m_dwAFKAccuracy;

	std::unordered_set<std::string> bannedIPs;
	std::unordered_set <char> m_vecValidNameCharacters;
};

#endif