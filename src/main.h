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

#ifndef PAGESIZE
	#define PAGESIZE (4096)
#endif
// ------------
// VERSION INFO
// ------------

#define PROJECT_NAME		"YSF"
#define PROJECT_VERSION		"R19-2"

// ------------
// OS SPECIFICS
// ------------

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define SLEEP(x) { Sleep(x); }
	#define popen _popen
	#define pclose  _pclose
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
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <thread>
#include <mutex>
	
// Subhook
#include <subhook/subhook.h>

// YSF
#include "CAddresses.h"
#include "amxfunctions.h"
#include "CScriptParams.h"
#include "CCallbackManager.h"
#include "CGangZonePool.h"
#include "CModelSizes.h"
#include "CPickupPool.h"
#include "CPlayerData.h"
#include "CVector.h"
#include "CServer.h"
#include "CFunctions.h"
#include "Hooks.h"
#include "RPCs.h"
#include "Natives.h"
#include "Structs.h"
#include "Utils.h"

// ---------
// EXTERNALS
// ---------
extern void *pAMXFunctions;
extern void **ppPluginData;
	
extern CNetGame *pNetGame;
extern void *pConsole;
extern void *pRakServer;

extern CPlayerData *pPlayerData[MAX_PLAYERS];

#endif
