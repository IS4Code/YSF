/* YSF - kurta999's version */
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

// Server settings
native GetServerSettings(&showplayermarkes, &shownametags, &stuntbonus, &useplayerpedanims, &bLimitchatradius, &disableinteriorenterexits, &nametaglos, &manualvehicleengine, &limitplayermarkers, &vehiclefriendlyfire, &defaultcameracollision, &Float:fGlobalchatradius, &Float:fNameTagDrawDistance, &Float:fPlayermarkerslimit);
native GetNPCCommandLine(npcid, npcscript[], length = sizeof(npcscript));

// Player position sync bounds
native GetSyncBounds(&Float:hmin, &Float:hmax, &Float:vmin, &Float:vmax);
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
native YSF_EnableNightVisionFix(enable);
native YSF_IsNightVisionFixEnabled();
native YSF_ToggleOnServerMessage(toggle);
native YSF_IsOnServerMessageEnabled();
native YSF_SetExtendedNetStatsEnabled(enable);
native YSF_IsExtendedNetStatsEnabled();
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
native GetPlayerClass(classid, &teamid, &modelid, &Float:spawn_x, &Float:spawn_y, &Float:spawn_z, &Float:z_angle, &weapon1, &weapon1_ammo, &weapon2, &weapon2_ammo,& weapon3, &weapon3_ammo);
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
native GetSpawnInfo(playerid, &teamid, &modelid, &Float:spawn_x, &Float:spawn_y, &Float:spawn_z, &Float:z_angle, &weapon1, &weapon1_ammo, &weapon2, &weapon2_ammo,& weapon3, &weapon3_ammo);
native GetPlayerSkillLevel(playerid, skill);
native IsPlayerCheckpointActive(playerid);
native GetPlayerCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fSize);
native IsPlayerRaceCheckpointActive(playerid);
native GetPlayerRaceCheckpoint(playerid, &Float:fX, &Float:fY, &Float:fZ, &Float:fNextX, &Float:fNextY, &Float:fNextZ, &Float:fSize);
native GetPlayerWorldBounds(playerid, &Float:x_max, &Float:x_min, &Float:y_max, &Float:y_min);
native IsPlayerInModShop(playerid);
native GetPlayerSirenState(playerid);
native GetPlayerLandingGearState(playerid);
native GetPlayerHydraReactorAngle(playerid);
native Float:GetPlayerTrainSpeed(playerid);
native Float:GetPlayerZAim(playerid);
native GetPlayerSurfingOffsets(playerid, &Float:fOffsetX, &Float:fOffsetY, &Float:fOffsetZ);
native GetPlayerRotationQuat(playerid, &Float:w, &Float:x, &Float:y, &Float:z);
native GetPlayerDialogID(playerid);
native GetPlayerSpectateID(playerid);
native GetPlayerSpectateType(playerid);
native GetPlayerLastSyncedVehicleID(playerid);
native GetPlayerLastSyncedTrailerID(playerid);

native SendBulletData(senderid, forplayerid = -1, weaponid, hittype, hitid, Float:fHitOriginX, Float:fHitOriginY, Float:fHitOriginZ, Float:fHitTargetX, Float:fHitTargetY, Float:fHitTargetZ, Float:fCenterOfHitX, Float:fCenterOfHitY, Float:fCenterOfHitZ);
native ShowPlayerForPlayer(forplayerid, playerid);
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

// Actors
native GetActorSpawnInfo(actorid, &skinid, &Float:fX, &Float:fY, &Float:fZ, &Float:fAngle);
native GetActorSkin(actorid);
native GetActorAnimation(actorid, animlib[], animlibsize = sizeof(animlib), animname[], animnamesize = sizeof(animname), &Float:fDelta, &loop, &lockx, &locky, &freeze, &time);

// Scoreboard manipulation
native TogglePlayerScoresPingsUpdate(playerid, bool:toggle);
native TogglePlayerFakePing(playerid, bool:toggle);
native SetPlayerFakePing(playerid, ping);
native TogglePlayerInServerQuery(playerid, bool:toggle);
native IsPlayerToggledInServerQuery(playerid);

// Pause functions
native IsPlayerPaused(playerid);
native GetPlayerPausedTime(playerid);

// Objects get - global
native Float:GetObjectDrawDistance(objectid);
native SetObjectMoveSpeed(objectid, Float:fSpeed);
native Float:GetObjectMoveSpeed(objectid);
native GetObjectTarget(objectid, &Float:fX, &Float:fY, &Float:fZ);
native GetObjectAttachedData(objectid, &attached_vehicleid, &attached_objectid, &attached_playerid);
native GetObjectAttachedOffset(objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
native IsObjectMaterialSlotUsed(objectid, materialindex); // Return values: 1 = material, 2 = material text
native GetObjectMaterial(objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcoor);
native GetObjectMaterialText(objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
native IsObjectNoCameraCol(objectid);

// Objects get - player
native Float:GetPlayerObjectDrawDistance(playerid, objectid);
native SetPlayerObjectMoveSpeed(playerid, objectid, Float:fSpeed);
native Float:GetPlayerObjectMoveSpeed(playerid, objectid);
native Float:GetPlayerObjectTarget(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ);
native GetPlayerObjectAttachedData(playerid, objectid, &attached_vehicleid, &attached_objectid, &attached_playerid);
native GetPlayerObjectAttachedOffset(playerid, objectid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ);
native IsPlayerObjectMaterialSlotUsed(playerid, objectid, materialindex); // Return values: 1 = material, 2 = material text
native GetPlayerObjectMaterial(playerid, objectid, materialindex, &modelid, txdname[], txdnamelen = sizeof(txdname), texturename[], texturenamelen = sizeof(texturename), &materialcolor);
native GetPlayerObjectMaterialText(playerid, objectid, materialindex, text[], textlen = sizeof(text), &materialsize, fontface[], fontfacelen = sizeof(fontface), &fontsize, &bold, &fontcolor, &backcolor, &textalignment);
native IsPlayerObjectNoCameraCol(playerid, objectid);
native GetPlayerSurfingPlayerObjectID(playerid);
native GetPlayerCameraTargetPlayerObj(playerid);
native GetObjectType(playerid, objectid);

// special - for attached objects
native GetPlayerAttachedObject(playerid, index, &modelid, &bone, &Float:fX, &Float:fY, &Float:fZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fSacleX, &Float:fScaleY, &Float:fScaleZ, &materialcolor1, &materialcolor2);

//n_ative AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ);
native AttachPlayerObjectToObject(playerid, objectid, attachtoid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ, SyncRotation = 1);

// RakNet ban functions
native ClearBanList();
native IsBanned(const ipaddress[]);

// RakNet
native SetTimeoutTime(playerid, time_ms);
native GetLocalIP(index, localip[], len = sizeof(localip));

// Exclusive RPC broadcast
native SetExclusiveBroadcast(toggle);
native BroadcastToPlayer(playerid, toggle=1);

// Vehicle functions
native GetVehicleSpawnInfo(vehicleid, &Float:fX, &Float:fY, &Float:fZ, &Float:fRot, &color1, &color2);
native SetVehicleSpawnInfo(vehicleid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, respawntime = -2, interior = -2);
#if !defined GetVehicleColor
native GetVehicleColor(vehicleid, &color1, &color2);
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
native SetVehicleBeenOccupied(vehicleid, occupied);
native IsVehicleOccupied(vehicleid);
native IsVehicleDead(vehicleid);
native SetVehicleParamsSirenState(vehicleid, sirenState);
native ToggleVehicleSirenEnabled(vehicleid, enabled);
native IsVehicleSirenEnabled(vehicleid);
native GetVehicleMatrix(vehicleid, &Float:rightX, &Float:rightY, &Float:rightZ, &Float:upX, &Float:upY, &Float:upZ, &Float:atX, &Float:atY, &Float:atZ);
native GetVehicleModelCount(modelid);
native GetVehicleModelsUsed();

// Gangzones - Global
native IsValidGangZone(zoneid);
native IsPlayerInGangZone(playerid, zoneid);
native IsGangZoneVisibleForPlayer(playerid, zoneid);
native GangZoneGetColorForPlayer(playerid, zoneid);
native GangZoneGetFlashColorForPlayer(playerid, zoneid);
native IsGangZoneFlashingForPlayer(playerid, zoneid);
native GangZoneGetPos(zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);

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
native PlayerGangZoneGetPos(playerid, zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);

// Textdraw - Global
native IsValidTextDraw(Text:textdrawid);
native IsTextDrawVisibleForPlayer(playerid, Text:textdrawid);
native TextDrawGetString(Text:textdrawid, text[], len = sizeof(text));
native TextDrawSetPos(Text:textdrawid, Float:fX, Float:fY); // You can change textdraw pos with it, but you need to use TextDrawShowForPlayer() after that
native TextDrawGetLetterSize(Text:textdrawid, &Float:fX, &Float:fY);
native TextDrawGetTextSize(Text:textdrawid, &Float:fX, &Float:fY);
native TextDrawGetPos(Text:textdrawid, &Float:fX, &Float:fY);
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
native TextDrawGetPreviewRot(Text:textdrawid, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fZoom);
native TextDrawGetPreviewVehCol(Text:textdrawid, &color1, &color2);
native TextDrawSetStringForPlayer(Text:textdrawid, playerid, const string[], {Float,_}:...);

// Textdraw - Player
native IsValidPlayerTextDraw(playerid, PlayerText:textdrawid);
native IsPlayerTextDrawVisible(playerid, PlayerText:textdrawid);
native PlayerTextDrawGetString(playerid, PlayerText:textdrawid, text[], len = sizeof(text));
native PlayerTextDrawSetPos(playerid, PlayerText:textdrawid, Float:fX, Float:fY);
native PlayerTextDrawGetLetterSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
native PlayerTextDrawGetTextSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
native PlayerTextDrawGetPos(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
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
native PlayerTextDrawGetPreviewRot(playerid, PlayerText:textdrawid, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fZoom);
native PlayerTextDrawGetPreviewVehCol(playerid, PlayerText:textdrawid, &color1, &color2);

// 3D Text - Global
native IsValid3DTextLabel(Text3D:id);
native Is3DTextLabelStreamedIn(playerid, Text3D:id);
native Get3DTextLabelText(Text3D:id, text[], len = sizeof(text));
native Get3DTextLabelColor(Text3D:id);
native Get3DTextLabelPos(Text3D:id, &Float:fX, &Float:fY, &Float:fZ);
native Float:Get3DTextLabelDrawDistance(Text3D:id);
native Get3DTextLabelLOS(Text3D:id);
native Get3DTextLabelVirtualWorld(Text3D:id);
native Get3DTextLabelAttachedData(Text3D:id, &attached_playerid, &attached_vehicleid);

// 3D Text - Player
native IsValidPlayer3DTextLabel(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelText(playerid, PlayerText3D:id, text[], len = sizeof(text));
native GetPlayer3DTextLabelColor(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelPos(playerid, PlayerText3D:id, &Float:fX, &Float:fY, &Float:fZ);
native Float:GetPlayer3DTextLabelDrawDist(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelLOS(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelVirtualW(playerid, PlayerText3D:id);
native GetPlayer3DTextLabelAttached(playerid, PlayerText3D:id, &attached_playerid, &attached_vehicleid);

// Menu
native IsMenuDisabled(Menu:menuid);
native IsMenuRowDisabled(Menu:menuid, row);
native GetMenuColumns(Menu:menuid);
native GetMenuItems(Menu:menuid, column);
native GetMenuPos(Menu:menuid, &Float:fX, &Float:fY);
native GetMenuColumnWidth(Menu:menuid, &Float:fColumn1, &Float:fColumn2);
native GetMenuColumnHeader(Menu:menuid, column, header[], len = sizeof(header));
native GetMenuItem(Menu:menuid, column, itemid, item[], len = sizeof(item));

// Pickups - Global
native IsValidPickup(pickupid);
native IsPickupStreamedIn(playerid, pickupid);
native GetPickupPos(pickupid, &Float:fX, &Float:fY, &Float:fZ);
native GetPickupModel(pickupid);
native GetPickupType(pickupid);
native GetPickupVirtualWorld(pickupid);
/*
// Pickups - Player
native CreatePlayerPickup(playerid, model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
native DestroyPlayerPickup(playerid, pickupid);
native IsValidPlayerPickup(playerid, pickupid);
native IsPlayerPickupStreamedIn(playerid, pickupid);
native GetPlayerPickupPos(playerid, pickupid, &Float:fX, &Float:fY, &Float:fZ);
native GetPlayerPickupModel(playerid, pickupid);
native GetPlayerPickupType(playerid, pickupid);
native GetPlayerPickupVirtualWorld(playerid, pickupid);
*/
// Y_Less's model sizes inc
native GetColCount();
native Float:GetColSphereRadius(modelid);
native GetColSphereOffset(modelid, &Float:fX, &Float:fY, &Float:fZ);

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
forward OnPlayerStatsAndWeaponsUpdate(playerid);
forward OnRemoteRCONPacket(const ipaddr[], port, const password[], success, const command[]);
forward OnServerMessage(const msg[]);
forward OnPlayerClientGameInit(playerid, &usecjwalk, &limitglobalchat, &Float:globalchatradius, &Float:nametagdistance, &disableenterexits, &nametaglos, &manualvehengineandlights,
				&spawnsavailable, &shownametags, &showplayermarkers, &onfoot_rate, &incar_rate, &weapon_rate, &lacgompmode, &vehiclefriendlyfire);
forward OnOutcomeScmEvent(playerid, issuerid, E_SCM_EVENT_TYPE:eventid, vehicleid, arg1, arg2);
forward OnServerQueryInfo(const ipaddr[], hostname[51], gamemode[31], language[31]);
forward OnSystemCommandExecute(const line_output[], retval, index, success, line_current, line_total);

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
		BS_INT, 1222,    // int - modelid
		BS_INT, 19,     // int   - type
		BS_FLOAT, X + 2.0,   // float
		BS_FLOAT, Y,     // float
		BS_FLOAT, Z);    // float
	return 1;
}

CMD:destroypickup(playerid, params[])
{
	SendRPC(playerid, 63, // rpcid
		BS_INT, strval(params));    // int - pickupid
	return 1;
}
*/
