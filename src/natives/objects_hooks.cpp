#include "../Natives.h"
#include "../includes/platform.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CConfig.h"
#include "../CScriptParams.h"
#include "../CFunctions.h"
#include "../Globals.h"
#include "../Utils.h"
#include "../RPCs.h"
#include "../linux_fix.h"

namespace Natives
{
	// native GetPlayerSurfingPlayerObjectID(playerid);
	AMX_DECLARE_NATIVE(GetPlayerSurfingPlayerObjectID)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();

		cell surf = INVALID_OBJECT_ID;
		CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &data)
		{
			surf = data.wSurfingInfo - MAX_VEHICLES;
			if(!CServer::Get()->PlayerObjectPool.IsValid(playerid, surf))
			{
				surf = INVALID_OBJECT_ID;
			}
		});
		return surf;
	}

	// native HideNewObjectsForPlayer(playerid, bool:toggle);
	AMX_DECLARE_NATIVE(HideNewObjectsForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		bool toggle = CScriptParams::Get()->ReadBool();

		auto &pool = CServer::Get()->PlayerPool;
		if(!pool.IsValid(playerid)) return 0;

		pool.Extra(playerid).HideNewObjects(toggle);
		return 1;
	}

	// native NewObjectsHiddenForPlayer(playerid);
	AMX_DECLARE_NATIVE(NewObjectsHiddenForPlayer)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->PlayerPool.MapExtra(playerid, [](CPlayerData &data)
		{
			return static_cast<cell>(data.NewObjectsHidden());
		});
	}
}


namespace Original
{
	AMX_NATIVE CreateObject;
	AMX_NATIVE AttachObjectToPlayer;
	AMX_NATIVE AttachPlayerObjectToPlayer;
	AMX_NATIVE DestroyObject;
	AMX_NATIVE CreatePlayerObject;
	AMX_NATIVE SetPlayerObjectPos;
	AMX_NATIVE SetPlayerObjectRot;
	AMX_NATIVE MovePlayerObject;
	AMX_NATIVE StopPlayerObject;
	AMX_NATIVE EditPlayerObject;
	AMX_NATIVE DestroyPlayerObject;
	AMX_NATIVE SetPlayerObjectMaterial;
	AMX_NATIVE SetPlayerObjectMaterialText;
}

cell min_player_object = INVALID_OBJECT_ID;

namespace Hooks
{
	// native DestroyObject(objectid)
	AMX_DECLARE_NATIVE(DestroyObject)
	{
		CHECK_PARAMS(1, LOADED);

		if(Original::DestroyObject(amx, params))
		{
			int objectid = CScriptParams::Get()->ReadInt();
			CServer::Get()->ObjectPool.RemoveExtra(objectid);

			auto &pool = CServer::Get()->PlayerPool;
			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				if(pool.IsValid(i))
				{
					pool.Extra(i).ShowObject(objectid, false);
				}
			}

			return 1;
		}
		return 0;
	}

	// native CreatePlayerObject(playerid, ...)
	AMX_DECLARE_NATIVE(CreatePlayerObject)
	{
		if(CConfig::Get()->m_bGroupPlayerObjects)
		{
			CHECK_PARAMS(1, MORE_PARAMETER_ALLOWED);
			int playerid = CScriptParams::Get()->ReadInt();
			if(playerid >= 0 && playerid < MAX_PLAYERS && min_player_object != INVALID_OBJECT_ID)
			{
				auto pool = pNetGame->pObjectPool;
				if(!pool->bPlayersObject[min_player_object])
				{
					min_player_object = INVALID_OBJECT_ID;
				}
				if(min_player_object != INVALID_OBJECT_ID)
				{
					cell create_at = min_player_object;
					if(CServer::Get()->PlayerObjectPool.IsValid(playerid, create_at))
					{
						for(cell i = create_at + 1; i < MAX_OBJECTS; i++)
						{
							if(!pool->bObjectSlotState[i] && pool->bPlayersObject[i] && !pool->bPlayerObjectSlotState[playerid][i])
							{
								create_at = i;
								break;
							}
						}
						if(create_at == min_player_object)
						{
							cell result = Original::CreatePlayerObject(amx, params);
							if(result != INVALID_OBJECT_ID && result < min_player_object)
							{
								min_player_object = result;
							}
							return result;
						}
					}
					for(cell i = 1; i < create_at; i++)
					{
						if(!pool->bObjectSlotState[i] && !pool->bPlayersObject[i])
						{
							pool->bObjectSlotState[i] = true;
							pool->bPlayersObject[i] = true;
						}
					}
					cell result = Original::CreatePlayerObject(amx, params);
					for(cell i = 1; i < create_at; i++)
					{
						if(pool->bObjectSlotState[i] && pool->bPlayersObject[i])
						{
							pool->bObjectSlotState[i] = false;
							pool->bPlayersObject[i] = false;
						}
					}
					return result;
				}
			}
			cell result = Original::CreatePlayerObject(amx, params);
			if(result != INVALID_OBJECT_ID && result < min_player_object)
			{
				min_player_object = result;
			}
			return result;
		} else {
			return Original::CreatePlayerObject(amx, params);
		}
	}

	// native DestroyPlayerObject(playerid, objectid)
	AMX_DECLARE_NATIVE(DestroyPlayerObject)
	{
		cell result = Original::DestroyPlayerObject(amx, params);

		if(result)
		{
			CHECK_PARAMS(2, LOADED);

			int playerid = CScriptParams::Get()->ReadInt();
			int objectid = CScriptParams::Get()->ReadInt();

			CServer::Get()->PlayerPool.MapExtra(playerid, [=](CPlayerData &data)
			{
				auto o = data.m_PlayerObjectMaterialText.begin();
				while(o != data.m_PlayerObjectMaterialText.end())
				{
					if(o->first == objectid)
					{
						o = data.m_PlayerObjectMaterialText.erase(o);
					} else {
						++o;
					}
				}
				data.DeleteObjectAddon(static_cast<WORD>(objectid));
			});

			if(objectid == min_player_object)
			{
				auto pool = pNetGame->pObjectPool;
				cell i = min_player_object;
				min_player_object = INVALID_OBJECT_ID;
				for(; i < MAX_OBJECTS; i++)
				{
					if(!pool->bObjectSlotState[i] && pool->bPlayersObject[i])
					{
						min_player_object = i;
						break;
					}
				}
			}

			return 1;
		}
		return 0;
	}

	// native AttachObjectToPlayer(objectid, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ)
	AMX_DECLARE_NATIVE(AttachObjectToPlayer)
	{
		CHECK_PARAMS(8, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();
		int playerid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [=](CObject *&obj)
		{
			// FUCK SAMP -.- n_AttachObjectToPlayer always return 0
			Original::AttachObjectToPlayer(amx, params);

			// Store values which should be server purpose not mine
			CServer::Get()->ObjectPool.Extra(objectid).attachedToPlayer = static_cast<WORD>(playerid);
			CScriptParams::Get()->Read(obj->vecAttachedOffset, obj->vecAttachedRotation);
			return 1;
		});
	}

	// native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ, onlyaddtoinstance = 0)
	AMX_DECLARE_NATIVE(AttachPlayerObjectToPlayer)
	{
		CHECK_PARAMS(9, MORE_PARAMETER_ALLOWED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int objectid = CScriptParams::Get()->ReadInt();
		const int attachplayerid = CScriptParams::Get()->ReadInt();
		bool bOnlyAddToInstance;

		CServer &server = *CServer::Get();

		if(!server.PlayerObjectPool.IsValid(playerid, objectid)) return 0;
		if(!server.PlayerPool.IsValid(attachplayerid)) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);

		// Find the space where to store data
		std::shared_ptr<CPlayerObjectAttachAddon> pAddon = data.GetObjectAddon(objectid);
		if(pAddon == NULL)
			return logprintf("AttachPlayerObjectToPlayer: ERROR!!!!"), 0;

		// Store data
		pAddon->wObjectID = static_cast<WORD>(objectid);
		pAddon->wAttachPlayerID = static_cast<WORD>(attachplayerid);
		pAddon->creation_timepoint = default_clock::now();

		// Read parameters into our map pointer
		CScriptParams::Get()->Read(pAddon->vecOffset, pAddon->vecRot, bOnlyAddToInstance);

		// If it's allowed to create object immendiately or player attach this object to herself, then create it now
		if(!bOnlyAddToInstance || playerid == attachplayerid)
		{
			if(pNetGame->pPlayerPool->pPlayer[playerid]->byteStreamedIn[attachplayerid] || playerid == attachplayerid)
			{
				RakNet::BitStream bs;
				bs.Write((WORD)objectid); // wObjectID
				bs.Write((WORD)attachplayerid); // wPlayerid
				bs.Write((char*)&pAddon->vecOffset, sizeof(CVector));
				bs.Write((char*)&pAddon->vecRot, sizeof(CVector));
				pAddon->bCreated = true;
				pAddon->bAttached = true;
				CSAMPFunctions::RPC(&RPC_AttachObject, &bs, LOW_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), 0, 0);
			}
		} else
		{
			// We'll attach it later to prevent crashes
			if(data.m_PlayerObjectsAttachQueue.find(objectid) != data.m_PlayerObjectsAttachQueue.end())
			{
				data.m_PlayerObjectsAttachQueue.erase(objectid);
			}

			// This case GOTTA called only from streamer when object ALREADY created.
			data.m_PlayerObjectsAttachQueue.insert(objectid);
			pAddon->bCreated = true;
		}
		return 1;
	}

	// native SetPlayerObjectMaterial(playerid, objectid, materialindex, modelid, txdname[], texturename[], materialcolor=0);
	AMX_DECLARE_NATIVE(SetPlayerObjectMaterial)
	{
		CHECK_PARAMS(7, LOADED);

		if(Original::SetPlayerObjectMaterial(amx, params))
		{
			if(!CConfig::Get()->m_bStorePlayerObjectsMaterial) return 1;

			int playerid = CScriptParams::Get()->ReadInt();
			int objectid = CScriptParams::Get()->ReadInt();

			CObject *pObject = CServer::Get()->PlayerObjectPool[playerid][objectid];
			int index = pObject->dwMaterialCount;
			if(index < MAX_OBJECT_MATERIAL)
			{
				BYTE slot;
				WORD modelid;
				DWORD color;
				std::string szTXD, szTexture;
				CScriptParams::Get()->Read(slot, modelid, szTXD, szTexture, color);

				CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);
				auto o = data.m_PlayerObjectMaterialText.begin();
				while(o != data.m_PlayerObjectMaterialText.end())
				{
					if(o->first == objectid)
					{
						o = data.m_PlayerObjectMaterialText.erase(o);
					} else {
						++o;
					}
				}

				pObject->Material[index].byteSlot = slot;
				pObject->Material[index].wModelID = modelid;
				pObject->Material[index].byteUsed = 1;
				pObject->Material[index].dwMaterialColor = color;

				strncpy(pObject->Material[index].szMaterialTXD, szTXD.c_str(), 64u);
				strncpy(pObject->Material[index].szMaterialTexture, szTexture.c_str(), 64u);
				pObject->dwMaterialCount++;
			}
			return 1;
		}
		return 0;
	}

	// native SetPlayerObjectMaterialText(playerid, objectid, text[], materialindex = 0, materialsize = OBJECT_MATERIAL_SIZE_256x128, fontface[] = "Arial", fontsize = 24, bold = 1, fontcolor = 0xFFFFFFFF, backcolor = 0, textalignment = 0);
	AMX_DECLARE_NATIVE(SetPlayerObjectMaterialText)
	{
		CHECK_PARAMS(11, LOADED);

		if(Original::SetPlayerObjectMaterialText(amx, params))
		{
			if(!CConfig::Get()->m_bStorePlayerObjectsMaterial) return 1;

			int playerid = CScriptParams::Get()->ReadInt();
			int objectid = CScriptParams::Get()->ReadInt();

			CObject *pObject = CServer::Get()->PlayerObjectPool[playerid][objectid];
			int index = pObject->dwMaterialCount;
			if(index < MAX_OBJECT_MATERIAL)
			{
				std::string szText, szFontFace;
				BYTE slot, materialsize, fontsize, bold, textalignment;
				DWORD fontcolor, backcolor;
				CScriptParams::Get()->Read(szText, slot, materialsize, szFontFace, fontsize, bold, fontcolor, backcolor, textalignment);

				WORD objid = objectid;
				CServer::Get()->PlayerPool.Extra(playerid).m_PlayerObjectMaterialText.emplace(objid, std::make_pair((BYTE)slot, std::move(szText)));
				pObject->Material[index].byteSlot = slot;
				pObject->Material[index].byteUsed = 2;
				pObject->Material[index].byteMaterialSize = materialsize;
				strncpy(pObject->Material[index].szFont, szFontFace.c_str(), 64u);
				pObject->Material[index].byteBold = bold;
				pObject->Material[index].byteFontSize = fontsize;
				pObject->Material[index].byteAlignment = textalignment;
				pObject->Material[index].dwFontColor = fontcolor;
				pObject->Material[index].dwBackgroundColor = backcolor;
				pObject->dwMaterialCount++;
			}
			return 1;
		}
		return 0;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// Objects get - player
	AMX_DEFINE_NATIVE(GetPlayerSurfingPlayerObjectID) // R12

	AMX_DEFINE_NATIVE(HideNewObjectsForPlayer) //R20
	AMX_DEFINE_NATIVE(NewObjectsHiddenForPlayer) //R20
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(DestroyObject)
	AMX_DEFINE_HOOK(AttachObjectToPlayer)

	AMX_DEFINE_HOOK(CreatePlayerObject)
	AMX_DEFINE_HOOK(SetPlayerObjectMaterial)
	AMX_DEFINE_HOOK(SetPlayerObjectMaterialText)
	AMX_DEFINE_HOOK(AttachPlayerObjectToPlayer)
	AMX_DEFINE_HOOK(DestroyPlayerObject)
};

void ObjectsHooksLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
