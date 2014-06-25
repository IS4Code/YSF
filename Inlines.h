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
#ifndef __YSF_INLINES
#define __YSF_INLINES

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

void AddServerRule(char * rule, char * value, int flags);
void SetServerRule(char * rule, char * value);
void SetServerRuleInt(char * rule, int value);
void ModifyFlag(char * rule, int varflag);
int LoadFilterscript(char * name);
int UnLoadFilterscript(char * name);
bool IsPlayerConnected(int playerid);
int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
const char* GetWeaponName(BYTE weaponid);
BYTE GetWeaponSlot(BYTE weaponid);

// Don't touch these, they're used to call fake methods
class FakeClass
{
};

#ifdef WIN32
	typedef void (FakeClass::* FakeMethod_int)(int);
	typedef void (FakeClass::* FakeMethod_float)(float);
	typedef void (FakeClass::* FakeMethod_void)();
	//typedef void (* FakeMethod_int)(int);
	//typedef void (* FakeMethod_float)(float);
	
	//#define CALL(m,n,o) ((FakeClass *)(n)->*m)(o)
#else
	typedef void (* FakeMethod_int)(FakeClass *, int);
	typedef void (* FakeMethod_float)(FakeClass *, float);
	typedef void (* FakeMethod_void)(FakeClass *);
	
	//#define CALL(m,n,o) m((FakeClass *)(n), o)
#endif

#ifdef WIN32
	typedef void (FakeClass::* AddServerRule_t)(char *, int, char *, int);
	typedef void (FakeClass::* SetServerRule_t)(char *, char *);
	typedef void (FakeClass::* SetServerRuleInt_t)(char *, int);
	typedef void (FakeClass::* RemoveServerRule_t)(char *);
	typedef void (FakeClass::* ModifyFlag_t)(char *, int);
	typedef int (FakeClass::* LoadFS_t)(char *);
	typedef int (FakeClass::* UnLoadFS_t)(char *);
#else
	typedef void (* AddServerRule_t)(FakeClass *, char *, int, char *, int);
	typedef void (* SetServerRule_t)(FakeClass *, char *, char *);
	typedef void (* SetServerInt_t)(FakeClass *, char *, int);
	typedef void (* RemoveServerRule_t)(FakeClass *, char *);
	typedef void (* ModifyFlag_t)(FakeClass *, char *, int);

	typedef int (* LoadFS_t)(FakeClass *, char *);
	typedef int (* UnLoadFS_t)(FakeClass *, char *);
#endif

//char *strdel(char *string, size_t first, size_t len);
#endif