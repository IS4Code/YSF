#ifndef YSF_SCRIPTING_H
#define YSF_SCRIPTING_H

#include <sdk/amx/amx.h>

typedef cell AMX_NATIVE_CALL(*AMX_Function_t)(AMX *amx, cell *params);

#define AMX_DECLARE_NATIVE(name) \
	cell AMX_NATIVE_CALL name(AMX* amx, cell* params)

#define AMX_DEFINE_NATIVE(name) \
	{#name, Natives::name},

#define AMX_DEFINE_HOOK(name) \
	{#name, Original::name, Hooks::name},

#define CHECK_PARAMS(m, flag) \
	if(CScriptParams::Get()->Setup(m, __FUNCTION__, CScriptParams::Flags::flag, amx, params)) return CScriptParams::Get()->HandleError()
	
struct AMX_HOOK_INFO
{
	const char _FAR *name PACKED;
	AMX_NATIVE &originalfunc PACKED;
	AMX_NATIVE func PACKED;
};

void LoadNatives(bool loadhooks);

bool ApplyHooks(AMX_NATIVE_INFO &native);

void RegisterHooks(const AMX_HOOK_INFO *hooks_list, size_t count);

template <size_t Count>
void RegisterHooks(const AMX_HOOK_INFO (&hooks_list)[Count])
{
	RegisterHooks(hooks_list, Count);
}

int RegisterAllNatives(AMX *amx);

void RegisterNatives(const AMX_NATIVE_INFO *natives_list, int count);

template <size_t Count>
void RegisterNatives(const AMX_NATIVE_INFO(&natives_list)[Count])
{
	RegisterNatives(natives_list, static_cast<int>(Count));
}

#endif
