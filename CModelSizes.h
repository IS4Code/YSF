#ifndef __YSF_CMODELSIZES
#define __YSF_CMODELSIZES

#include "CTypes.h"
#include "CVector.h"

ushort GetColCount(void);
float GetColSphereRadius(int objectmodel);
CVector *GetColSphereOffset(int objectmodel);

#endif