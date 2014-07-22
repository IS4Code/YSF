#ifndef __YSF_MAINH
#define __YSF_MAINH

#include <stdio.h>
#include <vector>
#include <list>
#include <iostream>
#include <stdlib.h>

// Includes
#include "Addresses.h"
#include "CPlayerData.h"
#include "CServer.h"
#include "CVector.h"
#include "Hooks.h"
#include "RPCs.h"
#include "Scripting.h"
#include "CModelSizes.h"

// Defines
#define CON_VARFLAG_DEBUG		1
#define CON_VARFLAG_READONLY	2
#define CON_VARFLAG_RULE		4

#ifdef WIN32
	#define OS_NAME			"Windows"
#else
	#define OS_NAME			"Linux"
	#include <algorithm>
#endif

#define PROJECT_NAME		"YSF"
#define PROJECT_VERSION		"R9"

//
template <typename T> 
const bool Contains( std::vector<T>& Vec, const T& Element ) 
{
    if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
        return true;

    return false;
}

typedef void (* logprintf_t)(char *, ...);
extern logprintf_t logprintf;

// Server instance
extern CServer *pServer;

// Internal server pointers
extern CNetGame *pNetGame;
extern void *pConsole;
extern RakServer *pRakServer;
extern CPlayerData *pPlayerData[MAX_PLAYERS];

// AMX list
extern std::list<AMX*> pAMXList;

typedef unsigned short PLAYERID;

#ifdef LINUX
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;


typedef int                 INT;
typedef unsigned int        UINT;
#endif

#endif