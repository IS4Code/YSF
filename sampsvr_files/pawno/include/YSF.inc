/* YSF 2.2.1 */
#if defined _YSF_included
	#endinput
#endif
#define _YSF_included

#define YSF_ERROR_ISNT_LOADED 					cellmin
#define YSF_ERROR_PARAMETER_COUNT_ISNT_EQUAL 	(cellmin + 1)
#define YSF_ERROR_PARAMETER_COUNT_ISNT_ENOUGH 	(cellmin + 2)

// Definitions
enum E_SERVER_RULE_FLAGS (<<= 1)
{
	CON_VARFLAG_DEBUG = 1,
	CON_VARFLAG_READONLY,
	CON_VARFLAG_RULE,
	CON_VARFLAG_UNREMOVABLE
}

enum E_SCM_EVENT_TYPE
{
	SCM_EVENT_PAINTJOB = 1,
	SCM_EVENT_MOD = 2,
	SCM_EVENT_RESPRAY = 3,
	SCM_EVENT_MOD_SHOP = 4
}

enum E_PLAYER_CONNECT_MODE
{
	NO_ACTION,
	DISCONNECT_ASAP,
	DISCONNECT_ASAP_SILENTLY,
	DISCONNECT_ON_NO_ACK,
	REQUESTED_CONNECTION,
	HANDLING_CONNECTION_REQUEST,
	UNVERIFIED_SENDER,
	SET_ENCRYPTION_ON_MULTIPLE_16_B,
	CONNECTED
}
#define SET_ENCRYPTION_ON_MULTIPLE_16_BYTE_PACKET SET_ENCRYPTION_ON_MULTIPLE_16_B

// Execute
native execute(const command[], saveoutput=0, index=0);

// File functions
native ffind(const pattern[], filename[], len, &idx);
native frename(const oldname[], const newname[]);

// Directory functions
native dfind(const pattern[], filename[], len, &idx);
native dcreate(const name[]);
native drename(const oldname[], const newname[]);

// Gamemode restart time
native SetModeRestartTime(Float:time);
native Float:GetModeRestartTime();

// Max player/npc change at runtime
native SetMaxPlayers(maxplayers);
native SetMaxNPCs(maxnpcs);

// Filterscript functions
native LoadFilterScript(const scriptname[]); // difference between "rcon loadfs": Return -> True if success, false if not
native UnLoadFilterScript(const scriptname[]); // ^
native GetFilterScriptCount();
native GetFilterScriptName(id, name[], len = sizeof(name));

// Server rule modifications
native AddServerRule(const name[], const value[], E_SERVER_RULE_FLAGS:flags = CON_VARFLAG_RULE);
native SetServerRule(const name[], const value[]);
native IsValidServerRule(const name[]);
native SetServerRuleFlags(const name[], E_SERVER_RULE_FLAGS:flags);
native E_SERVER_RULE_FLAGS:GetServerRuleFlags(const name[]);

// Chat character replacement
native ToggleChatTextReplacement(bool:toggle);
native bool:ChatTextReplacementToggled();

// Server settings
native GetServerSettings(&showplayermarkes, &shownametags = 0, &stuntbonus = 0, &useplayerpedanims = 0, &bLimitchatradius = 0, &disableinteriorenterexits = 0, &nametaglos = 0, &manualvehicleengine = 0, &limitplayermarkers = 0, &vehiclefriendlyfire = 0, &defaultcameracollision = 0, &Float:fGlobalchatradius = 0.0, &Float:fNameTagDrawDistance = 0.0, &Float:fPlayermarkerslimit = 0.0);
native GetNPCCommandLine(npcid, npcscript[], length = sizeof(npcscript));

// Player position sync bounds
native GetSyncBounds(&Float:hmin, &Float:hmax = 0.0, &Float:vmin = 0.0, &Float:vmax = 0.0);
native SetSyncBounds(Float:hmin, Float:hmax, Float:vmin, Float:vmax);

// Toggling RCON commands
native GetRCONCommandName(const cmdname[], changedname[], len = sizeof(changedname));
native ChangeRCONCommandName(const cmdname[], changedname[]);

// Per AMX function calling
native CallFunctionInScript(const scriptname[], const function[], const format[], {Float,_}:...);

// Broadcasting console messages
native EnableConsoleMSGsForPlayer(playerid, color);
native DisableConsoleMSGsForPlayer(playerid);
native HasPlayerConsoleMessages(playerid, &color = 0);

// YSF settings
native YSF_SetTickRate(ticks);
native YSF_GetTickRate();
native YSF_EnableNightVisionFix(bool:enable);
native bool:YSF_IsNightVisionFixEnabled();
native YSF_ToggleOnServerMessage(bool:toggle);
native bool:YSF_IsOnServerMessageEnabled();
native YSF_SetExtendedNetStatsEnabled(bool:enable);
native bool:YSF_IsExtendedNetStatsEnabled();
native YSF_SetAFKAccuracy(time_ms);
native YSF_GetAFKAccuracy();

native SetRecordingDirectory(const dir[]);
native GetRecordingDirectory(dir[], len = sizeof(dir));

// Nickname
native IsValidNickName(const name[]);
native AllowNickNameCharacter(character, bool:allow);
native IsNickNameCharacterAllowed(character);

// Classes
native GetAvailableClasses();
native GetPlayerClass(classid, &teamid, &modelid = 0, &Float:spawn_x = 0.0, &Float:spawn_y = 0.0, &Float:spawn_z = 0.0, &Float:z_angle = 0.0, &weapon1 = 0, &weapon1_ammo = 0, &weapon2 = 0, &weapon2_ammo = 0, &weapon3 = 0, &weapon3_ammo = 0);
native EditPlayerClass(classid, teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);

// Timers
native GetRunningTimers();

// Per player things
native SetPlayerGravity(playerid, Float:gravity);
native Float:GetPlayerGravity(playerid);
native SetPlayerAdmin(playerid, bool:admin); // Set player as RCON admin
native SetPlayerTeamForPlayer(playerid, teamplayerid, teamid);
native GetPlayerTeamForPlayer(playerid, teamplayerid);
native SetPlayerSkinForPlayer(playerid, skinplayerid, skin);
native GetPlayerSkinForPlayer(playerid, skinplayerid);
native SetPlayerNameForPlayer(playerid, nameplayerid, const playername[]);
native GetPlayerNameForPlayer(playerid, nameplayerid, playername[], size = sizeof(playername));
native SetPlayerFightStyleForPlayer(playerid, styleplayerid, style);
native GetPlayerFightStyleForPlayer(playerid, skinplayerid);
native SetPlayerPosForPlayer(playerid, posplayerid, Float:fX, Float:fY, Float:fZ, bool:forcesync = true);
native SetPlayerRotationQuatForPlayer(playerid, quatplayerid, Float:w, Float:x, Float:y, Float:z, bool:forcesync = true);
native ApplyAnimationForPlayer(playerid, animplayerid, const animlib[], const animname[], Float:fDelta, loop, lockx, locky, freeze, time); // DOESN'T WORK
native GetPlayerWeather(playerid);
native TogglePlayerWidescreen(playerid, bool:set);
native IsPlayerWidescreenToggled(playerid);
native GetSpawnInfo(playerid, &teamid, &modelid = 0, &Float:spawn_x = 0.0, &Float:spawn_y = 0.0, &Float:spawn_z = 0.0, &Float:z_angle = 0.0, &weapon1 = 0, &weapon1_ammo = 0, &weapon2 = 0, &weapon2_ammo = 0, &weapon3 = 0, &weapon3_ammo = 0);
native GetPlayerSkillLevel(playerid, skill);
native IsPlayerCheckpointActive(playerid);
native GetPlayerCheckpoint(playerid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fSize = 0.0);
native IsPlayerRaceCheckpointActive(playerid);
native GetPlayerRaceCheckpoint(playerid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fNextX = 0.0, &Float:fNextY = 0.0, &Float:fNextZ = 0.0, &Float:fSize = 0.0);
#if !defined GetPlayerWorldBounds
native GetPlayerWorldBounds(playerid, &Float:x_max, &Float:x_min = 0.0, &Float:y_max = 0.0, &Float:y_min = 0.0);
#endif
native IsPlayerInModShop(playerid);
native GetPlayerSirenState(playerid);
native GetPlayerLandingGearState(playerid);
native GetPlayerHydraReactorAngle(playerid);
native Float:GetPlayerTrainSpeed(playerid);
native Float:GetPlayerZAim(playerid);
native GetPlayerSurfingOffsets(playerid, &Float:fOffsetX, &Float:fOffsetY = 0.0, &Float:fOffsetZ = 0.0);
native GetPlayerRotationQuat(playerid, &Float:w, &Float:x = 0.0, &Float:y = 0.0, &Float:z = 0.0);
native GetPlayerDialogID(playerid);
native GetPlayerSpectateID(playerid);
native GetPlayerSpectateType(playerid);
native GetPlayerLastSyncedVehicleID(playerid);
native GetPlayerLastSyncedTrailerID(playerid);
native GetPlayerRawIp(playerid);

native SendBulletData(senderid, forplayerid = -1, weaponid, hittype, hitid, Float:fHitOriginX, Float:fHitOriginY, Float:fHitOriginZ, Float:fHitTargetX, Float:fHitTargetY, Float:fHitTargetZ, Float:fCenterOfHitX, Float:fCenterOfHitY, Float:fCenterOfHitZ);
native ShowPlayerForPlayer(forplayerid, playerid, bool:setskin=false);
native HidePlayerForPlayer(forplayerid, playerid);
native AddPlayerForPlayer(forplayerid, playerid, isnpc = 0);
native RemovePlayerForPlayer(forplayerid, playerid);
native SetPlayerChatBubbleForPlayer(forplayerid, playerid, const text[], color, Float:drawdistance, expiretime);
native ResetPlayerMarkerForPlayer(playerid, resetplayerid);
native SetPlayerVersion(playerid, const version[]);
native IsPlayerSpawned(playerid);
native IsPlayerControllable(playerid);
native SpawnForWorld(playerid);
native BroadcastDeath(playerid);
native IsPlayerCameraTargetEnabled(playerid);
native SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0);
native GetPlayerDisabledKeysSync(playerid, &keys, &updown = 0, &leftright = 0);
native IgnorePacketsFromPlayer(playerid, fromplayerid, bool:ignore = true);
native ArePacketsIgnoredFromPlayer(playerid, fromplayerid);

native GetPlayerBuildingsRemoved(playerid);
native IsBuildingRemovedForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius);

native TogglePlayerGhostMode(playerid, bool:toggle);
native GetPlayerGhostMode(playerid);
native SetPlayerSyncKeys(playerid, keys, updown, leftright);
native SetPlayerSyncPos(playerid, Float:x, Float:y, Float:z);
native SetPlayerSyncVelocity(playerid, Float:x, Float:y, Float:z);
native SetPlayerSyncRotationQuat(playerid, Float:w, Float:x, Float:y, Float:z);
native SetPlayerSyncCameraFrontVector(playerid, Float:x, Float:y, Float:z);
native SetPlayerSyncCameraPos(playerid, Float:x, Float:y, Float:z);
native SetPlayerSyncCameraMode(playerid, mode);
native SetPlayerSyncWeapon(playerid, weaponid);
native GetPlayerSyncWeapon(playerid);
native SetPlayerSyncWeaponState(playerid, weaponstate);
native SetPlayerSyncSpecialAction(playerid, actionid);
native SetPlayerSyncHealth(playerid, Float:health);
native SetPlayerSyncArmour(playerid, Float:armour);
native SetPlayerSyncVehicleId(playerid, vehicleid, bool:setstate=false);
native SetPlayerSyncVehicleSeat(playerid, seat);
native SetPlayerSyncVehiclePosition(playerid, Float:x, Float:y, Float:z);
native SetPlayerSyncVehicleVelocity(playerid, Float:x, Float:y, Float:z);
native SetPlayerSyncVehicleRotQuat(playerid, Float:w, Float:x, Float:y, Float:z);
native SetPlayerSyncVehicleHealth(playerid, Float:health);
native SetPlayerSyncTrainSpeed(playerid, Float:speed);
native SendPlayerDeath(playerid, forplayerid=-1);
native UpdatePlayerSyncData(playerid, type=-1, bool:setstate=false);
native CopyPlayerSyncData(fromplayerid, toplayerid);
native SendPlayerClientGameInit(playerid, bool:usecjwalk=bool:-1, bool:limitglobalchat=bool:-1, Float:globalchatradius=Float:-1, Float:nametagdistance=Float:-1, bool:disableenterexits=bool:-1, bool:nametaglos=bool:-1, bool:manualvehengineandlights=bool:-1,
				spawnsavailable=-1, bool:shownametags=bool:-1, bool:showplayermarkers=bool:-1, onfoot_rate=-1, incar_rate=-1, weapon_rate=-1, lagcompmode=-1, bool:vehiclefriendlyfire=bool:-1, const hostname[]="");

native SetPlayerConnectMode(playerid, E_PLAYER_CONNECT_MODE:mode);
native E_PLAYER_CONNECT_MODE:GetPlayerConnectMode(playerid);

// Actors
native GetActorSpawnInfo(actorid, &skinid, &Float:fX = 0.0, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fAngle = 0.0);
native GetActorSkin(actorid);
native GetActorAnimation(actorid, animlib[], animlibsize = sizeof(animlib), animname[], animnamesize = sizeof(animname), &Float:fDelta, &loop = 0, &lockx = 0, &locky = 0, &freeze = 0, &time = 0);

// Scoreboard manipulation
native TogglePlayerScoresPingsUpdate(playerid, bool:toggle);
native TogglePlayerFakePing(playerid, bool:toggle);
native SetPlayerFakePing(playerid, ping);
native SetPlayerNameInServerQuery(playerid, const name[]);
native GetPlayerNameInServerQuery(playerid, name[], len = sizeof(name));
native ResetPlayerNameInServerQuery(playerid);

// Pause functions
native IsPlayerPaused(playerid);
native GetPlayerPausedTime(playerid);

// Objects
native HideObjectForPlayer(playerid, objectid);
native ShowObjectForPlayer(playerid, objectid);
native HideNewObjectsForPlayer(playerid, bool:toggle);
native IsObjectHiddenForPlayer(playerid, objectid);
native NewObjectsHiddenForPlayer(playerid);

// Objects get - global
native Float:GetObjectDrawDistance(objectid);
native SetObjectMoveSpeed(objectid, Float:fSpeed);
native Float:GetObjectMoveSpeed(objectid);
native GetObjectTarget(objectid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0);
native GetObjectAttachedData(objectid, &attached_vehicleid, &attached_objectid = 0, &attached_playerid = 0);
native GetObjectAttachedOffset(objectid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fRotX = 0.0, &Float:fRotY = 0.0, &Float:fRotZ = 0.0);
native GetObjectSyncRotation(objectid);
native IsObjectMaterialSlotUsed(objectid, materialindex); // Return values: 1 = material, 2 = material text
native GetObjectMaterial(objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor = 0);
native GetObjectMaterialText(objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize = 0, &bold = 0, &fontcolor = 0, &backcolor = 0, &textalignment = 0);
native IsObjectNoCameraCol(objectid);

// Objects get - player
native Float:GetPlayerObjectDrawDistance(playerid, objectid);
native SetPlayerObjectMoveSpeed(playerid, objectid, Float:fSpeed);
native Float:GetPlayerObjectMoveSpeed(playerid, objectid);
native Float:GetPlayerObjectTarget(playerid, objectid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0);
native GetPlayerObjectAttachedData(playerid, objectid, &attached_vehicleid, &attached_objectid = 0, &attached_playerid = 0);
native GetPlayerObjectAttachedOffset(playerid, objectid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fRotX = 0.0, &Float:fRotY = 0.0, &Float:fRotZ = 0.0);
native GetPlayerObjectSyncRotation(playerid, objectid);
native IsPlayerObjectMaterialSlotUsed(playerid, objectid, materialindex); // Return values: 1 = material, 2 = material text
native GetPlayerObjectMaterial(playerid, objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor = 0);
native GetPlayerObjectMaterialText(playerid, objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize = 0, &bold = 0, &fontcolor = 0, &backcolor = 0, &textalignment = 0);
native IsPlayerObjectNoCameraCol(playerid, objectid);
native GetPlayerSurfingPlayerObjectID(playerid);
native GetPlayerCameraTargetPlayerObj(playerid);
native GetObjectType(playerid, objectid);

// Get function for GetPlayerAttachedObject
native GetPlayerAttachedObject(playerid, index, &modelid, &bone = 0, &Float:fX = 0.0, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fRotX = 0.0, &Float:fRotY = 0.0, &Float:fRotZ = 0.0, &Float:fSacleX = 0.0, &Float:fScaleY = 0.0, &Float:fScaleZ = 0.0, &materialcolor1 = 0, &materialcolor2 = 0);

// Per-player attached (aka holding) objects
//native SetPlayerAttachedObjForPlayer(forplayerid, attachtoplayerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:OffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0);
//native GetPlayerAttachedObjForPlayer(forplayerid, attachtoplayerid, index, &modelid, &bone = 0, &Float:fX = 0.0, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fRotX = 0.0, &Float:fRotY = 0.0, &Float:fRotZ = 0.0, &Float:fSacleX = 0.0, &Float:fScaleY = 0.0, &Float:fScaleZ = 0.0, &materialcolor1 = 0, &materialcolor2 = 0);
//native RemPlayerAttachedObjForPlayer(forplayerid, removefromplayerid, index);
//native IsPlayerAttachedObjForPlayer(forplayerid, attachtoplayerid, index);

// Already declared in a_objects.inc
//native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ);
native AttachPlayerObjectToObject(playerid, objectid, attachtoid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ, SyncRotation = 1);

// RakNet ban functions
native ClearBanList();
native IsBanned(const ipaddress[]);

// RakNet
native SetTimeoutTime(playerid, time_ms);
native GetLocalIP(index, localip[], len = sizeof(localip));

// Exclusive RPC broadcast

enum
{
	EXCLUSIVE_BROADCAST_ONCE_OFF = -1,
	EXCLUSIVE_BROADCAST_OFF = 0,
	EXCLUSIVE_BROADCAST_ON = 1,
	EXCLUSIVE_BROADCAST_ONCE_ON = 2
};

native SetExclusiveBroadcast(status);
native BroadcastToPlayer(playerid, bool:toggle = true);

native ToggleCloseConnectionFix(bool:toggle);
native ToggleInvalidSkinProtection(bool:toggle);
native ToggleCreateVehicleModelCheck(bool:toggle);

// Vehicle functions
native GetVehicleSpawnInfo(vehicleid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0, &Float:fRot = 0.0, &color1 = 0, &color2 = 0);
native SetVehicleSpawnInfo(vehicleid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, respawntime = -2, interior = -2);
#if !defined GetVehicleColor
native GetVehicleColor(vehicleid, &color1, &color2 = 0);
#endif
#if !defined GetVehiclePaintjob
native GetVehiclePaintjob(vehicleid);
#endif
#if !defined GetVehicleInterior
native GetVehicleInterior(vehicleid);
#endif
native GetVehicleNumberPlate(vehicleid, plate[], len = sizeof(plate));
native SetVehicleRespawnDelay(vehicleid, delay);
native GetVehicleRespawnDelay(vehicleid);
native SetVehicleOccupiedTick(vehicleid, ticks);
native GetVehicleOccupiedTick(vehicleid); // GetTickCount() - GetVehicleOccupiedTick(vehicleid) = time passed since vehicle is occupied, in ms
native SetVehicleRespawnTick(vehicleid, ticks);
native GetVehicleRespawnTick(vehicleid); // GetTickCount() - GetVehicleRespawnTick(vehicleid) = time passed since vehicle spawned, in ms
native GetVehicleLastDriver(vehicleid);
native GetVehicleCab(vehicleid);
native HasVehicleBeenOccupied(vehicleid);
native SetVehicleBeenOccupied(vehicleid, bool:occupied);
native IsVehicleOccupied(vehicleid);
native IsVehicleDead(vehicleid);
native SetVehicleDead(vehicleid, bool:dead);
native SetVehicleParamsSirenState(vehicleid, sirenState);
native ToggleVehicleSirenEnabled(vehicleid, enabled);
native IsVehicleSirenEnabled(vehicleid);
native GetVehicleMatrix(vehicleid, &Float:rightX, &Float:rightY = 0.0, &Float:rightZ = 0.0, &Float:upX = 0.0, &Float:upY = 0.0, &Float:upZ = 0.0, &Float:atX = 0.0, &Float:atY = 0.0, &Float:atZ = 0.0);
native GetVehicleModelCount(modelid);
native GetVehicleModelsUsed();
native HideVehicle(vehicleid);
native ShowVehicle(vehicleid);
native IsVehicleHidden(vehicleid);

// Gangzones - Global
native IsValidGangZone(zoneid);
native IsPlayerInGangZone(playerid, zoneid);
native IsGangZoneVisibleForPlayer(playerid, zoneid);
native GangZoneGetColorForPlayer(playerid, zoneid);
native GangZoneGetFlashColorForPlayer(playerid, zoneid);
native IsGangZoneFlashingForPlayer(playerid, zoneid);
native GangZoneGetPos(zoneid, &Float:fMinX, &Float:fMinY = 0.0, &Float:fMaxX = 0.0, &Float:fMaxY = 0.0);

// Gangzones - Player
native CreatePlayerGangZone(playerid, Float:minx, Float:miny, Float:maxx, Float:maxy);
native PlayerGangZoneDestroy(playerid, zoneid);
native PlayerGangZoneShow(playerid, zoneid, color);
native PlayerGangZoneHide(playerid, zoneid);
native PlayerGangZoneFlash(playerid, zoneid, color);
native PlayerGangZoneStopFlash(playerid, zoneid);
native IsValidPlayerGangZone(playerid, zoneid);
native IsPlayerInPlayerGangZone(playerid, zoneid);
native IsPlayerGangZoneVisible(playerid, zoneid);
native PlayerGangZoneGetColor(playerid, zoneid);
native PlayerGangZoneGetFlashColor(playerid, zoneid);
native IsPlayerGangZoneFlashing(playerid, zoneid);
native PlayerGangZoneGetPos(playerid, zoneid, &Float:fMinX, &Float:fMinY = 0.0, &Float:fMaxX = 0.0, &Float:fMaxY = 0.0);

// Textdraw - Global
native IsValidTextDraw(Text:textdrawid);
native IsTextDrawVisibleForPlayer(playerid, Text:textdrawid);
native TextDrawGetString(Text:textdrawid, text[], len = sizeof(text));
native TextDrawSetPos(Text:textdrawid, Float:fX, Float:fY); // You can change textdraw pos with it, but you need to use TextDrawShowForPlayer() after that
native TextDrawGetLetterSize(Text:textdrawid, &Float:fX, &Float:fY = 0.0);
native TextDrawGetTextSize(Text:textdrawid, &Float:fX, &Float:fY = 0.0);
native TextDrawGetPos(Text:textdrawid, &Float:fX, &Float:fY = 0.0);
native TextDrawGetColor(Text:textdrawid);
native TextDrawGetBoxColor(Text:textdrawid);
native TextDrawGetBackgroundColor(Text:textdrawid);
native TextDrawGetShadow(Text:textdrawid);
native TextDrawGetOutline(Text:textdrawid);
native TextDrawGetFont(Text:textdrawid);
native TextDrawIsBox(Text:textdrawid);
native TextDrawIsProportional(Text:textdrawid);
native TextDrawIsSelectable(Text:textdrawid);
native TextDrawGetAlignment(Text:textdrawid);
native TextDrawGetPreviewModel(Text:textdrawid);
native TextDrawGetPreviewRot(Text:textdrawid, &Float:fRotX, &Float:fRotY = 0.0, &Float:fRotZ = 0.0, &Float:fZoom = 0.0);
native TextDrawGetPreviewVehCol(Text:textdrawid, &color1, &color2 = 0);
native TextDrawSetStringForPlayer(Text:textdrawid, playerid, const string[], {Float,_}:...);

// Textdraw - Player
native IsValidPlayerTextDraw(playerid, PlayerText:textdrawid);
native IsPlayerTextDrawVisible(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetString(playerid, PlayerText:textdrawid, text[], len = sizeof(text));
native PlayerTextDrawSetPos(playerid, PlayerText:textdrawid, Float:fX, Float:fY);
native PlayerTextDrawGetLetterSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY = 0.0);
native PlayerTextDrawGetTextSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY = 0.0);
native PlayerTextDrawGetPos(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY = 0.0);
native PlayerTextDrawGetColor(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetBoxColor(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetBackgroundCol(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetShadow(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetOutline(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetFont(playerid, PlayerText:textdrawid);
native PlayerTextDrawIsBox(playerid, PlayerText:textdrawid);
native PlayerTextDrawIsProportional(playerid, PlayerText:textdrawid);
native PlayerTextDrawIsSelectable(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetAlignment(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetPreviewModel(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetPreviewRot(playerid, PlayerText:textdrawid, &Float:fRotX, &Float:fRotY = 0.0, &Float:fRotZ = 0.0, &Float:fZoom = 0.0);
native PlayerTextDrawGetPreviewVehCol(playerid, PlayerText:textdrawid, &color1, &color2 = 0);

// 3D Text - Global
native IsValid3DTextLabel(Text3D:id);
native Is3DTextLabelStreamedIn(playerid, Text3D:id);
native Get3DTextLabelText(Text3D:id, text[], len = sizeof(text));
native Get3DTextLabelColor(Text3D:id);
native Get3DTextLabelPos(Text3D:id, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0);
native Float:Get3DTextLabelDrawDistance(Text3D:id);
native Get3DTextLabelLOS(Text3D:id);
native Get3DTextLabelVirtualWorld(Text3D:id);
native Get3DTextLabelAttachedData(Text3D:id, &attached_playerid, &attached_vehicleid = 0);

// 3D Text - Player
native IsValidPlayer3DTextLabel(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelText(playerid, PlayerText3D:id, text[], len = sizeof(text));
native GetPlayer3DTextLabelColor(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelPos(playerid, PlayerText3D:id, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0);
native Float:GetPlayer3DTextLabelDrawDist(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelLOS(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelVirtualW(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelAttached(playerid, PlayerText3D:id, &attached_playerid, &attached_vehicleid = 0);

// Menu
native IsMenuDisabled(Menu:menuid);
native IsMenuRowDisabled(Menu:menuid, row);
native GetMenuColumns(Menu:menuid);
native GetMenuItems(Menu:menuid, column);
native GetMenuPos(Menu:menuid, &Float:fX, &Float:fY = 0.0);
native GetMenuColumnWidth(Menu:menuid, &Float:fColumn1, &Float:fColumn2 = 0.0);
native GetMenuColumnHeader(Menu:menuid, column, header[], len = sizeof(header));
native GetMenuItem(Menu:menuid, column, itemid, item[], len = sizeof(item));

// Pickups - Global
native IsValidPickup(pickupid);
native IsPickupStreamedIn(playerid, pickupid);
native GetPickupPos(pickupid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0);
native GetPickupModel(pickupid);
native GetPickupType(pickupid);
native GetPickupVirtualWorld(pickupid);
native SetPickupPos(pickupid, Float:X, Float:Y, Float:Z, bool:update=true);
native SetPickupModel(pickupid, model, bool:update=true);
native SetPickupType(pickupid, type, bool:update=true);
native SetPickupVirtualWorld(pickupid, virtualworld);
native ShowPickupForPlayer(playerid, pickupid);
native HidePickupForPlayer(playerid, pickupid);
native SetPickupForPlayer(playerid, id, model, type, Float:X, Float:Y, Float:Z);

/*
// Pickups - Player
native CreatePlayerPickup(playerid, model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
native DestroyPlayerPickup(playerid, pickupid);
native IsValidPlayerPickup(playerid, pickupid);
native IsPlayerPickupStreamedIn(playerid, pickupid);
native GetPlayerPickupPos(playerid, pickupid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0);
native GetPlayerPickupModel(playerid, pickupid);
native GetPlayerPickupType(playerid, pickupid);
native GetPlayerPickupVirtualWorld(playerid, pickupid);
*/

// Y_Less's model sizes inc
native GetColCount();
native Float:GetColSphereRadius(modelid);
native GetColSphereOffset(modelid, &Float:fX, &Float:fY = 0.0, &Float:fZ = 0.0);

// Formatting
native SendClientMessagef(playerid, color, const message[], {Float,_}:...);
native SendClientMessageToAllf(color, const message[], {Float,_}:...);
native GameTextForPlayerf(playerid, displaytime, style, const message[], {Float,_}:...);
native GameTextForAllf(displaytime, style, const message[], {Float,_}:...);
native SendPlayerMessageToPlayerf(playerid, senderid, const message[], {Float,_}:...);
native SendPlayerMessageToAllf(senderid, const message[], {Float,_}:...);
native SendRconCommandf(const command[], {Float,_}:...);

// Callbacks
forward OnPlayerEnterGangZone(playerid, zoneid);
forward OnPlayerLeaveGangZone(playerid, zoneid);
forward OnPlayerEnterPlayerGangZone(playerid, zoneid);
forward OnPlayerLeavePlayerGangZone(playerid, zoneid);
forward OnPlayerPickUpPlayerPickup(playerid, pickupid);
forward OnPlayerPauseStateChange(playerid, pausestate);
forward OnPlayerStatsUpdate(playerid);
forward OnPlayerWeaponsUpdate(playerid);
forward OnRemoteRCONPacket(const ipaddr[], port, const password[], success, const command[]);
forward OnServerMessage(const msg[]);
forward OnPlayerClientGameInit(playerid, &usecjwalk, &limitglobalchat, &Float:globalchatradius, &Float:nametagdistance, &disableenterexits, &nametaglos, &manualvehengineandlights,
				&spawnsavailable, &shownametags, &showplayermarkers, &onfoot_rate, &incar_rate, &weapon_rate, &lagcompmode, &vehiclefriendlyfire);
forward OnOutcomeScmEvent(playerid, issuerid, E_SCM_EVENT_TYPE:eventid, vehicleid, arg1, arg2);
forward OnServerQueryInfo(const ipaddr[], hostname[51], gamemode[31], language[31]);
forward OnSystemCommandExecute(const line_output[], retval, index, success, line_current, line_total);
forward OnPickupStreamIn(pickupid, forplayerid);
forward OnPickupStreamOut(pickupid, forplayerid);

//////////////////////////////////////////////////////////////
// Fixes
//////////////////////////////////////////////////////////////

// No comment..
#if !defined IsValidVehicle
	native IsValidVehicle(vehicleid);
#endif

#if !defined GetGravity
	native Float:GetGravity();
#endif

#if !defined GetWeather
	#define GetWeather() GetConsoleVarAsInt("weather")
#endif
native GetWeaponSlot(weaponid);

enum
{
	BS_BOOL,
	BS_CHAR,
	BS_UNSIGNEDCHAR,
	BS_SHORT,
	BS_UNSIGNEDSHORT,
	BS_INT,
	BS_UNSIGNEDINT,
	BS_FLOAT,
	BS_STRING
};

// Developer functions
native SendRPC(playerid, RPC, {Float,_}:...); // playerid == -1 -> broadcast
native SendData(playerid, {Float,_}:...); // playerid == -1 -> broadcast

/* EXAMPLE
CMD:pickup(playerid, params[])
{
	new
		Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X, Y, Z);

	SendRPC(playerid, 95, // rpcid
		BS_INT, strval(params), // int - pickupid
		BS_INT, 1222, // int - modelid
		BS_INT, 19, // int - type
		BS_FLOAT, X + 2.0, // float
		BS_FLOAT, Y, // float
		BS_FLOAT, Z); // float
	return 1;
}

CMD:destroypickup(playerid, params[])
{
	SendRPC(playerid, 63, // rpcid
		BS_INT, strval(params)); // int - pickupid
	return 1;
}
*/
