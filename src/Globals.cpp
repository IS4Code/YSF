#include "Globals.h"

logprintf_t logprintf = nullptr;

void **ppPluginData;
extern void *pAMXFunctions;

// Internal server pointers
CNetGame *pNetGame = NULL;
void *pConsole = NULL;
void *pRakServer = NULL;
//CPlayerData *pPlayerData[MAX_PLAYERS];