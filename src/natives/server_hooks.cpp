#include "../Natives.h"
#include "../includes/platform.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"
#include "../Hooks.h"
#include "../Globals.h"
#include "../Utils.h"
#include "../RPCs.h"

namespace Natives
{
	// native ToggleChatTextReplacement(bool:toggle);
	AMX_DECLARE_NATIVE(ToggleChatTextReplacement)
	{
		CHECK_PARAMS(1, LOADED);

		CPlugin::Get()->ToggleChatTextReplacement(CScriptParams::Get()->ReadBool());
		return 1;
	}

	// native GetNPCCommandLine(npcid, npcscript[], length = sizeof(npcscript));
	AMX_DECLARE_NATIVE(GetNPCCommandLine)
	{
		CHECK_PARAMS(3, LOADED);

		const int npcid = CScriptParams::Get()->ReadInt();
		if(!IsPlayerConnected(npcid)) return 0;

		char *szCommandLine = CPlugin::Get()->GetNPCCommandLine(static_cast<WORD>(npcid));
		if(szCommandLine == NULL) return 0;

		CScriptParams::Get()->Add(szCommandLine);
		return 1;
	}

	// native AllowNickNameCharacter(character, bool:allow);
	AMX_DECLARE_NATIVE(AllowNickNameCharacter)
	{
		CHECK_PARAMS(2, LOADED);

		const char character = static_cast<const char>(params[1]);

		// Enable %s is disallowed
		if(character == '%') return 0;

		CPlugin::Get()->AllowNickNameCharacter(character, static_cast<int>(params[2]) != 0);
		return 1;
	}
}

namespace Original
{
	AMX_NATIVE GetGravity;
}

namespace Hooks
{
	// native Float:GetGravity();
	AMX_DECLARE_NATIVE(GetGravity)
	{
		if(!CPlugin::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		float fGravity = pNetGame->fGravity;
		return amx_ftoc(fGravity);
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// Nick name
	AMX_DEFINE_NATIVE(AllowNickNameCharacter) // R7

	// Server settings
	AMX_DEFINE_NATIVE(GetNPCCommandLine) // R19

	AMX_DEFINE_NATIVE(ToggleChatTextReplacement) // R19
};

static AMX_HOOK_INFO hook_list[] =
{
	AMX_DEFINE_HOOK(GetGravity)
};

void ServerHooksLoadNatives()
{
	RegisterNatives(native_list);
	RegisterHooks(hook_list);
}
