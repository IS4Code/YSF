#include "../Natives.h"
#include "../CPlugin.h"
#include "../CServer.h"
#include "../CConfig.h"
#include "../CScriptParams.h"
#include "../Globals.h"
#include "../Utils.h"

namespace Original
{
	AMX_NATIVE CreatePlayerTextDraw;
}

namespace Hooks
{
	AMX_DECLARE_NATIVE(CreatePlayerTextDraw)
	{
		cell result = Original::CreatePlayerTextDraw(amx, params);
		if(result != INVALID_TEXT_DRAW)
		{
			CHECK_PARAMS(1, MORE_PARAMETER_ALLOWED);
			int playerid = CScriptParams::Get()->ReadInt();
			if(IsPlayerConnected(playerid) && result >= 0 && result < MAX_PLAYER_TEXT_DRAWS)
			{
				pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bHasText[result] = false;
			}
		}
		return result;
	}
}

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(CreatePlayerTextDraw)
};

void TextDrawsHooksLoadNatives()
{
	RegisterHooks(hook_list);
}