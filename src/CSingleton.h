#ifndef YSF_SINGLETON_H
#define YSF_SINGLETON_H

// maddinat0r - thx! https://github.com/maddinat0r/samp-discord-connector/blob/master/src/CSingleton.hpp

#include <memory>
#include <stdexcept>
#include "linux_fix.h"

template<class T>
class CSingleton
{
protected:
	static std::unique_ptr<T> m_Instance;

public:
	CSingleton()
	{

	}

	virtual ~CSingleton()
	{

	}

	template <class... Args>
	inline static T *Init(Args&&... args)
	{
		if (m_Instance != nullptr) return m_Instance.get();
		m_Instance = std::make_unique<T>(std::forward<Args>(args)...);
		return m_Instance.get();
	}

	inline static T *Get()
	{
		if (m_Instance == nullptr) throw std::logic_error("An instance must be initialized first.");
		return m_Instance.get();
	}

	inline static bool IsInitialized()
	{
		return m_Instance != nullptr;
	}

	inline static void Destroy()
	{
		m_Instance = nullptr;
	}
};

template <class T>
std::unique_ptr<T> CSingleton<T>::m_Instance = nullptr;

#endif