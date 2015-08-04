#ifndef YSF_MAIN_H
#define YSF_MAIN_H

// -------
// DEFINES
// -------
#pragma warning(disable: 4995)

//#define NEW_PICKUP_SYSTEM

#define PI 3.14159265

#define ARRAY_SIZE(a)	( sizeof((a)) / sizeof(*(a)) )
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }
#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p) = NULL; } }
#define PAD(a, b) char a[b]

#define _OFFSET(p_type, p_member) (size_t)(&((p_type *)NULL)->p_member)
// ------------
// VERSION INFO
// ------------

#define PROJECT_NAME		"YSF"
#define PROJECT_VERSION		"R14-3"

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
	#include <sys/socket.h>
	#include <signal.h>
	#include <sys/types.h>
	#include <sys/sysinfo.h>
	#include <sys/stat.h>
	#include <dirent.h>
	#include <cstring>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/mman.h>
	#include <fnmatch.h>

	typedef unsigned long DWORD;
	typedef const char *PCHAR;
	typedef int SOCKET;

	#define INVALID_SOCKET -1

	#ifndef stricmp
		#define stricmp strcasecmp
	#endif
	
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;

	typedef unsigned long       DWORD;
	typedef int                 BOOL;
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
	typedef float               FLOAT;

	#ifndef MAX_PATH
		#define MAX_PATH          260
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
extern CPlayerData *pPlayerData[MAX_PLAYERS];

extern CNetGame *pNetGame;
extern void *pConsole;
extern RakServer *pRakServer;

#endif
