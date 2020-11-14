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

void ActorsLoadNatives();
void FixesLoadNatives();
void GangZonesLoadNatives();
void GangZonesHooksLoadNatives();
void MenusLoadNatives();
void MiscLoadNatives();
void ModelSizesLoadNatives();
void ObjectsLoadNatives();
void ObjectsHooksLoadNatives();
void PickupsLoadNatives();
void PickupsHooksLoadNatives();
void PlayersLoadNatives();
void PlayersHooksLoadNatives();
void RakNetLoadNatives();
void RakNetHooksLoadNatives();
void ScoreBoardHooksLoadNatives();
void ServerLoadNatives();
void ServerHooksLoadNatives();
void TextDrawsLoadNatives();
void TextLabelsLoadNatives();
void VehiclesLoadNatives();
void VehiclesHooksLoadNatives();
void YSFSettingsLoadNatives();
void YSFSettingsHooksLoadNatives();

void LoadNatives(bool loadhooks)
{
	ActorsLoadNatives();
	FixesLoadNatives();
	GangZonesLoadNatives();
	if(loadhooks) GangZonesHooksLoadNatives();
	MenusLoadNatives();
	MiscLoadNatives();
	ModelSizesLoadNatives();
	ObjectsLoadNatives();
	if(loadhooks) ObjectsHooksLoadNatives();
	PickupsLoadNatives();
	if(loadhooks) PickupsHooksLoadNatives();
	PlayersLoadNatives();
	if(loadhooks) PlayersHooksLoadNatives();
	RakNetLoadNatives();
	if(loadhooks) RakNetHooksLoadNatives();
	if(loadhooks) ScoreBoardHooksLoadNatives();
	ServerLoadNatives();
	if(loadhooks) ServerHooksLoadNatives();
	TextDrawsLoadNatives();
	TextLabelsLoadNatives();
	VehiclesLoadNatives();
	if(loadhooks) VehiclesHooksLoadNatives();
	YSFSettingsLoadNatives();
	if(loadhooks) YSFSettingsHooksLoadNatives();
}
