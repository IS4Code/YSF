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

// ------------
// VERSION INFO
// ------------

#define PROJECT_NAME		"YSF"
#define PROJECT_VERSION		"2.0"

// ------------
// OS SPECIFICS
// ------------

#ifdef _WIN32
	#define OS_NAME "Windows"
#else
	#define OS_NAME "Linux"

	/*#include <dlfcn.h>
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
	#endif*/
#endif

// --------
// INCLUDES
// --------

// SDK
#include "sdk/plugin.h"

#endif
