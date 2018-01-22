#include "CPlayerObjectPool.h"

CPlayerObjectPool::PerPlayerPool &CPlayerObjectPool::GetPlayerPool(size_t playerid)
{
	auto iter = map.find(playerid);
	if (iter == map.end())
	{
		return map.emplace(playerid, playerid).first->second;
	}
	return iter->second;
}

CObject*(&CPlayerObjectPool::operator[](size_t playerid))[MAX_OBJECTS]
{
	return GetPlayerPool(playerid).Pool;
}

CObject* &CPlayerObjectPool::Get(size_t playerid, size_t index)
{
	if (!IsValid(playerid, index)) throw std::invalid_argument("Invalid index accessed.");
	return GetPlayerPool(playerid).Pool[index];
}

bool CPlayerObjectPool::IsValid(size_t playerid, size_t index) const
{
	return players.IsValid(playerid) && index >= 0 && index < MAX_OBJECTS &&
		MapPlayerPool
		(
			playerid, [index](const PerPlayerPool &pool)
			{
				return pool.Pool[index] != nullptr;
			}
		)
	;
}

size_t CPlayerObjectPool::TopIndex(size_t playerid) const
{
	return players.IsValid(playerid) && map.find(playerid) != map.end() ? MAX_PLAYERS - 1 : -1;
}


CPlayerObjectPool::ServerId CPlayerObjectPool::Create(CPoolBase<CObject*, MAX_OBJECTS> &objects, WORD playerid, WORD modelid, const CVector &pos, const CVector &rot, float drawdistance)
{
	if (!players.IsValid(playerid)) return INVALID_OBJECT_ID;

	PerPlayerPool &pool = GetPlayerPool(playerid);
	auto pair = pool.Create(objects);
	if (pair.first == INVALID_OBJECT_ID) return INVALID_OBJECT_ID;
	CObject &obj = *pair.second;
	obj.iModel = modelid;
	obj.matWorld.pos = pos;
	obj.vecRot = rot;
	obj.fDrawDistance = drawdistance;
	return pair.first;
}

bool CPlayerObjectPool::Destroy(WORD playerid, ServerId objectid)
{
	if (!players.IsValid(playerid)) return false;

	PerPlayerPool &pool = GetPlayerPool(playerid);
	return pool.Destroy(objectid);
}

void CPlayerObjectPool::PlayerDisconnect(WORD playerid)
{
	map.erase(playerid);
}


CPlayerObjectPool::PerPlayerPool::PerPlayerPool(size_t playerid) : playerId(playerid)
{

}

std::pair<CPlayerObjectPool::ServerId, CObject*> CPlayerObjectPool::PerPlayerPool::Create(CPoolBase<CObject*, MAX_OBJECTS> &objects)
{
	ClientId clientId = FindFreeSlot(objects);
	if (clientId != INVALID_OBJECT_ID)
	{
		for (ServerId i = 0; i < MAX_OBJECTS; i++)
		{
			if (!Pool[i])
			{
				Pool[i] = new CObject();
				Pool[i]->wObjectID = clientId;
				return std::make_pair(i, Pool[i]);
			}
		}
	}
	return std::make_pair<ServerId, CObject*>(INVALID_OBJECT_ID, nullptr);
}

CPlayerObjectPool::ClientId CPlayerObjectPool::PerPlayerPool::FindFreeSlot(CPoolBase<CObject*, MAX_OBJECTS> &objects)
{
	for (ClientId i = MAX_OBJECTS - 1; i >= 0; i++)
	{
		if (!idMap.find_r(i) && !objects.IsValid(i))
		{
			return i;
		}
	}
	return INVALID_OBJECT_ID;
}

bool CPlayerObjectPool::PerPlayerPool::Destroy(ServerId id)
{
	if (id < 0 || id >= MAX_OBJECTS) return false;
	if (Pool[id])
	{
		delete Pool[id];
		Pool[id] = nullptr;
		return true;
	}
	return false;
}

CPlayerObjectPool::PerPlayerPool::~PerPlayerPool()
{
	for (ServerId i = 0; i < MAX_OBJECTS; i++)
	{
		if (Pool[i])
		{
			delete Pool[i];
			Pool[i] = nullptr;
		}
	}
}
