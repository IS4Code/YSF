#include "../Natives.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"

namespace Natives
{
	// native YSF_EnableNightVisionFix(enable);
	AMX_DECLARE_NATIVE(YSF_EnableNightVisionFix)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->EnableNightVisionFix(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native YSF_ToggleOnServerMessage(toggle);
	AMX_DECLARE_NATIVE(YSF_ToggleOnServerMessage)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->ToggleOnServerMessage(CScriptParams::Get()->ReadBool());
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(YSF_EnableNightVisionFix)
	AMX_DEFINE_NATIVE(YSF_ToggleOnServerMessage) // R18-2
};

void YSFSettingsHooksLoadNatives()
{
	RegisterNatives(native_list);
}
