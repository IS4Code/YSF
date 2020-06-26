#ifndef YSF_POOL_H
#define YSF_POOL_H

#include <tuple>
#include <unordered_map>
#include <stdexcept>
#include <type_traits>
#include "includes/types.h"

namespace aux
{
	template <class ValueType>
	struct is_null_t
	{
		static constexpr bool call(const ValueType& value)
		{
			return false;
		}
	};

	template <class ValueType>
	struct is_null_t<ValueType*>
	{
		static constexpr bool call(ValueType * const & value)
		{
			return value == nullptr;
		}
	};

	template <class ValueType>
	bool is_null(const ValueType & value)
	{
		return is_null_t<ValueType>::call(value);
	}
}

template <class ObjectType, size_t MaxSize>
class CPoolBase
{
public:
	static constexpr size_t Capacity = MaxSize;
	static constexpr size_t InvalidIndex = static_cast<size_t>(-1);

	CPoolBase() = default;
	CPoolBase(const CPoolBase&) = delete;
	CPoolBase &operator=(const CPoolBase&) = delete;
	virtual ~CPoolBase() = default;

	virtual ObjectType &operator[](size_t index) = 0;
	virtual ObjectType &Get(size_t index) = 0;
	virtual bool IsValid(size_t index) const = 0;

	virtual size_t TopIndex() const
	{
		return CPoolBase::Capacity - 1;
	}

	template <class Function>
	typename std::result_of<Function(ObjectType &)>::type Map(size_t index, Function func)
	{
		if (IsValid(index))
		{
			return func((*this)[index]);
		}
		return typename std::result_of<Function(ObjectType &)>::type();
	}
};


template <class ObjectType, size_t MaxSize, class ExtraData = std::tuple<size_t>>
class CExtendedPool : public CPoolBase<ObjectType, MaxSize>
{
	typedef CPoolBase<ObjectType, MaxSize> Base;
	std::unordered_map<size_t, ExtraData> extraData;

public:
	CExtendedPool() = default;

	ExtraData &Extra(size_t index)
	{
		auto iter = extraData.find(index);
		if (iter == extraData.end())
		{
			return extraData.emplace(index, index).first->second;
		}
		return iter->second;
	}

	bool RemoveExtra(size_t index)
	{
		return extraData.erase(index) > 0;
	}

	template <class Function>
	typename std::result_of<Function(ExtraData &)>::type MapExtra(size_t index, Function func)
	{
		auto iter = extraData.find(index);
		if (iter == extraData.end())
		{
			return typename std::result_of<Function(ExtraData &)>::type();
		}
		return func(iter->second);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType (PoolType::*PoolArray)[MaxSize], class ExtraData = std::tuple<size_t>>
class CBasicPool : public CExtendedPool<ObjectType, MaxSize, ExtraData>
{
	typedef CExtendedPool<ObjectType, MaxSize, ExtraData> Base;
protected:
	PoolType& pool;
public:
	CBasicPool(PoolType& poolData) : pool(poolData)
	{

	}

	virtual ObjectType &operator[](size_t index) OVERRIDE
	{
		return (pool.*PoolArray)[index];
	}

	virtual ObjectType &Get(size_t index) OVERRIDE
	{
		if (!IsValid(index)) throw std::invalid_argument("Invalid index accessed.");
		return (pool.*PoolArray)[index];
	}

	virtual bool IsValid(size_t index) const OVERRIDE
	{
		return index >= 0 && index < MaxSize && !aux::is_null((pool.*PoolArray)[index]);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType (PoolType::*PoolArray)[MaxSize], BOOL(PoolType::*SlotArray)[MaxSize], class ExtraData = std::tuple<size_t>>
class CSlotPool : public CBasicPool<PoolType, ObjectType, MaxSize, PoolArray, ExtraData>
{
	typedef CBasicPool<PoolType, ObjectType, MaxSize, PoolArray, ExtraData> Base;
public:
	CSlotPool(PoolType& poolData) : Base(poolData)
	{

	}

	virtual bool IsValid(size_t index) const OVERRIDE
	{
		return index >= 0 && index < MaxSize && (Base::pool.*SlotArray)[index] && !aux::is_null((Base::pool.*PoolArray)[index]);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType (PoolType::*PoolArray)[MaxSize], BOOL(PoolType::*SlotArray)[MaxSize], DWORD PoolType::*PoolSize, class ExtraData = std::tuple<size_t>>
class CBoundedPool : public CSlotPool<PoolType, ObjectType, MaxSize, PoolArray, SlotArray, ExtraData>
{
	typedef CSlotPool<PoolType, ObjectType, MaxSize, PoolArray, SlotArray, ExtraData> Base;
public:
	CBoundedPool(PoolType& poolData) : Base(poolData)
	{

	}

	virtual size_t TopIndex() const OVERRIDE
	{
		return Base::pool.*PoolSize;
	}
};

#endif
