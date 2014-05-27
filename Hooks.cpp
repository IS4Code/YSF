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
 *  the Initial Developer. All Rights Reserved.
 *  
 *  Contributor(s):
 *  
 *  Peter Beverloo
 *  Marcus Bauer
 *  MaVe;
 *  Sammy91
 *  Incognito
 *  
 *  Special Thanks to:
 *  
 *  SA:MP Team past, present and future
 */

#include "main.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <Windows.h>
#else
	#include <stdio.h>
	#include <sys/mman.h>
	#include <limits.h>
	#include <string.h>
#endif

#ifndef PAGESIZE
	#define PAGESIZE (4096)
#endif

//typedef void far            *LPVOID;

AddServerRule_t g_pCConsole__AddRule = 0;
SetServerRule_t g_pCConsole__SetRule = 0;
//RemoveServerRule_t g_pCConsole__RemoveRule = 0;
ModifyFlag_t g_pCConsole__MFlag = 0;
LoadFS_t g_pCFilterscript__LoadFS_t = 0;
UnLoadFS_t g_pCFilterscript__UnLoadFS_t = 0;

bool Unlock(void *address, int len)
{
	#ifdef WIN32
		DWORD
			oldp;
		// Shut up the warnings :D
		return !!VirtualProtect(address, len, PAGE_EXECUTE_READWRITE, &oldp);
	#else
		return !mprotect(*((void*)(&(((int)address / PAGESIZE) * PAGESIZE)), PAGESIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
	#endif
}

void GetAddresses()
{
	DWORD temp;

	// Console for adding rules.
	POINTER_TO_MEMBER(g_pCConsole__AddRule, CAddress::FUNC_CConsole_AddStringVariable, AddServerRule_t);
	POINTER_TO_MEMBER(g_pCConsole__SetRule, CAddress::FUNC_CConsole_SetStringVariable, SetServerRule_t);
	//POINTER_TO_MEMBER(g_pCConsole__RemoveRule, CONSOLE_REMOVE_RULE_0341, RemoveServerRule_t);
	POINTER_TO_MEMBER(g_pCConsole__MFlag, CAddress::FUNC_CConsole_ModifyVariableFlags, ModifyFlag_t);

	POINTER_TO_MEMBER(g_pCFilterscript__LoadFS_t, CAddress::FUNC_CFilterscripts_LoadFilterscript, LoadFS_t);
	POINTER_TO_MEMBER(g_pCFilterscript__UnLoadFS_t, CAddress::FUNC_CFilterscripts_UnLoadFilterscript, UnLoadFS_t);
	 
	// Totally not fix, but will be fixed someday..
#ifdef WIN32
	Unlock((void*)CAddress::VAR_pRestartWaitTime, 4);
#endif
}

void InstallPreHooks()
{
	GetAddresses();
}

void InstallPostHooks()
{
	// For things which need setting up after pNetGame is initiated
}
