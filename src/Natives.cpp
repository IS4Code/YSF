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

#include <memory>

#include "Natives.h"

AMX_NATIVE Natives::ORIGINAL_AttachObjectToPlayer = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerWeather = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerWorldBounds = NULL;
AMX_NATIVE Natives::ORIGINAL_DestroyObject = NULL;
AMX_NATIVE Natives::ORIGINAL_DestroyPlayerObject = NULL;
AMX_NATIVE Natives::ORIGINAL_TogglePlayerControllable = NULL;
AMX_NATIVE Natives::ORIGINAL_ChangeVehicleColor = NULL;
AMX_NATIVE Natives::ORIGINAL_DestroyVehicle = NULL;
AMX_NATIVE Natives::ORIGINAL_ShowPlayerDialog = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerObjectMaterial = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerObjectMaterialText = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerTeam = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerSkin = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerName = NULL;
AMX_NATIVE Natives::ORIGINAL_SetPlayerFightingStyle = NULL;

stRedirectedNatives redirected_native_list[] =
{
	// File
	{ "AttachObjectToPlayer",			&Natives::ORIGINAL_AttachObjectToPlayer,		Natives::YSF_AttachObjectToPlayer },
	{ "AttachPlayerObjectToPlayer",		NULL,											Natives::YSF_AttachPlayerObjectToPlayer },
	{ "GetGravity",						NULL,											Natives::YSF_GetGravity },
	{ "SetPlayerWeather",				&Natives::ORIGINAL_SetPlayerWeather,			Natives::YSF_SetPlayerWeather },
	{ "SetPlayerWorldBounds",			&Natives::ORIGINAL_SetPlayerWorldBounds,		Natives::YSF_SetPlayerWorldBounds },
	{ "DestroyObject",					&Natives::ORIGINAL_DestroyObject,				Natives::YSF_DestroyObject },
	{ "DestroyPlayerObject",			&Natives::ORIGINAL_DestroyPlayerObject,			Natives::YSF_DestroyPlayerObject },
	{ "TogglePlayerControllable",		&Natives::ORIGINAL_TogglePlayerControllable,	Natives::YSF_TogglePlayerControllable},
	{ "ChangeVehicleColor",				&Natives::ORIGINAL_ChangeVehicleColor,			Natives::YSF_ChangeVehicleColor},
	{ "DestroyVehicle",					&Natives::ORIGINAL_DestroyVehicle,				Natives::YSF_DestroyVehicle},
	{ "ShowPlayerDialog",				&Natives::ORIGINAL_ShowPlayerDialog,			Natives::YSF_ShowPlayerDialog },
	{ "SetPlayerObjectMaterial",		&Natives::ORIGINAL_SetPlayerObjectMaterial,		Natives::YSF_SetPlayerObjectMaterial },
	{ "SetPlayerObjectMaterialText",	&Natives::ORIGINAL_SetPlayerObjectMaterialText,	Natives::YSF_SetPlayerObjectMaterialText },

	{ "GangZoneCreate",					NULL,											Natives::YSF_GangZoneCreate },
	{ "GangZoneDestroy",				NULL,											Natives::YSF_GangZoneDestroy },
	{ "GangZoneShowForPlayer",			NULL,											Natives::YSF_GangZoneShowForPlayer },
	{ "GangZoneHideForPlayer",			NULL,											Natives::YSF_GangZoneHideForPlayer },
	{ "GangZoneShowForAll",				NULL,											Natives::YSF_GangZoneShowForAll },
	{ "GangZoneHideForAll",				NULL,											Natives::YSF_GangZoneHideForAll },
								
	{ "GangZoneFlashForPlayer",			NULL,											Natives::YSF_GangZoneFlashForPlayer },
	{ "GangZoneFlashForAll",			NULL,											Natives::YSF_GangZoneFlashForAll },
	{ "GangZoneStopFlashForPlayer",		NULL,											Natives::YSF_GangZoneStopFlashForPlayer },
	{ "GangZoneStopFlashForAll",		NULL,											Natives::YSF_GangZoneStopFlashForAll },
#ifdef NEW_PICKUP_SYSTEM
	{ "CreatePickup",					NULL,											Natives::CreatePickup },
	{ "AddStaticPickup",				NULL,											Natives::CreatePickup },
	{ "DestroyPickup",					NULL,											Natives::DestroyPickup },
#endif
	{ "GetWeaponName",					NULL,											Natives::FIXED_GetWeaponName },
	{ "IsPlayerConnected",				NULL,											Natives::FIXED_IsPlayerConnected },

	{ "SetPlayerTeam",					&Natives::ORIGINAL_SetPlayerTeam,				Natives::YSF_SetPlayerTeam },
	{ "SetPlayerSkin",					&Natives::ORIGINAL_SetPlayerSkin,				Natives::YSF_SetPlayerSkin },
	{ "SetPlayerName",					&Natives::ORIGINAL_SetPlayerName,				Natives::YSF_SetPlayerName },
	{ "SetPlayerFightingStyle",			&Natives::ORIGINAL_SetPlayerFightingStyle,		Natives::YSF_SetPlayerFightingStyle },

	{ NULL,								NULL,									NULL }
};

int InitNatives(AMX *amx)
{
	ActorsInitNatives(amx);
	FixesInitNatives(amx);
	GangZonesInitNatives(amx);
	MenusInitNatives(amx);
	MiscInitNatives(amx);
	ModelSizesInitNatives(amx);
	ObjectsInitNatives(amx);
	PickupsInitNatives(amx);
	PlayersInitNatives(amx);
	RakNetInitNatives(amx);
	ScoreBoardInitNatives(amx);
	ServerInitNatives(amx);
	TextDrawsInitNatives(amx);
	TextLabelsInitNatives(amx);
	VehiclesInitNatives(amx);
	YSFSettingsInitNatives(amx);
	return 0;
}
