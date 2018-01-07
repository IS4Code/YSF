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
#include <memory>

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

// And an array containing the native function-names and the functions specified with them
AMX_NATIVE_INFO native_list[] =
{
	// Execute
	AMX_DEFINE_NATIVE(execute)

	// File
	AMX_DEFINE_NATIVE(ffind)
	AMX_DEFINE_NATIVE(frename)
	
	// Directory
	AMX_DEFINE_NATIVE(dfind)
	AMX_DEFINE_NATIVE(dcreate)
	AMX_DEFINE_NATIVE(drename)

	// Generic
	AMX_DEFINE_NATIVE(SetModeRestartTime)
	AMX_DEFINE_NATIVE(GetModeRestartTime)
	AMX_DEFINE_NATIVE(SetMaxPlayers) // R8
	AMX_DEFINE_NATIVE(SetMaxNPCs) // R8
	AMX_DEFINE_NATIVE(GetSyncBounds) // R19
	AMX_DEFINE_NATIVE(SetSyncBounds) // R19

	AMX_DEFINE_NATIVE(SetPlayerAdmin)
	AMX_DEFINE_NATIVE(LoadFilterScript)
	AMX_DEFINE_NATIVE(UnLoadFilterScript)
	AMX_DEFINE_NATIVE(GetFilterScriptCount)
	AMX_DEFINE_NATIVE(GetFilterScriptName)

	AMX_DEFINE_NATIVE(AddServerRule)
	AMX_DEFINE_NATIVE(SetServerRule)
	AMX_DEFINE_NATIVE(SetServerRuleInt)
	AMX_DEFINE_NATIVE(IsValidServerRule)
	AMX_DEFINE_NATIVE(RemoveServerRule) // Doesn't work!
	AMX_DEFINE_NATIVE(SetServerRuleFlags)
	AMX_DEFINE_NATIVE(GetServerRuleFlags)

	// Server settings
	AMX_DEFINE_NATIVE(GetServerSettings)
	AMX_DEFINE_NATIVE(GetNPCCommandLine) // R19

	// RCON Commands
	AMX_DEFINE_NATIVE(ChangeRCONCommandName) // R19
	AMX_DEFINE_NATIVE(GetRCONCommandName) // R19

	// Per AMX function calling
	AMX_DEFINE_NATIVE(CallFunctionInScript) // R19

	// Nick name
	AMX_DEFINE_NATIVE(IsValidNickName)	// R8
	AMX_DEFINE_NATIVE(AllowNickNameCharacter) // R7
	AMX_DEFINE_NATIVE(IsNickNameCharacterAllowed) // R7

	// Player classes
	AMX_DEFINE_NATIVE(GetAvailableClasses) // R6
	AMX_DEFINE_NATIVE(RemoveLastClass) // R16
	AMX_DEFINE_NATIVE(GetPlayerClass) // R6
	AMX_DEFINE_NATIVE(EditPlayerClass) // R6
	
	// Timers
	AMX_DEFINE_NATIVE(GetRunningTimers) // R8

	// Special
	AMX_DEFINE_NATIVE(SetPlayerGravity)
	AMX_DEFINE_NATIVE(GetPlayerGravity)
	
	AMX_DEFINE_NATIVE(SetPlayerTeamForPlayer) // R5
	AMX_DEFINE_NATIVE(GetPlayerTeamForPlayer) // R5
	AMX_DEFINE_NATIVE(SetPlayerSkinForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerSkinForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerNameForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerNameForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerFightStyleForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerFightStyleForPlayer) // R11
	AMX_DEFINE_NATIVE(SetPlayerPosForPlayer) // R12
	AMX_DEFINE_NATIVE(SetPlayerRotationQuatForPlayer) // R12
	AMX_DEFINE_NATIVE(ApplyAnimationForPlayer) // R11
	AMX_DEFINE_NATIVE(GetPlayerWeather)
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds)
	AMX_DEFINE_NATIVE(TogglePlayerWidescreen)
	AMX_DEFINE_NATIVE(IsPlayerWidescreenToggled)
	AMX_DEFINE_NATIVE(GetSpawnInfo) // R8
	AMX_DEFINE_NATIVE(GetPlayerSkillLevel) // R3
	AMX_DEFINE_NATIVE(IsPlayerCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerCheckpoint) // R4
	AMX_DEFINE_NATIVE(IsPlayerRaceCheckpointActive) // R10
	AMX_DEFINE_NATIVE(GetPlayerRaceCheckpoint) // R4
	AMX_DEFINE_NATIVE(GetPlayerWorldBounds) // R5
	AMX_DEFINE_NATIVE(IsPlayerInModShop) // R4
	AMX_DEFINE_NATIVE(SendBulletData) // R6
	AMX_DEFINE_NATIVE(ShowPlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(HidePlayerForPlayer) // R8
	AMX_DEFINE_NATIVE(AddPlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(RemovePlayerForPlayer) // R17
	AMX_DEFINE_NATIVE(SetPlayerChatBubbleForPlayer) // R10
	AMX_DEFINE_NATIVE(SetPlayerVersion) // R9
	AMX_DEFINE_NATIVE(IsPlayerSpawned) // R9
	AMX_DEFINE_NATIVE(IsPlayerControllable) // R11
	AMX_DEFINE_NATIVE(SpawnForWorld) // R10
	AMX_DEFINE_NATIVE(BroadcastDeath) // R13
	AMX_DEFINE_NATIVE(IsPlayerCameraTargetEnabled) // R13
	AMX_DEFINE_NATIVE(SetPlayerDisabledKeysSync) // R16
	AMX_DEFINE_NATIVE(GetPlayerDisabledKeysSync) // R16
	
	// Special things from syncdata
	AMX_DEFINE_NATIVE(GetPlayerSirenState)
	AMX_DEFINE_NATIVE(GetPlayerLandingGearState)
	AMX_DEFINE_NATIVE(GetPlayerHydraReactorAngle)
	AMX_DEFINE_NATIVE(GetPlayerTrainSpeed)
	AMX_DEFINE_NATIVE(GetPlayerZAim)
	AMX_DEFINE_NATIVE(GetPlayerSurfingOffsets)
	AMX_DEFINE_NATIVE(GetPlayerRotationQuat) // R3
	AMX_DEFINE_NATIVE(GetPlayerDialogID) // R8
	AMX_DEFINE_NATIVE(GetPlayerSpectateID) // R8
	AMX_DEFINE_NATIVE(GetPlayerSpectateType) // R8
	AMX_DEFINE_NATIVE(GetPlayerLastSyncedVehicleID) // R10
	AMX_DEFINE_NATIVE(GetPlayerLastSyncedTrailerID) // R10

	// Actor
	AMX_DEFINE_NATIVE(GetActorSpawnInfo) // R13
	AMX_DEFINE_NATIVE(GetActorSkin) // R13
	AMX_DEFINE_NATIVE(GetActorAnimation) // R17

	// Scoreboard manipulation
	AMX_DEFINE_NATIVE(TogglePlayerScoresPingsUpdate) // R8
	AMX_DEFINE_NATIVE(TogglePlayerFakePing) // R8
	AMX_DEFINE_NATIVE(SetPlayerFakePing) // R8
	AMX_DEFINE_NATIVE(SetPlayerNameInServerQuery) // R11
	AMX_DEFINE_NATIVE(GetPlayerNameInServerQuery) // R11
	AMX_DEFINE_NATIVE(SetPlayerNameInServerQuery) // R11
	AMX_DEFINE_NATIVE(GetPlayerNameInServerQuery) // R11

	// AFK
	AMX_DEFINE_NATIVE(IsPlayerPaused)
	AMX_DEFINE_NATIVE(GetPlayerPausedTime)
	
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

	// special - for attached objects
	AMX_DEFINE_NATIVE(GetPlayerAttachedObject) // R3

	// Vehicle functions
	AMX_DEFINE_NATIVE(GetVehicleSpawnInfo)
	AMX_DEFINE_NATIVE(SetVehicleSpawnInfo) // R16
	AMX_DEFINE_NATIVE(GetVehicleColor)
	AMX_DEFINE_NATIVE(GetVehiclePaintjob)
	AMX_DEFINE_NATIVE(GetVehicleInterior)
	AMX_DEFINE_NATIVE(GetVehicleNumberPlate)
	AMX_DEFINE_NATIVE(SetVehicleRespawnDelay)
	AMX_DEFINE_NATIVE(GetVehicleRespawnDelay)
	AMX_DEFINE_NATIVE(SetVehicleOccupiedTick) // R6
	AMX_DEFINE_NATIVE(GetVehicleOccupiedTick)
	AMX_DEFINE_NATIVE(SetVehicleRespawnTick)
	AMX_DEFINE_NATIVE(GetVehicleRespawnTick)
	AMX_DEFINE_NATIVE(GetVehicleLastDriver)
	AMX_DEFINE_NATIVE(GetVehicleCab) // R9
	AMX_DEFINE_NATIVE(HasVehicleBeenOccupied) // R9
	AMX_DEFINE_NATIVE(SetVehicleBeenOccupied) // R9
	AMX_DEFINE_NATIVE(IsVehicleOccupied) // R9
	AMX_DEFINE_NATIVE(IsVehicleDead) // R9
	AMX_DEFINE_NATIVE(SetVehicleParamsSirenState) // R19
	AMX_DEFINE_NATIVE(ToggleVehicleSirenEnabled) // R19
	AMX_DEFINE_NATIVE(IsVehicleSirenEnabled) // R19
	AMX_DEFINE_NATIVE(GetVehicleMatrix) // R19
	AMX_DEFINE_NATIVE(GetVehicleModelCount) // R17
	AMX_DEFINE_NATIVE(GetVehicleModelsUsed) // R17

	// Gangzone - Global
	AMX_DEFINE_NATIVE(IsValidGangZone)
	AMX_DEFINE_NATIVE(IsPlayerInGangZone)
	AMX_DEFINE_NATIVE(IsGangZoneVisibleForPlayer)
	AMX_DEFINE_NATIVE(GangZoneGetColorForPlayer)
	AMX_DEFINE_NATIVE(GangZoneGetFlashColorForPlayer)
	AMX_DEFINE_NATIVE(IsGangZoneFlashingForPlayer) // R6
	AMX_DEFINE_NATIVE(GangZoneGetPos)

	// Gangzone - Player
	AMX_DEFINE_NATIVE(CreatePlayerGangZone)
	AMX_DEFINE_NATIVE(PlayerGangZoneDestroy)
	AMX_DEFINE_NATIVE(PlayerGangZoneShow)
	AMX_DEFINE_NATIVE(PlayerGangZoneHide)
	AMX_DEFINE_NATIVE(PlayerGangZoneFlash)
	AMX_DEFINE_NATIVE(PlayerGangZoneStopFlash)
	AMX_DEFINE_NATIVE(IsValidPlayerGangZone)
	AMX_DEFINE_NATIVE(IsPlayerInPlayerGangZone)
	AMX_DEFINE_NATIVE(IsPlayerGangZoneVisible)
	AMX_DEFINE_NATIVE(PlayerGangZoneGetColor)
	AMX_DEFINE_NATIVE(PlayerGangZoneGetFlashColor)
	AMX_DEFINE_NATIVE(IsPlayerGangZoneFlashing) // R6
	AMX_DEFINE_NATIVE(PlayerGangZoneGetPos)

	// Textdraw functions
	AMX_DEFINE_NATIVE(IsValidTextDraw)
	AMX_DEFINE_NATIVE(IsTextDrawVisibleForPlayer)
	AMX_DEFINE_NATIVE(TextDrawGetString)
	AMX_DEFINE_NATIVE(TextDrawSetPos)
	AMX_DEFINE_NATIVE(TextDrawGetLetterSize)
	AMX_DEFINE_NATIVE(TextDrawGetTextSize)
	AMX_DEFINE_NATIVE(TextDrawGetPos)
	AMX_DEFINE_NATIVE(TextDrawGetColor)
	AMX_DEFINE_NATIVE(TextDrawGetBoxColor)
	AMX_DEFINE_NATIVE(TextDrawGetBackgroundColor)
	AMX_DEFINE_NATIVE(TextDrawGetShadow)
	AMX_DEFINE_NATIVE(TextDrawGetOutline)
	AMX_DEFINE_NATIVE(TextDrawGetFont)
	AMX_DEFINE_NATIVE(TextDrawIsBox)
	AMX_DEFINE_NATIVE(TextDrawIsProportional)
	AMX_DEFINE_NATIVE(TextDrawIsSelectable) // R6
	AMX_DEFINE_NATIVE(TextDrawGetAlignment)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewModel)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewRot)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewVehCol)
	AMX_DEFINE_NATIVE(TextDrawSetStringForPlayer) // R19

	// Per-Player Textdraw functions - R4
	AMX_DEFINE_NATIVE(IsValidPlayerTextDraw)
	AMX_DEFINE_NATIVE(IsPlayerTextDrawVisible)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetString)
	AMX_DEFINE_NATIVE(PlayerTextDrawSetPos)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetLetterSize)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetTextSize)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPos)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetColor)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetBoxColor)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetBackgroundCol)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetShadow)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetOutline)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetFont)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsBox)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsProportional)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsSelectable) // R6
	AMX_DEFINE_NATIVE(PlayerTextDrawGetAlignment)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewModel)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewRot)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewVehCol)
	AMX_DEFINE_NATIVE(TextDrawSetStringForPlayer) // R19

	// 3D Text
	AMX_DEFINE_NATIVE(IsValid3DTextLabel) // R4
	AMX_DEFINE_NATIVE(Is3DTextLabelStreamedIn) // R9
	AMX_DEFINE_NATIVE(Get3DTextLabelText)
	AMX_DEFINE_NATIVE(Get3DTextLabelColor)
	AMX_DEFINE_NATIVE(Get3DTextLabelPos)
	AMX_DEFINE_NATIVE(Get3DTextLabelDrawDistance)
	AMX_DEFINE_NATIVE(Get3DTextLabelLOS)
	AMX_DEFINE_NATIVE(Get3DTextLabelVirtualWorld)
	AMX_DEFINE_NATIVE(Get3DTextLabelAttachedData)

	// Per-Player 3D Text
	AMX_DEFINE_NATIVE(IsValidPlayer3DTextLabel) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelText) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelColor) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelPos) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelDrawDist)
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelLOS) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelVirtualW) // R4
	AMX_DEFINE_NATIVE(GetPlayer3DTextLabelAttached) // R9

	// Menus
	AMX_DEFINE_NATIVE(IsMenuDisabled) // R5 
	AMX_DEFINE_NATIVE(IsMenuRowDisabled) // R5
	AMX_DEFINE_NATIVE(GetMenuColumns)
	AMX_DEFINE_NATIVE(GetMenuItems)
	AMX_DEFINE_NATIVE(GetMenuPos)
	AMX_DEFINE_NATIVE(GetMenuColumnWidth)
	AMX_DEFINE_NATIVE(GetMenuColumnHeader)
	AMX_DEFINE_NATIVE(GetMenuItem)
	
	// Pickups - Global
	AMX_DEFINE_NATIVE(IsValidPickup) // R10
	AMX_DEFINE_NATIVE(IsPickupStreamedIn) // R10
	AMX_DEFINE_NATIVE(GetPickupPos) // R10
	AMX_DEFINE_NATIVE(GetPickupModel) // R10
	AMX_DEFINE_NATIVE(GetPickupType) // R10
	AMX_DEFINE_NATIVE(GetPickupVirtualWorld) // R10
#ifdef NEW_PICKUP_SYSTEM
	// Pickups - Per-player
	AMX_DEFINE_NATIVE(CreatePlayerPickup) // R10
	AMX_DEFINE_NATIVE(DestroyPlayerPickup) // R10
	AMX_DEFINE_NATIVE(IsValidPlayerPickup) // R10
	AMX_DEFINE_NATIVE(IsPlayerPickupStreamedIn) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupPos) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupModel) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupType) // R10
	AMX_DEFINE_NATIVE(GetPlayerPickupVirtualWorld) // R10
#endif
	// RakServer functions
	AMX_DEFINE_NATIVE(ClearBanList)
	AMX_DEFINE_NATIVE(IsBanned)

	AMX_DEFINE_NATIVE(SetTimeoutTime)
	AMX_DEFINE_NATIVE(GetLocalIP)

	AMX_DEFINE_NATIVE(SendRPC)
	AMX_DEFINE_NATIVE(SendData)
	AMX_DEFINE_NATIVE(YSF_SetTickRate)
	AMX_DEFINE_NATIVE(YSF_GetTickRate)
	AMX_DEFINE_NATIVE(YSF_EnableNightVisionFix)
	AMX_DEFINE_NATIVE(YSF_IsNightVisionFixEnabled)
	AMX_DEFINE_NATIVE(YSF_ToggleOnServerMessage) // R18-2
	AMX_DEFINE_NATIVE(YSF_IsOnServerMessageEnabled) // R18-2
	AMX_DEFINE_NATIVE(YSF_SetExtendedNetStatsEnabled) // R17
	AMX_DEFINE_NATIVE(YSF_IsExtendedNetStatsEnabled) // R17
	AMX_DEFINE_NATIVE(YSF_SetAFKAccuracy) // R17
	AMX_DEFINE_NATIVE(YSF_GetAFKAccuracy) // R17 
	AMX_DEFINE_NATIVE(EnableConsoleMSGsForPlayer) // R18
	AMX_DEFINE_NATIVE(DisableConsoleMSGsForPlayer) // R18
	AMX_DEFINE_NATIVE(HasPlayerConsoleMessages) // R18 

	AMX_DEFINE_NATIVE(AttachPlayerObjectToObject)

	// Exclusive RPC broadcast
	AMX_DEFINE_NATIVE(SetExclusiveBroadcast)
	AMX_DEFINE_NATIVE(BroadcastToPlayer)
	
	// Recording functions
	AMX_DEFINE_NATIVE(SetRecordingDirectory) // R17
	AMX_DEFINE_NATIVE(GetRecordingDirectory) // R17

	// Format functions
	AMX_DEFINE_NATIVE(SendClientMessagef)
	AMX_DEFINE_NATIVE(SendClientMessageToAllf)
	AMX_DEFINE_NATIVE(GameTextForPlayerf)
	AMX_DEFINE_NATIVE(GameTextForAllf)
	AMX_DEFINE_NATIVE(SendPlayerMessageToPlayerf)
	AMX_DEFINE_NATIVE(SendPlayerMessageToAllf)
	AMX_DEFINE_NATIVE(SendRconCommandf)

	// Other
	AMX_DEFINE_NATIVE(GetColCount)
	AMX_DEFINE_NATIVE(GetColSphereRadius)
	AMX_DEFINE_NATIVE(GetColSphereOffset)

	AMX_DEFINE_NATIVE(GetWeaponSlot)

#ifdef NEW_PICKUP_SYSTEM
	AMX_DEFINE_NATIVE(SetPickupStreamingEnabled)
#endif
	{ NULL,								NULL }
};

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
	return amx_Register(amx, native_list, -1);
}
