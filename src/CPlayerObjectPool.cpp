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