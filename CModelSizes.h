#ifndef __YSF_CMODELSIZES
#define __YSF_CMODELSIZES

#include "CTypes.h"
#include "CVector.h"

namespace CModelSizes
{
	WORD GetColCount(void);
	float GetColSphereRadius(int objectmodel);
	CVector GetColSphereOffset(int objectmodel);
}
#endif