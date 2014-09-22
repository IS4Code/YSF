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

#ifndef __YSF_SCRIPTING
#define __YSF_SCRIPTING

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

#define ABGR_RGBA(color) (((DWORD)color & 0xff) << 24) | (((DWORD)(color >> 8) & 0xff) << 16) | (((DWORD)(color >> 16) & 0xff) << 8) | (color >> 24) & 0xff
#define RGBA_ABGR(color) (((color >> 24) & 0x000000FF) | ((color >> 8) & 0x0000FF00) | ((color << 8) & 0x00FF0000) | ((color << 24) & 0xFF000000))

enum
{
	BS_BOOL,
	BS_CHAR,
	BS_UNSIGNEDCHAR,
	BS_SHORT,
	BS_UNSIGNEDSHORT,
	BS_INT,
	BS_UNSIGNEDINT,
	BS_FLOAT,
	BS_STRING
};

#ifndef CHECK_PARAMS
	#define CHECK_PARAMS(m,n)                                                                                           \
		do                                                                                                              \
		{                                                                                                               \
			if (params[0] != (m * 4))                                                                                   \
			{                                                                                                           \
				logprintf(PROJECT_NAME " Error: Incorrect parameter count on \"" n "\", %d != %d\n", m, params[0] / 4); \
				return 0;                                                                                               \
			}                                                                                                           \
		}                                                                                                               \
		while (0)
#endif

int InitScripting(AMX *amx);
extern AMX_NATIVE_INFO RedirectNatives[];
#endif