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
 *  the Initial Developer. All Rights Reserved.
 *  
 *  Contributor(s):
 *  
 *  Peter Beverloo
 *  Marcus Bauer
 *  MaVe;
 *  Sammy91
 *  Incognito
 *  
 *  Special Thanks to:
 *  
 *  SA:MP Team past, present and future
 */

#ifndef YSF_SCRIPTING_H
#define YSF_SCRIPTING_H

#include <sampgdk/sampgdk.h>
#include "main.h"

#define CHECK_PARAMS(m, n) \
	if (params[0] != (m * 4)) \
	{ \
		logprintf("YSF: %s: Expecting %d parameter(s), but found %d", n, m, params[0] / sizeof(cell)); \
		return 0; \
	}

namespace Natives
{
	static cell AMX_NATIVE_CALL ffind(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL frename(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL dfind(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL dcreate(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL drename(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL SetModeRestartTime(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetModeRestartTime(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetMaxPlayers(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL SetMaxNPCs(AMX *amx, cell *params); // R8

	static cell AMX_NATIVE_CALL SetPlayerAdmin(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL LoadFilterScript(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL UnLoadFilterScript(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetFilterScriptCount(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetFilterScriptName(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL AddServerRule(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetServerRule(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetServerRuleInt(AMX *amx, cell *params); // R12
	static cell AMX_NATIVE_CALL IsValidServerRule(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL RemoveServerRule(AMX *amx, cell *params); // Doesn't work!
	static cell AMX_NATIVE_CALL SetServerRuleFlags(AMX *amx, cell *params); 
	static cell AMX_NATIVE_CALL GetServerRuleFlags(AMX *amx, cell *params); // R12

	static cell AMX_NATIVE_CALL GetServerSettings(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL IsValidNickName(AMX *amx, cell *params);	// R8
	static cell AMX_NATIVE_CALL AllowNickNameCharacter(AMX *amx, cell *params); // R7
	static cell AMX_NATIVE_CALL IsNickNameCharacterAllowed(AMX *amx, cell *params); // R7

	static cell AMX_NATIVE_CALL GetAvailableClasses(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetPlayerClass(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL EditPlayerClass(AMX *amx, cell *params); // R6

	static cell AMX_NATIVE_CALL GetRunningTimers(AMX *amx, cell *params); // R8

	static cell AMX_NATIVE_CALL SetPlayerGravity(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerGravity(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetPlayerTeamForPlayer(AMX *amx, cell *params); // R5 - Exp
	static cell AMX_NATIVE_CALL GetPlayerTeamForPlayer(AMX *amx, cell *params); // R5
	static cell AMX_NATIVE_CALL SetPlayerSkinForPlayer(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL GetPlayerSkinForPlayer(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL SetPlayerNameForPlayer(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL GetPlayerNameForPlayer(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL SetPlayerFightStyleForPlayer(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL GetPlayerFightStyleForPlayer(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL SetPlayerPosForPlayer(AMX *amx, cell *params); // R12
	static cell AMX_NATIVE_CALL SetPlayerRotationQuatForPlayer(AMX *amx, cell *params); // R12
	static cell AMX_NATIVE_CALL ApplyAnimationForPlayer(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL GetPlayerWeather(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerWorldBounds(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TogglePlayerWidescreen(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsPlayerWidescreenToggled(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetSpawnInfo(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL GetPlayerSkillLevel(AMX *amx, cell *params); // R3
	static cell AMX_NATIVE_CALL IsPlayerCheckpointActive(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerCheckpoint(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL IsPlayerRaceCheckpointActive(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerRaceCheckpoint(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL GetPlayerWorldBounds(AMX *amx, cell *params); // R5
	static cell AMX_NATIVE_CALL IsPlayerInModShop(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL SendBulletData(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL ShowPlayerForPlayer(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL HidePlayerForPlayer(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL SetPlayerChatBubbleForPlayer(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL SetPlayerVersion(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL IsPlayerSpawned(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL IsPlayerControllable(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL SpawnForWorld(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL BroadcastDeath(AMX *amx, cell *params); // R13
	static cell AMX_NATIVE_CALL IsPlayerCameraTargetEnabled(AMX *amx, cell *params); // R13
	
	static cell AMX_NATIVE_CALL GetPlayerSirenState(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerLandingGearState(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerHydraReactorAngle(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerTrainSpeed(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerZAim(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerSurfingOffsets(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerRotationQuat(AMX *amx, cell *params); // R3
	static cell AMX_NATIVE_CALL GetPlayerDialogID(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL GetPlayerSpectateID(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL GetPlayerSpectateType(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL GetPlayerLastSyncedVehicleID(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerLastSyncedTrailerID(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerFPS(AMX *amx, cell *params); // R10

	static cell AMX_NATIVE_CALL GetActorSpawnInfo(AMX *amx, cell *params); // R13
	static cell AMX_NATIVE_CALL GetActorSkin(AMX *amx, cell *params); // R13

	static cell AMX_NATIVE_CALL TogglePlayerScoresPingsUpdate(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL TogglePlayerFakePing(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL SetPlayerFakePing(AMX *amx, cell *params); // R8
	static cell AMX_NATIVE_CALL TogglePlayerOnPlayerList(AMX *amx, cell *params); // R11
	static cell AMX_NATIVE_CALL IsPlayerToggledOnPlayerList(AMX *amx, cell *params); // R11

	static cell AMX_NATIVE_CALL IsPlayerPaused(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerPausedTime(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL GetObjectDrawDistance(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetObjectMoveSpeed(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetObjectMoveSpeed(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetObjectTarget(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetObjectAttachedData(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetObjectAttachedOffset(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsObjectMaterialSlotUsed(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetObjectMaterial(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetObjectMaterialText(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL IsObjectNoCameraCol(AMX *amx, cell *params); // R13

	static cell AMX_NATIVE_CALL GetPlayerObjectDrawDistance(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetPlayerObjectMoveSpeed(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetPlayerObjectMoveSpeed(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetPlayerObjectTarget(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetPlayerObjectAttachedData(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayerObjectAttachedOffset(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsPlayerObjectMaterialSlotUsed(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetPlayerObjectMaterial(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetPlayerObjectMaterialText(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL IsPlayerObjectNoCameraCol(AMX *amx, cell *params); // R13
	static cell AMX_NATIVE_CALL GetPlayerSurfingPlayerObjectID(AMX *amx, cell *params); // R12
	static cell AMX_NATIVE_CALL GetPlayerCameraTargetPlayerObj(AMX *amx, cell *params); // R13
	static cell AMX_NATIVE_CALL GetObjectType(AMX *amx, cell *params); // R12

	static cell AMX_NATIVE_CALL GetPlayerAttachedObject(AMX *amx, cell *params); // R3

	static cell AMX_NATIVE_CALL GetVehicleSpawnInfo(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehicleColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehiclePaintjob(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehicleInterior(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehicleNumberPlate(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetVehicleRespawnDelay(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehicleRespawnDelay(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetVehicleOccupiedTick(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GetVehicleOccupiedTick(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetVehicleRespawnTick(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehicleRespawnTick(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehicleLastDriver(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetVehicleCab(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL HasVehicleBeenOccupied(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL SetVehicleBeenOccupied(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL IsVehicleOccupied(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL IsVehicleDead(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL SetVehicleSpawnInfo(AMX *amx, cell *params); // R12

	static cell AMX_NATIVE_CALL IsValidGangZone(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsPlayerInGangZone(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsGangZoneVisibleForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GangZoneGetColorForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GangZoneGetFlashColorForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsGangZoneFlashingForPlayer(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL GangZoneGetPos(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL CreatePlayerGangZone(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerGangZoneDestroy(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerGangZoneShow(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerGangZoneHide(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerGangZoneFlash(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerGangZoneStopFlash(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsValidPlayerGangZone(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsPlayerInPlayerGangZone(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsPlayerGangZoneVisible(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerGangZoneGetColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerGangZoneGetFlashColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsPlayerGangZoneFlashing(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL PlayerGangZoneGetPos(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL IsValidTextDraw(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsTextDrawVisibleForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetString(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawSetPos(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetLetterSize(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetFontSize(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetPos(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetBoxColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetBackgroundColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetShadow(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetOutline(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetFont(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawIsBox(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawIsProportional(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawIsSelectable(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL TextDrawGetAlignment(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetPreviewModel(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetPreviewRot(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL TextDrawGetPreviewVehCol(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL IsValidPlayerTextDraw(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsPlayerTextDrawVisible(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetString(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawSetPos(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetLetterSize(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetFontSize(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetPos(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetBoxColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetBackgroundCol(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetShadow(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetOutline(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetFont(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawIsBox(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawIsProportional(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawIsSelectable(AMX *amx, cell *params); // R6
	static cell AMX_NATIVE_CALL PlayerTextDrawGetAlignment(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetPreviewModel(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetPreviewRot(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL PlayerTextDrawGetPreviewVehCol(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL IsValid3DTextLabel(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL Is3DTextLabelStreamedIn(AMX *amx, cell *params); // R9
	static cell AMX_NATIVE_CALL Get3DTextLabelText(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Get3DTextLabelColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Get3DTextLabelPos(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Get3DTextLabelDrawDistance(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Get3DTextLabelLOS(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Get3DTextLabelVirtualWorld(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Get3DTextLabelAttachedData(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL IsValidPlayer3DTextLabel(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL GetPlayer3DTextLabelText(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL GetPlayer3DTextLabelColor(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL GetPlayer3DTextLabelPos(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL GetPlayer3DTextLabelDrawDist(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetPlayer3DTextLabelLOS(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL GetPlayer3DTextLabelVirtualW(AMX *amx, cell *params); // R4
	static cell AMX_NATIVE_CALL GetPlayer3DTextLabelAttached(AMX *amx, cell *params); // R9

	static cell AMX_NATIVE_CALL IsMenuDisabled(AMX *amx, cell *params); // R5 
	static cell AMX_NATIVE_CALL IsMenuRowDisabled(AMX *amx, cell *params); // R5
	static cell AMX_NATIVE_CALL GetMenuColumns(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetMenuItems(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetMenuPos(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetMenuColumnWidth(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetMenuColumnHeader(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetMenuItem(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL IsValidPickup(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL IsPickupStreamedIn(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPickupPos(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPickupModel(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPickupType(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPickupVirtualWorld(AMX *amx, cell *params); // R10

	static cell AMX_NATIVE_CALL CreatePlayerPickup(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL DestroyPlayerPickup(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL IsValidPlayerPickup(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL IsPlayerPickupStreamedIn(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerPickupPos(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerPickupModel(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerPickupType(AMX *amx, cell *params); // R10
	static cell AMX_NATIVE_CALL GetPlayerPickupVirtualWorld(AMX *amx, cell *params); // R10

	static cell AMX_NATIVE_CALL ClearBanList(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsBanned(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL SetTimeoutTime(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetMTUSize(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetMTUSize(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetLocalIP(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL SendRPC(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SendData(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL YSF_AddPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_RemovePlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_StreamIn(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_StreamOut(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetTickRate(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GetTickRate(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL AttachPlayerObjectToObject(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL SendClientMessagef(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SendClientMessageToAllf(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GameTextForPlayerf(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GameTextForAllf(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SendPlayerMessageToPlayerf(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SendPlayerMessageToAllf(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SendRconCommandf(AMX *amx, cell *params);
	
	static cell AMX_NATIVE_CALL GetColCount(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetColSphereRadius(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetColSphereOffset(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL GetWeaponSlot(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL YSF_AttachPlayerObjectToPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetGravity(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GetGravity(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetWeather(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetPlayerWeather(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetPlayerWorldBounds(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_DestroyPlayerObject(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_TogglePlayerControllable(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_ChangeVehicleColor(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_DestroyVehicle(AMX *amx, cell *params);
	
	static cell AMX_NATIVE_CALL YSF_SetPlayerTeam(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetPlayerSkin(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetPlayerFightingStyle(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_SetPlayerName(AMX *amx, cell *params);
	
	static cell AMX_NATIVE_CALL YSF_GangZoneCreate(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneDestroy(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneShowForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneHideForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneShowForAll(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneHideForAll(AMX *amx, cell *params);

	static cell AMX_NATIVE_CALL YSF_GangZoneFlashForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneFlashForAll(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneStopFlashForPlayer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL YSF_GangZoneStopFlashForAll(AMX *amx, cell *params);
#ifdef NEW_PICKUP_SYSTEM
	static cell AMX_NATIVE_CALL CreatePickup(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL DestroyPickup(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetPickupStreamingEnabled(AMX *amx, cell *params);
#endif
	static cell AMX_NATIVE_CALL FIXED_GetWeaponName(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL FIXED_IsPlayerConnectedEx(AMX *amx, cell *params);
};

int InitScripting(AMX *amx);

extern AMX_NATIVE_INFO RedirectedNatives[];
#endif