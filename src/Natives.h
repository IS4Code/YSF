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
#include "main.h"

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

	AMX_DECLARE_NATIVE(execute); // R18

	AMX_DECLARE_NATIVE(ffind);
	AMX_DECLARE_NATIVE(frename);

	AMX_DECLARE_NATIVE(dfind);
	AMX_DECLARE_NATIVE(dcreate);
	AMX_DECLARE_NATIVE(drename);

	AMX_DECLARE_NATIVE(SetModeRestartTime);
	AMX_DECLARE_NATIVE(GetModeRestartTime);
	AMX_DECLARE_NATIVE(SetMaxPlayers); // R8
	AMX_DECLARE_NATIVE(SetMaxNPCs); // R8
	AMX_DECLARE_NATIVE(GetSyncBounds); // R19
	AMX_DECLARE_NATIVE(SetSyncBounds); // R19

	AMX_DECLARE_NATIVE(SetPlayerAdmin);
	AMX_DECLARE_NATIVE(LoadFilterScript);
	AMX_DECLARE_NATIVE(UnLoadFilterScript);
	AMX_DECLARE_NATIVE(GetFilterScriptCount);
	AMX_DECLARE_NATIVE(GetFilterScriptName);
	AMX_DECLARE_NATIVE(AddServerRule);
	AMX_DECLARE_NATIVE(SetServerRule);
	AMX_DECLARE_NATIVE(SetServerRuleInt); // R12
	AMX_DECLARE_NATIVE(IsValidServerRule);
	AMX_DECLARE_NATIVE(RemoveServerRule); // Doesn't work!
	AMX_DECLARE_NATIVE(SetServerRuleFlags); 
	AMX_DECLARE_NATIVE(GetServerRuleFlags); // R12

	AMX_DECLARE_NATIVE(GetServerSettings);
	AMX_DECLARE_NATIVE(GetNPCCommandLine); // R19

	AMX_DECLARE_NATIVE(ChangeRCONCommandName); // R19
	AMX_DECLARE_NATIVE(GetRCONCommandName); // R19

	AMX_DECLARE_NATIVE(CallFunctionInScript); // R19

	AMX_DECLARE_NATIVE(IsValidNickName);	// R8
	AMX_DECLARE_NATIVE(AllowNickNameCharacter); // R7
	AMX_DECLARE_NATIVE(IsNickNameCharacterAllowed); // R7

	AMX_DECLARE_NATIVE(GetAvailableClasses); // R6
	AMX_DECLARE_NATIVE(RemoveLastClass); // R16
	AMX_DECLARE_NATIVE(GetPlayerClass); // R6
	AMX_DECLARE_NATIVE(EditPlayerClass); // R6

	AMX_DECLARE_NATIVE(GetRunningTimers); // R8

	AMX_DECLARE_NATIVE(SetPlayerGravity);
	AMX_DECLARE_NATIVE(GetPlayerGravity);
	AMX_DECLARE_NATIVE(SetPlayerTeamForPlayer); // R5 - Exp
	AMX_DECLARE_NATIVE(GetPlayerTeamForPlayer); // R5
	AMX_DECLARE_NATIVE(SetPlayerSkinForPlayer); // R11
	AMX_DECLARE_NATIVE(GetPlayerSkinForPlayer); // R11
	AMX_DECLARE_NATIVE(SetPlayerNameForPlayer); // R11
	AMX_DECLARE_NATIVE(GetPlayerNameForPlayer); // R11
	AMX_DECLARE_NATIVE(SetPlayerFightStyleForPlayer); // R11
	AMX_DECLARE_NATIVE(GetPlayerFightStyleForPlayer); // R11
	AMX_DECLARE_NATIVE(SetPlayerPosForPlayer); // R12
	AMX_DECLARE_NATIVE(SetPlayerRotationQuatForPlayer); // R12
	AMX_DECLARE_NATIVE(ApplyAnimationForPlayer); // R11
	AMX_DECLARE_NATIVE(GetPlayerWeather);
	AMX_DECLARE_NATIVE(GetPlayerWorldBounds);
	AMX_DECLARE_NATIVE(TogglePlayerWidescreen);
	AMX_DECLARE_NATIVE(IsPlayerWidescreenToggled);
	AMX_DECLARE_NATIVE(GetSpawnInfo); // R8
	AMX_DECLARE_NATIVE(GetPlayerSkillLevel); // R3
	AMX_DECLARE_NATIVE(IsPlayerCheckpointActive); // R10
	AMX_DECLARE_NATIVE(GetPlayerCheckpoint); // R4
	AMX_DECLARE_NATIVE(IsPlayerRaceCheckpointActive); // R10
	AMX_DECLARE_NATIVE(GetPlayerRaceCheckpoint); // R4
	AMX_DECLARE_NATIVE(GetPlayerWorldBounds); // R5
	AMX_DECLARE_NATIVE(IsPlayerInModShop); // R4
	AMX_DECLARE_NATIVE(SendBulletData); // R6
	AMX_DECLARE_NATIVE(ShowPlayerForPlayer); // R8
	AMX_DECLARE_NATIVE(HidePlayerForPlayer); // R8
	AMX_DECLARE_NATIVE(AddPlayerForPlayer); // R17
	AMX_DECLARE_NATIVE(RemovePlayerForPlayer); // R17
	AMX_DECLARE_NATIVE(SetPlayerChatBubbleForPlayer); // R10
	AMX_DECLARE_NATIVE(ResetPlayerMarkerForPlayer); // R18	
	AMX_DECLARE_NATIVE(SetPlayerVersion); // R9
	AMX_DECLARE_NATIVE(IsPlayerSpawned); // R9
	AMX_DECLARE_NATIVE(IsPlayerControllable); // R11
	AMX_DECLARE_NATIVE(SpawnForWorld); // R10
	AMX_DECLARE_NATIVE(BroadcastDeath); // R13
	AMX_DECLARE_NATIVE(IsPlayerCameraTargetEnabled); // R13
	AMX_DECLARE_NATIVE(SetPlayerDisabledKeysSync); // R16
	AMX_DECLARE_NATIVE(GetPlayerDisabledKeysSync); // R16

	AMX_DECLARE_NATIVE(GetPlayerSirenState);
	AMX_DECLARE_NATIVE(GetPlayerLandingGearState);
	AMX_DECLARE_NATIVE(GetPlayerHydraReactorAngle);
	AMX_DECLARE_NATIVE(GetPlayerTrainSpeed);
	AMX_DECLARE_NATIVE(GetPlayerZAim);
	AMX_DECLARE_NATIVE(GetPlayerSurfingOffsets);
	AMX_DECLARE_NATIVE(GetPlayerRotationQuat); // R3
	AMX_DECLARE_NATIVE(GetPlayerDialogID); // R8
	AMX_DECLARE_NATIVE(GetPlayerSpectateID); // R8
	AMX_DECLARE_NATIVE(GetPlayerSpectateType); // R8
	AMX_DECLARE_NATIVE(GetPlayerLastSyncedVehicleID); // R10
	AMX_DECLARE_NATIVE(GetPlayerLastSyncedTrailerID); // R10

	AMX_DECLARE_NATIVE(GetActorSpawnInfo); // R13
	AMX_DECLARE_NATIVE(GetActorSkin); // R13
	AMX_DECLARE_NATIVE(GetActorAnimation); // R17

	AMX_DECLARE_NATIVE(TogglePlayerScoresPingsUpdate); // R8
	AMX_DECLARE_NATIVE(TogglePlayerFakePing); // R8
	AMX_DECLARE_NATIVE(SetPlayerFakePing); // R8
	AMX_DECLARE_NATIVE(SetPlayerNameInServerQuery); // R11
	AMX_DECLARE_NATIVE(GetPlayerNameInServerQuery); // R11

	AMX_DECLARE_NATIVE(IsPlayerPaused);
	AMX_DECLARE_NATIVE(GetPlayerPausedTime);

	AMX_DECLARE_NATIVE(GetObjectDrawDistance);
	AMX_DECLARE_NATIVE(SetObjectMoveSpeed); // R6
	AMX_DECLARE_NATIVE(GetObjectMoveSpeed); // R6
	AMX_DECLARE_NATIVE(GetObjectTarget); // R6
	AMX_DECLARE_NATIVE(GetObjectAttachedData);
	AMX_DECLARE_NATIVE(GetObjectAttachedOffset);
	AMX_DECLARE_NATIVE(IsObjectMaterialSlotUsed); // R6
	AMX_DECLARE_NATIVE(GetObjectMaterial); // R6
	AMX_DECLARE_NATIVE(GetObjectMaterialText); // R6
	AMX_DECLARE_NATIVE(IsObjectNoCameraCol); // R13

	AMX_DECLARE_NATIVE(GetPlayerObjectDrawDistance);
	AMX_DECLARE_NATIVE(SetPlayerObjectMoveSpeed); // R6
	AMX_DECLARE_NATIVE(GetPlayerObjectMoveSpeed); // R6
	AMX_DECLARE_NATIVE(GetPlayerObjectTarget); // R6
	AMX_DECLARE_NATIVE(GetPlayerObjectAttachedData);
	AMX_DECLARE_NATIVE(GetPlayerObjectAttachedOffset);
	AMX_DECLARE_NATIVE(IsPlayerObjectMaterialSlotUsed); // R6
	AMX_DECLARE_NATIVE(GetPlayerObjectMaterial); // R6
	AMX_DECLARE_NATIVE(GetPlayerObjectMaterialText); // R6
	AMX_DECLARE_NATIVE(IsPlayerObjectNoCameraCol); // R13
	AMX_DECLARE_NATIVE(GetPlayerSurfingPlayerObjectID); // R12
	AMX_DECLARE_NATIVE(GetPlayerCameraTargetPlayerObj); // R13
	AMX_DECLARE_NATIVE(GetObjectType); // R12

	AMX_DECLARE_NATIVE(GetPlayerAttachedObject); // R3

	AMX_DECLARE_NATIVE(GetVehicleSpawnInfo);
	AMX_DECLARE_NATIVE(GetVehicleColor);
	AMX_DECLARE_NATIVE(GetVehiclePaintjob);
	AMX_DECLARE_NATIVE(GetVehicleInterior);
	AMX_DECLARE_NATIVE(GetVehicleNumberPlate);
	AMX_DECLARE_NATIVE(SetVehicleRespawnDelay);
	AMX_DECLARE_NATIVE(GetVehicleRespawnDelay);
	AMX_DECLARE_NATIVE(SetVehicleOccupiedTick); // R6
	AMX_DECLARE_NATIVE(GetVehicleOccupiedTick);
	AMX_DECLARE_NATIVE(SetVehicleRespawnTick);
	AMX_DECLARE_NATIVE(GetVehicleRespawnTick);
	AMX_DECLARE_NATIVE(GetVehicleLastDriver);
	AMX_DECLARE_NATIVE(GetVehicleCab); // R9
	AMX_DECLARE_NATIVE(HasVehicleBeenOccupied); // R9
	AMX_DECLARE_NATIVE(SetVehicleBeenOccupied); // R9
	AMX_DECLARE_NATIVE(IsVehicleOccupied); // R9
	AMX_DECLARE_NATIVE(IsVehicleDead); // R9
	AMX_DECLARE_NATIVE(SetVehicleParamsSirenState); // R19
	AMX_DECLARE_NATIVE(ToggleVehicleSirenEnabled); // R19
	AMX_DECLARE_NATIVE(IsVehicleSirenEnabled); // R19
	AMX_DECLARE_NATIVE(GetVehicleMatrix); // R19
	AMX_DECLARE_NATIVE(SetVehicleSpawnInfo); // R12
	AMX_DECLARE_NATIVE(GetVehicleModelCount); // R17
	AMX_DECLARE_NATIVE(GetVehicleModelsUsed); // R17

	AMX_DECLARE_NATIVE(IsValidGangZone);
	AMX_DECLARE_NATIVE(IsPlayerInGangZone);
	AMX_DECLARE_NATIVE(IsGangZoneVisibleForPlayer);
	AMX_DECLARE_NATIVE(GangZoneGetColorForPlayer);
	AMX_DECLARE_NATIVE(GangZoneGetFlashColorForPlayer);
	AMX_DECLARE_NATIVE(IsGangZoneFlashingForPlayer); // R6
	AMX_DECLARE_NATIVE(GangZoneGetPos);

	AMX_DECLARE_NATIVE(CreatePlayerGangZone);
	AMX_DECLARE_NATIVE(PlayerGangZoneDestroy);
	AMX_DECLARE_NATIVE(PlayerGangZoneShow);
	AMX_DECLARE_NATIVE(PlayerGangZoneHide);
	AMX_DECLARE_NATIVE(PlayerGangZoneFlash);
	AMX_DECLARE_NATIVE(PlayerGangZoneStopFlash);
	AMX_DECLARE_NATIVE(IsValidPlayerGangZone);
	AMX_DECLARE_NATIVE(IsPlayerInPlayerGangZone);
	AMX_DECLARE_NATIVE(IsPlayerGangZoneVisible);
	AMX_DECLARE_NATIVE(PlayerGangZoneGetColor);
	AMX_DECLARE_NATIVE(PlayerGangZoneGetFlashColor);
	AMX_DECLARE_NATIVE(IsPlayerGangZoneFlashing); // R6
	AMX_DECLARE_NATIVE(PlayerGangZoneGetPos);

	AMX_DECLARE_NATIVE(IsValidTextDraw);
	AMX_DECLARE_NATIVE(IsTextDrawVisibleForPlayer);
	AMX_DECLARE_NATIVE(TextDrawGetString);
	AMX_DECLARE_NATIVE(TextDrawSetPos);
	AMX_DECLARE_NATIVE(TextDrawGetLetterSize);
	AMX_DECLARE_NATIVE(TextDrawGetTextSize);
	AMX_DECLARE_NATIVE(TextDrawGetPos);
	AMX_DECLARE_NATIVE(TextDrawGetColor);
	AMX_DECLARE_NATIVE(TextDrawGetBoxColor);
	AMX_DECLARE_NATIVE(TextDrawGetBackgroundColor);
	AMX_DECLARE_NATIVE(TextDrawGetShadow);
	AMX_DECLARE_NATIVE(TextDrawGetOutline);
	AMX_DECLARE_NATIVE(TextDrawGetFont);
	AMX_DECLARE_NATIVE(TextDrawIsBox);
	AMX_DECLARE_NATIVE(TextDrawIsProportional);
	AMX_DECLARE_NATIVE(TextDrawIsSelectable); // R6
	AMX_DECLARE_NATIVE(TextDrawGetAlignment);
	AMX_DECLARE_NATIVE(TextDrawGetPreviewModel);
	AMX_DECLARE_NATIVE(TextDrawGetPreviewRot);
	AMX_DECLARE_NATIVE(TextDrawGetPreviewVehCol);
	AMX_DECLARE_NATIVE(TextDrawSetStringForPlayer); // R19

	AMX_DECLARE_NATIVE(IsValidPlayerTextDraw);
	AMX_DECLARE_NATIVE(IsPlayerTextDrawVisible);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetString);
	AMX_DECLARE_NATIVE(PlayerTextDrawSetPos);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetLetterSize);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetTextSize);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPos);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetColor);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetBoxColor);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetBackgroundCol);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetShadow);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetOutline);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetFont);
	AMX_DECLARE_NATIVE(PlayerTextDrawIsBox);
	AMX_DECLARE_NATIVE(PlayerTextDrawIsProportional);
	AMX_DECLARE_NATIVE(PlayerTextDrawIsSelectable); // R6
	AMX_DECLARE_NATIVE(PlayerTextDrawGetAlignment);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPreviewModel);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPreviewRot);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPreviewVehCol);

	AMX_DECLARE_NATIVE(IsValid3DTextLabel); // R4
	AMX_DECLARE_NATIVE(Is3DTextLabelStreamedIn); // R9
	AMX_DECLARE_NATIVE(Get3DTextLabelText);
	AMX_DECLARE_NATIVE(Get3DTextLabelColor);
	AMX_DECLARE_NATIVE(Get3DTextLabelPos);
	AMX_DECLARE_NATIVE(Get3DTextLabelDrawDistance);
	AMX_DECLARE_NATIVE(Get3DTextLabelLOS);
	AMX_DECLARE_NATIVE(Get3DTextLabelVirtualWorld);
	AMX_DECLARE_NATIVE(Get3DTextLabelAttachedData);

	AMX_DECLARE_NATIVE(IsValidPlayer3DTextLabel); // R4
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelText); // R4
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelColor); // R4
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelPos); // R4
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelDrawDist);
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelLOS); // R4
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelVirtualW); // R4
	AMX_DECLARE_NATIVE(GetPlayer3DTextLabelAttached); // R9

	AMX_DECLARE_NATIVE(IsMenuDisabled); // R5 
	AMX_DECLARE_NATIVE(IsMenuRowDisabled); // R5
	AMX_DECLARE_NATIVE(GetMenuColumns);
	AMX_DECLARE_NATIVE(GetMenuItems);
	AMX_DECLARE_NATIVE(GetMenuPos);
	AMX_DECLARE_NATIVE(GetMenuColumnWidth);
	AMX_DECLARE_NATIVE(GetMenuColumnHeader);
	AMX_DECLARE_NATIVE(GetMenuItem);

	AMX_DECLARE_NATIVE(IsValidPickup); // R10
	AMX_DECLARE_NATIVE(IsPickupStreamedIn); // R10
	AMX_DECLARE_NATIVE(GetPickupPos); // R10
	AMX_DECLARE_NATIVE(GetPickupModel); // R10
	AMX_DECLARE_NATIVE(GetPickupType); // R10
	AMX_DECLARE_NATIVE(GetPickupVirtualWorld); // R10

	AMX_DECLARE_NATIVE(CreatePlayerPickup); // R10
	AMX_DECLARE_NATIVE(DestroyPlayerPickup); // R10
	AMX_DECLARE_NATIVE(IsValidPlayerPickup); // R10
	AMX_DECLARE_NATIVE(IsPlayerPickupStreamedIn); // R10
	AMX_DECLARE_NATIVE(GetPlayerPickupPos); // R10
	AMX_DECLARE_NATIVE(GetPlayerPickupModel); // R10
	AMX_DECLARE_NATIVE(GetPlayerPickupType); // R10
	AMX_DECLARE_NATIVE(GetPlayerPickupVirtualWorld); // R10

	AMX_DECLARE_NATIVE(ClearBanList);
	AMX_DECLARE_NATIVE(IsBanned);

	AMX_DECLARE_NATIVE(SetTimeoutTime);
	AMX_DECLARE_NATIVE(GetLocalIP);

	AMX_DECLARE_NATIVE(SendRPC);
	AMX_DECLARE_NATIVE(SendData);

	AMX_DECLARE_NATIVE(YSF_SetTickRate);
	AMX_DECLARE_NATIVE(YSF_GetTickRate);
	AMX_DECLARE_NATIVE(YSF_EnableNightVisionFix);
	AMX_DECLARE_NATIVE(YSF_IsNightVisionFixEnabled);
	AMX_DECLARE_NATIVE(YSF_ToggleOnServerMessage);
	AMX_DECLARE_NATIVE(YSF_IsOnServerMessageEnabled);
	AMX_DECLARE_NATIVE(YSF_SetExtendedNetStatsEnabled);
	AMX_DECLARE_NATIVE(YSF_IsExtendedNetStatsEnabled);
	AMX_DECLARE_NATIVE(YSF_SetAFKAccuracy);
	AMX_DECLARE_NATIVE(YSF_GetAFKAccuracy);
	AMX_DECLARE_NATIVE(EnableConsoleMSGsForPlayer);
	AMX_DECLARE_NATIVE(DisableConsoleMSGsForPlayer);
	AMX_DECLARE_NATIVE(HasPlayerConsoleMessages);

	AMX_DECLARE_NATIVE(AttachPlayerObjectToObject);

	AMX_DECLARE_NATIVE(SetExclusiveBroadcast);
	AMX_DECLARE_NATIVE(BroadcastToPlayer);

	AMX_DECLARE_NATIVE(SetRecordingDirectory);
	AMX_DECLARE_NATIVE(GetRecordingDirectory);

	AMX_DECLARE_NATIVE(SendClientMessagef);
	AMX_DECLARE_NATIVE(SendClientMessageToAllf);
	AMX_DECLARE_NATIVE(GameTextForPlayerf);
	AMX_DECLARE_NATIVE(GameTextForAllf);
	AMX_DECLARE_NATIVE(SendPlayerMessageToPlayerf);
	AMX_DECLARE_NATIVE(SendPlayerMessageToAllf);
	AMX_DECLARE_NATIVE(SendRconCommandf);
	
	AMX_DECLARE_NATIVE(GetColCount);
	AMX_DECLARE_NATIVE(GetColSphereRadius);
	AMX_DECLARE_NATIVE(GetColSphereOffset);

	AMX_DECLARE_NATIVE(GetWeaponSlot);

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

int InitNatives(AMX *amx);

extern stRedirectedNatives redirected_native_list[];
#endif