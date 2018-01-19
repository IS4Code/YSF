#ifndef YSF_POOL_H
#define YSF_POOL_H

#include <tuple>
#include <unordered_map>
#include <exception>
#include "includes/types.h"

namespace aux
{
	template <class ValueType>
	struct assign_t
	{
		static ValueType& call(ValueType& variable, const ValueType& value)
		{
			return variable = value;
		}
	};

	template <class ValueType, size_t Size>
	struct assign_t<ValueType[Size]>
	{
		using Type = ValueType[Size];

		static Type& call(Type& variable, const Type& value)
		{
			for (size_t i = 0; i < Size; i++)
			{
				variable[i] = value[i];
			}
			return variable;
		}
	};

	template <class ValueType>
	ValueType& assign(ValueType& variable, const ValueType& value)
	{
		return assign_t<ValueType>::call(variable, value);
	}

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
			return value != nullptr;
		}
	};

	template <class ValueType>
	bool is_null(const ValueType & value)
	{
		return is_null_t<ValueType>::call(value);
	}
}

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

	/*bool HasExtra(size_t index)
	{
		return extraData.find(index) != extraData.end();
	}*/

	bool RemoveExtra(size_t index)
	{
		return extraData.erase(index) > 0;
	}

	virtual ObjectType &operator[](size_t index) = 0;
	virtual ObjectType &Get(size_t index) = 0;
	virtual bool IsValid(size_t index) const = 0;
	virtual size_t Top() const
	{
		return CPoolBase::Capacity - 1;
	}

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
		return CPoolBase::InvalidIndex;
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType (PoolType::*PoolArray)[MaxSize], class ExtraData = std::tuple<size_t>>
class CBasicPool : public CPoolBase<ObjectType, MaxSize, ExtraData>
{
	typedef CPoolBase<ObjectType, MaxSize, ExtraData> Base;
protected:
	PoolType& pool;
public:
	CBasicPool(PoolType& poolData) : pool(poolData)
	{

	}

	virtual ObjectType &operator[](size_t index) override
	{
		return (pool.*PoolArray)[index];
	}

	virtual ObjectType &Get(size_t index) override
	{
		if (!IsValid(index)) throw std::invalid_argument("Invalid index accessed.");
		return (pool.*PoolArray)[index];
	}

	virtual bool IsValid(size_t index) const override
	{
		return index >= 0 && index < MaxSize && !aux::is_null((pool.*PoolArray)[index]);
	}

protected:
	virtual void Set(size_t index, const ObjectType &object)
	{
		aux::assign<ObjectType>((pool.*PoolArray)[index], std::move(object));
	}

public:
	size_t Add(ObjectType &object)
	{
		size_t i = Base::EmptySlot();
		if (i != Base::InvalidIndex)
		{
			Set(i, object);
		}
		return i;
	}

	template <class... Args>
	size_t New(Args&&... args)
	{
		size_t i = Base::EmptySlot();
		if (i != Base::InvalidIndex)
		{
			Set(i, new ObjectType(std::forward<Args>(args)...));
		}
		return i;
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

	virtual bool IsValid(size_t index) const override
	{
		return index >= 0 && index < MaxSize && (Base::pool.*SlotArray)[index] && !aux::is_null((Base::pool.*PoolArray)[index]);
	}
};

template <class PoolType, class ObjectType, size_t MaxSize, ObjectType (PoolType::*PoolArray)[MaxSize], BOOL(PoolType::*SlotArray)[MaxSize], DWORD PoolType::*TopIndex, class ExtraData = std::tuple<size_t>>
class CBoundedPool : public CSlotPool<PoolType, ObjectType, MaxSize, PoolArray, SlotArray, ExtraData>
{
	typedef CSlotPool<PoolType, ObjectType, MaxSize, PoolArray, SlotArray, ExtraData> Base;
public:
	CBoundedPool(PoolType& poolData) : Base(poolData)
	{

	}

	virtual size_t Top() const override
	{
		return Base::pool.*TopIndex;
	}

protected:
	virtual void Set(size_t index, const ObjectType &object)
	{
		Base::Set(index, object);
		if (index > Top())
		{
			Base::pool.*TopIndex = index;
		}
	}
};

#endif
