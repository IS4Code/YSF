#include "../main.h"
#include "../Natives.h"
#include "../linux_fix.h"

// native Float:GetObjectDrawDistance(objectid);
AMX_DECLARE_NATIVE(Natives::GetObjectDrawDistance)
{
	CHECK_PARAMS(1, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pObjects[objectid]->fDrawDistance);
}

// native Float:SetObjectMoveSpeed(objectid, Float:fSpeed);
AMX_DECLARE_NATIVE(Natives::SetObjectMoveSpeed)
{
	CHECK_PARAMS(2, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	pNetGame->pObjectPool->pObjects[objectid]->fMoveSpeed = CScriptParams::Get()->ReadFloat();
	return 1;
}

// native Float:GetObjectMoveSpeed(objectid);
AMX_DECLARE_NATIVE(Natives::GetObjectMoveSpeed)
{
	CHECK_PARAMS(1, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pObjects[objectid]->fMoveSpeed);
}

// native GetObjectTarget(objectid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetObjectTarget)
{
	CHECK_PARAMS(4, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	CScriptParams::Get()->Add(pObject->matTarget.pos);
	return 1;
}

// native GetObjectAttachedData(objectid, &vehicleid, &objectid, &attachedplayerid);
AMX_DECLARE_NATIVE(Natives::GetObjectAttachedData)
{
	CHECK_PARAMS(4, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	CScriptParams::Get()->Add(pObject->wAttachedVehicleID, pObject->wAttachedObjectID, CServer::Get()->COBJECT_AttachedObjectPlayer[objectid]);
	return 1;
}

// native GetObjectAttachedOffset(objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
AMX_DECLARE_NATIVE(Natives::GetObjectAttachedOffset)
{
	CHECK_PARAMS(7, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	CScriptParams::Get()->Add(pObject->vecAttachedOffset, pObject->vecAttachedRotation);
	return 1;
}

// native IsObjectMaterialSlotUsed(objectid, materialindex); // Return values: 1 = material, 2 = material text
AMX_DECLARE_NATIVE(Natives::IsObjectMaterialSlotUsed)
{
	CHECK_PARAMS(2, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	const int materialindex = CScriptParams::Get()->ReadInt();
	if (materialindex < 0 || materialindex >= 16) return 0;

	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];

	// Nothing to comment here..
	while (i != MAX_OBJECT_MATERIAL)
	{
		if (pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if (i == MAX_OBJECT_MATERIAL) return 0;

	return pObject->Material[i].byteUsed;
}

// native GetObjectMaterial(objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor);
AMX_DECLARE_NATIVE(Natives::GetObjectMaterial)
{
	CHECK_PARAMS(8, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	const int materialindex = CScriptParams::Get()->ReadInt();

	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (materialindex < 0 || materialindex >= 16) return 0;

	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];

	// Nothing to comment here..
	while (i != MAX_OBJECT_MATERIAL)
	{
		if (pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if (i == MAX_OBJECT_MATERIAL) return 0;

	CScriptParams::Get()->Add(pObject->Material[i].wModelID, &pObject->Material[i].szMaterialTXD[0], &pObject->Material[i].szMaterialTexture[0], ABGR_ARGB(pObject->Material[i].dwMaterialColor));
	return 1;
}

// native GetObjectMaterialText(objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
AMX_DECLARE_NATIVE(Natives::GetObjectMaterialText)
{
	CHECK_PARAMS(12, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	const int materialindex = CScriptParams::Get()->ReadInt();

	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (materialindex < 0 || materialindex >= 16) return 0;

	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];

	// Nothing to comment here..
	while (i != MAX_OBJECT_MATERIAL)
	{
		if (pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if (i == MAX_OBJECT_MATERIAL) return 0;

	CScriptParams::Get()->Add(pObject->szMaterialText[i], pObject->Material[i].byteMaterialSize, pObject->Material[i].szFont, pObject->Material[i].byteFontSize,
		pObject->Material[i].byteBold, pObject->Material[i].dwFontColor, pObject->Material[i].dwBackgroundColor, pObject->Material[i].byteAlignment);
	return 1;
}

// native IsObjectNoCameraCol(objectid);
AMX_DECLARE_NATIVE(Natives::IsObjectNoCameraCol)
{
	CHECK_PARAMS(1, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bObjectSlotState[objectid]) return 0;

	return pNetGame->pObjectPool->pObjects[objectid]->bNoCameraCol;
}

// native Float:GetPlayerObjectDrawDistance(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectDrawDistance)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= 1000) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->fDrawDistance);
}

// native Float:SetPlayerObjectMoveSpeed(playerid, objectid, Float:fSpeed);
AMX_DECLARE_NATIVE(Natives::SetPlayerObjectMoveSpeed)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->fMoveSpeed = amx_ctof(params[3]);
	return 1;
}

// native Float:GetPlayerObjectMoveSpeed(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectMoveSpeed)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	return amx_ftoc(pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->fMoveSpeed);
}

// native Float:GetPlayerObjectTarget(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectTarget)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	CScriptParams::Get()->Add(pObject->matTarget.pos);
	return 1;
}

// native GetPlayerObjectAttachedData(playerid, objectid, &vehicleid, &objectid, &attachedplayerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectAttachedData)
{
	CHECK_PARAMS(5, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];

	WORD attachedobjectid = INVALID_OBJECT_ID;
	WORD attachedplayerid = INVALID_PLAYER_ID;
	const std::shared_ptr<CPlayerObjectAttachAddon> pAddon = pPlayerData[playerid]->FindObjectAddon(objectid);
	if (pAddon)
	{
		attachedobjectid = pAddon->wObjectID;
		attachedplayerid = pAddon->wAttachPlayerID;
	}

	CScriptParams::Get()->Add(pObject->wAttachedVehicleID, attachedobjectid, attachedplayerid);
	return 1;
}

// native GetPlayerObjectAttachedOffset(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectAttachedOffset)
{
	CHECK_PARAMS(8, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
	CVector vecOffset;
	CVector vecRot;

	if (pObject->wAttachedVehicleID)
	{
		vecOffset = pObject->vecAttachedOffset;
		vecRot = pObject->vecAttachedRotation;
	}
	else
	{
		const std::shared_ptr<CPlayerObjectAttachAddon> pAddon = pPlayerData[playerid]->FindObjectAddon(objectid);
		if (pAddon)
		{
			vecOffset = pAddon->vecOffset;
			vecOffset = pAddon->vecRot;
		}
	}

	CScriptParams::Get()->Add(vecOffset, vecRot);
	return 1;
}

// native IsPlayerObjectMaterialSlotUsed(playerid, objectid, materialindex); // Return values: 1 = material, 2 = material text
AMX_DECLARE_NATIVE(Natives::IsPlayerObjectMaterialSlotUsed)
{
	CHECK_PARAMS(3, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	const int materialindex = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (materialindex < 0 || materialindex >= 16) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];

	// Nothing to comment here..
	while (i != MAX_OBJECT_MATERIAL)
	{
		if (pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if (i == MAX_OBJECT_MATERIAL) return 0;

	return pObject->Material[i].byteUsed;
}

// native GetPlayerObjectMaterial(playerid, objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectMaterial)
{
	CHECK_PARAMS(9, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	const int materialindex = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (materialindex < 0 || materialindex >= 16) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];

	// Nothing to comment here..
	while (i != MAX_OBJECT_MATERIAL)
	{
		if (pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if (i == MAX_OBJECT_MATERIAL) return 0;

	CScriptParams::Get()->Add(pObject->Material[i].wModelID, &pObject->Material[i].szMaterialTXD[0], &pObject->Material[i].szMaterialTexture[0], ABGR_ARGB(pObject->Material[i].dwMaterialColor));
	return 1;
}

// native GetPlayerObjectMaterialText(playerid, objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
AMX_DECLARE_NATIVE(Natives::GetPlayerObjectMaterialText)
{
	CHECK_PARAMS(13, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	const int materialindex = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;
	if (materialindex < 0 || materialindex >= 16) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	int i = 0;
	CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];

	// Nothing to comment here..
	while (i != MAX_OBJECT_MATERIAL)
	{
		if (pObject->Material[i].byteSlot == materialindex) break;
		i++;
	}
	if (i == MAX_OBJECT_MATERIAL) return 0;

	std::string text;
	for (std::multimap<WORD, std::pair<BYTE, std::string>>::iterator o = pPlayerData[playerid]->m_PlayerObjectMaterialText.begin(); o != pPlayerData[playerid]->m_PlayerObjectMaterialText.end(); ++o)
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

	CScriptParams::Get()->Add(text, pObject->Material[i].byteMaterialSize, &pObject->Material[i].szFont[0], pObject->Material[i].byteFontSize,
		pObject->Material[i].byteBold, pObject->Material[i].dwFontColor, pObject->Material[i].dwBackgroundColor, pObject->Material[i].byteAlignment);
	return 1;
}

// native IsPlayerObjectNoCameraCol(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::IsPlayerObjectNoCameraCol)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	return pNetGame->pObjectPool->pPlayerObjects[playerid][objectid]->bNoCameraCol;
}

// native GetPlayerSurfingPlayerObjectID(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerSurfingPlayerObjectID)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return INVALID_OBJECT_ID;

	const int surf = pPlayerData[playerid]->wSurfingInfo - MAX_VEHICLES;
	if (surf >= 0 && surf < MAX_OBJECTS)
	{
		if (pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][surf])
			return surf;
	}
	return INVALID_OBJECT_ID;
}

// native GetPlayerCameraTargetPlayerObj(playerid);
AMX_DECLARE_NATIVE(Natives::GetPlayerCameraTargetPlayerObj)
{
	CHECK_PARAMS(1, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return INVALID_OBJECT_ID;

	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[playerid];
	if (!pPlayer->bCameraTarget) return INVALID_OBJECT_ID;

	const int target = pNetGame->pPlayerPool->pPlayer[playerid]->wCameraObject;
	if (target >= 0 && target < MAX_OBJECTS)
	{
		if (pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][target])
			return target;
	}
	return INVALID_OBJECT_ID;
}

// native GetObjectType(playerid, objectid);
AMX_DECLARE_NATIVE(Natives::GetObjectType)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (objectid < 0 || objectid >= MAX_OBJECTS) return 0;

	BYTE ret;
	if (pNetGame->pObjectPool->bObjectSlotState[objectid])
		ret = SELECT_OBJECT_GLOBAL_OBJECT;
	else if (pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid])
		ret = SELECT_OBJECT_PLAYER_OBJECT;
	else
		ret = 0;
	return ret;
}

// native GetPlayerAttachedObject(playerid, index, &modelid, &bone, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ, Float:&fSacleX, Float:&fScaleY, Float:&fScaleZ, &materialcolor1, &materialcolor2);
AMX_DECLARE_NATIVE(Natives::GetPlayerAttachedObject)
{
	CHECK_PARAMS(15, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int slot = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;
	if (slot < 0 || slot >= MAX_PLAYER_ATTACHED_OBJECTS) return 0;
	if (!pNetGame->pPlayerPool->pPlayer[playerid]->attachedObjectSlot[slot]) return 0;

	CAttachedObject *pObject = &pNetGame->pPlayerPool->pPlayer[playerid]->attachedObject[slot];

	CScriptParams::Get()->Add(pObject->iModelID, pObject->iBoneiD, pObject->vecPos, pObject->vecRot, pObject->vecScale,
		RGBA_ABGR(pObject->dwMaterialColor1), RGBA_ABGR(pObject->dwMaterialColor2));
	return 1;
}

/* --------------------------- HOOKS --------------------------- */

// native DestroyObject(objectid)
AMX_DECLARE_NATIVE(Natives::YSF_DestroyObject)
{
	CHECK_PARAMS(1, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();

	if (objectid < 0 || objectid > MAX_OBJECTS) return 0;
	if (!pNetGame->pObjectPool->pObjects[objectid]) return 0;

	if (Natives::ORIGINAL_DestroyObject(amx, params))
	{
		CServer::Get()->COBJECT_AttachedObjectPlayer[objectid] = INVALID_PLAYER_ID;
		return 1;
	}
	return 0;
}

// native DestroyPlayerObject(playerid, objectid)
AMX_DECLARE_NATIVE(Natives::YSF_DestroyPlayerObject)
{
	CHECK_PARAMS(2, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();

	if (objectid < 0 || objectid > MAX_OBJECTS) return 0;
	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	if (IsPlayerConnected(playerid))
	{
		for (std::multimap<WORD, std::pair<BYTE, std::string>>::iterator o = pPlayerData[playerid]->m_PlayerObjectMaterialText.begin(); o != pPlayerData[playerid]->m_PlayerObjectMaterialText.end(); ++o)
		{
			if (o->first == objectid)
			{
				o = pPlayerData[playerid]->m_PlayerObjectMaterialText.erase(o);
			}
		}
		pPlayerData[playerid]->DeleteObjectAddon(static_cast<WORD>(objectid));
	}

	if (Natives::ORIGINAL_DestroyPlayerObject(amx, params))
	{
		return 1;
	}
	return 0;
}

// native AttachObjectToPlayer(objectid, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ)
AMX_DECLARE_NATIVE(Natives::YSF_AttachObjectToPlayer)
{
	CHECK_PARAMS(8, LOADED);

	const int objectid = CScriptParams::Get()->ReadInt();
	if (objectid < 1 || objectid >= MAX_OBJECTS) return 0;

	const int playerid = CScriptParams::Get()->ReadInt();
	if (!IsPlayerConnected(playerid)) return 0;

	CObject *pObject = pNetGame->pObjectPool->pObjects[objectid];
	if (!pObject) return 0;

	// FUCK SAMP -.- n_AttachObjectToPlayer always return 0
	Natives::ORIGINAL_AttachObjectToPlayer(amx, params);

	// Store values which should be server purpose not mine
	CServer::Get()->COBJECT_AttachedObjectPlayer[objectid] = static_cast<WORD>(playerid);
	CScriptParams::Get()->Read(&pObject->vecAttachedOffset, &pObject->vecAttachedRotation);
	return 1;
}

// native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ, onlyaddtoinstance = 0)
AMX_DECLARE_NATIVE(Natives::YSF_AttachPlayerObjectToPlayer)
{
	CHECK_PARAMS(9, MORE_PARAMETER_ALLOWED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	const int attachplayerid = CScriptParams::Get()->ReadInt();
	bool bOnlyAddToInstance;

	if (!IsPlayerConnected(playerid)) return 0;
	if (!IsPlayerConnected(attachplayerid)) return 0;

	if (objectid < 1 || objectid >= MAX_OBJECTS) return 0;
	if (!pNetGame->pObjectPool->bPlayerObjectSlotState[playerid][objectid]) return 0;

	// Find the space where to store data
	std::shared_ptr<CPlayerObjectAttachAddon> pAddon = pPlayerData[playerid]->GetObjectAddon(objectid);
	if (pAddon == NULL)
		return logprintf("AttachPlayerObjectToPlayer: ERROR!!!!"), 0;

	// Store data
	pAddon->wObjectID = static_cast<WORD>(objectid);
	pAddon->wAttachPlayerID = static_cast<WORD>(attachplayerid);
	pAddon->creation_timepoint = default_clock::now();

	// Read parameters into our map pointer
	CScriptParams::Get()->Read(&pAddon->vecOffset, &pAddon->vecRot, &bOnlyAddToInstance);

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
		if (pPlayerData[playerid]->m_PlayerObjectsAttachQueue.find(objectid) != pPlayerData[playerid]->m_PlayerObjectsAttachQueue.end())
			pPlayerData[playerid]->m_PlayerObjectsAttachQueue.erase(objectid);

		// This case GOTTA called only from streamer when object ALREADY created.
		pPlayerData[playerid]->m_PlayerObjectsAttachQueue.insert(objectid);
		pAddon->bCreated = true;
	}
	return 1;
}

// native AttachPlayerObjectToObject(playerid, objectid, attachtoid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ, SyncRotation = 1);
AMX_DECLARE_NATIVE(Natives::AttachPlayerObjectToObject)
{
//	logprintf("attach");

	CHECK_PARAMS(10, LOADED);

	const int forplayerid = CScriptParams::Get()->ReadInt();
	const int objectid = CScriptParams::Get()->ReadInt();
	const int attachtoid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(forplayerid)) return 0;

	if (objectid < 1 || objectid >= MAX_OBJECTS) return 0;
	if (attachtoid < 1 || attachtoid >= MAX_OBJECTS) return 0;

	CObjectPool *pObjectPool = pNetGame->pObjectPool;
	if (!pObjectPool->pPlayerObjects[forplayerid][objectid] || !pObjectPool->pPlayerObjects[forplayerid][attachtoid]) return 0; // Check if object is exist

	// Find the space where to store data
	std::shared_ptr<CPlayerObjectAttachAddon> pAddon = pPlayerData[forplayerid]->GetObjectAddon(objectid);
	if (pAddon == NULL)
		return logprintf("AttachPlayerObjectToPlayer: ERROR!!!!"), 0;

	// Geting data
	BYTE byteSyncRot;
	CScriptParams::Get()->Read(&pAddon->vecOffset, &pAddon->vecRot, &byteSyncRot);

	// Storing data
	pAddon->wObjectID = static_cast<WORD>(attachtoid);
	pAddon->creation_timepoint = default_clock::now();

	// Attach it
	int &iModelID = pObjectPool->pPlayerObjects[forplayerid][objectid]->iModel;
	CVector &vecPos = pObjectPool->pPlayerObjects[forplayerid][objectid]->matWorld.pos;
	CVector &vecRot = pObjectPool->pPlayerObjects[forplayerid][objectid]->vecRot;
	float fDrawDistance = 299.0;
	BYTE byteNoCameraCol = pObjectPool->pPlayerObjects[forplayerid][objectid]->bNoCameraCol;

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
}

// native SetPlayerObjectMaterial(playerid, objectid, materialindex, modelid, txdname[], texturename[], materialcolor=0);
AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerObjectMaterial)
{
	CHECK_PARAMS(7, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();

	if (Natives::ORIGINAL_SetPlayerObjectMaterial(amx, params) && IsPlayerConnected(playerid))
	{
		const int objectid = CScriptParams::Get()->ReadInt();

		CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
		int index = pObject->dwMaterialCount;
		if (index < MAX_OBJECT_MATERIAL)
		{
			BYTE slot;
			WORD modelid;
			DWORD color;
			std::string szTXD, szTexture;
			CScriptParams::Get()->Read(&slot, &modelid, &szTXD, &szTexture, &color);
			/*
			if (pObject->szMaterialText[index])
			{
			free(pObject->szMaterialText[index]);
			pObject->szMaterialText[index] = NULL;
			}
			*/

			for (std::multimap<WORD, std::pair<BYTE, std::string>>::iterator o = pPlayerData[playerid]->m_PlayerObjectMaterialText.begin(); o != pPlayerData[playerid]->m_PlayerObjectMaterialText.end(); ++o)
			{
				if (o->first == objectid)
				{
					o = pPlayerData[playerid]->m_PlayerObjectMaterialText.erase(o);
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
AMX_DECLARE_NATIVE(Natives::YSF_SetPlayerObjectMaterialText)
{
	CHECK_PARAMS(11, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();

	if (Natives::ORIGINAL_SetPlayerObjectMaterialText(amx, params) && IsPlayerConnected(playerid))
	{
		const int objectid = CScriptParams::Get()->ReadInt();

		CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[playerid][objectid];
		int index = pObject->dwMaterialCount;
		if (index < MAX_OBJECT_MATERIAL && CServer::Get()->m_bStorePlayerObjectsMaterial)
		{
			std::string szText, szFontFace;
			BYTE slot, materialsize, fontsize, bold, textalignment;
			DWORD fontcolor, backcolor;
			CScriptParams::Get()->Read(&szText, &slot, &materialsize, &szFontFace, &fontsize, &bold, &fontcolor, &backcolor, &textalignment);

			/*
			if (pObject->szMaterialText[index])
			free(pObject->szMaterialText[index]);

			pObject->szMaterialText[index] = (char *)calloc(1u, szText.length() + 1);
			strcpy(pObject->szMaterialText[index], szText.c_str());
			*/

			WORD objid = objectid;
			pPlayerData[playerid]->m_PlayerObjectMaterialText.emplace(objid, std::make_pair((BYTE)slot, std::move(szText)));
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