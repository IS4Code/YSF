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

#include <typeinfo>
#include <sdk/amx/amx.h>
#include <string>

#include "CSingleton.h"
#include "CVector.h"
#include "CVector2D.h"

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

	template<typename T> void Read(T a);
	template<typename T, typename... Args> void Read(T a, Args &&...args);

	inline void Skip() { m_pos++; }
	
	const int ReadInt()
	{
		int var;
		ReadInternal(&var);
		return var;
	}
	
	const float ReadFloat()
	{
		float var;
		ReadInternal(&var);
		return var;
	}
	
	const bool ReadBool()
	{
		bool var;
		ReadInternal(&var);
		return var;
	}
private:
	template <class templateType> inline void AddInternal(templateType var);
	template <class templateType> void ReadInternal(templateType *var);

	void DetectError();

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

// Numbers & Floats
template <class templateType> inline void CScriptParams::AddInternal(templateType var)
{
	cell *address;
	if (amx_GetAddr(m_AMX, m_params[m_pos++], &address) == AMX_ERR_NONE)
	{
		if (typeid(var).hash_code() == typeid(float).hash_code() || typeid(var).hash_code() == typeid(double).hash_code())
			*address = amx_ftoc(var);
		else
			*address = static_cast<cell>(var);
	}
}

// Vectors
template <> inline void CScriptParams::AddInternal(CVector2D vec)
{
	cell *address;
	if (amx_GetAddr(m_AMX, m_params[m_pos++], &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fX);
	if (amx_GetAddr(m_AMX, m_params[m_pos++], &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fY);
}

template <> inline void CScriptParams::AddInternal(CVector vec)
{
	cell *address;
	if (amx_GetAddr(m_AMX, m_params[m_pos++], &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fX);
	if (amx_GetAddr(m_AMX, m_params[m_pos++], &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fY);
	if (amx_GetAddr(m_AMX, m_params[m_pos++], &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fZ);
}

// Strings
template <> inline void CScriptParams::AddInternal(char* szString)
{
	set_amxstring(m_AMX, m_params[m_pos], szString, m_params[m_pos + 1]);

	m_pos += 2;
}
template <> inline void CScriptParams::AddInternal(std::string str)
{
	set_amxstring(m_AMX, m_params[m_pos], str.c_str(), m_params[m_pos + 1]);

	m_pos += 2;
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

// Numbers & Floats
template <class templateType> void CScriptParams::ReadInternal(templateType* var)
{
	*var = (*reinterpret_cast<const templateType*>(&static_cast<const cell&>(m_params[m_pos++])));
}

// Vectors
template <> inline void CScriptParams::ReadInternal(CVector2D* vec)
{
	vec->fX = amx_ctof(m_params[m_pos++]);
	vec->fY = amx_ctof(m_params[m_pos++]);
}
template <> inline void CScriptParams::ReadInternal(CVector* vec)
{
	vec->fX = amx_ctof(m_params[m_pos++]);
	vec->fY = amx_ctof(m_params[m_pos++]);
	vec->fZ = amx_ctof(m_params[m_pos++]);
}
// Strings
template <> inline void CScriptParams::ReadInternal(std::string *result)
{
	// FUCK amx_StrParam
	cell *amx_cstr;
	int amx_length;
	
	amx_GetAddr(m_AMX, m_params[m_pos++], &amx_cstr);
	amx_StrLen(amx_cstr, &amx_length);
	char* temp = new char[amx_length + 1];

	if (amx_length > 0 && temp != nullptr)
	{
		amx_GetString(temp, amx_cstr, 0, amx_length + 1);
		result->append(temp);
	}
	
	SAFE_DELETE(temp);
}


//----------------------------------------------------

template<typename T>
void inline CScriptParams::Read(T a)
{
	ReadInternal(a);
}

template<typename T, typename... Args>
void inline CScriptParams::Read(T a, Args &&...args)
{
	ReadInternal(a);
	Read(std::forward<Args>(args)...);
}
#endif
