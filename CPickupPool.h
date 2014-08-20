#ifndef __YSF_PICKUPPOOL
#define __YSF_PICKUPPOOL

#include "CTypes.h"
#include "CVector.h"
//#include "Structs.h"
#include <vector>
#include <bitset>
#include <map>

class CPickup;
typedef std::map<int, CPickup*> PickupMap;


class CPickupPool
{
public:
	void InitializeForPlayer(WORD playerid);

	int New(int modelid, int type, CVector vecPos);
	int New(WORD playerid, int modelid, int type, CVector vecPos);
	void Destroy(int pickupid);
	void Destroy(WORD playerid, int pickupid);

	CPickup* FindPickup(int pickupid);
	CPickup* CPickupPool::FindPickup(WORD playerid, int pickupid);
	int FindPickup(CPickup *pPickup);
	int CPickupPool::FindPickup(WORD playerid, CPickup *pPickup);
	
private:
	PickupMap m_Pickups;
	std::bitset<4096> m_bPickupSlots;
};

#endif