#include "main.h"

CServer::CServer(eSAMPVersion version)
{
	m_iTicks = 0;
	m_iTickRate = 5;
	m_bNightVisionFix = true;
	m_dwAFKAccuracy = 1500;
	NPCTrackingEnabled = false;

	memset(&pPlayerData, NULL, sizeof(pPlayerData));
	bChangedVehicleColor.reset();
	memset(&COBJECT_AttachedObjectPlayer, INVALID_PLAYER_ID, sizeof(COBJECT_AttachedObjectPlayer));

	// Initialize addresses
	CAddress::Initialize(version);
	// Initialize SAMP Function
	CSAMPFunctions::PreInitialize();

	// Initialize default valid name characters
	for(BYTE i = '0'; i <= '9'; i++)
	{
		m_vecValidNameCharacters.push_back(i);
	}
	for(BYTE i = 'A'; i <= 'Z'; i++)
	{
		m_vecValidNameCharacters.push_back(i);
	}
	for(BYTE i = 'a'; i <= 'z'; i++)
	{
		m_vecValidNameCharacters.push_back(i);
	}
	m_vecValidNameCharacters.push_back(']');
	m_vecValidNameCharacters.push_back('[');
	m_vecValidNameCharacters.push_back('_');
	m_vecValidNameCharacters.push_back('$');
	m_vecValidNameCharacters.push_back('=');
	m_vecValidNameCharacters.push_back('(');
	m_vecValidNameCharacters.push_back(')');
	m_vecValidNameCharacters.push_back('@');
	m_vecValidNameCharacters.push_back('.');
}

CServer::~CServer()
{
	for(int i = 0; i != MAX_PLAYERS; i++)
		RemovePlayer(i);

	SAFE_DELETE(pGangZonePool);
}

bool CServer::AddPlayer(int playerid)
{
	if(!pPlayerData[playerid])
	{
		pPlayerData[playerid] = new CPlayerData(static_cast<WORD>(playerid));
		return 1;
	}
	return 0;
}

bool CServer::RemovePlayer(int playerid)
{
	if(pPlayerData[playerid])
	{
		SAFE_DELETE(pPlayerData[playerid]);
		return 1;
	}
	return 0;
}

void CServer::Process()
{
	if(m_iTickRate == -1) return;

	if(++m_iTicks >= m_iTickRate)
	{
		m_iTicks = 0;
		for(WORD playerid = 0; playerid != MAX_PLAYERS; playerid++)
		{
			if(!IsPlayerConnectedEx(playerid)) continue;
			
			// Process player
			pPlayerData[playerid]->Process();
		}
#ifdef NEW_PICKUP_SYSTEM
		if(pServer->pPickupPool)
			pServer->pPickupPool->Process();
#endif
	}
}

bool CServer::OnPlayerStreamIn(WORD playerid, WORD forplayerid)
{
	//logprintf("join stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);
	PlayerID playerId = pRakServer->GetPlayerIDFromIndex(playerid);
	PlayerID forplayerId = pRakServer->GetPlayerIDFromIndex(forplayerid);
	
	// For security..
	if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress || forplayerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress)
		return 0;

	if(!IsPlayerConnectedEx(playerid) || !IsPlayerConnectedEx(forplayerid))
		return 0;

	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	for(WORD i = 0; i != MAX_OBJECTS; i++)
	{
		if(pPlayerData[forplayerid]->stObj[i].wAttachPlayerID == playerid && !pPlayerData[forplayerid]->bAttachedObjectCreated)
		{
			//logprintf("should work");
			if(!pObjectPool->pPlayerObjects[forplayerid][i]) 
			{
				logprintf("YSF ASSERTATION FAILED <OnPlayerStreamIn> - m_pPlayerObjects = 0");
				return 0;
			}

			//logprintf("attach objects i: %d, forplayerid: %d", i, forplayerid);
			// First create the object for the player. We don't remove it from the pools, so we need to send RPC for the client to create object
			RakNet::BitStream bs;
			bs.Write(pObjectPool->pPlayerObjects[forplayerid][i]->wObjectID); // m_wObjectID
			bs.Write(pObjectPool->pPlayerObjects[forplayerid][i]->iModel);  // iModel

			bs.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fX);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fY);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecOffset.fZ);

			bs.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fX);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fY);
			bs.Write(pPlayerData[forplayerid]->stObj[i].vecRot.fZ);
			bs.Write(pObjectPool->pPlayerObjects[forplayerid][i]->fDrawDistance);
			bs.Write(pObjectPool->pPlayerObjects[forplayerid][i]->bNoCameraCol); 
			bs.Write((WORD)-1); // wAttachedVehicleID
			bs.Write((WORD)-1); // wAttachedObjectID
			bs.Write((BYTE)0); // dwMaterialCount

			pRakServer->RPC(&RPC_CreateObject, &bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, forplayerId, 0, 0);
			
			pPlayerData[forplayerid]->dwCreateAttachedObj = GetTickCount();
			pPlayerData[forplayerid]->dwObjectID = i;
			pPlayerData[forplayerid]->bAttachedObjectCreated = true;
			/*
			logprintf("join, modelid: %d, %d, %f, %f, %f, %f, %f, %f", pObjectPool->pPlayerObjects[forplayerid][i]->iModel,
				pPlayerData[forplayerid]->stObj[i].wAttachPlayerID,
				pPlayerData[forplayerid]->stObj[i].vecOffset.fX, pPlayerData[forplayerid]->stObj[i].vecOffset.fY, pPlayerData[forplayerid]->stObj[i].vecOffset.fZ,
				pPlayerData[forplayerid]->stObj[i].vecRot.fX, pPlayerData[forplayerid]->stObj[i].vecRot.fY, pPlayerData[forplayerid]->stObj[i].vecRot.fZ);
			*/
		}
	}
	return 1;
}

bool CServer::OnPlayerStreamOut(WORD playerid, WORD forplayerid)
{
	//logprintf("leave stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);
	PlayerID playerId = pRakServer->GetPlayerIDFromIndex(playerid);
	PlayerID forplayerId = pRakServer->GetPlayerIDFromIndex(forplayerid);
	
	if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress || forplayerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress)
		return 0;

	if(!IsPlayerConnectedEx(playerid) || !IsPlayerConnectedEx(forplayerid))
		return 0;

	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	for(WORD i = 0; i != MAX_OBJECTS; i++)
	{
		if(pPlayerData[forplayerid]->stObj[i].wAttachPlayerID == playerid && pPlayerData[forplayerid]->bAttachedObjectCreated)
		{
			if(!pObjectPool->pPlayerObjects[forplayerid][i]) 
			{
				logprintf("YSF ASSERTATION FAILED <OnPlayerStreamOut> - m_pPlayerObjects = 0");
				return 0;
			}

			//logprintf("remove objects i: %d, forplayerid: %d", i, forplayerid);
			if(pPlayerData[playerid]->bAttachedObjectCreated)
			{
				pPlayerData[playerid]->DestroyObject_(i);
			}
			pPlayerData[playerid]->dwCreateAttachedObj = 0;
			pPlayerData[forplayerid]->bAttachedObjectCreated = false;
			/*
			logprintf("leave, modelid: %d, %d, %f, %f, %f, %f, %f, %f", pObjectPool->pPlayerObjects[forplayerid][i]->iModel,
				pPlayerData[forplayerid]->stObj[i].wAttachPlayerID,
				pPlayerData[forplayerid]->stObj[i].vecOffset.fX, pPlayerData[forplayerid]->stObj[i].vecOffset.fY, pPlayerData[forplayerid]->stObj[i].vecOffset.fZ,
				pPlayerData[forplayerid]->stObj[i].vecRot.fX, pPlayerData[forplayerid]->stObj[i].vecRot.fY, pPlayerData[forplayerid]->stObj[i].vecRot.fZ);
			*/
		}
	}
	return 1;
}

void CServer::SetGravity_(float fGravity)
{
	// Update console
	char szGravity[16];
	sprintf(szGravity, "%f", fGravity);

	pNetGame->fGravity = fGravity;
	CSAMPFunctions::SetStringVariable("gravity", szGravity);

	// Minden játékos gravitációja átállítása arra, amire a szerver gravitációját beállítottuk
	for(WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if(IsPlayerConnectedEx(i))
			pPlayerData[i]->fGravity = fGravity; 
	}
	
	RakNet::BitStream bs;
	bs.Write(fGravity);
	pRakServer->RPC(&RPC_Gravity, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, 0);
}

float CServer::GetGravity_(void)
{
	return pNetGame->fGravity;
}

void CServer::SetWeather_(BYTE byteWeather)
{
	// Update console
	char szWeather[8];
	sprintf(szWeather, "%d", byteWeather);
	
	pNetGame->byteWeather = byteWeather;
	CSAMPFunctions::SetStringVariable("weather", szWeather);

	// Minden játékos idõjárása átállítása arra, amire a szerver idõjárást beállítottuk

	RakNet::BitStream bs;
	bs.Write(byteWeather);
	pRakServer->RPC(&RPC_Weather, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, 0);
}

BYTE CServer::GetWeather_(void)
{
	return pNetGame->byteWeather;
}

void CServer::AllowNickNameCharacter(char character, bool enable)
{
	if (enable)
	{
		std::vector<char>::iterator it = std::find(m_vecValidNameCharacters.begin(), m_vecValidNameCharacters.end(), character);
		if (it == m_vecValidNameCharacters.end())
		{
			m_vecValidNameCharacters.push_back(character);
		}
	}
	else
	{
		std::vector<char>::iterator it = std::find(m_vecValidNameCharacters.begin(), m_vecValidNameCharacters.end(), character);
		if (it != m_vecValidNameCharacters.end())
		{
			m_vecValidNameCharacters.erase(it);
		}
	}
}

bool CServer::IsNickNameCharacterAllowed(char character)
{
	std::vector<char>::iterator it = std::find(m_vecValidNameCharacters.begin(), m_vecValidNameCharacters.end(), character);
	return (it != m_vecValidNameCharacters.end());
}

bool CServer::IsValidNick(char *szName)
{
	while (*szName)
	{
		if (IsNickNameCharacterAllowed(*szName))
		{
			szName++;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void CServer::SetExtendedNetStatsEnabled(bool enable)
{
	if(CAddress::ADDR_GetNetworkStats_VerbosityLevel)
	{
		*(BYTE*)(CAddress::ADDR_GetNetworkStats_VerbosityLevel + 1) = enable ? 2 : 1;
		*(BYTE*)(CAddress::ADDR_GetPlayerNetworkStats_VerbosityLevel + 1) = enable ? 2 : 1;
	}
}

bool CServer::IsExtendedNetStatsEnabled(void)
{
	if(CAddress::ADDR_GetNetworkStats_VerbosityLevel)
	{
		return static_cast<int>(*(BYTE*)(CAddress::ADDR_GetNetworkStats_VerbosityLevel + 1) != 1);
	}
	return false;
}

WORD CServer::GetMaxPlayers_()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; i++)
		if (pPlayerPool->bIsNPC[i])
			count++;
	return static_cast<WORD>(CSAMPFunctions::GetIntVariable("maxplayers")) - count;
}

WORD CServer::GetPlayerCount()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; i++)
		if (IsPlayerConnectedEx(i) && !pPlayerPool->bIsNPC[i] && !pPlayerData[i]->bHidden)
			count++;
	return count;
}

WORD CServer::GetNPCCount()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; i++)
		if (pPlayerPool->bIsPlayerConnectedEx[i] && pPlayerPool->bIsNPC[i])
			count++;
	return count;
}

void CServer::ToggleNPCTracking(bool enable)
{
	NPCTrackingEnabled = enable;
}

bool CServer::IsNPCTrackingEnabled()
{
	return NPCTrackingEnabled;
}