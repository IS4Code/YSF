#include "../Natives.h"
#include "../CServer.h"
#include "../CScriptParams.h"

namespace Natives
{
	// native YSF_SetTickRate(ticks);
	AMX_DECLARE_NATIVE(YSF_SetTickRate)
	{
		CHECK_PARAMS(1, LOADED);

		int rate = CScriptParams::Get()->ReadInt();
		if (rate < -1 || rate == 0) return 0; // -1 = no update

		CServer::Get()->SetTickRate(rate);
		return 1;
	}

	// native YSF_GetTickRate();
	AMX_DECLARE_NATIVE(YSF_GetTickRate)
	{
		if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CServer::Get()->GetTickRate());
	}

	// native YSF_EnableNightVisionFix(enable);
	AMX_DECLARE_NATIVE(YSF_EnableNightVisionFix)
	{
		CHECK_PARAMS(1, LOADED);

		CServer::Get()->EnableNightVisionFix(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native YSF_IsNightVisionFixEnabled();
	AMX_DECLARE_NATIVE(YSF_IsNightVisionFixEnabled)
	{
		if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CServer::Get()->IsNightVisionFixEnabled());
	}

	// native YSF_ToggleOnServerMessage(toggle);
	AMX_DECLARE_NATIVE(YSF_ToggleOnServerMessage)
	{
		CHECK_PARAMS(1, LOADED);

		CServer::Get()->ToggleOnServerMessage(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native YSF_IsOnServerMessageEnabled();
	AMX_DECLARE_NATIVE(YSF_IsOnServerMessageEnabled)
	{
		if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CServer::Get()->IsOnServerMessageEnabled());
	}

	// native YSF_SetExtendedNetStatsEnabled(enable);
	AMX_DECLARE_NATIVE(YSF_SetExtendedNetStatsEnabled)
	{
		CHECK_PARAMS(1, LOADED);

		CServer::Get()->SetExtendedNetStatsEnabled(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native YSF_IsExtendedNetStatsEnabled();
	AMX_DECLARE_NATIVE(YSF_IsExtendedNetStatsEnabled)
	{
		if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CServer::Get()->IsExtendedNetStatsEnabled());
	}

	// native YSF_SetAFKAccuracy(time_ms);
	AMX_DECLARE_NATIVE(YSF_SetAFKAccuracy)
	{
		CHECK_PARAMS(1, LOADED);

		CServer::Get()->SetAFKAccuracy(static_cast<DWORD>(CScriptParams::Get()->ReadInt()));
		return 1;
	}

	// native YSF_GetAFKAccuracy();
	AMX_DECLARE_NATIVE(YSF_GetAFKAccuracy)
	{
		if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		return static_cast<cell>(CServer::Get()->GetAFKAccuracy());
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
};

int YSFSettingsInitNatives(AMX *amx)
{
	return amx_Register(amx, native_list, sizeof(native_list) / sizeof(*native_list));
}
