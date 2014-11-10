#ifndef CSERVER_H
#define CSERVER_H

class CNetGame;
class RakServer;
struct Packet;

#include "CTypes.h"
#include "Addresses.h"

#include <vector>

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

	void Packet_WeaponsUpdate(Packet *p);
	void Packet_StatsUpdate(Packet *p);

private:
	eSAMPVersion m_Version;
	float m_fGravity;
	unsigned char m_byteWeather;
	int m_iTicks;

	std::vector <char> m_vecValidNameCharacters;
};

#endif