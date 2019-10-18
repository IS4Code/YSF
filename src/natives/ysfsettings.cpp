#include "../Natives.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"

namespace Natives
{
	// native YSF_SetTickRate(ticks);
	AMX_DECLARE_NATIVE(YSF_SetTickRate)
	{
		CHECK_PARAMS(1, LOADED);

		int rate = CScriptParams::Get()->ReadInt();
		if (rate < -1 || rate == 0) return 0; // -1 = no update

		CPlugin::Get()->SetTickRate(rate);
		return 1;
	}

	// native YSF_GetTickRate();
	AMX_DECLARE_NATIVE(YSF_GetTickRate)
	{
		if (!CPlugin::IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CPlugin::Get()->GetTickRate());
	}

	// native YSF_EnableNightVisionFix(enable);
	AMX_DECLARE_NATIVE(YSF_EnableNightVisionFix)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->EnableNightVisionFix(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native YSF_IsNightVisionFixEnabled();
	AMX_DECLARE_NATIVE(YSF_IsNightVisionFixEnabled)
	{
		if (!CPlugin::IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CPlugin::Get()->IsNightVisionFixEnabled());
	}

	// native YSF_ToggleOnServerMessage(toggle);
	AMX_DECLARE_NATIVE(YSF_ToggleOnServerMessage)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->ToggleOnServerMessage(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native YSF_IsOnServerMessageEnabled();
	AMX_DECLARE_NATIVE(YSF_IsOnServerMessageEnabled)
	{
		if (!CPlugin::IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CPlugin::Get()->IsOnServerMessageEnabled());
	}

	// native YSF_SetExtendedNetStatsEnabled(enable);
	AMX_DECLARE_NATIVE(YSF_SetExtendedNetStatsEnabled)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->SetExtendedNetStatsEnabled(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native YSF_IsExtendedNetStatsEnabled();
	AMX_DECLARE_NATIVE(YSF_IsExtendedNetStatsEnabled)
	{
		if (!CPlugin::IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CPlugin::Get()->IsExtendedNetStatsEnabled());
	}

	// native YSF_SetAFKAccuracy(time_ms);
	AMX_DECLARE_NATIVE(YSF_SetAFKAccuracy)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->SetAFKAccuracy(static_cast<DWORD>(CScriptParams::Get()->ReadInt()));
		return 1;
	}

	// native YSF_GetAFKAccuracy();
	AMX_DECLARE_NATIVE(YSF_GetAFKAccuracy)
	{
		if (!CPlugin::IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CPlugin::Get()->GetAFKAccuracy());
	}

	// native YSF_SetQueryFloodCheckType(type);
	AMX_DECLARE_NATIVE(YSF_SetQueryFloodCheckType)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->SetQueryFloodCheckType(CScriptParams::Get()->ReadInt());
		return 1;
	}

	// native YSF_GetQueryFloodCheckType();
	AMX_DECLARE_NATIVE(YSF_GetQueryFloodCheckType)
	{
		if (!CPlugin::IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CPlugin::Get()->GetQueryFloodCheckType());
	}

}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(YSF_SetTickRate)
	AMX_DEFINE_NATIVE(YSF_GetTickRate)
	AMX_DEFINE_NATIVE(YSF_EnableNightVisionFix)
	AMX_DEFINE_NATIVE(YSF_IsNightVisionFixEnabled)
	AMX_DEFINE_NATIVE(YSF_ToggleOnServerMessage) // R18-2
	AMX_DEFINE_NATIVE(YSF_IsOnServerMessageEnabled) // R18-2
	AMX_DEFINE_NATIVE(YSF_SetExtendedNetStatsEnabled) // R17
	AMX_DEFINE_NATIVE(YSF_IsExtendedNetStatsEnabled) // R17
	AMX_DEFINE_NATIVE(YSF_SetAFKAccuracy) // R17
	AMX_DEFINE_NATIVE(YSF_GetAFKAccuracy) // R17
	AMX_DEFINE_NATIVE(YSF_SetQueryFloodCheckType) // Nobody: no tag yet.
	AMX_DEFINE_NATIVE(YSF_GetQueryFloodCheckType) // Nobody: no tag yet.
};

void YSFSettingsLoadNatives()
{
	RegisterNatives(native_list);
}
