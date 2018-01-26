/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

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
