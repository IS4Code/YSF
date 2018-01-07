#include "../main.h"
#include "../Natives.h"

// native GetColCount();
AMX_DECLARE_NATIVE(Natives::GetColCount)
{
	return CModelSizes::GetColCount();
}

// native Float:GetColSphereRadius(modelid);
AMX_DECLARE_NATIVE(Natives::GetColSphereRadius)
{
	CHECK_PARAMS(1, NO_FLAGS);

	float fRet = CModelSizes::GetColSphereRadius(CScriptParams::Get()->ReadInt());
	return amx_ftoc(fRet);
}

// native GetColSphereOffset(modelid, &Float:fX, &Float:fY, &Float:fZ);
AMX_DECLARE_NATIVE(Natives::GetColSphereOffset)
{
	CHECK_PARAMS(4, NO_FLAGS);

	CVector vecOffset = CModelSizes::GetColSphereOffset(CScriptParams::Get()->ReadInt());
	CScriptParams::Get()->Add(vecOffset);
	return 1;
}
