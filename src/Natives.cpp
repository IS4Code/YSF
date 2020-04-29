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

#include <unordered_map>
#include <string>
#include <vector>

#include "Natives.h"
#include "CPlugin.h"
#include "Globals.h"

static std::unordered_multimap<std::string, const AMX_HOOK_INFO *> redirected_native_list;

void RegisterHooks(const AMX_HOOK_INFO *hooks_list, size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		const AMX_HOOK_INFO *hook = &hooks_list[i];
		std::string name(hook->name);
		redirected_native_list.emplace(name, hook);
	}
}

bool ApplyHooks(AMX_NATIVE_INFO &native)
{
	bool hooked = false;

	auto range = redirected_native_list.equal_range(std::string(native.name));
	while (range.first != range.second)
	{
		const AMX_HOOK_INFO &hook = *range.first->second;

		hook.originalfunc = native.func;
		native.func = hook.func;
		hooked = true;

		range.first++;
	}
	return hooked;
}

static std::vector<std::pair<const AMX_NATIVE_INFO *, int>> native_list;

void RegisterNatives(const AMX_NATIVE_INFO *natives_list, int count)
{
	native_list.push_back(std::make_pair(natives_list, count));
}

int RegisterAllNatives(AMX *amx)
{
	int return_code = 0;

	for (const auto& pair : native_list)
	{
		int ret = amx_Register(amx, pair.first, pair.second);
		if (ret != 0) return_code = ret;
	}
	return return_code;
}

void LoadNatives()
{
	ActorsLoadNatives();
	FixesLoadNatives();
	GangZonesLoadNatives();
	MenusLoadNatives();
	MiscLoadNatives();
	ModelSizesLoadNatives();
	ObjectsLoadNatives();
	PickupsLoadNatives();
	PlayersLoadNatives();
	RakNetLoadNatives();
	ScoreBoardLoadNatives();
	ServerLoadNatives();
	TextDrawsLoadNatives();
	TextLabelsLoadNatives();
	VehiclesLoadNatives();
	YSFSettingsLoadNatives();
}
