#ifndef YSF_CPLUGIN_H
#define YSF_CPLUGIN_H

struct CNetGame;
struct Packet;
class CGangZonePool;
class CYSFPickupPool;

#include <vector>
#include <set>
#include <bitset>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <string>

#include "utils/pair_hash.h"
#include "raknet/NetworkTypes.h"
#include "CFunctions.h"
#include "CSingleton.h"
#include "CAddresses.h"
#include "CGangZonePool.h"
#include "Structs.h"

class CPlugin : public CSingleton<CPlugin>
{
	friend class CSingleton<CPlugin>;
public:
	CPlugin(SAMPVersion version);
	~CPlugin();

	void Process();

	void AddPlayer(int playerid);
	bool RemovePlayer(int playerid);
 
	bool OnPlayerStreamIn(WORD playerid, WORD forplayerid);
	bool OnPlayerStreamOut(WORD playerid, WORD forplayerid);

	SAMPVersion const &GetVersion() { return m_Version; }
	
	void AllowNickNameCharacter(char character, bool enable);
	bool IsNickNameCharacterAllowed(char character);
	bool IsValidNick(char *szName);

	// RakServer::IsBanned fix
	void BanIP(const char* ip) { m_BannedIPs.insert(ip); } 
	void UnbanIP(const char* ip) { m_BannedIPs.erase(ip); }
	void ClearBans() { m_BannedIPs.clear(); }
	bool IsBanned(char* ip) { return m_BannedIPs.find(ip) != m_BannedIPs.end(); }

	// Toggling rcon commands
	bool ChangeRCONCommandName(std::string const &strCmd, std::string const &strNewCmd);
	bool GetRCONCommandName(std::string const &strCmd, std::string &strNewCmd);

	// Broadcasting console messages to players
	void AddConsolePlayer(WORD playerid, DWORD color);
	void RemoveConsolePlayer(WORD playerid); 
	bool IsConsolePlayer(WORD playerid, DWORD &color);
	void ProcessConsoleMessages(const char* str);

	void SetTickRate(int rate) { m_iTickRate = rate; }
	int GetTickRate(void) { return m_iTickRate; }

	void EnableNightVisionFix(bool enable) { m_bNightVisionFix = enable; }
	bool IsNightVisionFixEnabled(void) { return m_bNightVisionFix; }
	
	void ToggleOnServerMessage(bool toggle) { m_bOnServerMessage = toggle; }
	bool IsOnServerMessageEnabled(void) { return m_bOnServerMessage; }

	void SetExclusiveBroadcast(bool toggle);
	bool GetExclusiveBroadcast(void);

	void SetExtendedNetStatsEnabled(bool enable);
	bool IsExtendedNetStatsEnabled(void);

	void SetAFKAccuracy(DWORD time_ms) { m_dwAFKAccuracy = time_ms; }
	DWORD GetAFKAccuracy(void) { return m_dwAFKAccuracy; }

	WORD GetMaxPlayers();
	WORD GetPlayerCount();
	WORD GetNPCCount();

	RakNet::BitStream *BuildSyncData(RakNet::BitStream *bsSync, WORD toplayerid);
	bool RebuildRPCData(BYTE uniqueID, RakNet::BitStream *bsSync, WORD playerid);

	char* GetNPCCommandLine(WORD npcid);
	int FindNPCProcessID(WORD npcid); 
	bool CreatePlayerObjectLocalID(WORD playerid, WORD &objectid, bool playerobject);
	bool MapPlayerObjectIDToLocalID(WORD playerid, WORD &objectid);
	bool MapPlayerObjectIDToServerID(WORD playerid, WORD &objectid);

	bool IsMainThread() const;

	void ToggleChatTextReplacement(bool toggle) { chatReplacement = toggle; };
	bool ChatTextReplacementToggled() const { return chatReplacement; };

#ifdef SAMP_03DL
	void CacheModelInfo(CModelInfo* info);
	CModelInfo* FindCachedModelInfo(const char *dffname, const char *txdname);
#endif

	CGangZonePool *pGangZonePool;
	CYSFPickupPool *pPickupPool;

#ifdef _WIN32
	struct SysExec_t
	{
		std::string output;
		int retval;
		int index;
		bool success;
	};
	std::queue<SysExec_t> m_SysExecQueue;
	std::mutex m_SysExecMutex;
#endif
private:
#ifdef _WIN32
	void ProcessSysExec();
#endif
	SAMPVersion m_Version;
	int m_iTicks;
	int m_iTickRate;
	bool m_bNightVisionFix : 1;
	bool m_bOnServerMessage : 1;
	
	bool m_bExclusiveBroadcast = 0;

	DWORD m_dwAFKAccuracy;

	std::set<std::string> m_BannedIPs;
	std::vector<std::string> m_RCONCommands;
	std::unordered_map<WORD, DWORD> m_ConsoleMessagePlayers;
	std::set<char> m_vecValidNameCharacters;

	std::thread::id main_thread;
	bool chatReplacement = true;
#ifdef SAMP_03DL
	std::unordered_map<std::pair<std::string, std::string>, CModelInfo*, aux::pair_hash> m_modelCache;
#endif
};

#endif
