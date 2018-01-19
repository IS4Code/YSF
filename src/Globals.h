#ifndef YSF_GLOBALS_H
#define YSF_GLOBALS_H

#include "Structs.h"
//#include "CPlayerData.h"

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

extern void *pAMXFunctions;
extern void **ppPluginData;

extern CNetGame *pNetGame;
extern void *pConsole;
extern void *pRakServer;

//extern class CPlayerData *pPlayerData[MAX_PLAYERS];

#endif