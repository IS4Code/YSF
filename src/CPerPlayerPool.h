#ifndef YSF_PER_PLAYER_POOL_H
#define YSF_PER_PLAYER_POOL_H

#include <tuple>
#include <unordered_map>
#include <stdexcept>
#include <type_traits>
#include "includes/types.h"
#include "utils/pair_hash.h"
#include "CPool.h"
#include "CPlayerData.h"
#include "Structs.h"

template <class ObjectType, size_t MaxSize>
class CPerPlayerPoolBase
{
public:
	static constexpr size_t Capacity = MaxSize;
	static constexpr size_t InvalidIndex = static_cast<size_t>(-1);

	CPerPlayerPoolBase() = default;
	CPerPlayerPoolBase(const CPerPlayerPoolBase&) = delete;
	CPerPlayerPoolBase &operator=(const CPerPlayerPoolBase&) = delete;
	virtual ~CPerPlayerPoolBase() = default;

	virtual ObjectType(&operator[](size_t playerid))[MaxSize] = 0;
	virtual ObjectType &Get(size_t playerid, size_t index) = 0;
	virtual bool IsValid(size_t playerid, size_t index) const = 0;

	virtual size_t TopIndex(size_t playerid) const
	{
		return Capacity - 1;
	}

	template <class Function>
	typename std::result_of<Function(ObjectType &)>::type Map(size_t playerid, size_t index, Function func)
	{
		if (IsValid(playerid, index))
		{
			return func((*this)[playerid][index]);
		}
		return typename std::result_of<Function(ObjectType &)>::type();
	}
};


template <class ObjectType, size_t MaxSize, class ExtraData = std::tuple<size_t>>
class CExtendedPerPlayerPool : public CPerPlayerPoolBase<ObjectType, MaxSize>
{
	typedef CPerPlayerPoolBase<ObjectType, MaxSize> Base;
	std::unordered_map<std::pair<size_t, size_t>, ExtraData, aux::pair_hash> extraData;

public:
	CExtendedPerPlayerPool() = default;

	ExtraData &Extra(size_t playerid, size_t index)
	{
		auto iter = extraData.find(std::make_pair(playerid, index));
		if (iter == extraData.end())
		{
			return extraData.emplace(std::make_pair(playerid, index), playerid, index).first->second;
		}
		return iter->second;
	}

	bool RemoveExtra(size_t playerid, size_t index)
	{
		return extraData.erase(std::make_pair(playerid, index)) > 0;
	}

	template <class Function>
	typename std::result_of<Function(ExtraData &)>::type MapExtra(size_t playerid, size_t index, Function func)
	{
		auto iter = extraData.find(std::make_pair(playerid, index));
		if (iter == extraData.end())
		{
			return typename std::result_of<Function(ExtraData &)>::type();
		}
		return func(iter->second);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, size_t PlayerPoolMaxSize, ObjectType(PoolType::*PoolArray)[PlayerPoolMaxSize][MaxSize], class ExtraData = std::tuple<size_t>>
class CBasicPerPlayerPool : public CExtendedPerPlayerPool<ObjectType, MaxSize, ExtraData>
{
	typedef CExtendedPerPlayerPool<ObjectType, MaxSize, ExtraData> Base;
protected:
	PoolType& pool;
	const CPoolBase<CPlayer*, PlayerPoolMaxSize>& players;
public:
	CBasicPerPlayerPool(PoolType& poolData, const CPoolBase<CPlayer*, PlayerPoolMaxSize>& playerPool) : pool(poolData), players(playerPool)
	{

	}

	virtual ObjectType(&operator[](size_t playerid))[MaxSize] OVERRIDE
	{
		return (pool.*PoolArray)[playerid];
	}

	virtual ObjectType &Get(size_t playerid, size_t index) OVERRIDE
	{
		if (!IsValid(playerid, index)) throw std::invalid_argument("Invalid index accessed.");
		return (pool.*PoolArray)[playerid][index];
	}

	virtual bool IsValid(size_t playerid, size_t index) const OVERRIDE
	{
		return players.IsValid(playerid) && index >= 0 && index < MaxSize && !aux::is_null((pool.*PoolArray)[playerid][index]);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, size_t PlayerPoolMaxSize, ObjectType(PoolType::*PoolArray)[PlayerPoolMaxSize][MaxSize], BOOL(PoolType::*SlotArray)[PlayerPoolMaxSize][MaxSize], class ExtraData = std::tuple<size_t>>
class CSlotPerPlayerPool : public CBasicPerPlayerPool<PoolType, ObjectType, MaxSize, PlayerPoolMaxSize, PoolArray, ExtraData>
{
	typedef CBasicPerPlayerPool<PoolType, ObjectType, MaxSize, PlayerPoolMaxSize, PoolArray, ExtraData> Base;
public:
	CSlotPerPlayerPool(PoolType& poolData, const CPoolBase<CPlayer*, PlayerPoolMaxSize>& playerPool) : Base(poolData, playerPool)
	{

	}

	virtual bool IsValid(size_t playerid, size_t index) const OVERRIDE
	{
		return playerid >= 0 && playerid < PlayerPoolMaxSize && index >= 0 && index < MaxSize && (Base::pool.*SlotArray)[playerid][index] && !aux::is_null((Base::pool.*PoolArray)[playerid][index]);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, size_t PlayerPoolMaxSize, ObjectType(PoolType::*PoolArray)[PlayerPoolMaxSize][MaxSize], BOOL(PoolType::*SlotArray)[PlayerPoolMaxSize][MaxSize], DWORD(PoolType::*PoolSize)[PlayerPoolMaxSize], class ExtraData = std::tuple<size_t>>
class CBoundedPerPlayerPool : public CSlotPerPlayerPool<PoolType, ObjectType, MaxSize, PlayerPoolMaxSize, PoolArray, SlotArray, ExtraData>
{
	typedef CSlotPerPlayerPool<PoolType, ObjectType, MaxSize, PlayerPoolMaxSize, PoolArray, SlotArray, ExtraData> Base;
public:
	CBoundedPerPlayerPool(PoolType& poolData, const CPoolBase<CPlayer*, PlayerPoolMaxSize>& playerPool) : Base(poolData, playerPool)
	{

	}

	virtual size_t TopIndex(size_t playerid) const OVERRIDE
	{
		return (Base::pool.*PoolSize)[playerid];
	}
};

#endif
