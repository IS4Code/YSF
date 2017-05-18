#include "../main.h"
#include "../Natives.h"

// native YSF_SetTickRate(ticks);
AMX_DECLARE_NATIVE(Natives::YSF_SetTickRate)
{
	CHECK_PARAMS(1, LOADED);

	int rate = CScriptParams::Get()->ReadInt();
	if (rate < -1 || rate == 0) return 0; // -1 = no update

	CServer::Get()->SetTickRate(rate);
	return 1;
}

// native YSF_GetTickRate();
AMX_DECLARE_NATIVE(Natives::YSF_GetTickRate)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->GetTickRate());
}

// native YSF_EnableNightVisionFix(enable);
AMX_DECLARE_NATIVE(Natives::YSF_EnableNightVisionFix)
{
	CHECK_PARAMS(1, LOADED);

	CServer::Get()->EnableNightVisionFix(CScriptParams::Get()->ReadBool());
	return 1;
}

// native YSF_IsNightVisionFixEnabled();
AMX_DECLARE_NATIVE(Natives::YSF_IsNightVisionFixEnabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->IsNightVisionFixEnabled());
}

// native YSF_ToggleOnServerMessage(toggle);
AMX_DECLARE_NATIVE(Natives::YSF_ToggleOnServerMessage)
{
	CHECK_PARAMS(1, LOADED);

	CServer::Get()->ToggleOnServerMessage(CScriptParams::Get()->ReadBool());
	return 1;
}

// native YSF_IsOnServerMessageEnabled();
AMX_DECLARE_NATIVE(Natives::YSF_IsOnServerMessageEnabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->IsOnServerMessageEnabled());
}

// native YSF_SetExtendedNetStatsEnabled(enable);
AMX_DECLARE_NATIVE(Natives::YSF_SetExtendedNetStatsEnabled)
{
	CHECK_PARAMS(1, LOADED);

	CServer::Get()->SetExtendedNetStatsEnabled(CScriptParams::Get()->ReadBool());
	return 1;
}

// native YSF_IsExtendedNetStatsEnabled();
AMX_DECLARE_NATIVE(Natives::YSF_IsExtendedNetStatsEnabled)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->IsExtendedNetStatsEnabled());
}

// native YSF_SetAFKAccuracy(time_ms);
AMX_DECLARE_NATIVE(Natives::YSF_SetAFKAccuracy)
{
	CHECK_PARAMS(1, LOADED);

	CServer::Get()->SetAFKAccuracy(static_cast<DWORD>(CScriptParams::Get()->ReadInt()));
	return 1;
}

// native YSF_GetAFKAccuracy();
AMX_DECLARE_NATIVE(Natives::YSF_GetAFKAccuracy)
{
	if (!CServer::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

	return static_cast<cell>(CServer::Get()->GetAFKAccuracy());
}
