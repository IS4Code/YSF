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

#include "CPlugin.h"
#include "CServer.h"
#include "CConfig.h"
#include "CCallbackManager.h"
#include "Hooks.h"
#include "Globals.h"
#include "Utils.h"
#include "System.h"
#include "Natives.h"
#include "RPCs.h"
#include "includes/platform.h"

CPlugin::CPlugin(SAMPVersion version) : main_thread(std::this_thread::get_id())
{
	m_iTicks = 0;
	m_iTickRate = 5;
	m_bNightVisionFix = true;
	m_bOnServerMessage = false;
	m_bQueryFloodCheck = true;
	m_dwAFKAccuracy = 1500;
	
	// Loading configurations from plugins/YSF.cfg
	CConfig::Init();
	LoadTickCount();

	//memset(&pPlayerData, NULL, sizeof(pPlayerData));
	
	LoadNatives();

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
	ConsoleCommand_s *cmds = *CAddress::ARRAY_ConsoleCommands;
	do
	{
		m_RCONCommands.push_back(std::string(cmds->szName));
		cmds++;
	} while (cmds->szName[0] && !cmds->dwFlags);
	//logprintf("cussess");
}

CPlugin::~CPlugin()
{

}

void CPlugin::AddPlayer(int playerid)
{
	CServer::Get()->PlayerPool.Extra(playerid);
}

bool CPlugin::RemovePlayer(int playerid)
{
	return CServer::Get()->PlayerPool.RemoveExtra(playerid);
}

void CPlugin::Process()
{
	if(m_iTickRate == -1) return;

	if(++m_iTicks >= m_iTickRate)
	{
		m_iTicks = 0;
		auto &pool = CServer::Get()->PlayerPool;
		for(WORD playerid = 0; playerid != MAX_PLAYERS; ++playerid)
		{
			if(!IsPlayerConnected(playerid)) continue;
			
			// Process player
			pool.Extra(playerid).Process();
		}
#ifdef _WIN32
		ProcessSysExec();
#endif

#ifdef NEW_PICKUP_SYSTEM
		if(CPlugin::Get()->pPickupPool)
			CPlugin::Get()->pPickupPool->Process();
#endif
	}
}
#ifdef _WIN32
void CPlugin::ProcessSysExec()
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
#endif

bool CPlugin::OnPlayerStreamIn(WORD playerid, WORD forplayerid)
{
	//logprintf("join stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);

	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(forplayerid))
		return false;

	RakNet::BitStream bs;
	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	CPlayerData &fordata = CServer::Get()->PlayerPool.Extra(forplayerid);
	for (auto& o : fordata.m_PlayerObjectsAddon)
	{
		if (o.second->wAttachPlayerID == playerid && !o.second->bCreated)
		{
			// If object isn't present in waiting queue then add it
			if (fordata.m_PlayerObjectsAttachQueue.find(o.first) == fordata.m_PlayerObjectsAttachQueue.end())
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
				fordata.m_PlayerObjectsAttachQueue.insert(o.first);

				//logprintf("add to waiting queue streamin");
			}
		}
	}
	return true;
}

bool CPlugin::OnPlayerStreamOut(WORD playerid, WORD forplayerid)
{
	//logprintf("leave stream zone playerid = %d, forplayerid = %d", playerid, forplayerid);

	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(forplayerid))
		return false;

	auto &data = CServer::Get()->PlayerPool.Extra(playerid);
	auto &fordata = CServer::Get()->PlayerPool.Extra(forplayerid);
	for (auto& o : fordata.m_PlayerObjectsAddon)
	{
		if (o.second->wAttachPlayerID == playerid)
		{
			//logprintf("object found: %d - %d", forplayerid, playerid);

			// If object isn't present in waiting queue then destroy it
			if (fordata.m_PlayerObjectsAttachQueue.find(o.first) != fordata.m_PlayerObjectsAttachQueue.end())
				fordata.m_PlayerObjectsAttachQueue.erase(o.first);

			if (o.second->bCreated)
			{
				data.DestroyObject(o.first);
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
	return true;
}

void CPlugin::AllowNickNameCharacter(char character, bool enable)
{
	if (enable)
		m_vecValidNameCharacters.insert(character);
	else
		m_vecValidNameCharacters.erase(character);

}

bool CPlugin::IsNickNameCharacterAllowed(char character)
{
	return m_vecValidNameCharacters.find(character) != m_vecValidNameCharacters.end();
}

bool CPlugin::IsValidNick(char *szName)
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
bool CPlugin::ChangeRCONCommandName(std::string const &strCmd, std::string const &strNewCmd)
{
	auto it = std::find(m_RCONCommands.begin(), m_RCONCommands.end(), strCmd);
	if (it != m_RCONCommands.end())
	{
		if (strCmd == strNewCmd)
			return 0;

		auto pos = std::distance(m_RCONCommands.begin(), it);

		// Find command in array by it's position in vector
		ConsoleCommand_s *cmds = *CAddress::ARRAY_ConsoleCommands;
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

bool CPlugin::GetRCONCommandName(std::string const &strCmd, std::string &strNewCmd)
{
	auto it = std::find(m_RCONCommands.begin(), m_RCONCommands.end(), strCmd);
	if (it != m_RCONCommands.end())
	{
		auto pos = std::distance(m_RCONCommands.begin(), it);

		// Find command in array by it's position in vector
		ConsoleCommand_s *cmds = *CAddress::ARRAY_ConsoleCommands;
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
void CPlugin::AddConsolePlayer(WORD playerid, DWORD color)
{
	if (m_ConsoleMessagePlayers.find(playerid) == m_ConsoleMessagePlayers.end())
	{
		m_ConsoleMessagePlayers.emplace(playerid, color);
	}
}

void CPlugin::RemoveConsolePlayer(WORD playerid)
{
	if (m_ConsoleMessagePlayers.find(playerid) != m_ConsoleMessagePlayers.end())
	{
		m_ConsoleMessagePlayers.erase(playerid);
	}
}

bool CPlugin::IsConsolePlayer(WORD playerid, DWORD &color)
{
	auto it = m_ConsoleMessagePlayers.find(playerid);
	if (it != m_ConsoleMessagePlayers.end())
	{
		color = it->second;
		return 1;
	}
	return 0;
}

void CPlugin::ProcessConsoleMessages(const char* str)
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

void CPlugin::SetExtendedNetStatsEnabled(bool enable)
{
	if(CAddress::ADDR_GetNetworkStats_VerbosityLevel)
	{
		*CAddress::ADDR_GetNetworkStats_VerbosityLevel = enable ? 2 : 1;
		*CAddress::ADDR_GetPlayerNetworkStats_VerbosityLevel = enable ? 2 : 1;
	}
}

bool CPlugin::IsExtendedNetStatsEnabled(void)
{
	if(CAddress::ADDR_GetNetworkStats_VerbosityLevel)
	{
		return static_cast<int>(*(BYTE*)(CAddress::ADDR_GetNetworkStats_VerbosityLevel + 1) != 1);
	}
	return false;
}

WORD CPlugin::GetMaxPlayers()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
		if (pPlayerPool->bIsNPC[i])
			count++;
	return static_cast<WORD>(CSAMPFunctions::GetIntVariable("maxplayers")) - count;
}

WORD CPlugin::GetPlayerCount()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	auto &pool = CServer::Get()->PlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
		if (IsPlayerConnected(i) && !pPlayerPool->bIsNPC[i] && !pool.Extra(i).HiddenInQuery())
			count++;
	return count;
}

WORD CPlugin::GetNPCCount()
{
	WORD count = 0;
	CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
	for (WORD i = 0; i != MAX_PLAYERS; ++i)
		if (pPlayerPool->bIsPlayerConnected[i] && pPlayerPool->bIsNPC[i])
			count++;
	return count;
}

void CPlugin::SetExclusiveBroadcast(bool toggle) 
{ 
	m_bExclusiveBroadcast = toggle;
	if (toggle) // if we just activated exclusive broadcast, exclude all players from broadcast list and let scripter readd them
	{
		auto &pool = CServer::Get()->PlayerPool;
		for (WORD i = 0; i != MAX_PLAYERS; ++i)
			if (IsPlayerConnected(i))
				pool.Extra(i).bBroadcastTo = 0;
	}
}

bool CPlugin::GetExclusiveBroadcast(void) 
{ 
	return m_bExclusiveBroadcast; 
}

RakNet::BitStream *CPlugin::BuildSyncData(RakNet::BitStream *bsOrig, WORD toplayerid)
{
	BYTE id;
	WORD playerid;

	bsOrig->SetReadOffset(0);

	bsOrig->Read(id);
	bsOrig->Read(playerid);
	
	//logprintf("BuildSyncData id %d %d to %d", id, playerid, toplayerid);
	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(toplayerid)) return nullptr;

	//logprintf("RebuildSyncData %d - %d", id, playerid);
	auto &data = CServer::Get()->PlayerPool.Extra(playerid);
	auto &todata = CServer::Get()->PlayerPool.Extra(toplayerid);
	switch (id)
	{
		case ID_PLAYER_SYNC:
		{
			if (!data.wDisabledKeysLR && !data.wDisabledKeysUD && !data.wDisabledKeys
				&& todata.customPos.find(playerid) == todata.customPos.end() && !todata.bCustomQuat[playerid] && !todata.ghostMode) break;
			
			RakNet::BitStream *bsSync = new RakNet::BitStream();

			CPlayer *p = pNetGame->pPlayerPool->pPlayer[playerid];
			WORD keys;
			
			bsSync->Write((BYTE)ID_PLAYER_SYNC);
			bsSync->Write(playerid);

			// LEFT/RIGHT KEYS
			if (p->syncData.wLRAnalog)
			{
				bsSync->Write(true);

				keys = p->syncData.wLRAnalog;
				keys &= ~data.wDisabledKeysLR;
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
				keys &= ~data.wDisabledKeysUD;
				bsSync->Write(keys);
			}
			else
			{
				bsSync->Write(false);
			}

			// Keys
			keys = p->syncData.wKeys;
			keys &= ~data.wDisabledKeys;
			bsSync->Write(keys);

			// Position
			if (todata.customPos.find(playerid) != todata.customPos.end())
				bsSync->Write((char*)&todata.customPos[playerid], sizeof(CVector));
			else
				bsSync->Write((char*)&p->syncData.vecPosition, sizeof(CVector));

			// Rotation (in quaternion)
			if (todata.bCustomQuat[playerid])
				bsSync->WriteNormQuat(todata.fCustomQuat[playerid][0], todata.fCustomQuat[playerid][1], todata.fCustomQuat[playerid][2], todata.fCustomQuat[playerid][3]);
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
			if(todata.ghostMode)
			{
				bsSync->Write((BYTE)3);
			} else {
				bsSync->Write(p->syncData.byteSpecialAction);
			}

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
			
			return bsSync;
		}
		case ID_VEHICLE_SYNC:
		{
			if (!data.wDisabledKeysLR && !data.wDisabledKeysUD && !data.wDisabledKeys) break;

			RakNet::BitStream *bsSync = new RakNet::BitStream();

			CPlayer *p = pNetGame->pPlayerPool->pPlayer[playerid];
			WORD keys;

			bsSync->Write((BYTE)ID_VEHICLE_SYNC);
			bsSync->Write(playerid);

			bsSync->Write(p->vehicleSyncData.wVehicleId);

			keys = p->vehicleSyncData.wLRAnalog;
			keys &= ~data.wDisabledKeysLR;
			bsSync->Write((short)keys);

			keys = p->vehicleSyncData.wUDAnalog;
			keys &= ~data.wDisabledKeysUD;
			bsSync->Write((short)keys);

			keys = p->vehicleSyncData.wKeys;
			keys &= ~data.wDisabledKeys;
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
			return bsSync;
		}
	}
	return nullptr;
}

char* CPlugin::GetNPCCommandLine(WORD npcid)
{
	int pid = CServer::Get()->PlayerPool.Extra(npcid).iNPCProcessID;
	return ::GetNPCCommandLine(pid);
}

int CPlugin::FindNPCProcessID(WORD npcid)
{
	char *name = pNetGame->pPlayerPool->szName[npcid];
	return ::FindNPCProcessID(name);
}

#ifdef NEW_PLAYER_OBJECT_SYSTEM
namespace Hooks
{
	AMX_DECLARE_NATIVE(DestroyPlayerObject);
}

bool CPlugin::CreatePlayerObjectLocalID(WORD playerid, WORD &objectid, bool playerobject)
{
	auto &pool = CServer::Get()->ObjectPool;
	auto &ppool = CServer::Get()->PlayerObjectPool;
	if (!playerobject && pool.IsValid(objectid))
	{
		CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &extra)
		{
			extra.localObjects.find_r(objectid).map([&](WORD id)
			{
				extra.localObjects.erase_l(id);
				WORD newobjectid = id;
				CreatePlayerObjectLocalID(playerid, newobjectid, true);
				if (newobjectid == INVALID_OBJECT_ID)
				{
					cell params[3] = {2, playerid, id};
					Hooks::DestroyPlayerObject(nullptr, params);
				}
				else {
					logprintf("replacing with %d", newobjectid);
					ppool[playerid][id]->wObjectID = newobjectid;
					CSAMPFunctions::SpawnObjectForPlayer(ppool[playerid][id], playerid);
				}
			});
		});
	}else if (ppool.IsValid(playerid, objectid))
	{
		auto &extra = CServer::Get()->PlayerPool.Extra(playerid);
		auto local = extra.localObjects.find_l(objectid);
		if (local.has_value()) return *local;
		logprintf("finding map for %d", objectid);
		for (WORD index = pool.Capacity - 1; index >= 1; index--)
		{
			if (!pool.IsValid(index) && !extra.localObjects.find_r(index).has_value())
			{
				ppool[playerid][objectid]->wObjectID = index;
				extra.localObjects.insert(objectid, index);
				objectid = index;
				return true;
			}
		}
		objectid = INVALID_OBJECT_ID;
		return true;
	}
	return false;
}

bool CPlugin::MapPlayerObjectIDToLocalID(WORD playerid, WORD &objectid)
{
	return CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &data)
	{
		auto local = data.localObjects.find_l(objectid);
		if (local.has_value())
		{
			objectid = *local;
			return true;
		}
		return false;
	});
}

bool CPlugin::MapPlayerObjectIDToServerID(WORD playerid, WORD &objectid)
{
	return CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &data)
	{
		auto server = data.localObjects.find_r(objectid);
		if (server.has_value())
		{
			objectid = *server;
			return true;
		}
		return false;
	});
}
#endif

bool CPlugin::RebuildRPCData(BYTE uniqueID, RakNet::BitStream *bsSync, WORD playerid)
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
				return false;
			
			break;
		}
		case RPC_CreateObject:
		{
			int read_offset = bsSync->GetReadOffset();
			WORD objectid;
			bsSync->Read<WORD>(objectid);
			bsSync->SetReadOffset(read_offset);

			bool hidden = CServer::Get()->PlayerPool.MapExtra(playerid, [=](CPlayerData &data)
			{
				if(data.NewObjectsHidden())
				{
					data.HideObject(objectid, false);
					return true;
				}
				return data.IsObjectHidden(objectid);
			});
			if (hidden) return false;

#ifdef NEW_PLAYER_OBJECT_SYSTEM
			if (CreatePlayerObjectLocalID(playerid, objectid, false))
			{
				if (objectid == INVALID_OBJECT_ID) return false;
				int write_offset = bsSync->GetWriteOffset();
				bsSync->SetWriteOffset(read_offset);
				bsSync->Write(objectid);
				bsSync->SetWriteOffset(write_offset);
			}
#endif
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
	return true;
}

bool CPlugin::IsMainThread() const
{
	return std::this_thread::get_id() == main_thread;
}

#ifdef SAMP_03DL

void CPlugin::CacheModelInfo(CModelInfo* info)
{
	m_modelCache[std::make_pair(std::string(info->szDffName), std::string(info->szTxdName))] = info;
}

CModelInfo* CPlugin::FindCachedModelInfo(const char *dffname, const char *txdname)
{
	auto it = m_modelCache.find(std::make_pair(std::string(dffname), std::string(txdname)));
	if (it == m_modelCache.end())
	{
		return nullptr;
	} else {
		return it->second;
	}
}

#endif
