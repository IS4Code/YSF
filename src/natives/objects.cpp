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
	// native Float:GetObjectDrawDistance(objectid);
	AMX_DECLARE_NATIVE(GetObjectDrawDistance)
	{
		CHECK_PARAMS(1, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [](CObject *&obj)
		{
			return amx_ftoc(obj->fDrawDistance);
		});
	}

	// native Float:SetObjectMoveSpeed(objectid, Float:fSpeed);
	AMX_DECLARE_NATIVE(SetObjectMoveSpeed)
	{
		CHECK_PARAMS(2, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [](CObject *&obj)
		{
			obj->fMoveSpeed = CScriptParams::Get()->ReadFloat();
			return 1;
		});
	}

	// native Float:GetObjectMoveSpeed(objectid);
	AMX_DECLARE_NATIVE(GetObjectMoveSpeed)
	{
		CHECK_PARAMS(1, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [](CObject *&obj)
		{
			return amx_ftoc(obj->fMoveSpeed);
		});
	}

	// native GetObjectTarget(objectid, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(GetObjectTarget)
	{
		CHECK_PARAMS(4, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [](CObject *&obj)
		{
			CScriptParams::Get()->Add(obj->matTarget.pos);
			return 1;
		});
	}

	// native GetObjectAttachedData(objectid, &vehicleid, &objectid, &attachedplayerid);
	AMX_DECLARE_NATIVE(GetObjectAttachedData)
	{
		CHECK_PARAMS(4, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

		return CServer::Get()->ObjectPool.Map(objectid, [=](CObject *&obj)
		{
			WORD attachedToPlayer = INVALID_PLAYER_ID;
			CServer::Get()->ObjectPool.MapExtra(objectid, [&](CObjectData &data)
			{
				attachedToPlayer = data.attachedToPlayer;
			});
			CScriptParams::Get()->Add(obj->wAttachedVehicleID, obj->wAttachedObjectID, attachedToPlayer);
			return 1;
		});
	}

	// native GetObjectAttachedOffset(objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
	AMX_DECLARE_NATIVE(GetObjectAttachedOffset)
	{
		CHECK_PARAMS(7, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [](CObject *&obj)
		{
			CScriptParams::Get()->Add(obj->vecAttachedOffset, obj->vecAttachedRotation);
			return 1;
		});
	}

	// native GetObjectSyncRotation(objectid);
	AMX_DECLARE_NATIVE(GetObjectSyncRotation)
	{
		CHECK_PARAMS(1, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [](CObject *&obj)
		{
			return obj->byteSyncRot;
		});
	}

	// native IsObjectMaterialSlotUsed(objectid, materialindex); // Return values: 1 = material, 2 = material text
	AMX_DECLARE_NATIVE(IsObjectMaterialSlotUsed)
	{
		CHECK_PARAMS(2, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();
		int materialindex = CScriptParams::Get()->ReadInt();
		if (materialindex < 0 || materialindex >= MAX_OBJECT_MATERIAL) return 0;

		return CServer::Get()->ObjectPool.Map(objectid, [=](CObject *&obj)
		{
			// We need to get the last matching material index, since SA-MP doesn't overwrite previous ones
			int index = -1;
			for (int i = 0; i < MAX_OBJECT_MATERIAL; i++)
			{
				if (obj->Material[i].byteUsed && obj->Material[i].byteSlot == materialindex) index = i;
			}

			if (index == -1) return 0;

			return static_cast<cell>(obj->Material[index].byteUsed);
		});
	}

	// native GetObjectMaterial(objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor);
	AMX_DECLARE_NATIVE(GetObjectMaterial)
	{
		CHECK_PARAMS(8, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();
		int materialindex = CScriptParams::Get()->ReadInt();

		if (materialindex < 0 || materialindex >= MAX_OBJECT_MATERIAL) return 0;

		return CServer::Get()->ObjectPool.Map(objectid, [=](CObject *&obj)
		{
			// We need to get the last matching material index, since SA-MP doesn't overwrite previous ones
			int index = -1;
			for (int i = 0; i < MAX_OBJECT_MATERIAL; i++)
			{
				if (obj->Material[i].byteUsed && obj->Material[i].byteSlot == materialindex) index = i;
			}

			if (index == -1) return 0;

			CScriptParams::Get()->Add(obj->Material[index].wModelID, obj->Material[index].szMaterialTXD, obj->Material[index].szMaterialTexture, ABGR_ARGB(obj->Material[index].dwMaterialColor));
			return 1;
		});
	}

	// native GetObjectMaterialText(objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
	AMX_DECLARE_NATIVE(GetObjectMaterialText)
	{
		CHECK_PARAMS(12, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();
		int materialindex = CScriptParams::Get()->ReadInt();

		if (materialindex < 0 || materialindex >= MAX_OBJECT_MATERIAL) return 0;

		return CServer::Get()->ObjectPool.Map(objectid, [=](CObject *&obj)
		{
			// We need to get the last matching material index, since SA-MP doesn't overwrite previous ones
			int index = -1;
			for (int i = 0; i < MAX_OBJECT_MATERIAL; i++)
			{
				if (obj->Material[i].byteUsed && obj->Material[i].byteSlot == materialindex) index = i;
			}

			if (index == -1) return 0;

			CScriptParams::Get()->Add(obj->szMaterialText[index], obj->Material[index].byteMaterialSize, obj->Material[index].szFont, obj->Material[index].byteFontSize,
				obj->Material[index].byteBold, obj->Material[index].dwFontColor, obj->Material[index].dwBackgroundColor, obj->Material[index].byteAlignment);
			return 1;
		});
	}

	// native IsObjectNoCameraCol(objectid);
	AMX_DECLARE_NATIVE(IsObjectNoCameraCol)
	{
		CHECK_PARAMS(1, LOADED);

		int objectid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->ObjectPool.Map(objectid, [](CObject *&obj)
		{
			return obj->bNoCameraCol;
		});
	}

	// native Float:GetPlayerObjectDrawDistance(playerid, objectid);
	AMX_DECLARE_NATIVE(GetPlayerObjectDrawDistance)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [](CObject *&obj)
		{
			return amx_ftoc(obj->fDrawDistance);
		});
	}

	// native Float:SetPlayerObjectMoveSpeed(playerid, objectid, Float:fSpeed);
	AMX_DECLARE_NATIVE(SetPlayerObjectMoveSpeed)
	{
		CHECK_PARAMS(3, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();
		float speed = CScriptParams::Get()->ReadFloat();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [=](CObject *&obj)
		{
			obj->fMoveSpeed = speed;
			return 1;
		});
	}

	// native Float:GetPlayerObjectMoveSpeed(playerid, objectid);
	AMX_DECLARE_NATIVE(GetPlayerObjectMoveSpeed)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [](CObject *&obj)
		{
			return amx_ftoc(obj->fMoveSpeed);
		});
	}

	// native Float:GetPlayerObjectTarget(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(GetPlayerObjectTarget)
	{
		CHECK_PARAMS(5, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [](CObject *&obj)
		{
			CScriptParams::Get()->Add(obj->matTarget.pos);
			return 1;
		});
	}

	// native GetPlayerObjectAttachedData(playerid, objectid, &vehicleid, &objectid, &attachedplayerid);
	AMX_DECLARE_NATIVE(GetPlayerObjectAttachedData)
	{
		CHECK_PARAMS(5, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [=](CObject *&obj)
		{
			WORD attachedobjectid = INVALID_OBJECT_ID;
			WORD attachedplayerid = INVALID_PLAYER_ID;
			CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &data)
			{
				auto pAddon = data.FindObjectAddon(objectid);
				if (pAddon)
				{
					attachedobjectid = pAddon->wObjectID;
					attachedplayerid = pAddon->wAttachPlayerID;
				}
			});

			CScriptParams::Get()->Add(obj->wAttachedVehicleID, attachedobjectid, attachedplayerid);
			return 1;
		});
	}

	// native GetPlayerObjectAttachedOffset(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
	AMX_DECLARE_NATIVE(GetPlayerObjectAttachedOffset)
	{
		CHECK_PARAMS(8, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [=](CObject *&obj)
		{
			CVector vecOffset, vecRot;

			if (obj->wAttachedVehicleID)
			{
				vecOffset = obj->vecAttachedOffset;
				vecRot = obj->vecAttachedRotation;
			}
			else
			{
				CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &data)
				{
					auto pAddon = data.FindObjectAddon(objectid);
					if (pAddon)
					{
						vecOffset = pAddon->vecOffset;
						vecRot = pAddon->vecRot;
					}
				});
			}

			CScriptParams::Get()->Add(vecOffset, vecRot);
			return 1;
		});
	}


	// native GetPlayerObjectSyncRotation(playerid, objectid);
	AMX_DECLARE_NATIVE(GetPlayerObjectSyncRotation)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [=](CObject *&obj)
		{
			return obj->byteSyncRot;
		});
	}

	// native IsPlayerObjectMaterialSlotUsed(playerid, objectid, materialindex); // Return values: 1 = material, 2 = material text
	AMX_DECLARE_NATIVE(IsPlayerObjectMaterialSlotUsed)
	{
		CHECK_PARAMS(3, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();
		int materialindex = CScriptParams::Get()->ReadInt();
		if (materialindex < 0 || materialindex >= MAX_OBJECT_MATERIAL) return 0;

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [=](CObject *&obj)
		{
			// We need to get the last matching material index, since SA-MP doesn't overwrite previous ones
			int index = -1;
			for (int i = 0; i < MAX_OBJECT_MATERIAL; i++)
			{
				if (obj->Material[i].byteUsed && obj->Material[i].byteSlot == materialindex) index = i;
			}

			if (index == -1) return 0;
			return static_cast<cell>(obj->Material[index].byteUsed);
		});
	}

	// native GetPlayerObjectMaterial(playerid, objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor);
	AMX_DECLARE_NATIVE(GetPlayerObjectMaterial)
	{
		CHECK_PARAMS(9, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();
		int materialindex = CScriptParams::Get()->ReadInt();
		if (materialindex < 0 || materialindex >= 16) return 0;

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [=](CObject *&obj)
		{
			// We need to get the last matching material index, since SA-MP doesn't overwrite previous ones
			int index = -1;
			for (int i = 0; i < MAX_OBJECT_MATERIAL; i++)
			{
				if (obj->Material[i].byteUsed && obj->Material[i].byteSlot == materialindex) index = i;
			}

			if (index == -1) return 0;

			CScriptParams::Get()->Add(obj->Material[index].wModelID, obj->Material[index].szMaterialTXD, obj->Material[index].szMaterialTexture, ABGR_ARGB(obj->Material[index].dwMaterialColor));
			return 1;
		});
	}

	// native GetPlayerObjectMaterialText(playerid, objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
	AMX_DECLARE_NATIVE(GetPlayerObjectMaterialText)
	{
		CHECK_PARAMS(13, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();
		int materialindex = CScriptParams::Get()->ReadInt();
		if (materialindex < 0 || materialindex >= 16) return 0;

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [=](CObject *&obj)
		{
			// We need to get the last matching material index, since SA-MP doesn't overwrite previous ones
			int index = -1;
			for (int i = 0; i < MAX_OBJECT_MATERIAL; i++)
			{
				if (obj->Material[i].byteUsed && obj->Material[i].byteSlot == materialindex) index = i;
			}

			if (index == -1) return 0;

			std::string text;
			CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &data)
			{
				for (auto o = data.m_PlayerObjectMaterialText.begin(); o != data.m_PlayerObjectMaterialText.end(); ++o)
				{
					if (o->first == objectid)
					{
						if (o->second.first == materialindex)
						{
							text = o->second.second;
						}
						break;
					}
				}
			});

			CScriptParams::Get()->Add(text, obj->Material[index].byteMaterialSize, obj->Material[index].szFont, obj->Material[index].byteFontSize,
				obj->Material[index].byteBold, obj->Material[index].dwFontColor, obj->Material[index].dwBackgroundColor, obj->Material[index].byteAlignment);
			return 1;
		});
	}

	// native IsPlayerObjectNoCameraCol(playerid, objectid);
	AMX_DECLARE_NATIVE(IsPlayerObjectNoCameraCol)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		auto &pool = CServer::Get()->PlayerObjectPool;
		return pool.Map(playerid, objectid, [](CObject *&obj)
		{
			return obj->bNoCameraCol;
		});
	}

	// native GetPlayerSurfingPlayerObjectID(playerid);
	AMX_DECLARE_NATIVE(GetPlayerSurfingPlayerObjectID)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();

		cell surf = INVALID_OBJECT_ID;
		CServer::Get()->PlayerPool.MapExtra(playerid, [&](CPlayerData &data)
		{
			surf = data.wSurfingInfo - MAX_VEHICLES;
			if (!CServer::Get()->PlayerObjectPool.IsValid(playerid, surf))
			{
				surf = INVALID_OBJECT_ID;
			}
		});
		return surf;
	}

	// native GetPlayerCameraTargetPlayerObj(playerid);
	AMX_DECLARE_NATIVE(GetPlayerCameraTargetPlayerObj)
	{
		CHECK_PARAMS(1, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();

		cell target = INVALID_OBJECT_ID;
		CServer::Get()->PlayerPool.Map(playerid, [&](CPlayer *&plr)
		{
			if (!plr->bCameraTarget) return;
			target = plr->wCameraObject;
			if (!CServer::Get()->PlayerObjectPool.IsValid(playerid, target))
			{
				target = INVALID_OBJECT_ID;
			}
		});
		return target;
	}

	// native GetObjectType(playerid, objectid);
	AMX_DECLARE_NATIVE(GetObjectType)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		CServer &server = *CServer::Get();
		if (server.PlayerObjectPool.IsValid(playerid, objectid)) return SELECT_OBJECT_PLAYER_OBJECT;
		if (server.ObjectPool.IsValid(objectid)) return SELECT_OBJECT_GLOBAL_OBJECT;
		return 0;
	}

	// native GetPlayerAttachedObject(playerid, index, &modelid, &bone, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ, Float:&fSacleX, Float:&fScaleY, Float:&fScaleZ, &materialcolor1, &materialcolor2);
	AMX_DECLARE_NATIVE(GetPlayerAttachedObject)
	{
		CHECK_PARAMS(15, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		int slot = CScriptParams::Get()->ReadInt();

		if (slot < 0 || slot >= MAX_PLAYER_ATTACHED_OBJECTS) return 0;
		return CServer::Get()->PlayerPool.Map(playerid, [=](CPlayer *&plr)
		{
			if (!plr->attachedObjectSlot[slot]) return 0;

			CAttachedObject &obj = plr->attachedObject[slot];
			CScriptParams::Get()->Add(obj.iModelID, obj.iBoneiD, obj.vecPos, obj.vecRot, obj.vecScale,
				RGBA_ABGR(obj.dwMaterialColor1), RGBA_ABGR(obj.dwMaterialColor2));
			return 1;
		});
	}

	// native AttachPlayerObjectToObject(playerid, objectid, attachtoid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ, SyncRotation = 1);
	AMX_DECLARE_NATIVE(AttachPlayerObjectToObject)
	{
		//	logprintf("attach");

		CHECK_PARAMS(10, LOADED);

		int forplayerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();
		int attachtoid = CScriptParams::Get()->ReadInt();

		return CServer::Get()->PlayerObjectPool.Map(forplayerid, objectid, [=](CObject *&obj)
		{
			return CServer::Get()->PlayerObjectPool.Map(forplayerid, attachtoid, [=](CObject *&attachto)
			{
				std::shared_ptr<CPlayerObjectAttachAddon> pAddon = CServer::Get()->PlayerPool.Extra(forplayerid).GetObjectAddon(objectid);
				if (pAddon == NULL)
					return logprintf("AttachPlayerObjectToObject: ERROR!!!!"), 0;

				// Geting data
				BYTE byteSyncRot;
				CScriptParams::Get()->Read(pAddon->vecOffset, pAddon->vecRot, byteSyncRot);

				// Storing data
				pAddon->wObjectID = static_cast<WORD>(attachtoid);
				pAddon->creation_timepoint = default_clock::now();

				// Attach it
				int iModelID = obj->iModel;
				CVector &vecPos = obj->matWorld.pos;
				CVector &vecRot = obj->vecRot;
				float fDrawDistance = 299.0;
				BYTE byteNoCameraCol = obj->bNoCameraCol;

				RakNet::BitStream bs;
				bs.Write((WORD)objectid);
				bs.Write(iModelID);
				bs.Write(vecPos);
				bs.Write(vecRot);
				bs.Write(fDrawDistance); // 159
				bs.Write(byteNoCameraCol);
				bs.Write((WORD)-1); // attached vehicle
				bs.Write((WORD)attachtoid); // attached object
				bs.Write(pAddon->vecOffset);
				bs.Write(pAddon->vecRot);
				bs.Write(byteSyncRot);

				CSAMPFunctions::RPC(&RPC_CreateObject, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CSAMPFunctions::GetPlayerIDFromIndex(forplayerid), 0, 0); // Send this on same RPC as CreateObject
				return 1;
			});
		});
	}


	// native HideObjectForPlayer(playerid, objectid);
	AMX_DECLARE_NATIVE(HideObjectForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		int forplayerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		CServer &server = *CServer::Get();
		if (!server.PlayerPool.IsValid(forplayerid)) return 0;
		if (!server.ObjectPool.IsValid(objectid)) return 0;

		server.PlayerPool.Extra(forplayerid).HideObject(objectid, true);
		return 1;
	}

	// native ShowObjectForPlayer(playerid, objectid);
	AMX_DECLARE_NATIVE(ShowObjectForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		int forplayerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		CServer &server = *CServer::Get();
		if (!server.PlayerPool.IsValid(forplayerid)) return 0;
		if (!server.ObjectPool.IsValid(objectid)) return 0;

		server.PlayerPool.Extra(forplayerid).ShowObject(objectid, true);
		return 1;
	}

	// native IsObjectHiddenForPlayer(playerid, objectid);
	AMX_DECLARE_NATIVE(IsObjectHiddenForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		int forplayerid = CScriptParams::Get()->ReadInt();
		int objectid = CScriptParams::Get()->ReadInt();

		CServer &server = *CServer::Get();
		if (!server.ObjectPool.IsValid(objectid)) return 0;
		return server.PlayerPool.MapExtra(forplayerid, [=](CPlayerData &data)
		{
			return data.IsObjectHidden(objectid);
		});
	}

	// native HideNewObjectsForPlayer(playerid, bool:toggle);
	AMX_DECLARE_NATIVE(HideNewObjectsForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		int playerid = CScriptParams::Get()->ReadInt();
		bool toggle = CScriptParams::Get()->ReadBool();

		auto &pool = CServer::Get()->PlayerPool;
		if (!pool.IsValid(playerid)) return 0;

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

		if (Original::DestroyObject(amx, params))
		{
			int objectid = CScriptParams::Get()->ReadInt();
			CServer::Get()->ObjectPool.RemoveExtra(objectid);

			auto &pool = CServer::Get()->PlayerPool;
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				if (pool.IsValid(i))
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

		if (result)
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

		if (!server.PlayerObjectPool.IsValid(playerid, objectid)) return 0;
		if (!server.PlayerPool.IsValid(attachplayerid)) return 0;

		CPlayerData &data = CServer::Get()->PlayerPool.Extra(playerid);

		// Find the space where to store data
		std::shared_ptr<CPlayerObjectAttachAddon> pAddon = data.GetObjectAddon(objectid);
		if (pAddon == NULL)
			return logprintf("AttachPlayerObjectToPlayer: ERROR!!!!"), 0;

		// Store data
		pAddon->wObjectID = static_cast<WORD>(objectid);
		pAddon->wAttachPlayerID = static_cast<WORD>(attachplayerid);
		pAddon->creation_timepoint = default_clock::now();

		// Read parameters into our map pointer
		CScriptParams::Get()->Read(pAddon->vecOffset, pAddon->vecRot, bOnlyAddToInstance);

		// If it's allowed to create object immendiately or player attach this object to herself, then create it now
		if (!bOnlyAddToInstance || playerid == attachplayerid)
		{
			if (pNetGame->pPlayerPool->pPlayer[playerid]->byteStreamedIn[attachplayerid] || playerid == attachplayerid)
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
		}
		else
		{
			// We'll attach it later to prevent crashes
			if (data.m_PlayerObjectsAttachQueue.find(objectid) != data.m_PlayerObjectsAttachQueue.end())
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

		if (Original::SetPlayerObjectMaterial(amx, params))
		{
			if (!CConfig::Get()->m_bStorePlayerObjectsMaterial) return 1;

			int playerid = CScriptParams::Get()->ReadInt();
			int objectid = CScriptParams::Get()->ReadInt();

			CObject *pObject = CServer::Get()->PlayerObjectPool[playerid][objectid];
			int index = pObject->dwMaterialCount;
			if (index < MAX_OBJECT_MATERIAL)
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

		if (Original::SetPlayerObjectMaterialText(amx, params))
		{
			if (!CConfig::Get()->m_bStorePlayerObjectsMaterial) return 1;

			int playerid = CScriptParams::Get()->ReadInt();
			int objectid = CScriptParams::Get()->ReadInt();

			CObject *pObject = CServer::Get()->PlayerObjectPool[playerid][objectid];
			int index = pObject->dwMaterialCount;
			if (index < MAX_OBJECT_MATERIAL)
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
	AMX_DEFINE_NATIVE(AttachPlayerObjectToObject)

	// Objects get - global
	AMX_DEFINE_NATIVE(GetObjectDrawDistance)
	AMX_DEFINE_NATIVE(SetObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetObjectTarget) // R6
	AMX_DEFINE_NATIVE(GetObjectAttachedData)
	AMX_DEFINE_NATIVE(GetObjectAttachedOffset)
	AMX_DEFINE_NATIVE(IsObjectMaterialSlotUsed) // R6
	AMX_DEFINE_NATIVE(GetObjectMaterial) // R6
	AMX_DEFINE_NATIVE(GetObjectMaterialText) // R6
	AMX_DEFINE_NATIVE(IsObjectNoCameraCol) // R13
	AMX_DEFINE_NATIVE(GetObjectSyncRotation) // R20

	// Objects get - player
	AMX_DEFINE_NATIVE(GetPlayerObjectDrawDistance)
	AMX_DEFINE_NATIVE(SetPlayerObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectMoveSpeed) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectTarget) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectAttachedData)
	AMX_DEFINE_NATIVE(GetPlayerObjectAttachedOffset)
	AMX_DEFINE_NATIVE(IsPlayerObjectMaterialSlotUsed) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectMaterial) // R6
	AMX_DEFINE_NATIVE(GetPlayerObjectMaterialText) // R6
	AMX_DEFINE_NATIVE(IsPlayerObjectNoCameraCol) // R13
	AMX_DEFINE_NATIVE(GetPlayerSurfingPlayerObjectID) // R12
	AMX_DEFINE_NATIVE(GetPlayerCameraTargetPlayerObj) // R13
	AMX_DEFINE_NATIVE(GetObjectType)// R12
	AMX_DEFINE_NATIVE(GetPlayerObjectSyncRotation) // R20

	// special - for attached objects
	AMX_DEFINE_NATIVE(GetPlayerAttachedObject) // R3

	AMX_DEFINE_NATIVE(HideObjectForPlayer) // R20
	AMX_DEFINE_NATIVE(ShowObjectForPlayer) // R20
	AMX_DEFINE_NATIVE(IsObjectHiddenForPlayer) //R20
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

void ObjectsLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
