#ifndef __YSF_CSERVER
#define __YSF_CSERVER

class CNetGame;
class RakServer;

class CServer
{
public:
	CServer();
	~CServer();

	bool AddPlayer(int playerid);
	bool RemovePlayer(int playerid);

	bool OnPlayerStreamIn(unsigned short playerid, unsigned short forplayerid);
	bool OnPlayerStreamOut(unsigned short playerid, unsigned short forplayerid);

	void SetGravity(float fGravity);
	float GetGravity(void);

	void SetWeather(unsigned char byteWeather);
	unsigned char GetWeather(void);

	void OnPlayerPauseStateChange(int playerid, bool afkstate);
private:
	float m_fGravity;
	unsigned char m_byteWeather;
};

#endif