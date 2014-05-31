//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2007 SA:MP Team
//
//----------------------------------------------------------

#include "main.h"
#include <fstream>
#include "CGangZonePool.h"

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

#ifdef LINUX
	#include <cstring>
	typedef unsigned char *PCHAR;
#endif

//----------------------------------------------------------

logprintf_t logprintf;
void **ppPluginData;
extern void *pAMXFunctions;

// Server instance
CServer *pServer = NULL;

// Internal server pointers
CSAMPServer *pNetGame = NULL;
void *pConsole = NULL;
RakServer *pRakServer = NULL;
CPlayerData *pPlayerData[MAX_PLAYERS];

// AMX list
std::list<AMX*> pAMXList;
//----------------------------------------------------------
// The Support() function indicates what possibilities this
// plugin has. The SUPPORTS_VERSION flag is required to check
// for compatibility with the server. 

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

//----------------------------------------------------------
// The Load() function gets passed on exported functions from
// the SA-MP Server, like the AMX Functions and logprintf().
// Should return true if loading the plugin has succeeded.

void *InternalNetGame = NULL;
void *InternalConsole = NULL;
void *InternalRakServer = NULL;

PLUGIN_EXPORT bool PLUGIN_CALL Load(void ** ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf("logprintf = 0x%08X\n", *((int*)(&logprintf)));

	// Store internal pointers
	InternalNetGame = ppData[PLUGIN_DATA_NETGAME];
	InternalConsole = ppData[PLUGIN_DATA_CONSOLE];
	InternalRakServer = ppData[PLUGIN_DATA_RAKSERVER];

	// Check server version
	eSAMPVersion version = SAMP_VERSION_UNKNOWN;
	char szVersion[16];
	if(logprintf == (logprintf_t)CAddress::FUNC_Logprintf_03Z)
	{
		version = SAMP_VERSION_03Z;
		strcpy(szVersion, "0.3z");
	}
	else if(logprintf == (logprintf_t)CAddress::FUNC_Logprintf_03ZR2_2)
	{
		version = SAMP_VERSION_03Z_R2_2;
		strcpy(szVersion, "0.3z R2-2");
	}

	// If not unknown, then initalize things
	if(version != SAMP_VERSION_UNKNOWN)
	{
		CAddress::Initialize(version);

		// Create server instance
		pServer = new CServer();
	}
	else
	{
		logprintf("Error: Unknown " OS_NAME " server version\n");
		return true;
	}

	InstallPreHooks();

	logprintf("\n");
	logprintf(" ===============================\n");
	logprintf("        " PROJECT_NAME " - kurta999 version " PROJECT_VERSION " loaded\n");
	logprintf("   (c) 2008 Alex \"Y_Less\" Cole - (c) 2010 - 2014 kurta999\n");
	logprintf("    Server version: %s\n", szVersion);
	logprintf("    Operating System: " OS_NAME "\n");
	logprintf("    Built on: " __DATE__ " at "__TIME__ "\n");
	logprintf(" ===============================\n");
	return true;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// Corrected apperance in log file
	logprintf("\n");
	logprintf(" ==============\n");
	logprintf("  %s unloaded\n", PROJECT_NAME);
	logprintf(" ==============");

	delete pServer;
	pServer = NULL;
}

//----------------------------------------------------------
// The AmxLoad() function gets called when a new gamemode or
// filterscript gets loaded with the server. In here we register
// the native functions we like to add to the scripts.

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX * amx) 
{
	if(!pServer) goto Ide;

	// Add AMX instance to our amxlist
	pAMXList.push_back(amx);
	//logprintf("AMXLoad1");

	static bool bFirst = false;
	if(!bFirst)
	{
		bFirst = true;
		//logprintf("AMXLoad2");

		// Get pNetGame
		int (*pfn_GetNetGame)(void) = (int (*)(void))InternalNetGame;
		pNetGame = (CSAMPServer*)pfn_GetNetGame();

		// Get pConsole
		int (*pfn_GetConsole)(void) = (int (*)(void))InternalConsole;
		pConsole = (void*)pfn_GetConsole();

		// Get pRakServer
		int (*pfn_GetRakServer)(void) = (int (*)(void))InternalRakServer;
		pRakServer = (RakServer*)pfn_GetRakServer();

		logprintf("YSF - pNetGame: 0x%X, pConsole: 0x%X, pRakServer: 0x%X", pNetGame, pConsole, pRakServer);

		// Recreate GangZone pool
		pNetGame->pGangZonePool = new CGangZonePool();
	}
	else
	{
#ifdef asdasd
		if(pNetGame->pPlayerPool->bIsPlayerConnected[4])
		{
			CSAMPPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[4];
			logprintf("playercolor: %d, spectype: %d, specid: %d", pPlayer->iNickNameColor, pPlayer->byteSpectateType, pPlayer->SpectateID);

			//logprintf("3dtext exists: %d, text: %s", pPlayer->p3DText->isCreated[1], pPlayer->p3DText->TextLabels[1].text);
			/*
			logprintf("td: %d, pos: %f, text: %s", pPlayer->pTextdraw->m_bSlotState[1], pPlayer->pTextdraw->m_TextDraw[1]->fX, pPlayer->pTextdraw->m_szFontText[1]);
			logprintf("ispresent: %d", pPlayer->pTextdraw->m_bHasText[1]);

			logprintf("vec: %f, %f, %f", pPlayer->vecBulletStart.fX, pPlayer->vecBulletStart.fY, pPlayer->vecBulletStart.fZ);
			logprintf("fcp: %f, %f, %f, %f", pPlayer->vecCPPos.fX, pPlayer->vecCPPos.fY, pPlayer->vecCPPos.fZ, pPlayer->fCPSize);
			logprintf("fracecp: %f, %f, %f - %f, %f, %f - size: %f, type: %d", pPlayer->vecRaceCPPos.fX, pPlayer->vecRaceCPPos.fY, pPlayer->vecRaceCPPos.fZ, 
				pPlayer->vecRaceCPNextPos.fX, pPlayer->vecRaceCPNextPos.fY, pPlayer->vecRaceCPNextPos.fZ, pPlayer->fRaceCPSize, pPlayer->byteRaceCPType);
			logprintf("bInModShop: %d - %d", pPlayer->bIsInModShop, pPlayer->vmifasssag);
			*/
		}
#endif
		/*
		CMenu *pMenu = pNetGame->pMenuPool->menu[1];
		logprintf("menu: %d", pNetGame->pMenuPool->isCreated[1]);
		logprintf("wid: %f, %f, colums: %d, headers: %s, %s", pMenu->column1Width, pMenu->column2Width, pMenu->columnsNumber, pMenu->headers[0], pMenu->headers[1]);
		logprintf("item: %s, count: %d, pos: %f, %f, title: %s", pMenu->items[0][1], pMenu->itemsCount[0], pMenu->posX, pMenu->posY, pMenu->title);
		
		for(int i = 0; i != 1024; i++)
		{
			if(!pNetGame->p3DTextPool->m_bIsCreated[i]) continue;

			Text3DLabels_t pText = pNetGame->p3DTextPool->m_TextLabels[i];
			logprintf("created %d", i);
			logprintf("text: %s", pText.text); 
			logprintf("pos: %f, %f, %f, color: %d, drawdistance: %f, los: %d, attach: %d, %d", pText.posX, pText.posY, pText.posZ, pText.color, pText.drawDistance, pText.useLineOfSight, pText.attachedToPlayerID, pText.attachedToVehicleID);
		}
		*/
		/*
		for(int i = 0; i != 1024; i++)
		{
			if(!pNetGame->pTextDrawPool->m_bSlotState[i]) continue;

			CTextdraw *pTD = pNetGame->pTextDrawPool->m_TextDraw[i];
			logprintf("td: %d, str: %s, pos: %f, %f", i, pNetGame->pTextDrawPool->m_szFontText[i], pTD->fX, pTD->fY);


			logprintf("", pTD->byteBox
		}
		*/
		/*
		for(int i = 0; i != 1000; i++)
		{
			if(!pNetGame->pObjectPool->m_bObjectSlotState[i]) continue;

			CObject *object = pNetGame->pObjectPool->m_pObjects[i];
			logprintf("txd: %s, texture: %s, something: %d, %d, vmigeci: %d", object->szTXDName, object->szTextureName, object->something1, object->something2, object->vmigeci);

			//for(int x = 0; x != 6; x++)
				//logprintf("mat %d - %X", x, object->paddin__g[x]);
		}
		*/
		/*
		if(pNetGame->pVehiclePool->pVehicle[500]) 
		{
			CSAMPVehicle *pVeh = pNetGame->pVehiclePool->pVehicle[500];
			logprintf("col: %d, %d, int: %d, respawn: %d", pNetGame->pVehiclePool->pVehicle[500]->color1, pNetGame->pVehiclePool->pVehicle[500]->color2, pNetGame->pVehiclePool->pVehicle[500]->interior, pNetGame->pVehiclePool->pVehicle[500]->respawndelay);
			logprintf("spawn: %f, %f, %f", pNetGame->pVehiclePool->pVehicle[500]->vecSpawnPos.fX, pNetGame->pVehiclePool->pVehicle[500]->vecSpawnPos.fY, pNetGame->pVehiclePool->pVehicle[500]->vecSpawnPos.fZ);
			logprintf("plate: %s, occ: %d, respawn: %d, lastdriver: %d", pVeh->szNumberplate, pVeh->vehOccupiedTick, pVeh->vehRespawnTick, pVeh->usLastDriverID);
			/*
			for(int i = 0; i != 42; i++)
				logprintf("plate %d: %d", i, pVeh->pad1[i]);
			
			
			if(pNetGame->pPlayerPool->bIsPlayerConnected[4])
			{
				CSAMPPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[4];
				logprintf("model: %d, bone: %d, %f, %f, %f, materialcolor: %d, %d, slotstate: %d", pPlayer->attachedObject[0].iModelID, pPlayer->attachedObject[0].iBoneiD, pPlayer->attachedObject[0].vecPos.fX, pPlayer->attachedObject[0].vecPos.fY, pPlayer->attachedObject[0].vecPos.fZ, pPlayer->attachedObject[0].dwMaterialColor1, pPlayer->attachedObject[0].dwMaterialColor2, pPlayer->attachedObjectSlot[0]);
				//logprintf("pos: %f, %f, %f, %f", pNetGame->pPlayerPool->pPlayer[4]->vecCPPos.fX, pNetGame->pPlayerPool->pPlayer[4]->vecCPPos.fY, pNetGame->pPlayerPool->pPlayer[4]->vecCPPos.fZ, pNetGame->pPlayerPool->pPlayer[4]->fCPSize);
				logprintf("quat: %f, %f, %f, %f", pPlayer->fQuaternion[0], pPlayer->fQuaternion[1], pPlayer->fQuaternion[2], pPlayer->fQuaternion[3]);
			}
		
			for(int i = 0; i != 11; i++)
			{
				logprintf("skillevel %d - %d", i, pNetGame->pPlayerPool->pPlayer[4]->wSkillLevel[i]);
			}
			*/
			/*
			for(int i = 0; i != 1024; i++)
			{
				if(!pNetGame->pPickupPool->m_bActive[i]) continue;
				logprintf("ppos %d - %f, %f, %f", i, pNetGame->pPickupPool->m_Pickup[i].vecPos.fX, pNetGame->pPickupPool->m_Pickup[i].position.fY, pNetGame->pPickupPool->m_Pickup[i].position.fZ);
			}
			*/
			/*
			for(int i = 0; i != 1000; i++)
			{
				if(!pNetGame->pObjectPool->m_bObjectSlotState[i]) continue;

				CObject *object = pNetGame->pObjectPool->m_pObjects[i];

				logprintf("pos %d: %d - %f, %f, %f, movespeed: NA", i, pNetGame->pObjectPool->m_pObjects[i]->m_iModel, pNetGame->pObjectPool->m_pObjects[i]->m_vecPos.fX, pNetGame->pObjectPool->m_pObjects[i]->m_vecPos.fY, pNetGame->pObjectPool->m_pObjects[i]->m_vecPos.fZ);
		
				logprintf("drawdistance: %f - %f, %f, %f", object->m_fDrawDistance, pNetGame->pObjectPool->m_pObjects[i]->m_vecRot.fX, pNetGame->pObjectPool->m_pObjects[i]->m_vecRot.fY, pNetGame->pObjectPool->m_pObjects[i]->m_vecRot.fZ);
				//logprintf("geci: %f, %d, %d", pNetGame->pObjectPool->m_pObjects[i]->m_fDrawDistance, pNetGame->pObjectPool->m_pObjects[i]->m_usAttachedVehicleID, pNetGame->pObjectPool->m_pObjects[i]->m_usAttachedObjectID);
				logprintf("attached: %d, %d", object->m_usAttachedVehicleID, object->m_usAttachedObjectID);
				logprintf("offset: %f, %f, %f, %f, %f, %f", object->m_vecAttachedOffset.fX, object->m_vecAttachedOffset.fY, object->m_vecAttachedOffset.fZ, object->m_vecAttachedRotation.fX, object->m_vecAttachedRotation.fY, object->m_vecAttachedRotation.fZ);
				logprintf("ismoving: %d, %f, %f, %f, %f, %f", object->m_bIsMoving, object->unk1, object->unk2, object->unk3, object->unk4);
			}
			logprintf("spawnpos: %f", pNetGame->pPlayerPool->pPlayer[0]->vecSpawnPosition.fX);
			
			
		}
		*/
	}
Ide:
	return InitScripting(amx);
}

//----------------------------------------------------------
// When a gamemode is over or a filterscript gets unloaded, this
// function gets called. No special actions needed in here.

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX * amx) 
{
	// Remove AMX instance from our amxlist
	pAMXList.remove(amx);
	return AMX_ERR_NONE;
}

BYTE g_Ticks = 0;
PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	if(++g_Ticks == 10)
	{
		//logprintf("process");
		g_Ticks = 0;
		for(WORD playerid = 0; playerid != MAX_PLAYERS; playerid++)
		{
			CPlayerData *pPlayer = pPlayerData[playerid];
			if(!pPlayer) continue;
			/*
			CSAMPPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[4];
			if(pPlayer)
			{
				logprintf("keys: %d, oldkeys: %d", pPlayer->dwKeys, pPlayer->dwOldKeys);
				//logprintf("siren: %d, gearstate: %d, trailer: %d, trainspeed: %f, hydrareactor: %d, %d", pPlayer->vehicleSyncData.byteSirenState, pPlayer->vehicleSyncData.byteGearState, pPlayer->vehicleSyncData.wTrailerID, 
					//pPlayer->vehicleSyncData.fTrainSpeed, pPlayer->vehicleSyncData.wHydraReactorAngle[0], pPlayer->vehicleSyncData.wHydraReactorAngle[1]);
				//logprintf("surfinginfo: %d, %f, %f, %f", pPlayer->syncData.wSurfingInfo, pPlayer->syncData.vecSurfing.fX, pPlayer->syncData.vecSurfing.fY, pPlayer->syncData.vecSurfing.fZ);
			}
			*/
			for(WORD zoneid = 0; zoneid != MAX_GANG_ZONES; zoneid++)
			{
				// If zone id is unused client side, then continue
				if(pPlayer->byteClientSideZoneIDUsed[zoneid] == 0xFF) continue;

				CGangZone *pGangZone = NULL;
				if(pPlayer->byteClientSideZoneIDUsed[zoneid] == 0)
				{
					if(pPlayer->wClientSideGlobalZoneID[zoneid] = 0xFFFF)
					{
						logprintf("pPlayer->wClientSideGlobalZoneID[zoneid] = 0xFFFF");
						return;
					}
					
					pGangZone = pNetGame->pGangZonePool->pGangZone[pPlayer->wClientSideGlobalZoneID[zoneid]];
				}
				else
				{
					if(pPlayer->wClientSidePlayerZoneID[zoneid] == 0xFFFF)
					{
						logprintf("pPlayer->wClientSidePlayerZoneID[zoneid]");
						return;
					}

					pGangZone = pPlayer->pPlayerZone[pPlayer->wClientSidePlayerZoneID[zoneid]];
				}

				if(!pGangZone) continue;

				// Mutatók létrehozása
				CVector *vecPos = &pNetGame->pPlayerPool->pPlayer[playerid]->vecPosition;
				float *fMinX = &pGangZone->fGangZone[0];
				float *fMinY = &pGangZone->fGangZone[1];
				float *fMaxX = &pGangZone->fGangZone[2];
				float *fMaxY = &pGangZone->fGangZone[3];

				//logprintf("validzone: %d, %f, %f, %f, %f", pPlayer->wClientSideGlobalZoneID[zoneid], *fMinX, *fMinY, *fMaxX, *fMaxY);

				// Ha benne van
				if(vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY && !pPlayer->bInGangZone[zoneid])
				{
					pPlayer->bInGangZone[zoneid] = true;
					//logprintf("enterzone: %d", zoneid);
					
					if(pPlayer->byteClientSideZoneIDUsed[zoneid] == 0)
					{
						// Call callback
						int idx = -1;
						std::list<AMX*>::iterator second;
						for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
						{
							if(!amx_FindPublic(*second, "OnPlayerEnterGangZone", &idx))
							{
								cell
									ret;
								amx_Push(*second, pPlayer->wClientSideGlobalZoneID[zoneid]);
								amx_Push(*second, playerid);

								amx_Exec(*second, &ret, idx);
							}
						}
					}
					else
					{
						// Call callback
						int idx = -1;
						std::list<AMX*>::iterator second;
						for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
						{
							if(!amx_FindPublic(*second, "OnPlayerEnterPlayerGangZone", &idx))
							{
								cell
									ret;
								amx_Push(*second, pPlayer->wClientSidePlayerZoneID[zoneid]);
								amx_Push(*second, playerid);

								amx_Exec(*second, &ret, idx);
							}
						}					
					}
				}
				else if(!(vecPos->fX >= *fMinX && vecPos->fX <= *fMaxX && vecPos->fY >= *fMinY && vecPos->fY <= *fMaxY) && pPlayer->bInGangZone[zoneid])
				{
					pPlayer->bInGangZone[zoneid] = false;
					//logprintf("leavezone: %d", zoneid);
					
					if(pPlayer->byteClientSideZoneIDUsed[zoneid] == 0)
					{
						// Call callback
						int idx = -1;
						std::list<AMX*>::iterator second;
						for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
						{
							if(!amx_FindPublic(*second, "OnPlayerLeaveGangZone", &idx))
							{
								cell
									ret;
								amx_Push(*second, pPlayer->wClientSideGlobalZoneID[zoneid]);
								amx_Push(*second, playerid);

								amx_Exec(*second, &ret, idx);
							}
						}
					}
					else
					{
						// Call callback
						int idx = -1;
						std::list<AMX*>::iterator second;
						for(second = pAMXList.begin(); second != pAMXList.end(); ++second)
						{
							if(!amx_FindPublic(*second, "OnPlayerLeavePlayerGangZone", &idx))
							{
								cell
									ret;
								amx_Push(*second, pPlayer->wClientSidePlayerZoneID[zoneid]);
								amx_Push(*second, playerid);

								amx_Exec(*second, &ret, idx);
							}
						}					
					}
				}
			}
		}
	}
}