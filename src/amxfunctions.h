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

#ifndef YSF_AMXFUNCTIONS_H
#define YSF_AMXFUNCTIONS_H

#include <string>
#include <sdk/amx/amx.h>

//void Redirect(AMX * amx, char const * const from, ucell to, AMX_NATIVE * store);
cell* get_amxaddr(AMX *amx, cell amx_addr);
int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
int set_amxstring(AMX *amx, cell amx_addr, const wchar_t *source, int max);
int set_amxstring(AMX *amx, cell amx_addr, const std::string &source, int max);

#endif
