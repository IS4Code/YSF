#include "main.h"

CServer::CServer(eSAMPVersion version)
{
	m_iTicks = 0;
	m_iTickRate = 5;

	memset(pPlayerData, NULL, MAX_PLAYERS);
	bChangedVehicleColor.reset();

	// Initialize addresses
	CAddress::Initialize(version);
	// Initialize SAMP Function
	CSAMPFunctions::Initialize();
}

CServer::~CServer()
{
	for(WORD i = 0; i != MAX_PLAYERS; i++)
		RemovePlayer(i);
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
		if(pNetGame)
		{
			pNetGame->pPickupPool->Process();
		}
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
		if(pPlayerData[forplayerid]->stObj[i].usAttachPlayerID == playerid)
		{
			// logprintf("should work");
			if(!pObjectPool->m_pPlayerObjects[forplayerid][i]) 
			{
				//logprintf("YSF ASSERTATION FAILED <OnPlayerStreamIn> - m_pPlayerObjects = 0");
				return 0;
			}

			//logprintf("attach objects i: %d, forplayerid: %d", i, forplayerid);
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

			pRakServer->RPC(&RPC_CreateObject, &bs2, HIGH_PRIORITY, RELIABLE_ORDERED, 0, forplayerId, 0, 0);

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

			pRakServer->RPC(&RPC_AttachObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, forplayerId, 0, 0);
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
		if(pPlayerData[forplayerid]->stObj[i].usAttachPlayerID == playerid)
		{
			if(!pObjectPool->m_pPlayerObjects[forplayerid][i]) 
			{
				//logprintf("YSF ASSERTATION FAILED <OnPlayerStreamOut> - m_pPlayerObjects = 0");
				return 0;
			}

			//logprintf("remove objects i: %d, forplayerid: %d", i, forplayerid);
			pPlayerData[playerid]->DestroyObject_(i);

			/*
			logprintf("leave %d, %f, %f, %f, %f, %f, %f", gAOData[forplayerid][i].AttachPlayerID,
				gAOData[forplayerid][i].vecOffset.fX, gAOData[forplayerid][i].vecOffset.fY, gAOData[forplayerid][i].vecOffset.fZ,
				gAOData[forplayerid][i].vecRot.fX, gAOData[forplayerid][i].vecRot.fY, gAOData[forplayerid][i].vecRot.fZ);
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
	pServer->SetStringVariable("gravity", szGravity);

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
	pServer->SetStringVariable("weather", szWeather);

	// Minden játékos idõjárása átállítása arra, amire a szerver idõjárást beállítottuk
	for (WORD i = 0; i != MAX_PLAYERS; i++)
	{
		if (IsPlayerConnectedEx(i))
			pPlayerData[i]->byteWeather = byteWeather;
	}

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
		// If vector already doesn't contain item, then add it
		if (!Contains(m_vecValidNameCharacters, character))
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
	return Contains(m_vecValidNameCharacters, character);
}

bool CServer::IsValidNick(char *szName)
{
	while (*szName)
	{
		if ((*szName >= '0' && *szName <= '9') || (*szName >= 'A' && *szName <= 'Z') || (*szName >= 'a' && *szName <= 'z') ||
			*szName == ']' || *szName == '[' || *szName == '_' || *szName == '$' || *szName == ':' || *szName == '=' ||
			*szName == '(' || *szName == ')' || *szName == '@' || *szName == '.')
		{

			szName++;
		}
		else
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
	}
	return true;
}

WORD CServer::GetMaxPlayers_()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; i++)
		if (pPlayerPool->bIsNPC[i])
			count++;
	return static_cast<WORD>(pServer->GetIntVariable("maxplayers")) - count;
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

//----------------------------------------------------

void CServer::Packet_StatsUpdate(Packet *p)
{
	RakNet::BitStream bsStats((unsigned char*)p->data, p->length, false);
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	WORD playerid = p->playerIndex;
	int money;
	int drunklevel;

	bsStats.SetReadOffset(8);
	bsStats.Read(money);
	bsStats.Read(drunklevel);

	if (!IsPlayerConnectedEx(playerid)) return;
	
	pPlayerPool->dwMoney[playerid] = money;
	pPlayerPool->dwDrunkLevel[playerid] = drunklevel;

	CCallbackManager::OnPlayerStatsAndWeaponsUpdate(playerid);
}

char* CServer::GetStringVariable(char* pVarName)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_STRING)
			return (char*)ConVar->VarPtr;
	}
	return NULL;
}

void CServer::SetStringVariable(char* pVarName, char* pString)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_STRING)
		{
			char* str = new char[strlen(pString) + 1];
			strcpy(str, pString);
			ConVar->VarPtr = str;
		}
	}
}

float CServer::GetFloatVariable(char* pVarName)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_FLOAT)
			return *(float*)ConVar->VarPtr;
	}
	return 0.0f;
}

void CServer::SetFloatVariable(char* pVarName, float fFloat)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_FLOAT)
			*(float*)ConVar->VarPtr = fFloat;
	}
}

int CServer::GetIntVariable(char* pVarName)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_INT)
			return *(int*)ConVar->VarPtr;
	}
	return 0;
}

void CServer::SetIntVariable(char* pVarName, int iInt)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_INT)
			*(int*)ConVar->VarPtr = iInt;
	}
}

bool CServer::GetBoolVariable(char* pVarName)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_BOOL)
			return *(bool*)ConVar->VarPtr;
	}
	return false;
}

void CServer::SetBoolVariable(char* pVarName, bool bBool)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		if (ConVar->VarType == CON_VARTYPE_BOOL)
			*(bool*)ConVar->VarPtr = bBool;
	}
}

DWORD CServer::GetVariableFlags(char* pVarName)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		return ConVar->VarFlags;
	}
	return 0;
}

void CServer::ModifyVariableFlags(char* pVarName, DWORD VarFlags)
{
	ConsoleVariable_s* ConVar = CSAMPFunctions::FindVariable(pVarName);
	if (ConVar != NULL)
	{
		ConVar->VarFlags = VarFlags;
	}
}
