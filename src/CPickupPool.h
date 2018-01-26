/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

#ifndef YSF_PICKUPPOOL_H
#define YSF_PICKUPPOOL_H

#include <vector>
#include <bitset>
#include <cstddef>

#include "CVector.h"

#ifdef NEW_PICKUP_SYSTEM
enum ePickupType : BYTE
{
	GLOBAL,
	PLAYER
};

class CPickup
{
public:
	CPickup(int model, int type, CVector pos, int world, ePickupType pickuptype)
	{
		this->iModel = model;
		this->iType = type;
		this->vecPos = pos;
		this->iWorld = world;
		this->type = pickuptype;
	}

	int	iModel;
	int	iType;
	CVector	vecPos;
	int iWorld;
	ePickupType type;
};

typedef std::unordered_map<int, CPickup*> PickupMap;

class CYSFPickupPool
{
public:
	CYSFPickupPool();
	~CYSFPickupPool();

	void InitializeForPlayer(WORD playerid);

	int New(int modelid, int type, CVector vecPos, int world);
	int New(WORD playerid, int modelid, int type, CVector vecPos, int world);
	void Destroy(int pickupid);
	void Destroy(WORD playerid, int pickupid);

	bool IsStreamed(WORD playerid, CPickup *pPickup);
	CPickup* FindPickup(int pickupid);
	CPickup* FindPickup(WORD playerid, int pickupid);
	int FindPickup(CPickup *pPickup);
	int FindPickup(WORD playerid, CPickup *pPickup);
	
	void Process(void);

	void SetStreamingEnabled(bool enabled);
	bool IsStreamingEnabled(void);
private:
	void ShowPickup(int pickupid, WORD playerid, CPickup *pPickup);
	void HidePickup(int pickupid, WORD playerid);

	PickupMap m_Pickups;
	std::bitset<4096> m_bPickupSlots;

	bool m_bStreamingEnabled;
};
#endif
#endif