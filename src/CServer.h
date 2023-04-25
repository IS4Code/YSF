#ifndef YSF_CSERVER_H
#define YSF_CSERVER_H

#include <memory>
#include "CSingleton.h"
#include "CPool.h"
#include "CPerPlayerPool.h"
#include "CPlayerData.h"
#include "CObjectData.h"
#include "CVehicleData.h"

class CServer : public CSingleton<CServer>
{
public:
	CNetGame &NetGame;
	CBoundedPool<CPlayerPool, &CNetGame::pPlayerPool, CPlayer*, MAX_PLAYERS, &CPlayerPool::pPlayer, &CPlayerPool::bIsPlayerConnected, &CPlayerPool::dwPlayerPoolSize, CPlayerData> PlayerPool;
	CBoundedPool<CVehiclePool, &CNetGame::pVehiclePool, CVehicle*, MAX_VEHICLES, &CVehiclePool::pVehicle, &CVehiclePool::bVehicleSlotState, &CVehiclePool::dwVehiclePoolSize, CVehicleData> VehiclePool;
	CBoundedPool<CActorPool, &CNetGame::pActorPool, CActor*, MAX_ACTORS, &CActorPool::pActor, &CActorPool::bValidActor, &CActorPool::dwActorPoolSize> ActorPool;
	CSlotPool<CObjectPool, &CNetGame::pObjectPool, CObject*, MAX_OBJECTS, &CObjectPool::pObjects, &CObjectPool::bObjectSlotState, CObjectData> ObjectPool;
	CSlotPool<CPickupPool, &CNetGame::pPickupPool, tPickup, MAX_PICKUPS, &CPickupPool::Pickup, &CPickupPool::bActive> PickupPool;
	CSlotPool<CMenuPool, &CNetGame::pMenuPool, CMenu*, MAX_MENUS, &CMenuPool::pMenu, &CMenuPool::bIsCreated> MenuPool;
	CSlotPool<C3DTextPool, &CNetGame::p3DTextPool, C3DText, MAX_3DTEXT_GLOBAL, &C3DTextPool::TextLabels, &C3DTextPool::bIsCreated> Text3DPool;
	CSlotPool<CTextDrawPool, &CNetGame::pTextDrawPool, CTextdraw*, MAX_TEXT_DRAWS, &CTextDrawPool::TextDraw, &CTextDrawPool::bSlotState> TextDrawPool;
	CSlotPool<CSAMPGangZonePool, &CNetGame::pGangZonePool, ARRAY<float, 4>, MAX_GANG_ZONES, &CSAMPGangZonePool::fGangZone, &CSAMPGangZonePool::bSlotState> GangZonePool;
	CSlotPerPlayerPool<CObjectPool, &CNetGame::pObjectPool, CObject*, MAX_OBJECTS, MAX_PLAYERS, &CObjectPool::pPlayerObjects, &CObjectPool::bPlayerObjectSlotState> PlayerObjectPool;

	CServer(CNetGame &netGame);
};

#endif
