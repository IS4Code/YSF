#ifndef YSF_MAIN_H
#define YSF_MAIN_H

// -------
// DEFINES
// -------

//#define NEW_PICKUP_SYSTEM

#define PI 3.14159265

#define ARRAY_SIZE(a)	( sizeof((a)) / sizeof(*(a)) )
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }
#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p) = NULL; } }
#define PAD(a, b) char a[b]
// ------------
// VERSION INFO
// ------------

#define PROJECT_NAME		"YSF"
#define PROJECT_VERSION		"R11"

// ------------
// OS SPECIFICS
// ------------

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define SLEEP(x) { Sleep(x); }
	#define OS_NAME "Windows"

	#include <windows.h>
	#include <Psapi.h>
	#include <tchar.h>
	#include <Strsafe.h>
	#include <mmsystem.h>
	#include <malloc.h>
	#include <shellapi.h>
	#include <time.h>
#else
	#define SLEEP(x) { usleep(x * 1000); }
	#define MAX_PATH 260
	#define OS_NAME "Linux"

	#include <dlfcn.h>
	#include <unistd.h>
	#include <sys/time.h>
	#include <sys/times.h>
	#include <signal.h>
	#include <sys/types.h>
	#include <sys/sysinfo.h>
	#include <dirent.h>

	typedef int SOCKET;

	#ifndef stricmp
		#define stricmp strcasecmp
	#endif
#endif

// --------
// INCLUDES
// --------

// Regular crap
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

// STL
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

// SAMP GDK
#include <sampgdk/sampgdk.h>
using sampgdk::logprintf;
	
// Subhook
#include <subhook/subhook.h>

// YSF
#include "Addresses.h"
#include "amxfunctions.h"
#include "CCallbackManager.h"
#include "CGangZonePool.h"
#include "CModelSizes.h"
#include "CPickupPool.h"
#include "CPlayerData.h"
#include "CTypes.h"
#include "CVector.h"
#include "Functions.h"
#include "Hooks.h"
#include "RPCs.h"
#include "Scripting.h"
#include "Structs.h"
#include "Utils.h"

// ---------
// EXTERNALS
// ---------

extern void **ppPluginData;

extern CServer *pServer;
extern CPlayerData *pPlayerData[500];

extern CNetGame *pNetGame;
extern void *pConsole;
extern RakServer *pRakServer;

#endif