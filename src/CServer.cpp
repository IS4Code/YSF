#include "CServer.h"
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
	PlayerObjectPool(*netGame.pObjectPool, PlayerPool)
{

}