#include "../Natives.h"
#include "../includes/platform.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

namespace Natives
{
	// native GetVehicleColor(vehicleid, &color1, &color2);
	AMX_DECLARE_NATIVE(GetVehicleColor)
	{
		CHECK_PARAMS(3, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if(vehicleid < 1 || vehicleid > MAX_VEHICLES) return 0;

		if(!pNetGame->pVehiclePool->pVehicle[vehicleid])
			return 0;

		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[vehicleid];

		bool changed = CServer::Get()->VehiclePool.MapExtra(vehicleid, [](CVehicleData &data) {return data.bChangedVehicleColor; });

		int color1 = changed ? pVehicle->vehModInfo.iColor1 : pVehicle->customSpawn.iColor1;
		int color2 = changed ? pVehicle->vehModInfo.iColor2 : pVehicle->customSpawn.iColor2;

		CScriptParams::Get()->Add(color1, color2);
		return 1;
	}
}

namespace Original
{
	AMX_NATIVE ChangeVehicleColor;
	AMX_NATIVE DestroyVehicle;
}

namespace Hooks
{
	// native ChangeVehicleColor(vehicleid, color1, color2)
	AMX_DECLARE_NATIVE(ChangeVehicleColor)
	{
		CHECK_PARAMS(3, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if(Original::ChangeVehicleColor(amx, params))
		{
			CServer::Get()->VehiclePool.Extra(vehicleid).bChangedVehicleColor = true;
			return 1;
		}
		return 0;
	}

	// native DestroyVehicle(vehicleid);
	AMX_DECLARE_NATIVE(DestroyVehicle)
	{
		CHECK_PARAMS(1, LOADED);

		const int vehicleid = CScriptParams::Get()->ReadInt();
		if(Original::DestroyVehicle(amx, params))
		{
			CServer::Get()->VehiclePool.RemoveExtra(vehicleid);
			return 1;
		}
		return 0;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(GetVehicleColor)
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(ChangeVehicleColor)
	AMX_DEFINE_HOOK(DestroyVehicle)
};

void VehiclesHooksLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
