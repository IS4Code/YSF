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

#ifndef YSF_SCRIPTING_H
#define YSF_SCRIPTING_H

#include <sdk/amx/amx.h>

typedef cell AMX_NATIVE_CALL(*AMX_Function_t)(AMX *amx, cell *params);

#define AMX_DECLARE_NATIVE(name) \
	cell AMX_NATIVE_CALL name(AMX* amx, cell* params)

#define AMX_DEFINE_NATIVE(name) \
	{#name, Natives::name},

#define AMX_DEFINE_HOOK(name) \
	{#name, Original::name, Hooks::name},

#define CHECK_PARAMS(m, flag) \
	if(CScriptParams::Get()->Setup(m, __FUNCTION__, CScriptParams::Flags::flag, amx, params)) return CScriptParams::Get()->HandleError()
	
struct AMX_HOOK_INFO
{
	const char _FAR *name PACKED;
	AMX_NATIVE &originalfunc PACKED;
	AMX_NATIVE func PACKED;
};

void ActorsLoadNatives();
void FixesLoadNatives();
void GangZonesLoadNatives();
void MenusLoadNatives();
void MiscLoadNatives();
void ModelSizesLoadNatives();
void ObjectsLoadNatives();
void PickupsLoadNatives();
void PlayersLoadNatives();
void RakNetLoadNatives();
void ScoreBoardLoadNatives();
void ServerLoadNatives();
void TextDrawsLoadNatives();
void TextLabelsLoadNatives();
void VehiclesLoadNatives();
void YSFSettingsLoadNatives();

void LoadNatives();

bool ApplyHooks(AMX_NATIVE_INFO &native);

void RegisterHooks(const AMX_HOOK_INFO *hooks_list, size_t count);

template <size_t Count>
void RegisterHooks(const AMX_HOOK_INFO (&hooks_list)[Count])
{
	RegisterHooks(hooks_list, Count);
}

int RegisterAllNatives(AMX *amx);

void RegisterNatives(const AMX_NATIVE_INFO *natives_list, int count);

template <size_t Count>
void RegisterNatives(const AMX_NATIVE_INFO(&natives_list)[Count])
{
	RegisterNatives(natives_list, static_cast<int>(Count));
}

#endif
