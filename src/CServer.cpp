/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

#include "main.h"

#ifdef _WIN32
#include <tlhelp32.h>
#include <winternl.h>

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#else
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#endif

void CServer::Initialize(SAMPVersion version)
{
	m_bInitialized = true;
	m_iTicks = 0;
	m_iTickRate = 5;
	m_bNightVisionFix = true;
	m_bOnServerMessage = false;
	m_dwAFKAccuracy = 1500;
	
	// Loading configurations from plugins/YSF.cfg
	LoadConfig();

#ifndef _WIN32
	LoadTickCount();
#endif

	memset(&pPlayerData, NULL, sizeof(pPlayerData));
	bChangedVehicleColor.reset();
	memset(&COBJECT_AttachedObjectPlayer, INVALID_PLAYER_ID, sizeof(COBJECT_AttachedObjectPlayer));

	// Initialize addresses
	CAddress::Initialize(version);
	// Initialize SAMP Function
	CSAMPFunctions::PreInitialize();
	// Install pre-hooks
	InstallPreHooks();

	// Initialize default valid name characters
	for(BYTE i = '0'; i <= '9'; ++i)
	{
		m_vecValidNameCharacters.insert(i);
	}
	for(BYTE i = 'A'; i <= 'Z'; ++i)
	{
		m_vecValidNameCharacters.insert(i);
	}
	for(BYTE i = 'a'; i <= 'z'; ++i)
	{
		m_vecValidNameCharacters.insert(i);
	}
	m_vecValidNameCharacters.insert({ ']', '[', '_', '$', '=', '(', ')', '@', '.' });

	// Create mirror from SAMP server's internal array of console commands
	ConsoleCommand_s *cmds = (ConsoleCommand_s*)CAddress::ARRAY_ConsoleCommands;
	do
	{
		m_RCONCommands.push_back(std::string(cmds->szName));
		cmds++;
	} while (cmds->szName[0] && !cmds->dwFlags);
	//logprintf("cussess");
}

CServer::~CServer()
{
	for(int i = 0; i != MAX_PLAYERS; ++i)
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
		for(WORD playerid = 0; playerid != MAX_PLAYERS; ++playerid)
		{
			if(!IsPlayerConnected(playerid)) continue;
			
			// Process player
			pPlayerData[playerid]->Process();
		}
		ProcessSysExec();

#ifdef NEW_PICKUP_SYSTEM
		if(CServer::Get()->pPickupPool)
			CServer::Get()->pPickupPool->Process();
#endif
	}
}

void CServer::ProcessSysExec()
{
	std::unique_lock<std::mutex> lock(m_SysExecMutex, std::try_to_lock);
	if (lock.owns_lock()) 
	{
		while (!m_SysExecQueue.empty())
		{
			bool called = false;
			int lineidx = 0;
			SysExec_t data = m_SysExecQueue.front();
			std::vector<std::string> lines;

			Utility::split(data.output, '\n', lines);

			for (auto &line : lines)
			{
				CCallbackManager::OnSystemCommandExecute(line.c_str(), data.retval, data.index, data.success, ++lineidx, lines.size());
				called = true;
			}
			if (!called)
			{
				// we notify scripts even if executed program didn't print anything
				CCallbackManager::OnSystemCommandExecute(data.output.c_str(), data.retval, data.index, data.success, 1, 1);
			}

			m_SysExecQueue.pop();
		}
	}
}

bool CServer::OnPlayerStreamIn(WORD playerid, WORD forplayerid)
{
	//logprintf("join stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);

	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(forplayerid))
		return 0;

	RakNet::BitStream bs;
	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	for (auto o : pPlayerData[forplayerid]->m_PlayerObjectsAddon)
	{
		if (o.second->wAttachPlayerID == playerid && !o.second->bCreated)
		{
			// If object isn't present in waiting queue then add it
			if (pPlayerData[forplayerid]->m_PlayerObjectsAttachQueue.find(o.first) == pPlayerData[forplayerid]->m_PlayerObjectsAttachQueue.end())
			{				
				bs.Reset();
				bs.Write(pObjectPool->pPlayerObjects[forplayerid][o.first]->wObjectID); // m_wObjectID
				bs.Write(pObjectPool->pPlayerObjects[forplayerid][o.first]->iModel);  // iModel
				bs.Write((char*)&o.second->vecOffset, sizeof(CVector));
				bs.Write((char*)&o.second->vecRot, sizeof(CVector));
				bs.Write(pObjectPool->pPlayerObjects[forplayerid][o.first]->fDrawDistance);
				bs.Write(pObjectPool->pPlayerObjects[forplayerid][o.first]->bNoCameraCol);
				bs.Write((WORD)-1); // wAttachedVehicleID
				bs.Write((WORD)-1); // wAttachedObjectID
				bs.Write((BYTE)0); // dwMaterialCount
				CSAMPFunctions::RPC(&RPC_CreateObject, &bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);

				o.second->bCreated = true;
				o.second->creation_timepoint = default_clock::now();
				pPlayerData[forplayerid]->m_PlayerObjectsAttachQueue.insert(o.first);

				//logprintf("add to waiting queue streamin");
			}
		}
	}
	return 1;
}

bool CServer::OnPlayerStreamOut(WORD playerid, WORD forplayerid)
{
	//logprintf("leave stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);

	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(forplayerid))
		return 0;

	for (auto o : pPlayerData[forplayerid]->m_PlayerObjectsAddon)
	{
		if (o.second->wAttachPlayerID == playerid)
		{
			//logprintf("object found: %d - %d", forplayerid, playerid);

			// If object isn't present in waiting queue then destroy it
			if (pPlayerData[forplayerid]->m_PlayerObjectsAttachQueue.find(o.first) != pPlayerData[forplayerid]->m_PlayerObjectsAttachQueue.end())
				pPlayerData[forplayerid]->m_PlayerObjectsAttachQueue.erase(o.first);

			if (o.second->bCreated)
			{
				pPlayerData[playerid]->DestroyObject(o.first);
				o.second->bCreated = false;
				//logprintf("destroy streamout");
			}
			else
			{
				//logprintf("isn't created streamout");
			}
			o.second->bAttached = false;
		}
	}
	return 1;
}

void CServer::AllowNickNameCharacter(char character, bool enable)
{
	if (enable)
		m_vecValidNameCharacters.insert(character);
	else
		m_vecValidNameCharacters.erase(character);

}

bool CServer::IsNickNameCharacterAllowed(char character)
{
	return m_vecValidNameCharacters.find(character) != m_vecValidNameCharacters.end();
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

// Toggling rcon commands
bool CServer::ChangeRCONCommandName(std::string const &strCmd, std::string const &strNewCmd)
{
	auto it = std::find(m_RCONCommands.begin(), m_RCONCommands.end(), strCmd);
	if (it != m_RCONCommands.end())
	{
		if (strCmd == strNewCmd)
			return 0;

		auto pos = std::distance(m_RCONCommands.begin(), it);

		// Find command in array by it's position in vector
		ConsoleCommand_s *cmds = (ConsoleCommand_s*)CAddress::ARRAY_ConsoleCommands;
		do
		{
			cmds++;
		} while (cmds->szName[0] && !cmds->dwFlags && --pos != 0);

		// Change RCON command in samp server's internal array
		memcpy(cmds->szName, strNewCmd.c_str(), sizeof(cmds->szName));
		return 1;
	}
	return 0;
}

bool CServer::GetRCONCommandName(std::string const &strCmd, std::string &strNewCmd)
{
	auto it = std::find(m_RCONCommands.begin(), m_RCONCommands.end(), strCmd);
	if (it != m_RCONCommands.end())
	{
		auto pos = std::distance(m_RCONCommands.begin(), it);

		// Find command in array by it's position in vector
		ConsoleCommand_s *cmds = (ConsoleCommand_s*)CAddress::ARRAY_ConsoleCommands;
		do
		{
			cmds++;
		} while (cmds->szName[0] && !cmds->dwFlags && --pos != 0);

		// Get changed RCON command
		strNewCmd.append(cmds->szName);
		return 1;
	}
	return 0;
}

// Broadcasting console messages to players
void CServer::AddConsolePlayer(WORD playerid, DWORD color)
{
	if (m_ConsoleMessagePlayers.find(playerid) == m_ConsoleMessagePlayers.end())
	{
		m_ConsoleMessagePlayers.emplace(playerid, color);
	}
}

void CServer::RemoveConsolePlayer(WORD playerid)
{
	if (m_ConsoleMessagePlayers.find(playerid) != m_ConsoleMessagePlayers.end())
	{
		m_ConsoleMessagePlayers.erase(playerid);
	}
}

bool CServer::IsConsolePlayer(WORD playerid, DWORD &color)
{
	auto it = m_ConsoleMessagePlayers.find(playerid);
	if (it != m_ConsoleMessagePlayers.end())
	{
		color = it->second;
		return 1;
	}
	return 0;
}

void CServer::ProcessConsoleMessages(const char* str)
{
	if (!m_ConsoleMessagePlayers.empty())
	{
		const size_t len = strlen(str);
		RakNet::BitStream bsParams;
		for (auto x : m_ConsoleMessagePlayers)
		{
			bsParams.Reset();
			bsParams.Write((DWORD)x.second);
			bsParams.Write((DWORD)len);
			bsParams.Write(str, len);
			CSAMPFunctions::RPC(&RPC_ClientMessage, &bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(x.first), false, false);
		}
	}
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

WORD CServer::GetMaxPlayers()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
		if (pPlayerPool->bIsNPC[i])
			count++;
	return static_cast<WORD>(CSAMPFunctions::GetIntVariable("maxplayers")) - count;
}

WORD CServer::GetPlayerCount()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
		if (IsPlayerConnected(i) && !pPlayerPool->bIsNPC[i] && !pPlayerData[i]->strNameInQuery.empty())
			count++;
	return count;
}

WORD CServer::GetNPCCount()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
		if (pPlayerPool->bIsPlayerConnected[i] && pPlayerPool->bIsNPC[i])
			count++;
	return count;
}

void CServer::SetExclusiveBroadcast(bool toggle) 
{ 
	m_bExclusiveBroadcast = toggle;
	if (toggle) // if we just activated exclusive broadcast, exclude all players from broadcast list and let scripter readd them
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
			if (IsPlayerConnected(i))
				pPlayerData[i]->bBroadcastTo = 0;
}

bool CServer::GetExclusiveBroadcast(void) 
{ 
	return m_bExclusiveBroadcast; 
}

void CServer::RebuildSyncData(RakNet::BitStream *bsSync, WORD toplayerid)
{
	const int read_offset = bsSync->GetReadOffset();
	const int write_offset = bsSync->GetWriteOffset();

	BYTE id;
	WORD playerid;

	bsSync->Read(id);
	bsSync->Read(playerid);

	//logprintf("RebuildSyncData pre %d - %d", id, playerid);
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(toplayerid)) return;

	//logprintf("RebuildSyncData %d - %d", id, playerid);
	switch (id)
	{
		case ID_PLAYER_SYNC:
		{
			if (!pPlayerData[playerid]->wDisabledKeysLR && !pPlayerData[playerid]->wDisabledKeysUD && !pPlayerData[playerid]->wDisabledKeys
				&& pPlayerData[toplayerid]->customPos.find(playerid) == pPlayerData[toplayerid]->customPos.end() && !pPlayerData[toplayerid]->bCustomQuat[playerid]) break;
			
			const int owerwrite_offset = bsSync->GetReadOffset(); // skip p->vehicleSyncData.wVehicleId
			//bsSync->SetReadOffset(owerwrite_offset);

			WORD wKeysLR, wKeysUD, wKeys;
			CVector vecPos;
			float fQuat[4];

			bsSync->Read(wKeysLR);
			bsSync->Read(wKeysUD);
			bsSync->Read(wKeys);
			bsSync->Read(vecPos);
			bsSync->Read(fQuat);

			wKeysLR &= ~pPlayerData[playerid]->wDisabledKeysLR;
			wKeysUD &= ~pPlayerData[playerid]->wDisabledKeysUD;
			wKeys &= ~pPlayerData[playerid]->wDisabledKeys;

			bsSync->SetWriteOffset(owerwrite_offset);
			
			// LEFT/RIGHT KEYS
			if(wKeysLR)
				bsSync->Write(wKeysLR);
			else
				bsSync->Write(false);
			
			// UP/DOWN KEYS
			if (wKeysUD)
				bsSync->Write(wKeysUD);
			else
				bsSync->Write(false);

			// Keys
			if (wKeys)
				bsSync->Write(wKeys);
			else
				bsSync->Write(false);
			
			// Position
			if (pPlayerData[toplayerid]->customPos.find(playerid) != pPlayerData[toplayerid]->customPos.end())
				bsSync->Write((char*)pPlayerData[toplayerid]->customPos[playerid].get(), sizeof(CVector));
			else
				bsSync->Write((char*)&vecPos, sizeof(CVector));

			// Rotation (in quaternion)
			if (pPlayerData[toplayerid]->bCustomQuat[playerid])
				bsSync->WriteNormQuat(pPlayerData[toplayerid]->fCustomQuat[playerid][0], pPlayerData[toplayerid]->fCustomQuat[playerid][1], pPlayerData[toplayerid]->fCustomQuat[playerid][2], pPlayerData[toplayerid]->fCustomQuat[playerid][3]);
			else
				bsSync->WriteNormQuat(fQuat[0], fQuat[1], fQuat[2], fQuat[3]);
			
			// restore default offsets
			bsSync->SetReadOffset(read_offset);
			bsSync->SetWriteOffset(write_offset);

			/*
			bsSync->Reset();
			bsSync->Write((BYTE)ID_PLAYER_SYNC);
			bsSync->Write(playerid);

			// LEFT/RIGHT KEYS
			if (p->syncData.wLRAnalog)
			{
				bsSync->Write(true);

				keys = p->syncData.wLRAnalog;
				keys &= ~pPlayerData[playerid]->wDisabledKeysLR;
				bsSync->Write(keys);
			}
			else
			{
				bsSync->Write(false);
			}

			// UP/DOWN KEYS
			if (p->syncData.wUDAnalog)
			{
				bsSync->Write(true);

				keys = p->syncData.wUDAnalog;
				keys &= ~pPlayerData[playerid]->wDisabledKeysUD;
				bsSync->Write(keys);
			}
			else
			{
				bsSync->Write(false);
			}

			// Keys
			keys = p->syncData.wKeys;
			keys &= ~pPlayerData[playerid]->wDisabledKeys;
			bsSync->Write(keys);

			// Position
			if (pPlayerData[toplayerid]->bCustomPos[playerid])
				bsSync->Write(*pPlayerData[toplayerid]->vecCustomPos[playerid]);
			else
				bsSync->Write((char*)&p->syncData.vecPosition, sizeof(CVector));

			// Rotation (in quaternion)
			if (pPlayerData[toplayerid]->bCustomQuat[playerid])
				bsSync->WriteNormQuat(pPlayerData[toplayerid]->fCustomQuat[playerid][0], pPlayerData[toplayerid]->fCustomQuat[playerid][1], pPlayerData[toplayerid]->fCustomQuat[playerid][2], pPlayerData[toplayerid]->fCustomQuat[playerid][3]);
			else
				bsSync->WriteNormQuat(p->syncData.fQuaternion[0], p->syncData.fQuaternion[1], p->syncData.fQuaternion[2], p->syncData.fQuaternion[3]);

			// Health & armour compression
			BYTE byteSyncHealthArmour = 0;
			if (p->syncData.byteHealth > 0 && p->syncData.byteHealth < 100)
			{
				byteSyncHealthArmour = ((BYTE)(p->syncData.byteHealth / 7)) << 4;
			}
			else if (p->syncData.byteHealth >= 100)
			{
				byteSyncHealthArmour = 0xF << 4;
			}

			if (p->syncData.byteArmour > 0 && p->syncData.byteArmour < 100)
			{
				byteSyncHealthArmour |= (BYTE)(p->syncData.byteArmour / 7);
			}
			else if (p->syncData.byteArmour >= 100)
			{
				byteSyncHealthArmour |= 0xF;
			}

			bsSync->Write(byteSyncHealthArmour);

			// Current weapon
			bsSync->Write(p->syncData.byteWeapon);

			// Special action
			bsSync->Write(p->syncData.byteSpecialAction);

			// Velocity
			bsSync->WriteVector(p->syncData.vecVelocity.fX, p->syncData.vecVelocity.fY, p->syncData.vecVelocity.fZ);

			// Vehicle surfing (POSITION RELATIVE TO CAR SYNC)
			if (p->syncData.wSurfingInfo)
			{
				bsSync->Write(true);
				bsSync->Write(p->syncData.wSurfingInfo);
				bsSync->Write(p->syncData.vecSurfing.fX);
				bsSync->Write(p->syncData.vecSurfing.fY);
				bsSync->Write(p->syncData.vecSurfing.fZ);
			}
			else
			{
				bsSync->Write(false);
			}

			// Animation
			if (p->syncData.dwAnimationData)
			{
				bsSync->Write(true);
				bsSync->Write((int)p->syncData.dwAnimationData);
			}
			else bsSync->Write(false);
			*/
			break;
		}
		case ID_VEHICLE_SYNC:
		{
			if (!pPlayerData[playerid]->wDisabledKeysLR && !pPlayerData[playerid]->wDisabledKeysUD && !pPlayerData[playerid]->wDisabledKeys) break;
			
			const int owerwrite_offset = bsSync->GetReadOffset() + 16; // skip p->vehicleSyncData.wVehicleId
			bsSync->SetReadOffset(owerwrite_offset); 

			WORD wKeys, wKeysLR, wKeysUD;
			bsSync->Read(wKeysLR);
			bsSync->Read(wKeysUD);
			bsSync->Read(wKeys);

			wKeysLR &= ~pPlayerData[playerid]->wDisabledKeysLR;
			wKeysUD &= ~pPlayerData[playerid]->wDisabledKeysUD;
			wKeys &= ~pPlayerData[playerid]->wDisabledKeys;

			bsSync->SetWriteOffset(owerwrite_offset);
			bsSync->Write(wKeysLR);
			bsSync->Write(wKeysUD);
			bsSync->Write(wKeys);

			// restore default offsets
			bsSync->SetReadOffset(read_offset);
			bsSync->SetWriteOffset(write_offset);

			/*
			CPlayer *p = pNetGame->pPlayerPool->pPlayer[playerid];

			bsSync->Reset();
			bsSync->Write((BYTE)ID_VEHICLE_SYNC);
			bsSync->Write(playerid);

			bsSync->Write(p->vehicleSyncData.wVehicleId);

			keys = p->vehicleSyncData.wLRAnalog;
			keys &= ~pPlayerData[playerid]->wDisabledKeysLR;
			bsSync->Write((short)keys);

			keys = p->vehicleSyncData.wUDAnalog;
			keys &= ~pPlayerData[playerid]->wDisabledKeysUD;
			bsSync->Write((short)keys);

			keys = p->vehicleSyncData.wKeys;
			keys &= ~pPlayerData[playerid]->wDisabledKeys;
			bsSync->Write(keys);

			bsSync->WriteNormQuat(p->vehicleSyncData.fQuaternion[0], p->vehicleSyncData.fQuaternion[1], p->vehicleSyncData.fQuaternion[2], p->vehicleSyncData.fQuaternion[3]);
			bsSync->Write((char*)&p->vehicleSyncData.vecPosition, sizeof(CVector));
			bsSync->WriteVector(p->vehicleSyncData.vecVelocity.fX, p->vehicleSyncData.vecVelocity.fY, p->vehicleSyncData.vecVelocity.fZ);
			bsSync->Write((short)p->vehicleSyncData.fHealth);

			// Health & armour compression
			BYTE byteSyncHealthArmour = 0;
			if (p->vehicleSyncData.bytePlayerHealth > 0 && p->vehicleSyncData.bytePlayerHealth < 100)
			{
				byteSyncHealthArmour = ((BYTE)(p->vehicleSyncData.bytePlayerHealth / 7)) << 4;
			}
			else if (p->vehicleSyncData.bytePlayerHealth >= 100)
			{
				byteSyncHealthArmour = 0xF << 4;
			}

			if (p->vehicleSyncData.bytePlayerArmour > 0 && p->vehicleSyncData.bytePlayerArmour < 100)
			{
				byteSyncHealthArmour |= (BYTE)(p->vehicleSyncData.bytePlayerArmour / 7);
			}
			else if (p->vehicleSyncData.bytePlayerArmour >= 100)
			{
				byteSyncHealthArmour |= 0xF;
			}

			bsSync->Write(byteSyncHealthArmour);
			bsSync->Write(p->vehicleSyncData.bytePlayerWeapon);

			if (p->vehicleSyncData.byteSirenState)
			{
				bsSync->Write(true);
			}
			else
			{
				bsSync->Write(false);
			}

			if (p->vehicleSyncData.byteGearState)
			{
				bsSync->Write(true);
			}
			else
			{
				bsSync->Write(false);
			}

			if (p->vehicleSyncData.fTrainSpeed)
			{
				bsSync->Write(true);
				bsSync->Write(p->vehicleSyncData.fTrainSpeed);
			}
			else
			{
				bsSync->Write(false);
			}

			if (p->vehicleSyncData.wTrailerID)
			{
				bsSync->Write(true);
				bsSync->Write(p->vehicleSyncData.wTrailerID);
			}
			else
			{
				bsSync->Write(false);
			}
			*/
			break;
		}
	}

}

bool CServer::RebuildRPCData(BYTE uniqueID, RakNet::BitStream *bsSync, WORD playerid)
{
	switch (uniqueID)
	{
		case RPC_ScmEvent:
		{
			const int read_offset = bsSync->GetReadOffset();
			
			WORD issuerid;
			int data[4];
			bsSync->Read<WORD>(issuerid);
			bsSync->Read<int>(data[0]);
			bsSync->Read<int>(data[1]);
			bsSync->Read<int>(data[2]);
			bsSync->Read<int>(data[3]);
			bsSync->SetReadOffset(read_offset);

			if (!CCallbackManager::OnOutcomeScmEvent(playerid, issuerid, data[0], data[1], data[2], data[3])) 
				return 0;
			
			break;
		}
		case RPC_InitGame:
		{
			bool usecjwalk = static_cast<int>(pNetGame->bUseCJWalk) != 0;
			bool limitglobalchat = static_cast<int>(pNetGame->bLimitGlobalChatRadius) != 0;
			float globalchatradius = pNetGame->fGlobalChatRadius;
			float nametagdistance = pNetGame->fNameTagDrawDistance;
			bool disableenterexits = static_cast<int>(pNetGame->byteDisableEnterExits) != 0;
			bool nametaglos = static_cast<int>(pNetGame->byteNameTagLOS) != 0;
			bool manualvehengineandlights = static_cast<int>(pNetGame->bManulVehicleEngineAndLights) != 0;
			int spawnsavailable = pNetGame->iSpawnsAvailable;
			bool shownametags = static_cast<int>(pNetGame->byteShowNameTags) != 0;
			bool showplayermarkers = static_cast<int>(pNetGame->bShowPlayerMarkers) != 0;
			int onfoot_rate = CSAMPFunctions::GetIntVariable("onfoot_rate");
			int incar_rate = CSAMPFunctions::GetIntVariable("incar_rate");
			int weapon_rate = CSAMPFunctions::GetIntVariable("weapon_rate");
			int lacgompmode = CSAMPFunctions::GetIntVariable("lagcompmode");
			bool vehiclefriendlyfire = static_cast<int>(pNetGame->bVehicleFriendlyFire) != 0;

			CCallbackManager::OnPlayerClientGameInit(playerid, &usecjwalk, &limitglobalchat, &globalchatradius, &nametagdistance, &disableenterexits, &nametaglos, &manualvehengineandlights,
				&spawnsavailable, &shownametags, &showplayermarkers, &onfoot_rate, &incar_rate, &weapon_rate, &lacgompmode, &vehiclefriendlyfire);

			bsSync->Reset();
			bsSync->Write((bool)!!pNetGame->byteEnableZoneNames);
			bsSync->Write((bool)usecjwalk);
			bsSync->Write((bool)!!pNetGame->byteAllowWeapons);
			bsSync->Write(limitglobalchat);
			bsSync->Write(globalchatradius);
			bsSync->Write((bool)!!pNetGame->byteStuntBonus);
			bsSync->Write(nametagdistance);
			bsSync->Write(disableenterexits);
			bsSync->Write(nametaglos);
			bsSync->Write(manualvehengineandlights);
			bsSync->Write(pNetGame->iSpawnsAvailable);
			bsSync->Write(playerid);
			bsSync->Write(shownametags);
			bsSync->Write((int)showplayermarkers);
			bsSync->Write(pNetGame->byteWorldTimeHour);
			bsSync->Write(pNetGame->byteWeather);
			bsSync->Write(pNetGame->fGravity);
			bsSync->Write((bool)!!pNetGame->bLanMode);
			bsSync->Write(pNetGame->iDeathDropMoney);
			bsSync->Write(false);
			bsSync->Write(onfoot_rate);
			bsSync->Write(incar_rate);
			bsSync->Write(weapon_rate);
			bsSync->Write((int)2);
			bsSync->Write(lacgompmode);

			const char* szHostName = CSAMPFunctions::GetStringVariable("hostname");
			if (szHostName)
			{
				size_t len = strlen(szHostName);
				bsSync->Write((BYTE)len);
				bsSync->Write(szHostName, len);
			}
			else
			{
				bsSync->Write((BYTE)0);
			}
			bsSync->Write((char*)&pNetGame->pVehiclePool, 212); // modelsUsed
			bsSync->Write((DWORD)vehiclefriendlyfire);
			break;
		}
	}
	return 1;
}

char* CServer::GetNPCCommandLine(WORD npcid)
{
	if (!pPlayerData[npcid]) return NULL;
	int pid = pPlayerData[npcid]->iNPCProcessID;
	char *line = NULL;

#ifdef _WIN32
	HRESULT hr = 0;
	IWbemLocator         *WbemLocator = NULL;
	IWbemServices        *WbemServices = NULL;
	IEnumWbemClassObject *EnumWbem = NULL;

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&WbemLocator);
	hr = WbemLocator->ConnectServer(L"ROOT\\CIMV2", NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);

	std::ostringstream query;
	query << "SELECT CommandLine FROM Win32_Process WHERE ProcessId=" << pid;
	hr = WbemServices->ExecQuery(L"WQL", bstr_t(query.str().c_str()), WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);

	if (EnumWbem == NULL) return 0;

	IWbemClassObject *result = NULL;
	ULONG returnedCount = 0;

	while ((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK)
	{
		VARIANT CommandLine;
		hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);

		line = _com_util::ConvertBSTRToString(CommandLine.bstrVal);

		VariantClear(&CommandLine);
		result->Release();

		break;
	}

	EnumWbem->Release();
	WbemServices->Release();
	WbemLocator->Release();

	CoUninitialize();
#else
	char fname[32];
	sprintf(fname, "/proc/%d/cmdline", pid);

	FILE *fcmd = fopen(fname, "r");
	if (fcmd != NULL)
	{
		size_t size = 128;
		line = (char*)malloc(size);
		size_t total = 0, read;
		while ((read = fread(line + total, 1, size - total, fcmd)) >= size - total)
		{
			total += read;
			size *= 2;
			line = (char*)realloc(line, size);
		}
		fclose(fcmd);
		total += read;
		for (unsigned int i = 0; i < total - 1; i++)
		{
			if (!line[i]) line[i] = ' ';
		}
	}
#endif

	return line;
}

int CServer::FindNPCProcessID(WORD npcid)
{
	char *name = pNetGame->pPlayerPool->szName[npcid];
#ifdef _WIN32
	DWORD cpid = GetCurrentProcessId();

	HRESULT hr = 0;
	IWbemLocator         *WbemLocator = NULL;
	IWbemServices        *WbemServices = NULL;
	IEnumWbemClassObject *EnumWbem = NULL;

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&WbemLocator);
	hr = WbemLocator->ConnectServer(L"ROOT\\CIMV2", NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);

	std::ostringstream query;
	query << "SELECT ProcessId, CommandLine FROM Win32_Process WHERE ParentProcessId=" << cpid;
	hr = WbemServices->ExecQuery(L"WQL", bstr_t(query.str().c_str()), WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);


	if (EnumWbem == NULL) return 0;

	IWbemClassObject *result = NULL;
	ULONG returnedCount = 0;

	DWORD pid = 0;
	while ((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK)
	{
		VARIANT ProcessId, CommandLine;
		hr = result->Get(L"ProcessId", 0, &ProcessId, 0, 0);
		hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);

		char *line = _com_util::ConvertBSTRToString(CommandLine.bstrVal);

		char *iter = line;

		bool found = false;
		while (*iter)
		{
			if (*iter == ' ')
			{
				if (*++iter == '-' && *++iter == 'n' && *++iter == ' ')
				{
					if (!strncmp(++iter, name, strlen(name)))
					{
						pid = ProcessId.uintVal;
						found = true;
						break;
					}
				}
			}
			else {
				iter++;
			}
		}

		free(line);
		VariantClear(&ProcessId);
		VariantClear(&CommandLine);
		result->Release();

		if (found) break;
	}

	EnumWbem->Release();
	WbemServices->Release();
	WbemLocator->Release();

	CoUninitialize();

	return pid;
#else
	pid_t pid, cpid = getpid();

	DIR *dp = opendir("/proc/");
	if (dp != NULL)
	{
		struct dirent *ep;

		char line[1024], fname[32];
		while (ep = readdir(dp))
		{
			if (sscanf(ep->d_name, "%d", &pid) == 1)
			{
				sprintf(fname, "/proc/%d/stat", pid);

				FILE *fstat = fopen(fname, "r");
				if (fstat != NULL)
				{
					fread(line, 1, sizeof(line), fstat);
					fclose(fstat);

					int gpid;
					if (sscanf(line, "%*d (samp-npc) %*c %*d %d", &gpid) == 1)
					{
						if (gpid == cpid)
						{
							sprintf(fname, "/proc/%d/cmdline", pid);

							FILE *fcmd = fopen(fname, "r");
							if (fcmd != NULL)
							{
								size_t total = fread(line, 1, sizeof(line), fcmd);
								fclose(fcmd);

								char *iter = line;
								char *end = line + total;

								bool found = false;
								while (iter < end)
								{
									if (*iter == '\0')
									{
										if (*++iter == '-' && *++iter == 'n' && *++iter == '\0')
										{
											if (!strncmp(++iter, name, strlen(name)))
											{
												found = true;
												break;
											}
										}
									}
									else {
										iter++;
									}
								}
								if (found) break;
							}
						}
					}
				}
			}
			pid = 0;
		}
		closedir(dp);
	}
	return pid;
#endif
}

void CServer::LoadConfig()
{
	// Avoid errors when somebody forgotten to copy YSF.cfg to plugins directory
	FILE* fileConfig = fopen("plugins/YSF.cfg", "r");
	if (!fileConfig)
	{
		fileConfig = fopen("plugins/YSF.cfg", "w");

		fprintf(fileConfig, "# Protection against fake pickup ids\n");
		fprintf(fileConfig, "PickupProtection 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Protection against fakekill\n");
		fprintf(fileConfig, "DeathProtection 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Protection against sproofed dialog ids\n");
		fprintf(fileConfig, "DialogProtection 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Use redirected YSF's own RPC for spawning\n");
		fprintf(fileConfig, "UseCustomSpawn 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Allowing remote RCON connections with banned IPs (its very good to enable when you need to unban yourself)\n");
		fprintf(fileConfig, "AllowRemoteRCONWithBannedIPs 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Use this if you want to use SetMaxPlayers to increase your server slots at runtime\n");
		fprintf(fileConfig, "# DANGER: With enabling this option server will allow to connect 1000 players, doesn't matter what is your \"maxplayers\" value in server.cfg!\n");
		fprintf(fileConfig, "IncreaseRakNetInternalPlayers 0\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# If the option above isn't enabled this option won't have any effect\n");
		fprintf(fileConfig, "# Change raknet internal threads sleeping time, lowering the value migh result in smoother sync - by default is 5ms\n");
		fprintf(fileConfig, "RakNetInternalSleepTime 5\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# Delay im ms - object will be attached to player after this delay passed, lowering this delay might result in crashes \n");
		fprintf(fileConfig, "AttachObjectDelay 2000\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# SA-MP by default doesn't store material info for per-player objects, which made GetPlayerObjectMaterial/MaterialText broken\n");
		fprintf(fileConfig, "# If you just use streamer for objects and you don't wanna use those two natives below, then disable this option\n");
		fprintf(fileConfig, "StorePlayerObjectsMaterial 1\n");
		fprintf(fileConfig, "\n");
		fprintf(fileConfig, "# With this option you can load YSF on whatever server version, but it can result unwanted behavior\n");
		fprintf(fileConfig, "SkipVersionCheck 0\n");

		fclose(fileConfig);
	}

	m_bPickupProtection = static_cast<int>(Utility::CFGLoad("PickupProtection") != 0);
	m_bDeathProtection = static_cast<int>(Utility::CFGLoad("DeathProtection") != 0);
	m_bDialogProtection = static_cast<int>(Utility::CFGLoad("DialogProtection") != 0);
	m_bUseCustomSpawn = static_cast<int>(Utility::CFGLoad("UseCustomSpawn") != 0);
	m_bAllowRemoteRCONWithBannedIPs = static_cast<int>(Utility::CFGLoad("AllowRemoteRCONWithBannedIPs") != 0);
	m_bIncreaseRakNetInternalPlayers = static_cast<int>(Utility::CFGLoad("IncreaseRakNetInternalPlayers") != 0);
	m_iRakNetInternalSleepTime = Utility::CFGLoad("RakNetInternalSleepTime");
	m_iAttachObjectDelay = Utility::CFGLoad("AttachObjectDelay");
	m_bStorePlayerObjectsMaterial = static_cast<int>(Utility::CFGLoad("StorePlayerObjectsMaterial") != 0);

	//logprintf("%d, %d, %d, %d", m_bPickupProtection, m_bDeathProtection, m_iRakNetInternalSleepTime, m_iAttachObjectDelay);
}
