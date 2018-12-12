#include "Globals.h"

logprintf_t logprintf = nullptr;

void **ppPluginData;
extern void *pAMXFunctions;

// Internal server pointers
CNetGame *pNetGame = NULL;
void *pConsole = NULL;
void *pRakServer = NULL;
//CPlayerData *pPlayerData[MAX_PLAYERS];

sentinel::sentinel()
{
	check(true);
}

sentinel::~sentinel()
{
	check(false);
}

void sentinel::check(bool ctor)
{
	if(pNetGame && pNetGame->pVehiclePool)
	{
		auto &pool = *pNetGame->pVehiclePool;
		for(WORD i = 0; i < MAX_VEHICLES; i++)
		{
			if(pool.bVehicleSlotState[i])
			{
				if(pool.pVehicle[i]->wVehicleID != i)
				{
					if(ctor)
					{
						logprintf("Vehicle pool corrupted before for %d", i);
					}else{
						logprintf("Vehicle pool corrupted here for %d", i);
					}
					((logprintf_t)pConsole)("");
				}
			}
		}
	}
}
