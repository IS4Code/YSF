#include "Structs.h"
#include "Globals.h"
#include "CPool.h"

CSlotPool<CPlayerPool, CPlayer, MAX_PLAYERS, &CPlayerPool::pPlayer, &CPlayerPool::bIsPlayerConnected, CPlayerData> PlayerPool(*pNetGame->pPlayerPool);
