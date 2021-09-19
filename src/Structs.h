#ifndef YSF_STRUCTS_H
#define YSF_STRUCTS_H

#include "CVector.h"
#include "CVector2D.h"
#include <map>
#include <type_traits>

#include <raknet/BitStream.h>
#include <raknet/NetworkTypes.h>
#include <sdk/amx/amx.h>
#include "includes/types.h"

/* -------------------------------------------------------- */

// Defines from SAMPGDK
#define MAX_PLAYER_NAME (24)
#define MAX_CLIENT_MESSAGE (144)
#define MAX_PLAYERS (1000)
#define MAX_VEHICLES (2000)
#define MAX_ACTORS (1000)
#define INVALID_PLAYER_ID (0xFFFF)
#define INVALID_VEHICLE_ID (0xFFFF)
#define INVALID_ACTOR_ID (0xFFFF)
#define NO_TEAM (255)
#ifndef SAMP_03DL
#define MAX_OBJECTS (1000)
#else
#define MAX_OBJECTS (2000)
#endif
#define INVALID_OBJECT_ID (0xFFFF)
#define MAX_GANG_ZONES (1024)
#define MAX_TEXT_DRAWS (2048)
#define MAX_PLAYER_TEXT_DRAWS (256)
#define MAX_MENUS (128)
#define MAX_3DTEXT_GLOBAL (1024)
#define MAX_3DTEXT_PLAYER (1024)
#define MAX_PICKUPS (4096)
#define INVALID_MENU (0xFF)
#define INVALID_TEXT_DRAW (0xFFFF)
#define INVALID_GANG_ZONE (-1)
#define INVALID_3DTEXT_ID (0xFFFF)
#define SERVER_VARTYPE_NONE (0)
#define SERVER_VARTYPE_INT (1)
#define SERVER_VARTYPE_STRING (2)
#define SERVER_VARTYPE_FLOAT (3)
#define TEXT_DRAW_FONT_SPRITE_DRAW (4)
#define TEXT_DRAW_FONT_MODEL_PREVIEW (5)
#define DIALOG_STYLE_MSGBOX (0)
#define DIALOG_STYLE_INPUT (1)
#define DIALOG_STYLE_LIST (2)
#define DIALOG_STYLE_PASSWORD (3)
#define DIALOG_STYLE_TABLIST (4)
#define DIALOG_STYLE_TABLIST_HEADERS (5)
#define PLAYER_STATE_NONE (0)
#define PLAYER_STATE_ONFOOT (1)
#define PLAYER_STATE_DRIVER (2)
#define PLAYER_STATE_PASSENGER (3)
#define PLAYER_STATE_EXIT_VEHICLE (4)
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER (5)
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER (6)
#define PLAYER_STATE_WASTED (7)
#define PLAYER_STATE_SPAWNED (8)
#define PLAYER_STATE_SPECTATING (9)
#define PLAYER_MARKERS_MODE_OFF (0)
#define PLAYER_MARKERS_MODE_GLOBAL (1)
#define PLAYER_MARKERS_MODE_STREAMED (2)
#define WEAPON_BRASSKNUCKLE (1)
#define WEAPON_GOLFCLUB (2)
#define WEAPON_NITESTICK (3)
#define WEAPON_KNIFE (4)
#define WEAPON_BAT (5)
#define WEAPON_SHOVEL (6)
#define WEAPON_POOLSTICK (7)
#define WEAPON_KATANA (8)
#define WEAPON_CHAINSAW (9)
#define WEAPON_DILDO (10)
#define WEAPON_DILDO2 (11)
#define WEAPON_VIBRATOR (12)
#define WEAPON_VIBRATOR2 (13)
#define WEAPON_FLOWER (14)
#define WEAPON_CANE (15)
#define WEAPON_GRENADE (16)
#define WEAPON_TEARGAS (17)
#define WEAPON_MOLTOV (18)
#define WEAPON_COLT45 (22)
#define WEAPON_SILENCED (23)
#define WEAPON_DEAGLE (24)
#define WEAPON_SHOTGUN (25)
#define WEAPON_SAWEDOFF (26)
#define WEAPON_SHOTGSPA (27)
#define WEAPON_UZI (28)
#define WEAPON_MP5 (29)
#define WEAPON_AK47 (30)
#define WEAPON_M4 (31)
#define WEAPON_TEC9 (32)
#define WEAPON_RIFLE (33)
#define WEAPON_SNIPER (34)
#define WEAPON_ROCKETLAUNCHER (35)
#define WEAPON_HEATSEEKER (36)
#define WEAPON_FLAMETHROWER (37)
#define WEAPON_MINIGUN (38)
#define WEAPON_SATCHEL (39)
#define WEAPON_BOMB (40)
#define WEAPON_SPRAYCAN (41)
#define WEAPON_FIREEXTINGUISHER (42)
#define WEAPON_CAMERA (43)
#define WEAPON_NIGHTVISION (44)
#define WEAPON_INFRARED (45)
#define WEAPON_PARACHUTE (46)
#define WEAPON_VEHICLE (49)
#define WEAPON_DROWN (53)
#define WEAPON_COLLISION (54)
#define KEY_ACTION (1)
#define KEY_CROUCH (2)
#define KEY_FIRE (4)
#define KEY_SPRINT (8)
#define KEY_SECONDARY_ATTACK (16)
#define KEY_JUMP (32)
#define KEY_LOOK_RIGHT (64)
#define KEY_HANDBRAKE (128)
#define KEY_LOOK_LEFT (256)
#define KEY_SUBMISSION (512)
#define KEY_LOOK_BEHIND (512)
#define KEY_WALK (1024)
#define KEY_ANALOG_UP (2048)
#define KEY_ANALOG_DOWN (4096)
#define KEY_ANALOG_LEFT (8192)
#define KEY_ANALOG_RIGHT (16384)
#define KEY_YES (65536)
#define KEY_NO (131072)
#define KEY_CTRL_BACK (262144)
#define KEY_UP (-128)
#define KEY_DOWN (128)
#define KEY_LEFT (-128)
#define KEY_RIGHT (128)
#define BODY_PART_TORSO (3)
#define BODY_PART_GROIN (4)
#define BODY_PART_LEFT_ARM (5)
#define BODY_PART_RIGHT_ARM (6)
#define BODY_PART_LEFT_LEG (7)
#define BODY_PART_RIGHT_LEG (8)
#define BODY_PART_HEAD (9)
#define CLICK_SOURCE_SCOREBOARD (0)
#define EDIT_RESPONSE_CANCEL (0)
#define EDIT_RESPONSE_FINAL (1)
#define EDIT_RESPONSE_UPDATE (2)
#define SELECT_OBJECT_GLOBAL_OBJECT (1)
#define SELECT_OBJECT_PLAYER_OBJECT (2)
#define BULLET_HIT_TYPE_NONE (0)
#define BULLET_HIT_TYPE_PLAYER (1)
#define BULLET_HIT_TYPE_VEHICLE (2)
#define BULLET_HIT_TYPE_OBJECT (3)
#define BULLET_HIT_TYPE_PLAYER_OBJECT (4)

// Additional Defines
#define MAX_OBJECT_MATERIAL			16
#define MAX_PLAYER_ATTACHED_OBJECTS 10
#define	MAX_FILTER_SCRIPTS			16

#define MAX_PVARS					800
#define MAX_PVAR_NAME				40

#define MAX_MENU_TEXT_SIZE			32
#define MAX_ITEMS					12
#define MAX_COLUMNS					2

#define GAMESTATE_STOPPED			0
#define GAMESTATE_RUNNING			1
#define GAMESTATE_RESTARTING		2
/* -------------------------------------------------------- */

// Server rules special
enum CON_VARTYPE { CON_VARTYPE_FLOAT, CON_VARTYPE_INT, CON_VARTYPE_BOOL, CON_VARTYPE_STRING };

#define CON_VARFLAG_DEBUG			1
#define CON_VARFLAG_READONLY		2
#define CON_VARFLAG_RULE			4	// Gets sent with a RULES query responce

typedef void(*VARCHANGEFUNC)();

/* -------------------------------------------------------- */

#pragma pack(push, 1)
#ifndef OTHERFIELD
#define OTHERFIELD(fname,ftype,...) \
	ftype fname __VA_ARGS__
#endif
#ifndef FIELD
#define FIELD(fname,ftype,...) \
	ftype fname __VA_ARGS__
#endif
#ifndef BITFIELD
#define BITFIELD(fname,ftype,size) \
	ftype fname:size
#endif
#ifndef CHECK_TYPE
#define CHECK_TYPE(type,size) \
	static_assert(std::is_standard_layout<type>::value, #type" is not standard-layout"); \
	static_assert(sizeof(type)==size, #type" must have size "#size)
#endif

#define PAD(a,b) char a[b]

// RakNet Remote system
struct PingAndClockDifferential
{
	FIELD(pingTime, unsigned short);
	FIELD(clockDifferential, unsigned int);
};
CHECK_TYPE(PingAndClockDifferential, 6);

struct RemoteSystemStruct
{
	FIELD(isActive, bool);
	FIELD(playerId, PlayerID); // The remote system associated with this reliability layer
	FIELD(myExternalPlayerId, PlayerID); // Your own IP, as reported by the remote system
	PAD(padD, 1895);
	FIELD(dword774, DWORD);
	FIELD(word778, WORD);
	PAD(pad77A, 2);
	FIELD(dword77C, DWORD);
	FIELD(dword780, DWORD);
	PAD(pad784, 276);
	FIELD(dword898, DWORD);
	PAD(pad89C, 16);
	FIELD(byte8AC, BYTE);
#ifdef _WIN32
	PAD(pad8AD, 1023);
#else
	PAD(pad8AD, 945);
#endif
	FIELD(connectionTime, unsigned int);
	enum ConnectMode : DWORD
	{
		NO_ACTION,
		DISCONNECT_ASAP,
		DISCONNECT_ASAP_SILENTLY,
		DISCONNECT_ON_NO_ACK,
		REQUESTED_CONNECTION,
		HANDLING_CONNECTION_REQUEST,
		UNVERIFIED_SENDER,
		SET_ENCRYPTION_ON_MULTIPLE_16_BYTE_PACKET,
		CONNECTED
	} connectMode;
	FIELD(byteAuthTableIndex, BYTE); // https://github.com/kurta999/YSF/pull/64
	FIELD(byteAuthType, BYTE);
	FIELD(byteIsLogon, BYTE);
};
#ifdef _WIN32
CHECK_TYPE(RemoteSystemStruct, 3255);
#else
CHECK_TYPE(RemoteSystemStruct, 3177);
#endif

/* -------------------------------------------------------- */
struct MATRIX4X4 
{
	FIELD(right, CVector);
	FIELD(flags, DWORD);
	FIELD(up, CVector);
	FIELD(pad_u, float);
	FIELD(at, CVector);
	FIELD(pad_a, float);
	FIELD(pos, CVector);
	FIELD(pad_p, float);
};
CHECK_TYPE(MATRIX4X4, 64);

struct ConsoleVariable_s
{
	FIELD(VarType, CON_VARTYPE);
	FIELD(VarFlags, DWORD);
	FIELD(VarPtr, void*);
	FIELD(VarChangeFunc, VARCHANGEFUNC);
};
CHECK_TYPE(ConsoleVariable_s, 16);

struct ConsoleCommand_s
{
	FIELD(szName, char, [255]);
	FIELD(dwFlags, DWORD);
	void (*fptrFunc)();
};
CHECK_TYPE(ConsoleCommand_s, 263);

/* -------------------------------------------------------- */
// CPlayer
/* -------------------------------------------------------- */

// Big thanks to OrMisicL
struct CAimSyncData
{
	FIELD(byteCameraMode, BYTE); // 0
	FIELD(vecFront, CVector); // 1 - 13
	FIELD(vecPosition, CVector); // 13 - 25
	FIELD(fZAim, float); // 25 - 29
	union
	{
		FIELD(byteCameraZoomWeaponState, BYTE);
		struct
		{
			BITFIELD(byteCameraZoom, BYTE, 6); // 29
			BITFIELD(byteWeaponState, BYTE, 2); // 29
		};
	};
	FIELD(byteAspectRatio, BYTE); // 30 - 31
};
CHECK_TYPE(CAimSyncData, 31);

struct CVehicleSyncData
{
	FIELD(wVehicleId, WORD); // 0x001F - 0x0021
	FIELD(wLRAnalog, WORD); // 0x0021 - 0x0023
	FIELD(wUDAnalog, WORD); // 0x0023 - 0x0025
	FIELD(wKeys, WORD); // 0x0025 - 0x0027
	FIELD(fQuaternion, float, [4]); // 0x002B - 0x0037
	FIELD(vecPosition, CVector); // 0x0037 - 0x0043
	FIELD(vecVelocity, CVector); // 0x0043 - 0x004F
	FIELD(fHealth, float); // 0x004F - 0x0053
	FIELD(bytePlayerHealth, BYTE); // 0x0053 - 0x0054
	FIELD(bytePlayerArmour, BYTE); // 0x0054 - 0x0055
	union
	{
		FIELD(bytePlayerWeaponUnk, BYTE);
		struct
		{
			BITFIELD(bytePlayerWeapon, BYTE, 6); // 0x0055 - 0x0056
			BITFIELD(unk_2, BYTE, 2); // 0x0055 - 0x0056
		};
	};
	FIELD(byteSirenState, BYTE); // 0x0056 - 0x0057
	FIELD(byteGearState, BYTE); // 0x0057 -	0x0058
	FIELD(wTrailerID, WORD); // 0x0058 - 0x005A
    union									// 
    {
		FIELD(wHydraReactorAngle, WORD, [2]);
		OTHERFIELD(fTrainSpeed, float);
    };
};
CHECK_TYPE(CVehicleSyncData, 63);

struct CPassengerSyncData
{
	FIELD(wVehicleId, WORD); // 0x005E - 0x0060
	union
	{
		FIELD(byteSeatFlagsDriveBy, BYTE);
		struct
		{
			BITFIELD(byteSeatFlags, BYTE, 7);
			BITFIELD(byteDriveBy, BYTE, 1);
		};
	};
	FIELD(bytePlayerWeapon, BYTE); // 0x0061 - 0x0062
	FIELD(bytePlayerHealth, BYTE); // 0x0062 - 0x0063
	FIELD(bytePlayerArmour, BYTE); // 0x0063 - 0x0064
	FIELD(wLRAnalog, WORD); // 0x0064 - 0x0066
	FIELD(wUDAnalog, WORD); // 0x0066 - 0x0068
	FIELD(wKeys, WORD); // 0x0068 - 0x006A
	FIELD(vecPosition, CVector); // 0x006A - 0x0076
};
CHECK_TYPE(CPassengerSyncData, 24);

struct CSyncData
{
	FIELD(wLRAnalog, WORD); // 0x0076 - 0x0078
	FIELD(wUDAnalog, WORD); // 0x0078 - 0x007A
	FIELD(wKeys, WORD); // 0x007A - 0x007C
	FIELD(vecPosition, CVector); // 0x007C - 0x0088
	FIELD(fQuaternion, float, [4]); // 0x0088 - 0x008C
	FIELD(byteHealth, BYTE); // 0x0098 - 0x0099
	FIELD(byteArmour, BYTE); // 0x0099 - 0x009A
	union
	{
		FIELD(byteWeaponUnk, BYTE);
		struct
		{
			BITFIELD(byteWeapon, BYTE, 6); // 0x009A - 0x009B
			BITFIELD(_unk_, BYTE, 2);
		};
	};
	FIELD(byteSpecialAction, BYTE); // 0x009B - 0x009C
	FIELD(vecVelocity, CVector); // 0x009C - 0x00A8
	FIELD(vecSurfing, CVector); // 0x00A8 - 0x00B4
	FIELD(wSurfingInfo, WORD); // 0x00B4 - 0x00B6
	union 
	{
		FIELD(dwAnimationData, DWORD); // 0x00B6 - 0x00BA
		struct 
		{
			OTHERFIELD(wAnimIndex, WORD);
			OTHERFIELD(wAnimFlags, WORD);
		};
	};
};
CHECK_TYPE(CSyncData, 68);

struct CUnoccupiedSyncData
{
	FIELD(wVehicleID, WORD); // + 0x0000
	FIELD(bytePassengerSlot, BYTE); // + 0x0002
	FIELD(vecRool, CVector); // + 0x0003
	FIELD(vecDirection, CVector); // + 0x000F
	FIELD(vecPosition, CVector); // + 0x001B
	FIELD(vecVelocity, CVector); // + 0x0027
	FIELD(vecTurnVelocity, CVector); // + 0x0033
	FIELD(fHealth, float); // + 0x003F
};
CHECK_TYPE(CUnoccupiedSyncData, 67);

struct CSpectatingSyncData
{
	FIELD(wLeftRightKeysOnSpectating, WORD); // + 0x0000
	FIELD(wUpDownKeysOnSpectating, WORD); // + 0x0002
	FIELD(wKeysOnSpectating, WORD); // + 0x0004
	FIELD(vecPosition, CVector); // + 0x0006
};
CHECK_TYPE(CSpectatingSyncData, 18);

struct CTrailerSyncData
{
	FIELD(wTrailerID, WORD); // + 0x0000
	FIELD(vecPosition, CVector); // + 0x0002
	FIELD(fQuaternion, float, [4]); // + 0x000E
	FIELD(vecVelocity, CVector); // + 0x001E
	FIELD(vecTurnVelocity, CVector); // + 0x002A
};
CHECK_TYPE(CTrailerSyncData, 54);

struct CTextdraw
{
	union
	{
		FIELD(byteFlags, BYTE); // 25
		struct
		{
			BITFIELD(byteBox, BYTE, 1);
			BITFIELD(byteLeft, BYTE, 1); 
			BITFIELD(byteRight, BYTE, 1);
			BITFIELD(byteCenter, BYTE, 1);
			BITFIELD(byteProportional, BYTE, 1);
			BITFIELD(bytePadding, BYTE, 3);
		};
	};
	FIELD(vecLetter, CVector2D); // 1 - 9
	FIELD(dwLetterColor, DWORD); // 9
	FIELD(vecLine, CVector2D);
	FIELD(dwBoxColor, DWORD); // 21
	FIELD(byteShadow, BYTE); // 26
	FIELD(byteOutline, BYTE); // 27
	FIELD(dwBackgroundColor, DWORD); // 31
	FIELD(byteStyle, BYTE); // 32
	FIELD(byteSelectable, BYTE); // 32
	FIELD(vecPos, CVector2D);
	FIELD(dwModelIndex, WORD); // 41 - 43
	FIELD(vecRot, CVector); // 43 - 55
	FIELD(fZoom, float); // 55 - 59
	FIELD(color1, WORD); // 59 - 61
	FIELD(color2, WORD); // 61 - 63
};
CHECK_TYPE(CTextdraw, 63);

struct CPlayerTextDraw
{
	FIELD(bSlotState, BOOL, [MAX_PLAYER_TEXT_DRAWS]);
	FIELD(TextDraw, CTextdraw*, [MAX_PLAYER_TEXT_DRAWS]);
	FIELD(szFontText, char*, [MAX_PLAYER_TEXT_DRAWS]);
	FIELD(bHasText, bool, [MAX_PLAYER_TEXT_DRAWS]);
};
CHECK_TYPE(CPlayerTextDraw, 2048 + 1024 + 256);

struct C3DText
{
	FIELD(szText, char*); // + 0x00
	FIELD(dwColor, DWORD); // + 0x04
	FIELD(vecPos, CVector);
	FIELD(fDrawDistance, float); // + 0x14
	FIELD(bLineOfSight, bool); // + 0x18
	FIELD(iWorld, int); // + 0x19
	FIELD(wAttachedToPlayerID, WORD); // + 0x1D
	FIELD(wAttachedToVehicleID, WORD); // + 0x1F
};
CHECK_TYPE(C3DText, 33);

struct CPlayerText3DLabels
{
	FIELD(TextLabels, C3DText, [MAX_3DTEXT_PLAYER]); // + 0x0000
	FIELD(isCreated, BOOL, [MAX_3DTEXT_PLAYER]); // + 0x8400
	FIELD(unknown9800, BYTE, [MAX_3DTEXT_PLAYER]); // + 0x9400
	FIELD(wOwnerID, WORD);
};
CHECK_TYPE(CPlayerText3DLabels, 38914);

struct CAttachedObject
{
    FIELD(iModelID, int);
    FIELD(iBoneiD, int);
    FIELD(vecPos, CVector);
    FIELD(vecRot, CVector);
    FIELD(vecScale, CVector);
	FIELD(dwMaterialColor1, DWORD);
	FIELD(dwMaterialColor2, DWORD);
};
CHECK_TYPE(CAttachedObject, 52);

struct CPlayerSpawnInfo
{
	FIELD(byteTeam, BYTE); // 0 - 1
	FIELD(iSkin, int); // 1 - 5
#ifdef SAMP_03DL
	FIELD(dwCustomSkin, DWORD);
#endif
	FIELD(unk, BYTE); // 5 - 6
	FIELD(vecPos, CVector); // 6 - 18
	FIELD(fRotation, float); // 18 - 22
	FIELD(iSpawnWeapons, int, [3]); // 22 - 34
	FIELD(iSpawnWeaponsAmmo, int, [3]); // 34 - 46
};
#ifndef SAMP_03DL
CHECK_TYPE(CPlayerSpawnInfo, 46);
#else
CHECK_TYPE(CPlayerSpawnInfo, 50);
#endif

struct CBulletSyncData
{
	FIELD(byteHitType, BYTE);
	FIELD(wHitID, WORD);
	FIELD(vecHitOrigin, CVector);
	FIELD(vecHitTarget, CVector);
	FIELD(vecCenterOfHit, CVector);
	FIELD(byteWeaponID, BYTE);
}; 
CHECK_TYPE(CBulletSyncData, 40);

struct CPVar
{
    FIELD(szVarName, char, [MAX_PVAR_NAME + 1]);
    FIELD(bIsReadOnly, BOOL);
    FIELD(iVarType, int);
    FIELD(iValue, int);
    FIELD(fValue, float);
    FIELD(szValue, char*);
};
CHECK_TYPE(CPVar, 61);

struct CPlayerVar
{
    FIELD(Vars, CPVar, [MAX_PVARS]);
	FIELD(bIsPVarActive, BOOL, [MAX_PVARS]);
    FIELD(iUpperIndex, int);
};
CHECK_TYPE(CPlayerVar, 48800 + 3200 + 4);

struct CPlayer
{
	FIELD(aimSyncData, CAimSyncData); // 0 - 31
	FIELD(wCameraObject, WORD); // 31 - 33
	FIELD(wCameraVehicle, WORD); // 33 - 35
	FIELD(wCameraPlayer, WORD); // 35 - 37
	FIELD(wCameraActor, WORD); // 37 - 39
	FIELD(vehicleSyncData, CVehicleSyncData); // 39 -
	FIELD(passengerSyncData, CPassengerSyncData); //
	FIELD(syncData, CSyncData); // 126 - 194
	FIELD(unoccupiedSyncData, CUnoccupiedSyncData); // 194 - 261
	FIELD(spectatingSyncData, CSpectatingSyncData); // 261 - 279
	FIELD(trailerSyncData, CTrailerSyncData); // 279 - 333
	FIELD(dwPlayerSyncUnused, DWORD); // 333 - 337
	FIELD(dwVehicleSyncUnused, DWORD); // 337 - 341
	FIELD(byteStreamedIn, BYTE, [MAX_PLAYERS]); // 341 - 1341
	FIELD(byteVehicleStreamedIn, BYTE, [MAX_VEHICLES]); // 1341 - 3341
	FIELD(byteSomethingUnused, BYTE, [1000]); // 3341 - 4341
#ifdef SAMP_03DL
	FIELD(byteSomethingUnused2, BYTE, [1000]); // 4341 - 5341
#endif
	FIELD(byte3DTextLabelStreamedIn, BYTE, [1024]); // 4341  - 5365
	FIELD(bPickupStreamedIn, BYTE, [MAX_PICKUPS]); // 5365 - 9461
	FIELD(byteActorStreamedIn, BYTE, [MAX_PLAYERS]); // 9461 - 10461
	FIELD(dwStreamedInPlayers, DWORD); // 10461 - 10465
	FIELD(dwStreamedInVehicles, DWORD); // 10465 - 10469
	FIELD(dwStreamedInSomethingUnused, DWORD); // 10469 - 10473
	FIELD(dwStreamedIn3DTextLabels, DWORD); // 10479 - 10477
	FIELD(dwStreamedInPickups, DWORD); // 10477 - 10481
	FIELD(dwStreamedInActors, DWORD); // 10481 - 10485
	FIELD(bHasSetVehiclePos, DWORD); // 10485 - 10489
	FIELD(dwSetVehiclePosTick, DWORD); // 10489 - 10493
	FIELD(vecVehicleNewPos, CVector); // 10493 - 10505
	FIELD(bCameraTarget, BOOL); // 10505
	FIELD(bHasSpawnInfo, DWORD); // 10509
	FIELD(bUpdateKeys, BOOL); // 10513
	FIELD(vecPosition, CVector); // 10517
	FIELD(fHealth, float); // 10529 - 10533
	FIELD(fArmour, float); // 10533 - 10537
	FIELD(fQuaternion, float, [4]); // 10537 - 10553
	FIELD(fAngle, float); // 10553 - 10557
	FIELD(vecVelocity, CVector); // 10557 - 10569
	FIELD(wLRAnalog, WORD); // 10569
	FIELD(wUDAnalog, WORD); // 10571
	FIELD(dwKeys, DWORD); // 10573 - 10577
	FIELD(dwOldKeys, DWORD); // 10577 - 10581
#ifdef SAMP_03DL
	FIELD(dwUnknown1, DWORD); // 11581 - 11585
#endif
	FIELD(bEditObject, BOOL); // 10581 - 10585
	FIELD(bEditAttachedObject, BOOL); // 10585 - 10589
	FIELD(wDialogID, WORD); // 10589 - 10591
	FIELD(pTextdraw, CPlayerTextDraw*); // 10591 - 10595
	FIELD(p3DText, CPlayerText3DLabels*); // 10595 - 10599
	FIELD(wPlayerId, WORD); // 10599 - 10601
	FIELD(iUpdateState, int); // 10601 - 10605
	FIELD(attachedObject, CAttachedObject, [MAX_PLAYER_ATTACHED_OBJECTS]); // 10605 - 11125
	FIELD(attachedObjectSlot, BOOL, [MAX_PLAYER_ATTACHED_OBJECTS]); // 11125 - 11165
	FIELD(bHasAimSync, BOOL); // 11165 - 11169
	FIELD(bHasTrailerSync, BOOL); // 11169 - 11173
	FIELD(bHasUnoccupiedSync, BOOL); // 11173 - 11177
	FIELD(byteState, BYTE); // 11177 - 11178
	FIELD(vecCPPos, CVector); // 11178 - 11190
	FIELD(fCPSize, float); // 11190 - 11194
	FIELD(bIsInCP, BOOL); // 11194 - 11198
	FIELD(vecRaceCPPos, CVector); // 11198 - 11210
	FIELD(vecRaceCPNextPos, CVector); // 11210 - 11222 
	FIELD(byteRaceCPType, BYTE); // 11222 - 11223 // TODO -> replace
	FIELD(fRaceCPSize, float); // 11223 - 11227
	FIELD(bIsInRaceCP, BOOL); // 11227 - 11231
	FIELD(bIsInModShop, BOOL); // 11231 - 11235
	FIELD(wSkillLevel, WORD, [11]); // 11235 - 11257
	FIELD(iLastMarkerUpdate, int); // 11257 - 11261
	FIELD(spawn, CPlayerSpawnInfo); // 11261 - 11307
	FIELD(bReadyToSpawn, BOOL); // 11307 - 11311
	FIELD(byteWantedLevel, BYTE); // 11311 - 11312
	FIELD(byteFightingStyle, BYTE); // 11312 - 11313
	FIELD(byteSeatId, BYTE); // 11313 - 11314
	FIELD(wVehicleId, WORD); // 11314 - 11316
	FIELD(dwNickNameColor, DWORD); // 11316 - 11320
	FIELD(bShowCheckpoint, BOOL); // 11320 - 11324
	FIELD(bShowRaceCheckpoint, BOOL); // 11324 - 11328
	FIELD(iInteriorId, int); // 11328 - 11332
	FIELD(wWeaponAmmo, WORD, [12]); // 11332 - 11356
	PAD(pad10, 28); // 11356 - 11384
	FIELD(byteWeaponId, BYTE, [12]); // 11384 - 11396
	FIELD(byteWeaponID_unknown, BYTE); // 11396 - 11397
	FIELD(byteCurrentWeapon, BYTE); // 11397 - 11398
	FIELD(wTargetId, WORD); // 11398 - 11400
	FIELD(wTargetActorId, WORD); // 11400 - 11402
	FIELD(dwLastShotTick, DWORD); // 11402 - 11406
	FIELD(dwLastShotWeapon, BYTE); // 11406 - 11407
	FIELD(bulletSyncData, CBulletSyncData); // 11407 - 11447	
	FIELD(m_byteTime, BYTE); // 11447 - 11448
	FIELD(m_fGameTime, float); // 11448 - 11452
	FIELD(byteSpectateType, BYTE); // 11452 - 11453
	FIELD(wSpectateID, DWORD); // 11453 - 11457
	FIELD(dwLastStreaming, DWORD); // 11457 - 11461
#ifdef SAMP_03DL
	FIELD(bUnknown2, BYTE); // 12469 - 12470
#endif
	FIELD(dwNPCRecordingType, DWORD); // 11461 - 11465
	FIELD(pRecordingFile, FILE*); // 11465 - 11469
	FIELD(dwFirstNPCWritingTime, DWORD); // 11469 - 11473 
	PAD(unused, 9); // 11473 - 11482
	FIELD(pPlayerVars, CPlayerVar*); // 11482 - 11486
#ifdef SAMP_03DL
	FIELD(dwUnknown3, DWORD); // 12495 - 12499
#endif
};
#ifndef SAMP_03DL
CHECK_TYPE(CPlayer, 11486);
#else
CHECK_TYPE(CPlayer, 12499);
#endif

struct CPlayerPool
{
	FIELD(dwVirtualWorld, DWORD, [MAX_PLAYERS]); // 0 - 4000
	FIELD(dwPlayersCount, DWORD); // 4000 - 4004
#ifdef SAMP_03DL
	PAD(pad1, 1000);
#endif
	FIELD(dwlastMarkerUpdate, DWORD); // 4004 - 4008
	FIELD(fUpdatePlayerGameTimers, float); // 4008 - 4012
	FIELD(dwScore, DWORD, [MAX_PLAYERS]); // 4012 - 8012
	FIELD(dwMoney, DWORD, [MAX_PLAYERS]); // 8012 - 12012
	FIELD(dwDrunkLevel, DWORD, [MAX_PLAYERS]); // 12012 - 16012
	FIELD(dwLastScoreUpdate, DWORD, [MAX_PLAYERS]); // 16012 - 20012
	FIELD(szSerial, char, [MAX_PLAYERS][101]); // 20012 - 121012				
	FIELD(szVersion, char, [MAX_PLAYERS][25]); // 121012 - 146012
	FIELD(pRemoteSystem, RemoteSystemStruct*, [MAX_PLAYERS]); // 146012 - 150012
	FIELD(bIsPlayerConnected, BOOL, [MAX_PLAYERS]); // 150012 - 154012
	FIELD(pPlayer, CPlayer*, [MAX_PLAYERS]); // 154012 - 158012
	FIELD(szName, char, [MAX_PLAYERS][MAX_PLAYER_NAME + 1]); // 158012 - 183012
	FIELD(bIsAnAdmin, BOOL, [MAX_PLAYERS]); // 183012 - 187012
	FIELD(bIsNPC, BOOL, [MAX_PLAYERS]); // 187012 - 191012
	PAD(pad0, 8000); // 191012 - 199012
	FIELD(dwConnectedPlayers, DWORD); // 199012 - 199016
	FIELD(dwPlayerPoolSize, DWORD); // 199016 - 199020
	FIELD(dwUnk, DWORD); // 199020 - 199024
};
#ifndef SAMP_03DL
CHECK_TYPE(CPlayerPool, 199024);
#else
CHECK_TYPE(CPlayerPool, 200024);
#endif

/* -------------------------------------------------------- */
// CVehicle
/* -------------------------------------------------------- */

struct CVehicleSpawn
{
	FIELD(iModelID, int);
    FIELD(vecPos, CVector);   
    FIELD(fRot, float);
    FIELD(iColor1, int);  
    FIELD(iColor2, int);  
    FIELD(iRespawnTime, int);
    FIELD(iInterior, int);
};
CHECK_TYPE(CVehicleSpawn, 36);

struct CVehicleModInfo
{
	FIELD(byteModSlots, BYTE, [14]); // + 0x0000
    FIELD(bytePaintJob, BYTE); // + 0x000E
    FIELD(iColor1, int); // + 0x000F
    FIELD(iColor2, int); // + 0x0010
};
CHECK_TYPE(CVehicleModInfo, 23);

struct CVehicleParams
{
	FIELD(engine, char);
	FIELD(lights, char);
	FIELD(alarm, char);
	FIELD(doors, char);
	FIELD(bonnet, char);
	FIELD(boot, char);
	FIELD(objective, char); // 6
	FIELD(siren, char); // 7
	FIELD(door_driver, char); // 8
	FIELD(door_passenger, char);
	FIELD(door_backleft, char);
	FIELD(door_backright, char); // 11
	FIELD(window_driver, char); // 12
	FIELD(window_passenger, char);
	FIELD(window_backleft, char);
	FIELD(window_backright, char); // 15 - 16
};
CHECK_TYPE(CVehicleParams, 16);

struct CVehicle
{
	FIELD(vecPosition, CVector); // 0 - 12
	FIELD(vehMatrix, MATRIX4X4); // 12 - 76
	FIELD(vecVelocity, CVector); // 76 - 88
	FIELD(vecTurnSpeed, CVector); // 88 - 100
	FIELD(wVehicleID, WORD); // 100 - 102
	FIELD(wTrailerID, WORD); // 102 - 104
	FIELD(wCabID, WORD); // 104 - 106
	FIELD(wLastDriverID, WORD); // 106 - 108
	FIELD(vehPassengers, WORD, [7]); // 108 - 122
	FIELD(vehActive, DWORD); // 122 - 126
	FIELD(vehWasted, DWORD); // 126 - 130	
	FIELD(customSpawn, CVehicleSpawn); // 130 - 166
	FIELD(fHealth, float); // 166 - 170
	FIELD(vehDoorStatus, DWORD); // 170 - 174
	FIELD(vehPanelStatus, DWORD); // 174 - 178
	FIELD(vehLightStatus, BYTE); // 178 - 179
	FIELD(vehTireStatus, BYTE); // 179 - 180
	FIELD(bDead, bool); // 180 - 181
	FIELD(wKillerID, WORD); // 181 - 183
	FIELD(vehModInfo, CVehicleModInfo); // 183 - 206
	FIELD(szNumberplate, char, [32 + 1]); // 206 - 239
	FIELD(vehParamEx, CVehicleParams); // 239 - 255
    FIELD(bDeathNotification, BYTE); // 255 - 256
    FIELD(bOccupied, BYTE); // 256 - 257
    FIELD(vehOccupiedTick, DWORD); // 257 - 261
    FIELD(vehRespawnTick, DWORD); // 261 - 265
	FIELD(byteSirenEnabled, BYTE); // 265 - 266
	FIELD(byteNewSirenState, BYTE); // 266 - 267 : passed to OnVehicleSirenStateChange
};
CHECK_TYPE(CVehicle, 267);

struct CVehiclePool
{
	FIELD(byteVehicleModelsUsed, BYTE, [212]); // 0 - 212
	FIELD(iVirtualWorld, int, [MAX_VEHICLES]); // 212 - 8212
	FIELD(bVehicleSlotState, BOOL, [MAX_VEHICLES]); // 8212 - 16212
	FIELD(pVehicle, CVehicle*, [MAX_VEHICLES]); // 16212 - 24212
	FIELD(dwVehiclePoolSize, DWORD); // 24212 - 24216
};
CHECK_TYPE(CVehiclePool, 24216);

/* -------------------------------------------------------- */
// CPickup
/* -------------------------------------------------------- */

struct tPickup
{
	FIELD(iModel, int);
	FIELD(iType, int);
	FIELD(vecPos, CVector);
};
CHECK_TYPE(tPickup, 20);

struct CPickupPool
{
	FIELD(Pickup, tPickup, [MAX_PICKUPS]); // + 0x0000
	FIELD(bActive, BOOL, [MAX_PICKUPS]); // + 0xA000
	FIELD(iWorld, int, [MAX_PICKUPS]); // + 0xC000
	FIELD(iPickupCount, int);
};
CHECK_TYPE(CPickupPool, 114692);

/* -------------------------------------------------------- */
// CObject
/* -------------------------------------------------------- */

struct CObjectMaterial
{
	FIELD(byteUsed, BYTE); // 197 - 198
	FIELD(byteSlot, BYTE); // 198 - 199
	FIELD(wModelID, WORD); // 199 - 201
	FIELD(dwMaterialColor, DWORD); // 201 - 205
	FIELD(szMaterialTXD, char, [64 + 1]); // 205 - 270
	FIELD(szMaterialTexture, char, [64 + 1]); // 270 - 335
	FIELD(byteMaterialSize, BYTE); // 335 - 336
	FIELD(szFont, char, [64 + 1]); // 336 - 401
	FIELD(byteFontSize, BYTE); // 401 - 402
	FIELD(byteBold, BYTE); // 402 - 403
	FIELD(dwFontColor, DWORD); // 403 - 407
	FIELD(dwBackgroundColor, DWORD); // 407 - 411
	FIELD(byteAlignment, BYTE); // 411 - 412
};
CHECK_TYPE(CObjectMaterial, 215);

struct CObject
{
	FIELD(wObjectID, WORD); // 0 - 2
	FIELD(iModel, int); // 2 - 6
	FIELD(bActive, BOOL); // 6 - 10
	FIELD(matWorld, MATRIX4X4); // 10 - 74 - pos - Object position
	FIELD(vecRot, CVector); // 74 - 86 - Object rotation
	FIELD(matTarget, MATRIX4X4); // 86 - 150	- 
	FIELD(bIsMoving, BYTE); // 150 - 151
	FIELD(bNoCameraCol, BYTE); // 151 - 152
	FIELD(fMoveSpeed, float); // 152 - 156
	FIELD(unk_4, DWORD); // 156 -160
	FIELD(fDrawDistance, float); // 160 - 164
	FIELD(wAttachedVehicleID, WORD); // 164 - 166
	FIELD(wAttachedObjectID, WORD); // 166 - 168
	FIELD(vecAttachedOffset, CVector); // 168 - 180
	FIELD(vecAttachedRotation, CVector); // 180 - 192
	FIELD(byteSyncRot, BYTE); // 192 - 193
	FIELD(dwMaterialCount, DWORD); // 193 - 197
	FIELD(Material, CObjectMaterial, [MAX_OBJECT_MATERIAL]); // 197 - 3637
	FIELD(szMaterialText, char*, [MAX_OBJECT_MATERIAL]); // 3637 - 3653
};
CHECK_TYPE(CObject, 3701);

struct CObjectPool
{
	FIELD(bPlayerObjectSlotState, BOOL, [MAX_PLAYERS][MAX_OBJECTS]); // 0 
	FIELD(bPlayersObject, BOOL, [MAX_OBJECTS]); // 4.000.000
	FIELD(pPlayerObjects, CObject*, [MAX_PLAYERS][MAX_OBJECTS]); // 4.004.000
	FIELD(bObjectSlotState, BOOL, [MAX_OBJECTS]); // 8.004.000
	FIELD(pObjects, CObject*, [MAX_OBJECTS]); // 8.008.000
};
#ifndef SAMP_03DL
CHECK_TYPE(CObjectPool, 8012000);
#else
CHECK_TYPE(CObjectPool, 16024000);
#endif

/* -------------------------------------------------------- */
// CMenu
/* -------------------------------------------------------- */

struct MenuInteraction
{
	FIELD(Menu, BOOL);
	FIELD(Row, BOOL, [MAX_ITEMS]);
	FIELD(unknown, char, [12]);
};
CHECK_TYPE(MenuInteraction, 64);

struct CMenu
{
	FIELD(menuID, BYTE); // + 0x0000
	FIELD(szTitle, char, [MAX_MENU_TEXT_SIZE]); // + 0x0001
	FIELD(szItems, char, [MAX_ITEMS][MAX_COLUMNS][MAX_MENU_TEXT_SIZE]); // + 0x0021
	FIELD(szHeaders, char, [MAX_COLUMNS][MAX_MENU_TEXT_SIZE]); // + 0x0321
	FIELD(bIsInitiedForPlayer, BOOL, [MAX_PLAYERS]); // + 0x0361
	FIELD(interaction, MenuInteraction); // + 0x0B31
	FIELD(vecPos, CVector2D);
	FIELD(fColumn1Width, float); // + 0x0B79
	FIELD(fColumn2Width, float); // + 0x0B7D
	FIELD(byteColumnsNumber, BYTE); // + 0x0B81
	FIELD(byteItemsCount, BYTE, [MAX_COLUMNS]); // + 0x0B82
};
CHECK_TYPE(CMenu, 4948);

struct CMenuPool
{
	FIELD(pMenu, CMenu*, [MAX_MENUS]); //	+ 0x0000
	FIELD(bIsCreated, BOOL, [MAX_MENUS]); //	+ 0x0200
	FIELD(bPlayerMenu, BOOL, [MAX_PLAYERS]); //	+ 0x0400
};
CHECK_TYPE(CMenuPool, 5024);

/* -------------------------------------------------------- */
// CTextDraw
/* -------------------------------------------------------- */
struct CTextDrawPool
{
	FIELD(bSlotState, BOOL, [MAX_TEXT_DRAWS]);
	FIELD(TextDraw, CTextdraw*, [MAX_TEXT_DRAWS]);
	FIELD(szFontText, char*, [MAX_TEXT_DRAWS]);
	FIELD(bHasText, bool, [MAX_TEXT_DRAWS][MAX_PLAYERS]);
};
CHECK_TYPE(CTextDrawPool, 2072576);

/* -------------------------------------------------------- */
// C3DText
/* -------------------------------------------------------- */

struct C3DTextPool
{
	FIELD(bIsCreated, BOOL, [MAX_3DTEXT_GLOBAL]); // 0 - 4096 <- OK
	FIELD(TextLabels, C3DText, [MAX_3DTEXT_GLOBAL]);
};
CHECK_TYPE(C3DTextPool, 37888);

/* -------------------------------------------------------- */
// CGangZone
/* -------------------------------------------------------- */

struct CSAMPGangZonePool
{
	FIELD(fGangZone, float, [MAX_GANG_ZONES][4]);
	FIELD(bSlotState, BOOL, [MAX_GANG_ZONES]);
};
CHECK_TYPE(CSAMPGangZonePool, 20480);

/* -------------------------------------------------------- */
// CActor
/* -------------------------------------------------------- */

struct CActorAnim // 140
{
	FIELD(szAnimLib, char, [64 + 1]); // 0 - 64
	FIELD(szAnimName, char, [64 + 1]); // 64 - 128
	FIELD(fDelta, float); // 128 - 132
	FIELD(byteLoop, BYTE); // 132 - 133
	FIELD(byteLockX, BYTE); // 133 - 134
	FIELD(byteLockY, BYTE); // 134 - 135
	FIELD(byteFreeze, BYTE); // 135 - 136
	FIELD(iTime, int); //  136 - 140
};
CHECK_TYPE(CActorAnim, 142);

struct CActor
{
	FIELD(pad0, BYTE);				
	FIELD(iSkinID, int);			
	FIELD(vecSpawnPos, CVector);	
	FIELD(fSpawnAngle, float);		
	FIELD(pad4, DWORD);				
	FIELD(pad5, DWORD);				
	FIELD(byteLoopAnim, BYTE);		
	FIELD(anim, CActorAnim);
	FIELD(fHealth, float);			
	FIELD(pad, DWORD);				
	FIELD(fAngle, float);			
	FIELD(vecPos, CVector);	
	FIELD(pad8, BYTE, [12]);			
	FIELD(byteInvulnerable, BYTE);	
	FIELD(wActorID, WORD);			
};
CHECK_TYPE(CActor, 211);

struct CActorPool
{
	FIELD(iActorVirtualWorld, int, [MAX_ACTORS]);
	FIELD(bValidActor, BOOL, [MAX_ACTORS]);
	FIELD(pActor, CActor*, [MAX_ACTORS]);
	FIELD(dwActorPoolSize, DWORD);
};
CHECK_TYPE(CActorPool, 12004);

struct CGameMode
{
	FIELD(amx, AMX);
	FIELD(bInitialised, bool);
	FIELD(bSleeping, bool);
	FIELD(fSleepTime, float);
};
CHECK_TYPE(CGameMode, sizeof(CGameMode));

struct CFilterScripts
{
	FIELD(pFilterScripts, AMX*, [MAX_FILTER_SCRIPTS]);
	FIELD(szFilterScriptName, char, [MAX_FILTER_SCRIPTS][255]);
	FIELD(iFilterScriptCount, int);
};
CHECK_TYPE(CFilterScripts, sizeof(CFilterScripts));

struct ScriptTimer_s
{
	FIELD(szScriptFunc, char, [255]);
	FIELD(iTotalTime, int);
	FIELD(iRemainingTime, int);
	FIELD(bRepeating, BOOL);
	FIELD(bKilled, BOOL);
	FIELD(pAMX, AMX*);
	FIELD(iParamCount, int);
	FIELD(cellParams, void*);
};
CHECK_TYPE(ScriptTimer_s, 283);

typedef std::map<DWORD, ScriptTimer_s*> DwordTimerMap;

struct CScriptTimers
{
	FIELD(Timers, DwordTimerMap);
	FIELD(dwTimerCount, DWORD);
};

struct CNetGame
{
	FIELD(pGameModePool, CGameMode*); // 0
	FIELD(pFilterScriptPool, CFilterScripts*); // 4
	FIELD(pPlayerPool, CPlayerPool*); // 8
	FIELD(pVehiclePool, CVehiclePool*); // 12
	FIELD(pPickupPool, CPickupPool*); // 16
	FIELD(pObjectPool, CObjectPool*); // 20
	FIELD(pMenuPool, CMenuPool*); // 24
	FIELD(pTextDrawPool, CTextDrawPool*); // 28
	FIELD(p3DTextPool, C3DTextPool*); // 32
	FIELD(pGangZonePool, CSAMPGangZonePool*); // 36 
	FIELD(pActorPool, CActorPool*); // 40 
	FIELD(iCurrentGameModeIndex, int); // 44
	FIELD(iCurrentGameModeRepeat, int); // 48
	FIELD(bFirstGameModeLoaded, BOOL); // 52
	FIELD(pHttpClient, void*); // 56
	FIELD(pScriptTimers, CScriptTimers*); // 60
	FIELD(pRak, void*); // 64
	FIELD(dwSomethingTick, DWORD);
	FIELD(dwUnk, DWORD);
	FIELD(dwUnk1, DWORD);
	FIELD(bLanMode, BOOL); // 
	FIELD(bShowPlayerMarkers, BOOL); // 84
	FIELD(byteShowNameTags, BYTE); // 
	FIELD(byteWorldTimeHour, BYTE); // 
	FIELD(byteAllowWeapons, BYTE); // 
	FIELD(byteStuntBonus, BYTE); // 91 - 92
	FIELD(byteDefaultCameraCollision, BYTE); // 92 - 93
	FIELD(byteWeather, BYTE); // 93 - 94
	FIELD(iGameState, int); // 94 - 98
	FIELD(fGravity, float); // 98 - 102
	FIELD(iDeathDropMoney, int); // 102 - 106
	FIELD(byteEnableZoneNames, BYTE); // 106 - 107
	FIELD(byteMode, BYTE); // 107 - 108
	FIELD(bLimitGlobalChatRadius, BYTE); // 108 - 109
	FIELD(bUseCJWalk, BYTE); // 109 - 110
	FIELD(fGlobalChatRadius, float); // 110 - 114
	FIELD(fNameTagDrawDistance, float); // 114 - 118
	FIELD(byteDisableEnterExits, BYTE); // 118 - 119
	FIELD(byteNameTagLOS, BYTE); // 119 - 120
	FIELD(bManulVehicleEngineAndLights, BYTE); // 120 - 121
	FIELD(bLimitPlayerMarkers, BYTE); // 121 - 122
	FIELD(fPlayerMarkesLimit, float); // 122 - 126 
	FIELD(bVehicleFriendlyFire, BOOL); // 126 - 130
#ifdef SAMP_03DL
	FIELD(dwUnk3, DWORD); // 130 - 134
#endif
#ifndef _WIN32
	FIELD(dElapsedTime, double);
#endif
	FIELD(iSpawnsAvailable, int); // 130 - 134
	FIELD(AvailableSpawns, CPlayerSpawnInfo, [319]); // 134 - 14808
};
#ifdef _WIN32
#ifndef SAMP_03DL
CHECK_TYPE(CNetGame, 14808);
#else
CHECK_TYPE(CNetGame, 16088);
#endif
#else
#ifndef SAMP_03DL
CHECK_TYPE(CNetGame, 14816);
#else
CHECK_TYPE(CNetGame, 16096);
#endif
#endif

#ifdef SAMP_03DL
enum MODEL_TYPE : BYTE { MODEL_TYPE_CHAR = 1, MODEL_TYPE_SIMPLE = 2 };
CHECK_TYPE(MODEL_TYPE, 1);

struct CModelInfo
{
	FIELD(bType, MODEL_TYPE); // 0 - 1
	FIELD(dwVirtualWorld, DWORD); // 1 - 5
	FIELD(dwBaseId, DWORD); // 5 - 9
	FIELD(dwNewId, DWORD); // 9 - 13
	FIELD(szDffName, char, [MAX_PATH + 1]); // 13 - 274
	FIELD(szTxdName, char, [MAX_PATH + 1]); // 274 - 535
	FIELD(dwDffCrc, DWORD); // 535 - 539
	FIELD(dwTxdCrc, DWORD); // 539 - 543
	FIELD(dwDffLength, DWORD); // 543 - 547
	FIELD(dwTxdLength, DWORD); // 547 - 551
	FIELD(bTimeOn, BYTE); // 551 - 552
	FIELD(bTimeOff, BYTE); // 552 - 553
};
CHECK_TYPE(CModelInfo, 553);

struct CArtList
{
	CModelInfo **pModelList; // 0 - 4
	FIELD(dwCapacity, DWORD); // 4 - 8
};
CHECK_TYPE(CArtList, 8);

struct CArtInfo
{
	FIELD(szArtPath, char, [MAX_PATH]); // 0 - 260
	FIELD(bUnknown, BYTE); // 260 - 261
	FIELD(artList, CArtList); // 261 - 269
};
CHECK_TYPE(CArtInfo, 269);
#endif

#undef CHECK_TYPE
#undef PAD
#undef BITFIELD
#undef OTHERFIELD
#undef FIELD
#pragma pack(pop)

#endif
