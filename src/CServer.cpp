#include "CServer.h"
#include "CPlayerObjectPool.h"
#include "linux_fix.h"

CServer::CServer(CNetGame &netGame) :
	NetGame(netGame),
	PlayerPool(*netGame.pPlayerPool),
	VehiclePool(*netGame.pVehiclePool),
	ActorPool(*netGame.pActorPool),
	ObjectPool(*netGame.pObjectPool),
	PickupPool(*netGame.pPickupPool),
	MenuPool(*netGame.pMenuPool),
	Text3DPool(*netGame.p3DTextPool),
	TextDrawPool(*netGame.pTextDrawPool),
	GangZonePool(*netGame.pGangZonePool),

	playerObjectPool(std::make_unique<CSlotPerPlayerPool<CObjectPool, CObject*, MAX_OBJECTS, MAX_PLAYERS, &CObjectPool::pPlayerObjects, &CObjectPool::bPlayerObjectSlotState>>(*netGame.pObjectPool, PlayerPool)),
	//playerObjectPool(std::make_unique<CPlayerObjectPool>(PlayerPool)),
	PlayerObjectPool(*playerObjectPool)
	//PlayerObjectPool(*netGame.pObjectPool, PlayerPool)
{

}