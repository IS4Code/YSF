#ifndef YSF_CPLAYEROBJECTPOOL_H
#define YSF_CPLAYEROBJECTPOOL_H

#include <unordered_map>
#include <type_traits>
#include "CPerPlayerPool.h"

class CPlayerObjectPool : public CExtendedPerPlayerPool<CObject*, MAX_OBJECTS>
{
	typedef CExtendedPerPlayerPool<CObject*, MAX_OBJECTS> Base;

	const CPoolBase<CPlayer*, MAX_PLAYERS>& players;
	
	class PerPlayerPool;
	std::unordered_map<size_t, PerPlayerPool> map;
	PerPlayerPool &GetPlayerPool(size_t playerid);

	template <class Function>
	typename std::result_of<Function(PerPlayerPool &)>::type MapPlayerPool(size_t playerid, Function func) const;

public:
	CPlayerObjectPool(const CPoolBase<CPlayer*, MAX_PLAYERS>& playerPool) : players(playerPool)
	{

	}

	virtual CObject*(&operator[](size_t playerid))[MAX_OBJECTS] OVERRIDE;
	virtual CObject* &Get(size_t playerid, size_t index) OVERRIDE;
	virtual bool IsValid(size_t playerid, size_t index) const OVERRIDE;
	virtual size_t TopIndex(size_t playerid) const OVERRIDE;

private:
	class PerPlayerPool
	{
	public:
		CObject* Pool[MAX_OBJECTS] = {};
		size_t playerId;

		PerPlayerPool(size_t playerid) : playerId(playerid)
		{

		}
	};
};

template <class Function>
typename std::result_of<Function(typename CPlayerObjectPool::PerPlayerPool &)>::type CPlayerObjectPool::MapPlayerPool(size_t playerid, Function func) const
{
	auto iter = map.find(playerid);
	if (iter == map.end())
	{
		return std::result_of<Function(CPlayerObjectPool::PerPlayerPool &)>::type();
	}
	return func(iter->second);
}


#endif
