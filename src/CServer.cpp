#include "CServer.h"
#include "linux_fix.h"

CServer::CServer(CNetGame &netGame) :
	NetGame(netGame),
	PlayerObjectPool(PlayerPool)
{

}