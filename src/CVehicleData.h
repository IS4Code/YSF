#ifndef YSF_CVEHICLEDATA_H
#define YSF_CVEHICLEDATA_H

#include <unordered_map>
#include "includes/types.h"
#include "Structs.h"

class CVehicleData
{
public:
	WORD wVehicleID;

	CVehicleData(WORD vehicleid);
	CVehicleData(size_t vehicleid) : CVehicleData(static_cast<WORD>(vehicleid))
	{

	}

	bool bChangedVehicleColor = false;

	bool customSpawnData = false;
	CVehicleSpawn spawnData = {};
};

#endif
