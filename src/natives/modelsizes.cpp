#include "../Natives.h"
#include "../CModelSizes.h"
#include "../CScriptParams.h"

namespace Natives
{
	// native GetColCount();
	AMX_DECLARE_NATIVE(GetColCount)
	{
		return CModelSizes::GetColCount();
	}

	// native Float:GetColSphereRadius(modelid);
	AMX_DECLARE_NATIVE(GetColSphereRadius)
	{
		CHECK_PARAMS(1, NO_FLAGS);

		float fRet = CModelSizes::GetColSphereRadius(CScriptParams::Get()->ReadInt());
		return amx_ftoc(fRet);
	}

	// native GetColSphereOffset(modelid, &Float:fX, &Float:fY, &Float:fZ);
	AMX_DECLARE_NATIVE(GetColSphereOffset)
	{
		CHECK_PARAMS(4, NO_FLAGS);

		CVector vecOffset = CModelSizes::GetColSphereOffset(CScriptParams::Get()->ReadInt());
		CScriptParams::Get()->Add(vecOffset);
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	AMX_DEFINE_NATIVE(GetColCount)
	AMX_DEFINE_NATIVE(GetColSphereRadius)
	AMX_DEFINE_NATIVE(GetColSphereOffset)
};

void ModelSizesLoadNatives()
{
	RegisterNatives(native_list);
}

