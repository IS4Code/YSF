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
#define PROJECT_VERSION		"2.1"

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
