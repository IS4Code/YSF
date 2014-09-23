#ifndef __YSF_CSERVER
#define __YSF_CSERVER

class CNetGame;
class RakServer;

#include "CTypes.h"

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

	void SetGravity(float fGravity);
	float GetGravity(void);

	void SetWeather(BYTE byteWeather);
	BYTE GetWeather(void);

	eSAMPVersion GetVersion() { return m_Version; }
	
private:
	eSAMPVersion m_Version;
	float m_fGravity;
	unsigned char m_byteWeather;
	int m_iTicks;
};

#endif