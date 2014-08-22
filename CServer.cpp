#include "main.h"

CServer::CServer()
{
	m_fGravity = 0.008f;
	m_byteWeather = 10;
	m_iTicks = 0;
}

CServer::~CServer()
{
	for(ushort i = 0; i != MAX_PLAYERS; i++)
		RemovePlayer(i);
}

bool CServer::AddPlayer(int playerid)
{
	if(!pPlayerData[playerid])
	{
		pPlayerData[playerid] = new CPlayerData(playerid);
		return 1;
	}
	return 0;
}

bool CServer::RemovePlayer(int playerid)
{
	if(pPlayerData[playerid])
	{
		delete pPlayerData[playerid];
		pPlayerData[playerid] = NULL;
		return 1;
	}
	return 0;
}

void CServer::Process()
{
	if(++m_iTicks == 10)
	{
		m_iTicks = 0;
		for(WORD playerid = 0; playerid != MAX_PLAYERS; playerid++)
		{
			CPlayerData *pPlayerPointer = pPlayerData[playerid];
			if(!pPlayerPointer) continue;

			// Process player
			pPlayerPointer->Process();
		}

		if(pNetGame)
		{
			pNetGame->pPickupPool->Process();
		}
	}
}

bool CServer::OnPlayerStreamIn(unsigned short playerid, unsigned short forplayerid)
{
	//logprintf("join stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);
	PlayerID playerId = pRakServer->GetPlayerIDFromIndex(playerid);
	PlayerID forplayerId = pRakServer->GetPlayerIDFromIndex(forplayerid);
	
	// For security..
	if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress || forplayerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress)
		return 0;

	if(!pPlayerData[playerid] || !pPlayerData[forplayerid])
		return 0;

	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	for(WORD i = 0; i != MAX_OBJECTS; i++)
	{
		if(pPlayerData[forplayerid]->stObj[i].usAttachPlayerID == playerid)
		{
			// logprintf("should work");
			if(!pObjectPool->m_pPlayerObjects[forplayerid][i]) 
			{
				//logprintf("YSF ASSERTATION FAILED <OnPlayerStreamIn> - m_pPlayerObjects = 0");
				return 0;
			}

			logprintf("attach objects i: %d, forplayerid: %d", i, forplayerid);
			// First create the object for the player. We don't remove it from the pools, so we need to send RPC for the client to create object
			RakNet::BitStream bs2;
			bs2.Write(pObjectPool->m_pPlayerObjects[forplayerid][i]->wObjectID); // m_wObjectID
			bs2.Write(pObjectPool->m_pPlayerObjects[forplayerid][i]->iModel);  // iModel

			bs2.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fX);
			bs2.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fY);
			bs2.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fZ);

			bs2.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fX);
			bs2.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fY);
			bs2.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fZ);
			bs2.Write(300.0f); // fDrawDistance

			pRakServer->RPC(&RPC_CreateObject, &bs2, HIGH_PRIORITY, RELIABLE_ORDERED, 2, forplayerId, 0, 0);

			// Attach created object to player
			RakNet::BitStream bs;
			bs.Write(pObjectPool->m_pPlayerObjects[forplayerid][i]->wObjectID); // m_wObjectID
			bs.Write(pPlayerData[forplayerid]->stObj[i].usAttachPlayerID); // playerid

			bs.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fX);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fY);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fZ);

			bs.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fX);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fY);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fZ);

			pRakServer->RPC(&RPC_AttachObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 2, forplayerId, 0, 0);
			/*
			logprintf("join, modelid: %d, %d, %f, %f, %f, %f, %f, %f",pObjectPool->m_pPlayerObjects[forplayerid][i]->m_iModel,
				gAOData[forplayerid][i].AttachPlayerID,
				gAOData[forplayerid][i].vecOffset.fX, gAOData[forplayerid][i].vecOffset.fY, gAOData[forplayerid][i].vecOffset.fZ,
				gAOData[forplayerid][i].vecRot.fX, gAOData[forplayerid][i].vecRot.fY, gAOData[forplayerid][i].vecRot.fZ);
			*/
		}
	}
	return 1;
}

bool CServer::OnPlayerStreamOut(unsigned short playerid, unsigned short forplayerid)
{
	//logprintf("leave stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);
	PlayerID playerId = pRakServer->GetPlayerIDFromIndex(playerid);
	PlayerID forplayerId = pRakServer->GetPlayerIDFromIndex(forplayerid);
	
	if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress || forplayerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress)
		return 0;

	if(!pPlayerData[playerid] || !pPlayerData[forplayerid])
		return 0;

	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	for(WORD i = 0; i != MAX_OBJECTS; i++)
	{
		if(pPlayerData[forplayerid]->stObj[i].usAttachPlayerID == playerid)
		{
			if(!pObjectPool->m_pPlayerObjects[forplayerid][i]) 
			{
				//logprintf("YSF ASSERTATION FAILED <OnPlayerStreamOut> - m_pPlayerObjects = 0");
				return 0;
			}

			logprintf("remove objects i: %d, forplayerid: %d", i, forplayerid);
			pPlayerData[playerid]->DestroyObject(i);

			/*
			logprintf("leave %d, %f, %f, %f, %f, %f, %f", gAOData[forplayerid][i].AttachPlayerID,
				gAOData[forplayerid][i].vecOffset.fX, gAOData[forplayerid][i].vecOffset.fY, gAOData[forplayerid][i].vecOffset.fZ,
				gAOData[forplayerid][i].vecRot.fX, gAOData[forplayerid][i].vecRot.fY, gAOData[forplayerid][i].vecRot.fZ);
			*/
		}
	}
	return 1;
}

void CServer::SetGravity(float fGravity)
{
	// Update console
	char szGravity[16];
	sprintf(szGravity, "%f", fGravity);
	SetServerRule("gravity", szGravity);

	// Minden játékos gravitációja átállítása arra, amire a szerver gravitációját beállítottuk
	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if(pPlayerData[i])
			pPlayerData[i]->fGravity = fGravity; 
	}
	
	RakNet::BitStream bs;
	bs.Write(fGravity);
	pRakServer->RPC(&RPC_Gravity, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 2, UNASSIGNED_PLAYER_ID, true, 0);
}

float CServer::GetGravity(void)
{
	return m_fGravity;
}

void CServer::SetWeather(BYTE byteWeather)
{
	// Update console
	char szWeather[8];
	sprintf(szWeather, "%d", byteWeather);
	SetServerRule("weather", szWeather);

	// Minden játékos idõjárása átállítása arra, amire a szerver idõjárást beállítottuk
	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if(pPlayerData[i])
			pPlayerData[i]->byteWeather = byteWeather; 
	}
	
	RakNet::BitStream bs;
	bs.Write(byteWeather);
	pRakServer->RPC(&RPC_Weather, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 2, UNASSIGNED_PLAYER_ID, true, 0);
}

BYTE CServer::GetWeather(void)
{
	return m_byteWeather;
}
