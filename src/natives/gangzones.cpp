#include "../Natives.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CConfig.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

namespace Natives
{
	// native IsValidGangZone(zoneid);
	AMX_DECLARE_NATIVE(IsValidGangZone)
	{
		CHECK_PARAMS(1, LOADED);

		const int zoneid = CScriptParams::Get()->ReadInt();
		if (zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;

		if(CConfig::Get()->m_bUsePerPlayerGangZones)
		{
			return CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid));
		}else{
			return pNetGame->pGangZonePool->bSlotState[static_cast<WORD>(zoneid)] != 0;
		}
	}

	// native GangZoneGetPos(zoneid, &Float:fMinX, &Float:fMinY, &Float:fMaxX, &Float:fMaxY);
	AMX_DECLARE_NATIVE(GangZoneGetPos)
	{
		CHECK_PARAMS(5, LOADED);

		const int zoneid = CScriptParams::Get()->ReadInt();
		if(zoneid < 0 || zoneid >= MAX_GANG_ZONES) return 0;
		if(CConfig::Get()->m_bUsePerPlayerGangZones)
		{
			if(!CPlugin::Get()->pGangZonePool->GetSlotState(static_cast<WORD>(zoneid))) return 0;

			auto &zone = CPlugin::Get()->pGangZonePool->pGangZone[zoneid]->fGangZone;
			CScriptParams::Get()->Add(zone[0], zone[1], zone[2], zone[3]);
		}else{
			if(!pNetGame->pGangZonePool->bSlotState[static_cast<WORD>(zoneid)]) return 0;

			auto &zone = pNetGame->pGangZonePool->fGangZone[static_cast<WORD>(zoneid)];
			CScriptParams::Get()->Add(zone[0], zone[1], zone[2], zone[3]);
		}
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// Gangzone - Global
	AMX_DEFINE_NATIVE(IsValidGangZone)
	AMX_DEFINE_NATIVE(GangZoneGetPos)
};

void GangZonesLoadNatives()
{
	RegisterNatives(native_list);
}
