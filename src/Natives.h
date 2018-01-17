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

#ifndef YSF_SCRIPTING_H
#define YSF_SCRIPTING_H

#include <sdk/amx/amx.h>

typedef cell AMX_NATIVE_CALL(*AMX_Function_t)(AMX *amx, cell *params);

#define AMX_DECLARE_NATIVE(name) \
	cell AMX_NATIVE_CALL name(AMX* amx, cell* params)

#define AMX_DEFINE_NATIVE(name) \
	{#name, Natives::name},

#define CHECK_PARAMS(m, flag) \
	if(CScriptParams::Get()->Setup(m, __FUNCTION__, CScriptParams::Flags::flag, amx, params)) return CScriptParams::Get()->HandleError()
	
struct stRedirectedNatives
{
	const char _FAR *name PACKED;
	AMX_NATIVE *originalfunc PACKED;
	AMX_NATIVE func PACKED;
};

namespace Natives
{
	extern AMX_NATIVE ORIGINAL_AttachObjectToPlayer;
	extern AMX_NATIVE ORIGINAL_SetPlayerWeather;
	extern AMX_NATIVE ORIGINAL_SetPlayerWorldBounds;
	extern AMX_NATIVE ORIGINAL_DestroyObject;
	extern AMX_NATIVE ORIGINAL_DestroyPlayerObject;
	extern AMX_NATIVE ORIGINAL_TogglePlayerControllable;
	extern AMX_NATIVE ORIGINAL_ChangeVehicleColor;
	extern AMX_NATIVE ORIGINAL_DestroyVehicle;
	extern AMX_NATIVE ORIGINAL_ShowPlayerDialog;
	extern AMX_NATIVE ORIGINAL_SetPlayerObjectMaterial;
	extern AMX_NATIVE ORIGINAL_SetPlayerObjectMaterialText;
	extern AMX_NATIVE ORIGINAL_SetPlayerTeam;
	extern AMX_NATIVE ORIGINAL_SetPlayerSkin;
	extern AMX_NATIVE ORIGINAL_SetPlayerName;
	extern AMX_NATIVE ORIGINAL_SetPlayerFightingStyle;

	AMX_DECLARE_NATIVE(YSF_AttachObjectToPlayer);
	AMX_DECLARE_NATIVE(YSF_AttachPlayerObjectToPlayer);
	AMX_DECLARE_NATIVE(YSF_GetGravity);
	AMX_DECLARE_NATIVE(YSF_SetPlayerWeather);
	AMX_DECLARE_NATIVE(YSF_SetPlayerWorldBounds);
	AMX_DECLARE_NATIVE(YSF_DestroyObject);
	AMX_DECLARE_NATIVE(YSF_DestroyPlayerObject);
	AMX_DECLARE_NATIVE(YSF_TogglePlayerControllable);
	AMX_DECLARE_NATIVE(YSF_ChangeVehicleColor);
	AMX_DECLARE_NATIVE(YSF_DestroyVehicle);
	AMX_DECLARE_NATIVE(YSF_ShowPlayerDialog);
	AMX_DECLARE_NATIVE(YSF_SetPlayerObjectMaterial);
	AMX_DECLARE_NATIVE(YSF_SetPlayerObjectMaterialText);
	
	AMX_DECLARE_NATIVE(YSF_SetPlayerTeam);
	AMX_DECLARE_NATIVE(YSF_SetPlayerSkin);
	AMX_DECLARE_NATIVE(YSF_SetPlayerFightingStyle);
	AMX_DECLARE_NATIVE(YSF_SetPlayerName);
	
	AMX_DECLARE_NATIVE(YSF_GangZoneCreate);
	AMX_DECLARE_NATIVE(YSF_GangZoneDestroy);
	AMX_DECLARE_NATIVE(YSF_GangZoneShowForPlayer);
	AMX_DECLARE_NATIVE(YSF_GangZoneHideForPlayer);
	AMX_DECLARE_NATIVE(YSF_GangZoneShowForAll);
	AMX_DECLARE_NATIVE(YSF_GangZoneHideForAll);

	AMX_DECLARE_NATIVE(YSF_GangZoneFlashForPlayer);
	AMX_DECLARE_NATIVE(YSF_GangZoneFlashForAll);
	AMX_DECLARE_NATIVE(YSF_GangZoneStopFlashForPlayer);
	AMX_DECLARE_NATIVE(YSF_GangZoneStopFlashForAll);
#ifdef NEW_PICKUP_SYSTEM
	AMX_DECLARE_NATIVE(CreatePickup);
	AMX_DECLARE_NATIVE(DestroyPickup);
	AMX_DECLARE_NATIVE(SetPickupStreamingEnabled);
#endif
	AMX_DECLARE_NATIVE(FIXED_GetWeaponName);
	AMX_DECLARE_NATIVE(FIXED_IsPlayerConnected);
};

int ActorsInitNatives(AMX *amx);
int FixesInitNatives(AMX *amx);
int GangZonesInitNatives(AMX *amx);
int MenusInitNatives(AMX *amx);
int MiscInitNatives(AMX *amx);
int ModelSizesInitNatives(AMX *amx);
int ObjectsInitNatives(AMX *amx);
int PickupsInitNatives(AMX *amx);
int PlayersInitNatives(AMX *amx);
int RakNetInitNatives(AMX *amx);
int ScoreBoardInitNatives(AMX *amx);
int ServerInitNatives(AMX *amx);
int TextDrawsInitNatives(AMX *amx);
int TextLabelsInitNatives(AMX *amx);
int VehiclesInitNatives(AMX *amx);
int YSFSettingsInitNatives(AMX *amx);

int InitNatives(AMX *amx);

extern stRedirectedNatives redirected_native_list[];
#endif