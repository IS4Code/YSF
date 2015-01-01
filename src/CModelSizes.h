#ifndef YSF_CMODELSIZES_H
#define YSF_CMODELSIZES_H

#include "CTypes.h"
#include "CVector.h"

namespace CModelSizes
{
	WORD GetColCount(void);
	float GetColSphereRadius(int objectmodel);
	CVector GetColSphereOffset(int objectmodel);
}
#endif