#ifndef __YSF_MAINH
#define __YSF_MAINH

class CServer;
class CNetGame;
class RakServer;
class CPlayerData;

extern CServer *pServer;

extern CNetGame *pNetGame;
extern void *pConsole;
extern RakServer *pRakServer;
extern CPlayerData *pPlayerData[500];

typedef void(*logprintf_t)(char *, ...);
extern logprintf_t logprintf;

// Defines
#define CON_VARFLAG_DEBUG		1
#define CON_VARFLAG_READONLY	2
#define CON_VARFLAG_RULE		4

#ifdef WIN32
	#define OS_NAME			"Windows"
#else
	#define OS_NAME			"Linux"
#endif

#define PROJECT_NAME		"YSF"
#define PROJECT_VERSION		"R10 BETA"

#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }

#endif