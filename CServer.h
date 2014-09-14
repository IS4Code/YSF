#ifndef __YSF_CSERVER
#define __YSF_CSERVER

class CNetGame;
class RakServer;

class CServer
{
public:
	CServer(eSAMPVersion version);
	~CServer();

	bool AddPlayer(int playerid);
	bool RemovePlayer(int playerid);

	void Process();

	bool OnPlayerStreamIn(unsigned short playerid, unsigned short forplayerid);
	bool OnPlayerStreamOut(unsigned short playerid, unsigned short forplayerid);

	void SetGravity(float fGravity);
	float GetGravity(void);

	eSAMPVersion GetVersion() { return m_Version; }

	unsigned char m_byteWeather;
private:
	eSAMPVersion m_Version;
	float m_fGravity;
	int m_iTicks;
};

#endif