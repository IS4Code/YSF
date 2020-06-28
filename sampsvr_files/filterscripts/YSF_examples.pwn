#include <a_samp>
#define FILTERSCRIPT
#include <zcmd>
#include <sscanf2>
#include <YSF>
#tryinclude <streamer>

#define XCOLOR_ORANGE			FF9900
#define XCOLOR_DEFAULT 			A9C4E4
#define XCOLOR_SAMP 			EE5555
#define XCOLOR_LIME 			99FF00

new
	g_szFormatString[4096], tmp[4096];

#define strcatf(%1,%3) \
    strcat(%1, (format(g_szFormatString, sizeof(g_szFormatString), %3), g_szFormatString))

#define isnull(%1) \
	((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))

public OnFilterScriptInit()
{
	ChangeRCONCommandName("gmx", ""); // disable GMX
	ChangeRCONCommandName("varlist", "vars"); // change "varlist" rcon cmd to "vars"
    SetSyncBounds(-40000.0, 40000.0, -40000.0, 40000.0);
	new Float:bound[4];
	GetSyncBounds(bound[0], bound[1], bound[2], bound[3]);
	printf("bounds: %f, %f, %f, %f", bound[0], bound[1], bound[2], bound[3]);
	
	new cmd[255];
	GetRCONCommandName("varlist", cmd);
	printf("varlist new command: %s", cmd);
	GetRCONCommandName("gmx", cmd);
	printf("gmx new command: %s", cmd);

/*
	AllowNickNameCharacter('!', true);
	AllowNickNameCharacter('"', true);
	AllowNickNameCharacter('#', true);
	AllowNickNameCharacter('&', true);
	AllowNickNameCharacter('*', true);
	AllowNickNameCharacter('+', true);
	AllowNickNameCharacter(',', true);
	AllowNickNameCharacter('-', true);
	AllowNickNameCharacter(':', true);
	AllowNickNameCharacter(';', true);
	AllowNickNameCharacter('<', true);
	AllowNickNameCharacter('>', true);
	AllowNickNameCharacter('?', true);
	AllowNickNameCharacter('{', true);
	AllowNickNameCharacter('|', true);
	AllowNickNameCharacter('}', true);
	AllowNickNameCharacter('~', true);
*/
	return 1;
}

CMD:setgravityforme(playerid, params[])
{
    new Float:fGravity;
    if(sscanf(params, "f", fGravity)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /SetGravityForMe <gravity>");
    
    SetExclusiveBroadcast(true); // every newcoming RPC will be sent only to the players specified with BroadcastToPlayer()
    BroadcastToPlayer(playerid, true); // add "playerid" to broadcast list
    SetGravity(fGravity); // set playerid gravity to "fGravity"
    BroadcastToPlayer(playerid, false); // remove "playerid" from broadcast list
    SetGravity(0.008); // this RPC won't be sent to anybody because SetExclusiveBroadcast() is still active, but broadcast list is empty
    SetExclusiveBroadcast(false); // disable 
    return 1;
}

CMD:serversettings(playerid, params[])
{
	new showplayermarkes, shownametags, stuntbonus, useplayerpedanims, bLimitchatradius, disableinteriorenterexits, nametaglos,
	    manualvehicleengine, limitplayermarkers, vehiclefriendlyfire, defaultcameracollision, Float:fGlobalchatradius, Float:fNameTagDrawDistance, Float:fPlayermarkerslimit;
	    
	GetServerSettings(showplayermarkes, shownametags, stuntbonus, useplayerpedanims, bLimitchatradius, disableinteriorenterexits, nametaglos,
		manualvehicleengine, limitplayermarkers, vehiclefriendlyfire, defaultcameracollision, fGlobalchatradius, fNameTagDrawDistance, fPlayermarkerslimit);

	new dir[128];
	GetRecordingDirectory(dir, sizeof(dir));

	SendClientMessagef(playerid, -1, "markers = %d, nametags = %d, stuntbonus = %d, playerpedanims = %d, limitcharadius = %d, disableinteriorenter = %d, nametaglos = %d", showplayermarkes, shownametags, stuntbonus, useplayerpedanims, bLimitchatradius, disableinteriorenterexits, nametaglos);
	SendClientMessagef(playerid, -1, "manuale = %d, limitmarkers = %d, vfriendlyfire = %d, dcamcol: %d, gchatr = %.2f, namedd = %.2f, pmarkerlimit = %.2f", manualvehicleengine, limitplayermarkers, vehiclefriendlyfire, defaultcameracollision, fGlobalchatradius, fNameTagDrawDistance, fPlayermarkerslimit);
	SendClientMessagef(playerid, -1, "number of timers = %d, recording directory = %s", GetRunningTimers(), dir);
	return 1;
}

CMD:setrecdir(playerid, params[])
{
    SetRecordingDirectory(params); // eg. "filterscripts", "scriptfiles", "npcmodes\recordings"
	SendClientMessagef(playerid, -1, "SetRecordingDirectory = %s", params);
	return 1;
}

CMD:ysftickrate(playerid, params[])
{
	new ticks;
	if(sscanf(params, "d", ticks)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /ysftickrate <ticks> = %d", YSF_GetTickRate());

	YSF_SetTickRate(ticks);
	SendClientMessagef(playerid, -1, "YSF_Tickrate = %d", ticks);
	return 1;
}

CMD:restarttime(playerid, params[])
{
	new Float:fTime;
	if(sscanf(params, "f", fTime)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /restarttime <time in milliseconds> = %.4f", GetModeRestartTime());

	SetModeRestartTime(fTime);
	SendClientMessagef(playerid, -1, "restart time = %.4f", fTime);
	return 1;
}

CMD:maxplayers(playerid, params[])
{
	new maxplayers;
	if(sscanf(params, "d", maxplayers)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /maxplayers <maxplayers> = %d", GetMaxPlayers());

	SetMaxPlayers(maxplayers);
	SendClientMessagef(playerid, -1, "maxplayers = %d", maxplayers);
	return 1;
}

CMD:maxnpcs(playerid, params[])
{
	new maxnpcs;
	if(sscanf(params, "d", maxnpcs)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /maxnpcs <maxnpcs> = %d", GetServerVarAsInt("maxnpc"));

	SetMaxNPCs(maxnpcs);
	SendClientMessagef(playerid, -1, "maxplayers = %d", maxnpcs);
	return 1;
}

CMD:listfs(playerid, params[])
{
	new scriptname[256];
	for(new i; i != 16; i++)
	{
		GetFilterScriptName(i, scriptname);
		if(isnull(scriptname)) continue; // If no FS

		SendClientMessagef(playerid, -1, "%d. - %s", i, scriptname);
	}
	return 1;
}

CMD:allownamechar(playerid, params[])
{
	new character, bool:allow;
	if(sscanf(params, "cd", character, bool:allow)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /allownamechar <character> <allow>");

	AllowNickNameCharacter(character, allow);
	SendClientMessagef(playerid, -1, "%c = %d", character, allow);
	return 1;
}

CMD:isnamechar(playerid, params[])
{
	new character;
	if(sscanf(params, "c", character)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /isnamechar <character>");

	SendClientMessagef(playerid, -1, "%c = %d", character, IsNickNameCharacterAllowed(character));
	return 1;
}

CMD:classinfo(playerid, params[])
{
	new classid, teamid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, weapon[3], weapon_ammo[3];
	if(sscanf(params, "d", classid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /classinfo <classid>");
	if(!(0 <= classid <= GetAvailableClasses())) return SendClientMessagef(playerid, 0xFF0000AA, "ERROR: Wrong ID! 0 - %d", GetAvailableClasses());

	GetPlayerClass(classid, teamid, modelid, fX, fY, fZ, fAngle, weapon[0], weapon_ammo[0], weapon[1], weapon_ammo[1], weapon[2], weapon_ammo[2]);
	SendClientMessagef(playerid, -1, "{FF0000}class ID: %d {FFFFFF}- teamid: %d, skinid: %d, spawn: %.4f, %.4f, %.4f, %.4f", classid, teamid, modelid, fX, fY, fZ, fAngle);
	SendClientMessagef(playerid, -1, "weapon1: %d, %d, weapon2: %d, %d, weapon3: %d, %d", weapon[0], weapon_ammo[0], weapon[1], weapon_ammo[1], weapon[2], weapon_ammo[2]);
	return 1;
}

CMD:scoreupdate(playerid, params[])
{
	new update;
	if(sscanf(params, "d", update)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /scoreupdate <update>");

    TogglePlayerScoresPingsUpdate(playerid, !!update);
	SendClientMessagef(playerid, -1, "TogglePlayerScoresPingsUpdate = %d", update);
	return 1;
}

CMD:fakeping(playerid, params[])
{
	new fakeping;
	if(sscanf(params, "d", fakeping)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /fakeping <value> (-1 = disable)");

	if(fakeping == -1)
	{
		TogglePlayerFakePing(playerid, false);
	}
	else
	{
        TogglePlayerFakePing(playerid, true);
        SetPlayerFakePing(playerid, fakeping);
	}
	SendClientMessagef(playerid, -1, "fakeping = %d", fakeping);
	return 1;
}

CMD:setnameinquery(playerid, params[])
{
	new szName[MAX_PLAYER_NAME];
	if(sscanf(params, "s[24]", szName)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /setnameinquery <szName>");

    SetPlayerNameInServerQuery(playerid, szName);
	SendClientMessagef(playerid, -1, "SetPlayerNameInServerQuery = '%s'", szName);
	return 1;
}

CMD:timeouttime(playerid, params[])
{
	new playertwo, time;
	if(sscanf(params, "ud", playertwo, time)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /timeouttime <playertwo> <time>");

	SetTimeoutTime(playertwo, time);
	SendClientMessagef(playerid, -1, "timeout %s(%d) = %d", pName(playertwo), playertwo, time);
	return 1;
}

CMD:mygrav(playerid, params[])
{
	new Float:gravity;
	if(sscanf(params, "f", gravity)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /mygrav <Float:gravity>");

    SendClientMessagef(playerid, -1, "gravity has been changed to %.3f; old was %.3f", gravity, GetPlayerGravity(playerid));
	SetPlayerGravity(playerid, gravity);
	return 1;
}

CMD:myteamforplayer(playerid, params[])
{
	new playertwo, teamid;
	if(sscanf(params, "ud", playertwo, teamid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /myteamforplayer <playertwo> <teamid>");

	SetPlayerTeamForPlayer(playertwo, playerid, teamid);
	SendClientMessagef(playerid, -1, "%s(%d) -> %s(%d) = %d", pName(playerid), playerid, pName(playertwo), playertwo, teamid);
	return 1;
}

CMD:nameforme(playerid, params[])
{
	new playertwo, szName[24];
	if(sscanf(params, "us[24]", playertwo, szName)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /nameforme <playertwo> <name>");

	SetPlayerNameForPlayer(playerid, playertwo, szName);
	return 1;
}

CMD:skinforme(playerid, params[])
{
	new playertwo, skinid;
	if(sscanf(params, "ud", playertwo, skinid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /nameforme <playertwo> <skinid>");

	SetPlayerSkinForPlayer(playerid, playertwo, skinid);
	return 1;
}

CMD:styleforme(playerid, params[])
{
	new playertwo, skinid;
	if(sscanf(params, "ud", playertwo, skinid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /nameforme <playertwo> <style>");

	SetPlayerFightStyleForPlayer(playerid, playertwo, skinid);
	return 1;
}

CMD:posforme(playerid, params[])
{
	new playertwo, bool:forcesync;
	if(sscanf(params, "uD(1)", playertwo, forcesync)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /posforme <playertwo> <forcesync = 1>");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPosForPlayer(playerid, playertwo, x + 1.0, y, z, forcesync);
	return 1;
}

CMD:quatforme(playerid, params[])
{
	new playertwo, bool:forcesync, Float:w, Float:x, Float:y, Float:z;
	if(sscanf(params, "uffffD(1)", playertwo, w, x, y, z, forcesync)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /quatforme <playertwo> <w> <x - SetPlayerFacingAngle> <y> <z> <forcesync = 1>");

	// Change X if you want to use something similar like SetPlayerFacingAngle - just you need to convert euler angles to quaternion
	SetPlayerRotationQuatForPlayer(playerid, playertwo, w, x, y, z, forcesync);
	return 1;
}

CMD:shotme(playerid, params[])
{
	new playertwo;
	if(sscanf(params, "u", playertwo)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /shotme <playertwo>");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playertwo, x, y, z);
	SendBulletData(playertwo, _, GetPlayerWeapon(playertwo), BULLET_HIT_TYPE_PLAYER, playerid, x, y, z, 0.0, 0.0, 0.0, 0.1, 0.1, 0.1);
	return 1;
}

CMD:showplayer(playerid, params[])
{
	new playertwo;
	if(sscanf(params, "u", playertwo)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /showplayer <playertwo>");

	ShowPlayerForPlayer(playerid, playertwo);
	SendClientMessagef(playerid, -1, "show player %s(%d)", pName(playertwo), playertwo);
	return 1;
}

CMD:hideplayer(playerid, params[])
{
	new playertwo;
	if(sscanf(params, "u", playertwo)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /hideplayer <playertwo>");

	HidePlayerForPlayer(playerid, playertwo);
	SendClientMessagef(playerid, -1, "show player %s(%d)", pName(playertwo), playertwo);
	return 1;
}

CMD:addplayer(playerid, params[])
{
	new playertwo, isnpc;
	if(sscanf(params, "uD(0)", playertwo, isnpc)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /addplayer <playertwo> <isnpc = 0>");

	AddPlayerForPlayer(playerid, playertwo, isnpc);
	SendClientMessagef(playerid, -1, "add player %s(%d)", pName(playertwo), playertwo);
	return 1;
}

CMD:removeplayer(playerid, params[])
{
	new playertwo;
	if(sscanf(params, "u", playertwo)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /removeplayer <playertwo>");

	RemovePlayerForPlayer(playerid, playertwo);
	SendClientMessagef(playerid, -1, "remove player %s(%d)", pName(playertwo), playertwo);
	return 1;
}

CMD:spawnforworld(playerid, params[])
{
	new playertwo;
	if(sscanf(params, "u", playertwo)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /spawnforworld <playertwo>");

	SpawnForWorld(playertwo);
	return 1;
}

CMD:broadcastdeath(playerid, params[])
{
	new playertwo;
	if(sscanf(params, "u", playertwo)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /broadcastdeath <playertwo>");

	BroadcastDeath(playertwo);
	return 1;
}

CMD:consolemsgs(playerid, params[])
{
	new enable, color;
	if(sscanf(params, "uN(0xFFFFFFAA)", enable, color)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /consolemsgs <enable> <color = 0xFFFFFFAA>");

	if(enable)
		EnableConsoleMSGsForPlayer(playerid, color);
	else
		DisableConsoleMSGsForPlayer(playerid);
		
    SendClientMessagef(playerid, 0xFF0000AA, "Console messages = %d, color: %x", enable, color);
	return 1;
}

CMD:psyncdata(playerid, params[])
{
	new player1;
	if(sscanf(params, "D(-1)", player1)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /psyncdata <playerid>");
	if(player1 == -1) player1 = playerid;

	new
	    Float:x, Float:y, Float:z, Float:qw, Float:qx, Float:qy, Float:qz, teamid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, weapon[3], weapon_ammo[3],
	    Float:fCP[4], Float:fRaceCP[7], Float:fBounds[4], keys, ud, lr, weaponid = GetPlayerWeapon(playerid), weaponname[45], inqueryname[MAX_PLAYER_NAME];

	GetPlayerSurfingOffsets(player1, x, y, z);
	GetPlayerRotationQuat(player1, qw, qx, qy, qz);
	GetSpawnInfo(player1, teamid, modelid, fX, fY, fZ, fAngle, weapon[0], weapon_ammo[0], weapon[1], weapon_ammo[1], weapon[2], weapon_ammo[2]);
	GetPlayerCheckpoint(player1, fCP[0], fCP[1], fCP[2], fCP[3]);
	GetPlayerRaceCheckpoint(player1, fRaceCP[0], fRaceCP[1], fRaceCP[2], fRaceCP[3], fRaceCP[4], fRaceCP[5], fRaceCP[6]);
	GetPlayerWorldBounds(player1, fBounds[0], fBounds[1], fBounds[2], fBounds[3]);
	GetPlayerDisabledKeysSync(player1, keys, ud, lr);
	GetWeaponName(weaponid, weaponname, sizeof(weaponname));
    GetPlayerNameInServerQuery(player1, inqueryname, sizeof(inqueryname));
    
    tmp[0] = EOS;
	strcatf(tmp, "siren: %d, gear: %d, reactor: %d, trainspeed: %f, zaim: %f, surf: %f, %f, %f\n", GetPlayerSirenState(player1), GetPlayerLandingGearState(player1), GetPlayerHydraReactorAngle(player1), GetPlayerTrainSpeed(player1), GetPlayerZAim(player1), x, y, z);
	strcatf(tmp, "server gravity: %.3f, player gravity: %.3f, quat: %f, %f, %f, %f, server weather: %d, player weather: %d, inmodshop: %d, cameratarget: %d\n", GetGravity(), GetPlayerGravity(player1), qw, qx, qy, qz, GetWeather(), GetPlayerWeather(player1), IsPlayerInModShop(player1), IsPlayerCameraTargetEnabled(player1));
	strcatf(tmp, "teamid: %d, skinid: %d, spawn: %.4f, %.4f, %.4f, %.4f, spawned: %d\n", teamid, modelid, fX, fY, fZ, fAngle, IsPlayerSpawned(player1));
	strcatf(tmp, "weapon1: %d, %d, weapon2: %d, %d, weapon3: %d, %d\n", weapon[0], weapon_ammo[0], weapon[1], weapon_ammo[1], weapon[2], weapon_ammo[2]);
	strcatf(tmp, "specid: %d, spectype: %d, dialogid: %d\n", GetPlayerSpectateID(player1), GetPlayerSpectateType(player1), GetPlayerDialogID(player1));
	strcatf(tmp, "checkpoint %d: %f, %f, %f, size: %f\n", IsPlayerCheckpointActive(player1), fCP[0], fCP[1], fCP[2], fCP[3]);
	strcatf(tmp, "race checkpoint %d: %f, %f, %f, next: %f, %f, %f, size: %f\n", IsPlayerRaceCheckpointActive(player1), fRaceCP[0], fRaceCP[1], fRaceCP[2], fRaceCP[3], fRaceCP[4], fRaceCP[5], fRaceCP[6]);
	strcatf(tmp, "worldbounds: %.2f, %.2f, %.2f, %.2f, widescreen: %d, surfplayerobject: %d\n", fBounds[0], fBounds[1], fBounds[2], fBounds[3], IsPlayerWidescreenToggled(player1), GetPlayerSurfingPlayerObjectID(player1));
	strcatf(tmp, "last synced - vehicle: %d, trailer: %d, pause: %d, controllable: %d, consolemessages: %d\n", GetPlayerLastSyncedVehicleID(player1), GetPlayerLastSyncedTrailerID(player1), GetPlayerPausedTime(player1), IsPlayerControllable(player1), HasPlayerConsoleMessages(player1));
	strcatf(tmp, "disabledkeys - keys: %d, ud: %d, lr: %d, inquery: '%s', current weapon: %s (%d) - slot: %d\n", keys, ud, lr, inqueryname, weaponname, weaponid, GetWeaponSlot(weaponid));

	new str[64];
	format(str, sizeof(str), "Info - YSF - {%06x}%s(%d)", GetPlayerColor(player1) >>> 8, pName(player1), player1);
	ShowPlayerDialog(playerid, 5000, DIALOG_STYLE_MSGBOX, str, tmp, "Ok", "");
	return 1;
}

CMD:actorinfo(playerid, params[])
{
	new actorid, skinid, Float:x, Float:y, Float:z, Float:a, animlib[32], animname[32], Float:fDelta, loop, lockx, locky, freeze, time;
	if(sscanf(params, "u", actorid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /actorinfo <actorid>");
	if(!IsValidActor(actorid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid actor ID");

	GetActorSpawnInfo(actorid, skinid, x, y, z, a);
	GetActorAnimation(actorid, animlib, _, animname, _, fDelta, loop, lockx, locky, freeze, time);
	
	SendClientMessagef(playerid, -1, "skinid: %d, pos: %.4f, %.4f, %.4f, %.4f", skinid, x, y, z, a);
	SendClientMessagef(playerid, -1, "anim: %s/%s, fDelta: %.4f, loop: %d, lockx: %d, locky: %d, freeze: %d, time: %d", animlib, animname, fDelta, loop, lockx, locky, freeze, time);
	return 1;
}

CMD:setplayerver(playerid, params[])
{
	SetPlayerVersion(playerid, params);
	return 1;
}

CMD:disablekey(playerid, params[])
{
	new keys, ud, lr;
	SetPlayerDisabledKeysSync(playerid, strval(params), 1, 1);
	GetPlayerDisabledKeysSync(playerid, keys, ud, lr);
	SendClientMessagef(playerid, -1, "disabled - keys: %d, ud: %d, lr: %d", keys, ud, lr);
	return 1;
}

CMD:vinfo(playerid, params[])
{
	new vehicleid;
    if(sscanf(params, "D(-1)", vehicleid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /vdata <vehicleid>");

    if(vehicleid == -1) vehicleid = GetPlayerVehicleID(playerid);
    if(!vehicleid) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: You're not in vehicle!");

	new modelid, scolor1, scolor2, color1, color2, Float:X, Float:Y, Float:Z, Float:A, plate[33], respawn, Float:CurrX, Float:CurrY, Float:CurrZ;

    modelid = GetVehicleModel(vehicleid);
    GetVehicleSpawnInfo(vehicleid, X, Y, Z, A, scolor1, scolor2);
    GetVehiclePos(vehicleid, CurrX, CurrY, CurrZ);
	GetVehicleColor(vehicleid, color1, color2);
	GetVehicleNumberPlate(vehicleid, plate, sizeof(plate));
	
	SendClientMessagef(playerid, 0xFF0000AA, "--- {33FF33}vehicleid: %d{FFFFFF}, {33FF33}modelid: %d {FF0000}---", vehicleid, modelid);
	SendClientMessagef(playerid, -1, "color: %d, %d; pj: %d; interior: %d; respawn: %d - %f, %f, %f, %f, spawn color: %d, %d", color1, color2, GetVehiclePaintjob(vehicleid), GetVehicleInterior(vehicleid), GetVehicleRespawnDelay(vehicleid), X, Y, Z, A, scolor1, scolor2);
	SendClientMessagef(playerid, -1, "current pos: %f, %f, %f", CurrX, CurrY, CurrZ);

	respawn = GetTickCount() - GetVehicleRespawnTick(vehicleid);
	SendClientMessagef(playerid, -1, "occupiedtick: %d, respawntick: %d, lastdriver: %d, plate: %s", GetVehicleOccupiedTick(vehicleid), respawn, GetVehicleLastDriver(vehicleid), plate);
	SendClientMessagef(playerid, -1, "hasbeenoccupied: %d, occupied: %d, dead: %d, trailer: %d, cab: %d", HasVehicleBeenOccupied(vehicleid), IsVehicleOccupied(vehicleid), IsVehicleDead(vehicleid), GetVehicleTrailer(vehicleid), GetVehicleCab(vehicleid));
	SendClientMessagef(playerid, -1, "models: %d, used models: %d/212", GetVehicleModelCount(modelid), GetVehicleModelsUsed());
	return 1;
}

CMD:vspawninfo(playerid, params[])
{
	new vehicleid, Float:X, Float:Y, Float:Z;
    if(sscanf(params, "d", vehicleid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /vspawninfo <vehicleid>");

	GetPlayerPos(playerid, X, Y, Z);
	SetVehicleSpawnInfo(vehicleid, GetVehicleModel(vehicleid), X, Y, Z, 0.0, random(255), random(255));
	return 1;
}

CMD:tdinfo(playerid, params[])
{
	new id;
	if(!strcmp(params, "all"))
	{
		for(new i; i != 2048; i++)
		{
			if(!IsTextDrawVisibleForPlayer(playerid, Text:i)) continue;

			TextDrawGetString(Text:i, tmp, 128);
			SendClientMessagef(playerid, -1, "%d - %s", i, tmp);
		}
		return 1;
	}

	if(!sscanf(params, "d", id))
	{
		if(!IsValidTextDraw(Text:id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid TextDraw id!");

		new str[114], Float:pos[2], Float:lettersize[2], Float:fontsize[2], Float:rot[3], Float:zoom, color[2];

		TextDrawGetString(Text:id, str, sizeof(str));
		TextDrawGetPos(Text:id, pos[0], pos[1]);
		TextDrawGetLetterSize(Text:id, lettersize[0], lettersize[1]);
		TextDrawGetTextSize(Text:id, fontsize[0], fontsize[1]);

		TextDrawGetPreviewRot(Text:id, rot[0], rot[1], rot[2], zoom);
		TextDrawGetPreviewVehCol(Text:id, color[0], color[1]);

		SendClientMessagef(playerid, 0x33FF33AA, "TextDrawID: {FF0000}%d, {33FF33}str: %s", id, str);
		SendClientMessagef(playerid, -1, "pos: %.2f, %.2f - lettersize: %.2f, %.2f - fontsize: %.2f, %.2f", pos[0], pos[1], lettersize[0], lettersize[1], fontsize[0], fontsize[1]);
		SendClientMessagef(playerid, -1, "color: %x, boxcolor: %x, backgroundcolor: %x, shadow: %d, outline: %d, font: %d", TextDrawGetColor(Text:id), TextDrawGetBoxColor(Text:id), TextDrawGetBackgroundColor(Text:id), TextDrawGetShadow(Text:id), TextDrawGetOutline(Text:id), TextDrawGetFont(Text:id));
		SendClientMessagef(playerid, -1, "box: %d, proportional: %d, alignment: %d, selectable: %d", TextDrawIsBox(Text:id), TextDrawIsProportional(Text:id), TextDrawGetAlignment(Text:id), TextDrawIsSelectable(Text:id));
		SendClientMessagef(playerid, -1, "prevmodel: %d, rot: %.2f, %.2f, %.2f - zoom: %.2f - color: %d, %d", TextDrawGetPreviewModel(Text:id), rot[0], rot[1], rot[2], zoom, color[0], color[1]);
		return 1;
	}

	SendClientMessage(playerid, 0xFF0000AA, "USAGE: /tdinfo <all/Textdraw ID here>");
	return 1;
}

CMD:tdpos(playerid, params[])
{
	new id, Float:fX, Float:fY;
	if(sscanf(params, "dff", id, fX, fY)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /tdpos <TextDrawID> <fX <fY>");
	if(!IsValidTextDraw(Text:id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid TextDraw id!");

	TextDrawSetPos(Text:id, fX, fY);
	TextDrawShowForAll(Text:id);
	return 1;
}

CMD:bubble(playerid, params[]) // illustrate how SendRPC works. this is equal to SetPlayerChatBubbleForPlayer
{
	SendRPC(playerid, 0x3B, // rpcid
		BS_UNSIGNEDSHORT, strval(params), // at player, where you want to create bubble
		BS_INT,	0x33FF33AA, // color
		BS_FLOAT, 50.0, // drawdistance
		BS_INT,	4325,   // expiretime
		BS_UNSIGNEDCHAR, 3, // string lenght
		BS_STRING, "asd"); // string
	return 1;
}


CMD:attachobject(playerid, params[])
{
	new objectid, attachplayer;
	if(sscanf(params, "dd", objectid, attachplayer)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /attachobject <objectid> <attachplayer>");

	AttachObjectToPlayer(objectid, attachplayer, 0.0, 0.1, 0.2, 1.0, 1.0, 1.0);
	return 1;
}

CMD:attachpobject(playerid, params[])
{
	new objectplayer, objectid, attachplayer, wait;
	if(sscanf(params, "dddD(0)", objectplayer, objectid, attachplayer, wait)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /attachpobject <objectplayer> <objectid> <attachplayer> <wait = 0>");

	AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, 0.0, 0.1, 0.2, 1.0, 1.0, 1.0);
	return 1;
}

#if defined STREAMER_TYPE_OBJECT
CMD:attachdynamic(playerid, params[])
{
	new player1, Float:X, Float:Y, Float:Z;
	if(sscanf(params, "d", player1)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /attachdynamic <player1>");

	new obj = CreateDynamicObject(1224, X, Y, Z, 1.0, 2.0, 3.0);
	AttachDynamicObjectToPlayer(obj, player1, 0.1, 0.7, 2.0, 30, 40, 50);
	return 1;
}
#endif

CMD:attachedobj(playerid, params[])
{
	new slot;
	if(sscanf(params, "n", slot)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /attachedobj <slot>");
	if(!(0 <= slot <= MAX_PLAYER_ATTACHED_OBJECTS)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid slot id! 0 - " #MAX_PLAYER_ATTACHED_OBJECTS "");

	new modelid, bone, Float:offset[3], Float:rotation[3], Float:scale[3], material1, material2;
	GetPlayerAttachedObject(playerid, slot, modelid, bone, offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2], scale[0], scale[1], scale[2], material1, material2);

	SendClientMessagef(playerid, -1, "modelid: %d, bone: %d", modelid, bone);
	SendClientMessagef(playerid, -1, "offset: %.4f, %.4f, %.4f, {33FF33}rotation: %.4f, %.4f, %.4f", offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2]);
	SendClientMessagef(playerid, -1, "scale: %.4f, %.4f, %.4f, materialcolor: %x, %x", scale[0], scale[1], scale[2], material1, material2);
	return 1;
}

CMD:attachobjforplayer(playerid, params[])
{
	new forplayerid, slot;
	if(sscanf(params, "dn", forplayerid, slot)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /attachedobj <forplayerid> <slot>");
	if(!(0 <= slot <= MAX_PLAYER_ATTACHED_OBJECTS)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid slot id! 0 - " #MAX_PLAYER_ATTACHED_OBJECTS "");

	new modelid = 1224, bone = 2, Float:offset[3], Float:rotation[3], Float:scale[3], material1, material2;
	//SetPlayerAttachedObjForPlayer(playerid, forplayerid, slot, modelid, bone, offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2], scale[0], scale[1], scale[2], material1, material2);
	//GetPlayerAttachedObjForPlayer(forplayerid, playerid, slot, modelid, bone, offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2], scale[0], scale[1], scale[2], material1, material2);

	SendClientMessagef(playerid, -1, "forplayerid: %d, modelid: %d, bone: %d", forplayerid, modelid, bone);
	SendClientMessagef(playerid, -1, "offset: %.4f, %.4f, %.4f, {33FF33}rotation: %.4f, %.4f, %.4f", offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2]);
	SendClientMessagef(playerid, -1, "scale: %.4f, %.4f, %.4f, materialcolor: %x, %x", scale[0], scale[1], scale[2], material1, material2);
	return 1;
}

CMD:objectinfo(playerid, params[])
{
	new objectid;
	if(sscanf(params, "n", objectid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /objectinfo <Object ID>");
	if(!IsValidObject(objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid object id!");

	new attached_vehicle, attached_object, attached_playerid, Float:offset[3], Float:rotation[3], Float:target[3];
	GetObjectAttachedData(objectid, attached_vehicle, attached_object, attached_playerid);
	GetObjectAttachedOffset(objectid, offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2]);
	GetObjectTarget(objectid, target[0], target[1], target[2]);

	SendClientMessagef(playerid, -1, "objectid: %d, model: %d, nocameracol: %d, drawdistance: %.4f, attached - vehicle: %d, object: %d, player: %d", objectid, GetObjectModel(objectid), IsObjectNoCameraCol(objectid), GetObjectDrawDistance(objectid), attached_vehicle, attached_object, attached_playerid);

	// If moving..
	if(IsObjectMoving(objectid))
		SendClientMessagef(playerid, -1, "movespeed: %.4f, target: %.4f, %.4f, %.4f", GetObjectMoveSpeed(objectid), target[0], target[1], target[2]);

	// If attached to something..
	if(attached_vehicle != 0xFFFF || attached_object != 0xFFFF || attached_playerid != 0xFFFF)
		SendClientMessagef(playerid, -1, "attached offsets: %.4f, %.4f, %.4f, rot: %.4f, %.4f, %.4f", offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2]);
	return 1;
}

CMD:pobjectinfo(playerid, params[])
{
	new objectid;
	if(sscanf(params, "n", objectid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /objectinfo <Object ID>");
	if(!IsValidPlayerObject(playerid, objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid object id!");

	new attached_vehicle, attached_object, attached_player, Float:offset[3], Float:rotation[3], Float:target[3];
	GetPlayerObjectAttachedData(playerid, objectid, attached_vehicle, attached_object, attached_player);
	GetPlayerObjectAttachedOffset(playerid, objectid, offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2]);
	GetPlayerObjectTarget(playerid, objectid, target[0], target[1], target[2]);

	SendClientMessagef(playerid, -1, "player objectid: %d, model: %d, nocameracol: %d, drawdistance: %.4f, attached - vehicle: %d, object: %d, player: %d", objectid, GetPlayerObjectModel(playerid, objectid), IsPlayerObjectNoCameraCol(playerid, objectid), GetPlayerObjectDrawDistance(playerid, objectid), attached_vehicle, attached_object, attached_player);

	// If moving..
	if(IsPlayerObjectMoving(playerid, objectid))
		SendClientMessagef(playerid, -1, "movespeed: %.4f, target: %.4f, %.4f, %.4f", GetObjectMoveSpeed(objectid), target[0], target[1], target[2]);

	// If attached to something..
	if(attached_vehicle != 0xFFFF || attached_object != 0xFFFF || attached_player != 0xFFFF)
		SendClientMessagef(playerid, -1, "attached offsets: %.4f, %.4f, %.4f, rot: %.4f, %.4f, %.4f", offset[0], offset[1], offset[2], rotation[0], rotation[1], rotation[2]);
	return 1;
}

CMD:objecttype(playerid, params[])
{
	new objectid;
	if(sscanf(params, "n", objectid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /objecttype <Object ID>");

	SendClientMessagef(playerid, -1, "objectid: %d, type: %d", objectid, GetObjectType(playerid, objectid));
	return 1;
}

CMD:objmovespeed(playerid, params[])
{
	new objectid, Float:fSpeed;
	if(sscanf(params, "nf", objectid, fSpeed)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /objmovespeed <Object ID> <fSpeed>");
	if(!IsValidObject(objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid object id!");

	SetObjectMoveSpeed(objectid, fSpeed);
	return 1;
}

CMD:ptdinfo(playerid, params[])
{
	new id;
	if(!strcmp(params, "all"))
	{
		for(new i; i != 2048; i++)
		{
			if(!IsPlayerTextDrawVisible(playerid, PlayerText:i)) continue;

			PlayerTextDrawGetString(playerid, PlayerText:i, tmp, 128);
			SendClientMessagef(playerid, -1, "%d - %s", i, tmp);
		}
		return 1;
	}

	if(!sscanf(params, "d", id))
	{
		if(!IsValidPlayerTextDraw(playerid, PlayerText:id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid TextDraw id!");

		new str[114], Float:pos[2], Float:lettersize[2], Float:fontsize[2], Float:rot[3], Float:zoom, color[2];

		PlayerTextDrawGetString(playerid, PlayerText:id, str, sizeof(str));
		PlayerTextDrawGetPos(playerid, PlayerText:id, pos[0], pos[1]);
		PlayerTextDrawGetLetterSize(playerid, PlayerText:id, lettersize[0], lettersize[1]);
		PlayerTextDrawGetTextSize(playerid, PlayerText:id, fontsize[0], fontsize[1]);

		PlayerTextDrawGetPreviewRot(playerid, PlayerText:id, rot[0], rot[1], rot[2], zoom);
		PlayerTextDrawGetPreviewVehCol(playerid, PlayerText:id, color[0], color[1]);

		SendClientMessagef(playerid, 0x33FF33AA, "TextDrawID: {FF0000}%d, {33FF33}str: %s", id, str);
		SendClientMessagef(playerid, -1, "pos: %.2f, %.2f - lettersize: %.2f, %.2f - fontsize: %.2f, %.2f", pos[0], pos[1], lettersize[0], lettersize[1], fontsize[0], fontsize[1]);
		SendClientMessagef(playerid, -1, "color: %x, boxcolor: %x, backgroundcolor: %x, shadow: %d, outline: %d, font: %d", PlayerTextDrawGetColor(playerid, PlayerText:id), PlayerTextDrawGetBoxColor(playerid, PlayerText:id), PlayerTextDrawGetBackgroundCol(playerid, PlayerText:id), PlayerTextDrawGetShadow(playerid, PlayerText:id), PlayerTextDrawGetOutline(playerid, PlayerText:id), PlayerTextDrawGetFont(playerid, PlayerText:id));
		SendClientMessagef(playerid, -1, "box: %d, proportional: %d, alignment: %d, selectable: %d", PlayerTextDrawIsBox(playerid, PlayerText:id), PlayerTextDrawIsProportional(playerid, PlayerText:id), PlayerTextDrawGetAlignment(playerid, PlayerText:id), PlayerTextDrawIsSelectable(playerid, PlayerText:id));
		SendClientMessagef(playerid, -1, "prevmodel: %d, rot: %.2f, %.2f, %.2f - zoom: %.2f - color: %d, %d", PlayerTextDrawGetPreviewModel(playerid, PlayerText:id), rot[0], rot[1], rot[2], zoom, color[0], color[1]);
		return 1;
	}

	SendClientMessage(playerid, 0xFF0000AA, "USAGE: /ptdinfo <all/Textdraw ID here>");
	return 1;
}

CMD:ptdpos(playerid, params[])
{
	new id, Float:fX, Float:fY;
	if(sscanf(params, "dff", id, fX, fY)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /ptdpos <TextDrawID> <fX <fY>");
	if(!IsValidPlayerTextDraw(playerid, PlayerText:id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid TextDraw id!");

	PlayerTextDrawSetPos(playerid, PlayerText:id, fX, fY);
	PlayerTextDrawShow(playerid, PlayerText:id);
	return 1;
}

CMD:ptdcolor(playerid, params[])
{
	new id, color;
	if(sscanf(params, "dn", id, color)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /ptdcolor <TextDrawID> <color>");
	if(!IsValidPlayerTextDraw(playerid, PlayerText:id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid TextDraw id!");

	SendClientMessagef(playerid, -1, "oldcolor: %x, newcolor: %x", PlayerTextDrawGetColor(playerid, PlayerText:id), color);

	PlayerTextDrawColor(playerid, PlayerText:id, color);
	PlayerTextDrawShow(playerid, PlayerText:id);
	return 1;
}

CMD:ismaterial(playerid, params[])
{
	new objectid, materialindex;
	if(sscanf(params, "dd", objectid, materialindex)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /ismaterial <ObjectID> <materialindex>");
	if(!IsValidObject(objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid Object id!");

    SendClientMessagef(playerid, -1, "used: %d", IsObjectMaterialSlotUsed(objectid, materialindex));
	return 1;
}

CMD:getmaterial(playerid, params[])
{
	new objectid, materialindex;
	if(sscanf(params, "dd", objectid, materialindex)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /getmaterial <ObjectID> <materialindex>");
	if(!IsValidObject(objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid Object id!");

	new modelid, txdname[64], texturename[64], materialcolor;

	GetObjectMaterial(objectid, materialindex, modelid, txdname, sizeof(txdname), texturename, sizeof(texturename), materialcolor);
	SendClientMessagef(playerid, -1, "modelid: %d, txdname: %s, texturename: %s, color: %x", modelid, txdname, texturename, materialcolor);
	return 1;
}

CMD:getpmaterial(playerid, params[])
{
	new objectid, materialindex;
	if(sscanf(params, "dd", objectid, materialindex)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /getpmaterial <ObjectID> <materialindex>");
	if(!IsValidPlayerObject(playerid, objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid Object id!");

	new modelid, txdname[64], texturename[64], materialcolor;

	GetPlayerObjectMaterial(playerid, objectid, materialindex, modelid, txdname, sizeof(txdname), texturename, sizeof(texturename), materialcolor);
	SendClientMessagef(playerid, -1, "modelid: %d, txdname: %s, texturename: %s, color: %x", modelid, txdname, texturename, materialcolor);
	return 1;
}

CMD:getmaterialtext(playerid, params[])
{
	new objectid, materialindex, text[144], materialsize, fontface[32], fontsize, bold, fontcolor, backcolor, textalignment;
	if(sscanf(params, "dd", objectid, materialindex)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /getmaterialtext <ObjectID> <materialindex>");
	if(!IsValidObject(objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid Object id!");

	GetObjectMaterialText(objectid, materialindex, text, sizeof(text), materialsize, fontface, sizeof(fontface), fontsize, bold, fontcolor, backcolor, textalignment);

    SendClientMessagef(playerid, -1, "text: %s", text);
	SendClientMessagef(playerid, -1, "materialsize: %d, fontface: %s, fontsize: %d, bold: %d, fontcolor: %x, backcolor: %x, alignment: %d", materialsize, fontface, fontsize, bold, fontcolor, backcolor, textalignment);
	return 1;
}

CMD:getpmaterialtext(playerid, params[])
{
	new objectid, materialindex, text[144], materialsize, fontface[32], fontsize, bold, fontcolor, backcolor, textalignment;
	if(sscanf(params, "dd", objectid, materialindex)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /getpmaterialtext <ObjectID> <materialindex>");
	if(!IsValidPlayerObject(playerid, objectid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid Object id!");

	GetPlayerObjectMaterialText(playerid, objectid, materialindex, text, sizeof(text), materialsize, fontface, sizeof(fontface), fontsize, bold, fontcolor, backcolor, textalignment);

    SendClientMessagef(playerid, -1, "text: %s", text);
	SendClientMessagef(playerid, -1, "materialsize: %d, fontface: %s, fontsize: %d, bold: %d, fontcolor: %x, backcolor: %x, alignment: %d", materialsize, fontface, fontsize, bold, fontcolor, backcolor, textalignment);
	return 1;
}

CMD:gzinfo(playerid, params[])
{
	new id;
	if(!strcmp(params, "all"))
	{
		for(new i; i != 1024; i++)
		{
			if(!IsGangZoneVisibleForPlayer(playerid, i)) continue;

			SendClientMessagef(playerid, -1, "%d", i);
		}
		return 1;
	}

	if(!sscanf(params, "d", id))
	{
		if(!IsValidGangZone(id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid GangZone ID!");

		new Float:fPos[4];
		GangZoneGetPos(id, fPos[0], fPos[1], fPos[2], fPos[3]);

		SendClientMessagef(playerid, 0x33FF33AA, "GangZoneID: {FF0000}%d, {33FF33}visible: %d - %x", id, IsGangZoneVisibleForPlayer(playerid, id), GangZoneGetColorForPlayer(playerid, id));
		SendClientMessagef(playerid, -1, "pos: %.4f, %.4f, %.4f, %.4f", fPos[0], fPos[1], fPos[2], fPos[3]);
		SendClientMessagef(playerid, -1, "isflashing: %d, flashcolor: %x", IsGangZoneFlashingForPlayer(playerid, id), GangZoneGetFlashColorForPlayer(playerid, id));
		return 1;
	}

	SendClientMessage(playerid, 0xFF0000AA, "USAGE: /gzinfo <all/Textdraw ID here>");
	return 1;
}

CMD:pgzinfo(playerid, params[])
{
	new id;
	if(!strcmp(params, "all"))
	{
		for(new i; i != 1024; i++)
		{
			if(!IsPlayerGangZoneVisible(playerid, i)) continue;

			SendClientMessagef(playerid, -1, "%d", i);
		}
		return 1;
	}

	if(!sscanf(params, "d", id))
	{
		if(!IsValidPlayerGangZone(playerid, id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid GangZone ID!");

		new Float:fPos[4];
		PlayerGangZoneGetPos(playerid, id, fPos[0], fPos[1], fPos[2], fPos[3]);

		SendClientMessagef(playerid, 0x33FF33AA, "GangZoneID: {FF0000}%d, {33FF33}visible: %d - %x", id, IsPlayerGangZoneVisible(playerid, id), PlayerGangZoneGetColor(playerid, id));
		SendClientMessagef(playerid, -1, "pos: %.4f, %.4f, %.4f, %.4f", fPos[0], fPos[1], fPos[2], fPos[3]);
		SendClientMessagef(playerid, -1, "isflashing: %d, flashcolor: %x", IsPlayerGangZoneFlashing(playerid, id), PlayerGangZoneGetFlashColor(playerid, id));
		return 1;
	}

	SendClientMessage(playerid, 0xFF0000AA, "USAGE: /pgzinfo <all/Textdraw ID here>");
	return 1;
}

CMD:3dtextinfo(playerid, params[])
{
	if(!strcmp(params, "all"))
	{
		for(new i; i != 1024; i++)
		{
			if(!IsValid3DTextLabel(Text3D:i)) continue;

			SendClientMessagef(playerid, -1, "%d", i);
		}
		return 1;
	}
	
	new Text3D:id, text[144], color, Float:fPos[3], attached_player, attached_vehicle;
	if(sscanf(params, "d", _:id)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /3dtextinfo <id>");
	if(!IsValid3DTextLabel(id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid ID!");

	Get3DTextLabelText(id, text);
	color = Get3DTextLabelColor(id);
	Get3DTextLabelPos(id, fPos[0], fPos[1], fPos[2]);
	Get3DTextLabelAttachedData(id, attached_player, attached_vehicle);

	SendClientMessagef(playerid, color, "text: %s", text);
	SendClientMessagef(playerid, color, "color: %x, pos: %.4f, %.4f, %.4f, drawdistance: %.4f, los: %d, world: %d, attached: %d, %d, streamed: %d", color, fPos[0], fPos[1], fPos[2], Get3DTextLabelDrawDistance(id), Get3DTextLabelLOS(id), Get3DTextLabelVirtualWorld(id), attached_player, attached_vehicle, Is3DTextLabelStreamedIn(playerid, id));
	return 1;
}

CMD:p3dtextinfo(playerid, params[])
{
	if(!strcmp(params, "all"))
	{
		for(new i; i != 1024; i++)
		{
			if(!IsValidPlayer3DTextLabel(playerid, PlayerText3D:i)) continue;

			SendClientMessagef(playerid, -1, "%d", i);
		}
		return 1;
	}

	new PlayerText3D:id, text[144], color, Float:fPos[3], attached_player, attached_vehicle;
	if(sscanf(params, "d", _:id)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /p3dtextinfo <id>");
	if(!IsValidPlayer3DTextLabel(playerid, id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid ID!");

	GetPlayer3DTextLabelText(playerid, id, text);
	color = GetPlayer3DTextLabelColor(playerid, id);
	GetPlayer3DTextLabelPos(playerid, id, fPos[0], fPos[1], fPos[2]);
	GetPlayer3DTextLabelAttached(playerid, id, attached_player, attached_vehicle);

	SendClientMessagef(playerid, color, "text: %s", text);
	SendClientMessagef(playerid, color, "color: %x, pos: %.4f, %.4f, %.4f, drawdistance: %.4f, los: %d, world: %d, attached: %d, %d, streamed: %d", color, fPos[0], fPos[1], fPos[2], GetPlayer3DTextLabelDrawDist(playerid, id), GetPlayer3DTextLabelLOS(playerid, id), GetPlayer3DTextLabelVirtualW(playerid, id), attached_player, attached_vehicle, Is3DTextLabelStreamedIn(playerid, Text3D:id));
	return 1;
}
/*
CMD:ppickup(playerid, params[])
{
	new modelid, type, Float:fPos[3];
	if(sscanf(params, "dd", modelid, type)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /ppickup <modelid> <type>");

    GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]);
	SendClientMessagef(playerid, -1, "id: %d", CreatePlayerPickup(playerid, modelid, type, fPos[0] + 2.0, fPos[1], fPos[2]));
	return 1;
}

CMD:dppickup(playerid, params[])
{
	new pickupid;
	if(sscanf(params, "d", pickupid)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /dppickup <pickupid>");
	if(!IsValidPlayerPickup(playerid, pickupid)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid ID!");

	SendClientMessagef(playerid, -1, "id: %d", DestroyPlayerPickup(playerid, pickupid));
	return 1;
}
*/
CMD:pickupinfo(playerid, params[])
{
	new id, Float:fPos[3];
	if(!strcmp(params, "all"))
	{
		for( ; id != MAX_PICKUPS; id++)
		{
			if(!IsPickupStreamedIn(playerid, id)) continue;

			GetPickupPos(id, fPos[0], fPos[1], fPos[2]);
			SendClientMessagef(playerid, -1, "id: %d, model: %d, type: %d, pos: %.4f, %.4f, %.4f, world: %d, streamed: %d", id, GetPickupModel(id), GetPickupType(id), fPos[0], fPos[1], fPos[2], GetPickupVirtualWorld(id), IsPickupStreamedIn(playerid, id));
		}
		return 1;
	}

	if(sscanf(params, "d", id)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /pickupinfo <id>");
	if(!IsValidPickup(id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid ID!");

	GetPickupPos(id, fPos[0], fPos[1], fPos[2]);
	SendClientMessagef(playerid, -1, "model: %d, type: %d, pos: %.4f, %.4f, %.4f, world: %d, streamed: %d", GetPickupModel(id), GetPickupType(id), fPos[0], fPos[1], fPos[2], GetPickupVirtualWorld(id), IsPickupStreamedIn(playerid, id));
	return 1;
}
/*
CMD:ppickupinfo(playerid, params[])
{
	new id, Float:fPos[3];
	if(!strcmp(params, "all"))
	{
		for( ; id != MAX_PICKUPS; id++)
		{
			if(!IsPlayerPickupStreamedIn(playerid, id)) continue;

			GetPlayerPickupPos(playerid, id, fPos[0], fPos[1], fPos[2]);
			SendClientMessagef(playerid, -1, "id: %d, model: %d, type: %d, pos: %.4f, %.4f, %.4f, world: %d, streamed: %d", id, GetPickupModel(id), GetPickupType(id), fPos[0], fPos[1], fPos[2], GetPickupVirtualWorld(id), IsPickupStreamedIn(playerid, id));
		}
		return 1;
	}

	if(sscanf(params, "d", id)) return SendClientMessage(playerid, 0xFF0000AA, "USAGE: /pickupinfo <id>");
	if(!IsValidPlayerPickup(playerid, id)) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid ID!");

	GetPlayerPickupPos(playerid, id, fPos[0], fPos[1], fPos[2]);
	SendClientMessagef(playerid, -1, "model: %d, type: %d, pos: %.4f, %.4f, %.4f, world: %d, streamed: %d", GetPlayerPickupModel(playerid, id), GetPlayerPickupType(playerid, id), fPos[0], fPos[1], fPos[2], GetPlayerPickupVirtualWorld(playerid, id), IsPlayerPickupStreamedIn(playerid, id));
	return 1;
}
*/
CMD:modelinfo(playerid, params[])
{
	new id, Float:fOffset[3];
	if(sscanf(params, "d", id)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /modelinfo <id> (max: %d)", GetColCount());

	GetColSphereOffset(id, fOffset[0], fOffset[1], fOffset[2]);
	SendClientMessagef(playerid, -1, "col sphere: %.4f, offsets: %.4f, %.4f, %.4f", GetColSphereRadius(id), fOffset[0], fOffset[1], fOffset[2]);
	return 1;
}

CMD:formatmsg(playerid, params[])
{
    SendClientMessagef(playerid, 0x33FF33AA, "Kurva %s %d %.3f", "anyád", -1547, 2654.6542);
    SendClientMessageToAllf(0x33FF33AA, "lofasz a %s", "füledbe");

    GameTextForPlayerf(playerid, 3000, 3, "Kis %s", "kecske");
    GameTextForAllf(3000, 3, "Kis %s", "kecske");

    SendPlayerMessageToPlayerf(playerid, playerid, "megbasztam %s %s hátán", "apádat", "anyád");
    SendPlayerMessageToAllf(playerid, "megbasztam %s %s hátán", "apádat", "anyád");
    return 1;
}

CMD:netstats(playerid, params[])
{
    SendClientMessagef(playerid, 0x33FF33AA, "ConTime: %d, ConStatus: %d", NetStats_GetConnectedTime(playerid), NetStats_ConnectionStatus(playerid));
    return 1;
}

CMD:ysf_netstats(playerid, params[])
{
    SendClientMessagef(playerid, -1, "Extended net stats: %d", YSF_IsExtendedNetStatsEnabled());
    
    YSF_SetExtendedNetStatsEnabled(!YSF_IsExtendedNetStatsEnabled());
    return 1;
}

CMD:isbannedip(playerid, params[])
{
    SendClientMessagef(playerid, -1, "IsBanned(%s): %d", params, IsBanned(params));
    return 1;
}

CMD:exec(playerid, params[])
{
	new command[128], saveoutput;
	if(sscanf(params, "s[128]d", command, saveoutput)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /exec <command> <save output (0/1)>");
    execute(command, saveoutput, playerid);
    SendClientMessagef(playerid, -1, "Command sent: %s", command);
    return 1;
}

public OnSystemCommandExecute(const line_output[], retval, index, success, line_current, line_total)
{
	SendClientMessagef(index, -1, "Command output: success: %d, returned value: %d, current line: %d, total lines: %d", success, retval, line_current, line_total);
	SendClientMessagef(index, -1, "Command output: %s", line_output);
}

CMD:localip(playerid, params[])
{
	new index;
	if(sscanf(params, "d", index)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /localip <index>");

    new ipaddr[32];
    GetLocalIP(index, ipaddr, sizeof(ipaddr));
    SendClientMessagef(playerid, -1, "localip <%d>: %s", index, ipaddr);
    return 1;
}

CMD:gotoplayer(playerid, params[])
{
	new toplayer;
	if(sscanf(params, "d", toplayer)) return SendClientMessagef(playerid, 0xFF0000AA, "USAGE: /gotoplayer <playerid>");

	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(toplayer, X, Y, Z);
	SetPlayerPos(playerid, X + 1.0, Y + 1.0, Z);
    return 1;
}

stock pName(playerid)
{
	static
	    szName[24];
	GetPlayerName(playerid, szName, sizeof(szName));
	return szName;
}

public OnPlayerEnterGangZone(playerid, zoneid)
{
    SendClientMessagef(playerid, -1, "OnPlayerEnterGangZone(playerid = %d, zoneid = %d)", playerid, zoneid);
	return 1;
}

public OnPlayerLeaveGangZone(playerid, zoneid)
{
    SendClientMessagef(playerid, -1, "OnPlayerLeaveGangZone(playerid = %d, zoneid = %d)", playerid, zoneid);
	return 1;
}

public OnPlayerEnterPlayerGangZone(playerid, zoneid)
{
    SendClientMessagef(playerid, 0xd3e836AA, "OnPlayerEnterPlayerGangZone(playerid = %d, zoneid = %d)", playerid, zoneid);
	return 1;
}

public OnPlayerLeavePlayerGangZone(playerid, zoneid)
{
    SendClientMessagef(playerid, 0xd3e836AA, "OnPlayerLeavePlayerGangZone(playerid = %d, zoneid = %d)", playerid, zoneid);
	return 1;
}
/*
public OnPlayerPickUpPickup(playerid, pickupid)
{
    SendClientMessagef(playerid, 0x00e893AA, "OnPlayerPickUpPickup(playerid = %d, pickupid = %d)", playerid, pickupid);
	return 1;
}

public OnPlayerPickUpPlayerPickup(playerid, pickupid)
{
    SendClientMessagef(playerid, 0xFFD700FF, "OnPlayerPickUpPlayerPickup(playerid = %d, pickupid = %d)", playerid, pickupid);
	return 1;
}
*/
public OnPlayerPauseStateChange(playerid, pausestate)
{
    SendClientMessagef(playerid, 0xFFD700FF, "OnPlayerPauseStateChange(playerid = %d, pausestate = %d <time: %d>)", playerid, pausestate, GetPlayerPausedTime(playerid));
	return 1;
}

public OnRemoteRCONPacket(const ipaddr[], port, const password[], success, const command[])
{
	printf("remote console - ip: %s, port: %d, pass: %s, command: %s", ipaddr, port, password, command);
	return 1;
}

public OnPlayerClientGameInit(playerid, &usecjwalk, &limitglobalchat, &Float:globalchatradius, &Float:nametagdistance, &disableenterexits, &nametaglos, &manualvehengineandlights,
				&spawnsavailable, &shownametags, &showplayermarkers, &onfoot_rate, &incar_rate, &weapon_rate, &lagcompmode, &vehiclefriendlyfire)

{
	printf("OnPlayerClientGameInit(playerid = %d, &usecjwalk = %d, &limitglobalchat = %d, &globalchatradius = %f, &nametagdistance = %f, &disableenterexits = %d, &nametaglos = %d, &manualvehengineandlights = %d, \
				&spawnsavailable = %d, &shownametags = %d, &showplayermarkers = %d, &onfoot_rate = %d, &incar_rate = %d, &weapon_rate = %d, &lagcompmode = %d, &vehiclefriendlyfire = %d",
	playerid, usecjwalk, limitglobalchat, globalchatradius, nametagdistance, disableenterexits, nametaglos, manualvehengineandlights,
				spawnsavailable, shownametags, showplayermarkers, onfoot_rate, incar_rate, weapon_rate, lagcompmode, vehiclefriendlyfire);
				

    onfoot_rate = 13;
	limitglobalchat = true;
	globalchatradius = 200;
	//nametagdistance = -50;
	disableenterexits = false;
    printf("lófasz - %d", limitglobalchat);
}
/*
public OnServerQueryInfo(const ipaddr[], hostname[51], gamemode[31], language[31])
{
	format(hostname, sizeof(hostname), "qwe");
	format(gamemode, sizeof(gamemode), "asd");
	format(language, sizeof(language), "1234");

	printf("OnServerQueryInfo: ip: %d, hostname: %s, gammeode: %s, language: %s", ipaddr, hostname, gamemode, language);
	return 1;
}
*/

