#include "../Natives.h"
#include "../CPlugin.h"
#include "../CAddresses.h"
#include "../CScriptParams.h"
#include "../Utils.h"

#include "raknet/NetworkTypes.h"

#include <cstring>

namespace Natives
{
	// native IsBanned(ipaddress[]);
	AMX_DECLARE_NATIVE(IsBanned)
	{
		CHECK_PARAMS(1, LOADED);

		char *ip;
		amx_StrParam(amx, params[1], ip);
		return (ip) ? CPlugin::Get()->IsBanned(ip) : 0;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// RakServer functions
	AMX_DEFINE_NATIVE(IsBanned)
};

void RakNetHooksLoadNatives()
{
	RegisterNatives(native_list);
}
