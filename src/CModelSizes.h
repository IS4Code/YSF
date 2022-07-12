#ifndef YSF_CMODELSIZES_H
#define YSF_CMODELSIZES_H

#include "CVector.h"

namespace CModelSizes
{
	int GetColCount(void);
	float GetColSphereRadius(int modelid);
	CVector GetColSphereOffset(int modelid);
}
#endif