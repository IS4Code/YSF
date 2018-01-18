#ifndef YSF_POOL_H
#define YSF_POOL_H

#include <tuple>
#include <map>
#include "includes/platform.h"

template <class ObjectType, size_t MaxSize, class ExtraData = std::tuple<size_t>>
class CPoolBase
{
	std::unordered_map<size_t, ExtraData> extraData;
public:
	static constexpr size_t Capacity = MaxSize;
	static constexpr size_t InvalidIndex = static_cast<size_t>(-1);

	CPoolBase() = default;
	CPoolBase(const CPoolBase&) = delete;
	CPoolBase &operator=(const CPoolBase&) = delete;

	ExtraData &Extra(size_t index)
	{
		auto iter = extraData.find(index);
		if (iter == extraData.end())
		{
			return extraData.emplace(index, index).first->second;
		}
		return iter->second;
	}

	void RemoveExtra(size_t index)
	{
		extraData.erase(index);
	}

	virtual ObjectType &operator[](size_t index) = 0;
	virtual ObjectType &Get(size_t index) = 0;
	virtual bool IsValid(size_t index) const = 0;
	virtual size_t Top() const = 0;

protected:
	virtual size_t EmptySlot() const
	{
		for (size_t i = 0; i < Capacity; i++)
		{
			if (!IsValid(i))
			{
				return i;
			}
		}
		return InvalidIndex;
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType *(PoolType::*PoolArray)[MaxSize], class ExtraData = std::tuple<size_t>>
class CBasicPool : public CPoolBase<ObjectType, MaxSize, ExtraData>
{
protected:
	PoolType& pool;
public:
	CBasicPool(PoolType& poolData) : pool(poolData)
	{

	}

	CBasicPool(const CBasicPool&) = delete;
	CBasicPool &operator=(const CBasicPool&) = delete;

	virtual ObjectType &operator[](size_t index) override
	{
		return *(pool.*PoolArray)[index];
	}

	virtual ObjectType &Get(size_t index) override
	{
		if (!IsValid(index)) throw std::exception("Invalid index accessed.");
		return *(pool.*PoolArray)[index];
	}

	virtual bool IsValid(size_t index) const override
	{
		return (pool.*PoolArray)[index] != nullptr;
	}

	virtual size_t Top() const override
	{
		return Capacity - 1;
	}

protected:
	virtual void Set(size_t index, ObjectType &object)
	{
		(pool.*PoolArray)[index] = &object;
	}

public:
	size_t Add(ObjectType &object)
	{
		size_t i = EmptySlot();
		if (i != InvalidIndex)
		{
			Set(i, object);
		}
		return i;
	}

	template <class... Args>
	size_t New(Args&&... args)
	{
		size_t i = EmptySlot();
		if (i != InvalidIndex)
		{
			Set(i, new ObjectType(std::forward<Args>(args)...));
		}
		return i;
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType *(PoolType::*PoolArray)[MaxSize], BOOL(PoolType::*SlotArray)[MaxSize], class ExtraData = std::tuple<size_t>>
class CSlotPool : public CBasicPool<PoolType, ObjectType, MaxSize, PoolArray, ExtraData>
{
public:
	CSlotPool(PoolType& poolData) : CBasicPool(poolData)
	{

	}

	/*CSlotPool(const CSlotPool&) = delete;
	CSlotPool &operator=(const CSlotPool&) = delete;*/

	virtual bool IsValid(size_t index) const override
	{
		return (pool.*SlotArray)[index] && (pool.*PoolArray)[index] != nullptr;
	}

protected:
	virtual void Set(size_t index, ObjectType &object) override
	{
		CBasicPool::Set(index, object);
		(pool.*SlotArray)[index] = static_cast<BOOL>(true);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType *(PoolType::*PoolArray)[MaxSize], BOOL(PoolType::*SlotArray)[MaxSize], DWORD PoolType::*TopIndex, class ExtraData = std::tuple<size_t>>
class CBoundedPool : public CSlotPool<PoolType, ObjectType, MaxSize, PoolArray, SlotArray, ExtraData>
{
public:
	CBoundedPool(PoolType& poolData) : CSlotPool(poolData)
	{

	}

	virtual size_t Top() const override
	{
		return pool.*TopIndex;
	}

protected:
	virtual void Set(size_t index, ObjectType &object)
	{
		CSlotPool::Set(index, object);
		if (index > Top())
		{
			pool.*TopIndex = index;
		}
	}
};

#endif
