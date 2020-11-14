#ifndef YSF_CScriptParams_H
#define YSF_CScriptParams_H

#include <type_traits>
#include <sdk/amx/amx.h>
#include <string>

#include "CSingleton.h"
#include "CVector.h"
#include "CVector2D.h"
#include "amxfunctions.h"

class CScriptParams : public CSingleton<CScriptParams>
{
	friend class CSingleton<CScriptParams>;
public:
	CScriptParams();
	~CScriptParams() = default;

	enum Flags
	{
		NO_FLAGS = 0,
		LOADED = 1,
		MORE_PARAMETER_ALLOWED = 2,
	};

	bool Setup(size_t paramscount, std::string &&strNativeName, Flags flags, AMX* amx, cell* params, size_t start = 1);
	cell HandleError();

	template<typename T> void Add(T a);
	template<typename T, typename... Args> void Add(T a, Args &&...args);

	template<typename T> void Read(T &a);
	template<typename T, typename... Args> void Read(T &a, Args &&...args);

	inline void Skip() { m_pos++; }

private:
	template <class numType>
	void AddInternal(numType num);
	void AddInternal(float num);
	void AddInternal(double num);
	void AddInternal(CVector2D vec);
	void AddInternal(CVector vec);
	void AddInternal(const char *str);
	void AddInternal(const wchar_t *str);
	void AddInternal(const std::string &str);
	void AddInternal(char *str)
	{
		AddInternal(static_cast<const char *>(str));
	}
	void AddInternal(wchar_t *str)
	{
		AddInternal(static_cast<const wchar_t *>(str));
	}
	void AddInternal(std::string &str)
	{
		AddInternal(static_cast<const std::string &>(str));
	}

	template <class numType>
	void ReadInternal(numType &num);
	void ReadInternal(float &num);
	void ReadInternal(double &num);
	void ReadInternal(CVector2D &vec);
	void ReadInternal(CVector &vec);
	void ReadInternal(std::string &str);
	void ReadInternal(std::wstring &str);

	inline const cell &ReadCell()
	{
		return m_params[m_pos++];
	}

public:
	
	int ReadInt()
	{
		return static_cast<int>(ReadCell());
	}
	
	float ReadFloat()
	{
		float var;
		ReadInternal(var);
		return var;
	}
	
	bool ReadBool()
	{
		return static_cast<bool>(ReadCell());
	}

	void DetectError();

private:

	// private variables
	int m_paramscount;
	std::string m_strNativeName;
	Flags m_flags;
	AMX* m_AMX;
	cell *m_params;
	size_t m_pos;

	enum Error 
	{
		NONE,
		ISNT_LOADED,
		PARAMETER_COUNT_ISNT_EQUAL,
		PARAMETER_COUNT_ISNT_ENOUGH
	};
	int m_error;
};

//----------------------------------------------------

template <class numType>
void CScriptParams::AddInternal(numType num)
{
	static_assert(std::is_integral<numType>::value, "Only integral type can be converted.");
	cell *address;
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
	{
		*address = static_cast<cell>(num);
	}
}

template <class numType>
void CScriptParams::ReadInternal(numType &num)
{
	static_assert(std::is_integral<numType>::value, "Only integral type can be converted.");
	num = static_cast<numType>(ReadCell());
}

//----------------------------------------------------

template<typename T>
void inline CScriptParams::Add(T a)
{
	AddInternal(a);
}

template<typename T, typename... Args>
void inline CScriptParams::Add(T a, Args &&...args)
{
	AddInternal(a);
	Add(std::forward<Args>(args)...);
}

//----------------------------------------------------

template<typename T>
void inline CScriptParams::Read(T &a)
{
	ReadInternal(a);
}

template<typename T, typename... Args>
void inline CScriptParams::Read(T &a, Args &&...args)
{
	ReadInternal(a);
	Read(std::forward<Args>(args)...);
}
#endif
