#ifndef YSF_CPLAYEROBJECTPOOL_H
#define YSF_CPLAYEROBJECTPOOL_H

#include <unordered_map>
#include <type_traits>
#include "CPerPlayerPool.h"
#include "utils/bimap.h"

class CPlayerObjectPool : public CExtendedPerPlayerPool<CObject*, MAX_OBJECTS>
{
	typedef CExtendedPerPlayerPool<CObject*, MAX_OBJECTS> Base;

	const CPoolBase<CPlayer*, MAX_PLAYERS>& players;
	const CPoolBase<CObject*, MAX_OBJECTS>& objects;
	
	class PerPlayerPool;
	std::unordered_map<size_t, PerPlayerPool> map;
	PerPlayerPool &GetPlayerPool(size_t playerid);

	template <class Function>
	typename std::result_of<Function(PerPlayerPool &)>::type MapPlayerPool(size_t playerid, Function func) const;

	using ServerId = size_t;
	using ClientId = WORD;

public:
	CPlayerObjectPool(const CPoolBase<CPlayer*, MAX_PLAYERS>& playerpool, const CPoolBase<CObject*, MAX_OBJECTS>& objectpool) : players(playerpool), objects(objectpool)
	{

	}

	ServerId Create(CPoolBase<CObject*, MAX_OBJECTS> &objects, WORD playerid, WORD modelid, const CVector &pos, const CVector &rot, float drawdistance);
	bool Destroy(WORD playerid, ServerId objectid);
	void PlayerDisconnect(WORD playerid);

	virtual CObject*(&operator[](size_t playerid))[MAX_OBJECTS] OVERRIDE;
	virtual CObject* &Get(size_t playerid, size_t index) OVERRIDE;
	virtual bool IsValid(size_t playerid, size_t index) const OVERRIDE;
	virtual size_t TopIndex(size_t playerid) const OVERRIDE;

private:
	class PerPlayerPool
	{
		aux::bimap<ServerId, ClientId> idMap;
		size_t playerId;
	public:
		CObject* Pool[MAX_OBJECTS] = {};

		PerPlayerPool(size_t playerid);
		std::pair<ServerId, CObject*> Create(CPoolBase<CObject*, MAX_OBJECTS> &objects);
		bool Destroy(ServerId id);
		~PerPlayerPool();

		PerPlayerPool(const PerPlayerPool*) = delete;
		PerPlayerPool& operator=(const PerPlayerPool*) = delete;
	private:
		ClientId FindFreeSlot(CPoolBase<CObject*, MAX_OBJECTS> &objects);
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
