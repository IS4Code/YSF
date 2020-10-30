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
	AMX_DEFINE_NATIVE(GetPlayerCameraTargetPlayerObj) // R13
	AMX_DEFINE_NATIVE(GetObjectType)// R12
	AMX_DEFINE_NATIVE(GetPlayerObjectSyncRotation) // R20

	// special - for attached objects
	AMX_DEFINE_NATIVE(GetPlayerAttachedObject) // R3

	AMX_DEFINE_NATIVE(HideObjectForPlayer) // R20
	AMX_DEFINE_NATIVE(ShowObjectForPlayer) // R20
	AMX_DEFINE_NATIVE(IsObjectHiddenForPlayer) //R20
};

void ObjectsLoadNatives()
{
	RegisterNatives(native_list);
}
