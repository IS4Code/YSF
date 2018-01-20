#ifndef YSF_COBJECTDATA_H
#define YSF_COBJECTDATA_H

#include <unordered_map>
#include "includes/types.h"
#include "Structs.h"

class CObjectData
{
public:
	WORD wObjectID;

	CObjectData(WORD objectid);
	CObjectData(size_t objectid) : CObjectData(static_cast<WORD>(objectid))
	{

	}

	WORD attachedToPlayer = INVALID_PLAYER_ID;
};

#endif
