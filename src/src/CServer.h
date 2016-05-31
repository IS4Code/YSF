#ifndef YSF_CSERVER_H
#define YSF_CSERVER_H

struct CNetGame;
class RakServer;
struct Packet;
struct CVehicleSpawn;
class CGangZonePool;
class CYSFPickupPool;

#include "Addresses.h"
#include "CGangZonePool.h"

#include <vector>
#include <set>
#include <bitset>

class CServer
{
public:
	CServer(eSAMPVersion version);
	~CServer();

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

	std::map<int, CVehicleSpawn> vehicleSpawnData;
	std::bitset<MAX_VEHICLES> bChangedVehicleColor;
	WORD COBJECT_AttachedObjectPlayer[MAX_OBJECTS];

private:
	eSAMPVersion m_Version;
	int m_iTicks;
	int m_iTickRate;
	bool m_bNightVisionFix : 1;
	bool m_bExtendedNetStats : 1;
	DWORD m_dwAFKAccuracy;

	std::vector <char> m_vecValidNameCharacters;
};

#endif