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

	bool Setup(size_t paramscount, std::string strNativeName, Flags flags, AMX* amx, cell* params, size_t start = 1);
	cell HandleError();

	template <class templateType> inline void Add(templateType var);
	template<typename T> void AddInline(T a);
	template<typename T, typename... Args> void AddInline(T a, Args... args);

	template <class templateType> void Read(templateType *var);
	template<typename T> void ReadInline(T a);
	template<typename T, typename... Args> void ReadInline(T a, Args... args);

	inline void Skip() { m_pos++; }
	
	int ReadInt()
	{
		int var;
		Read(&var);
		return var;
	}
	
	float ReadFloat()
	{
		float var;
		Read(&var);
		return var;
	}
	
	bool ReadBool()
	{
		bool var;
		Read(&var);
		return var;
	}
private:	
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
template <class templateType> inline void CScriptParams::Add(templateType var)
{
	cell *address;
	amx_GetAddr(m_AMX, m_params[m_pos++], &address);
	if (typeid(var) == typeid(float) || typeid(var) == typeid(double))
		*address = amx_ftoc(var);
	else
		*address = static_cast<cell>(var);
}

// Vectors
template <> inline void CScriptParams::Add(CVector2D vec)
{
	cell *address;
	amx_GetAddr(m_AMX, m_params[m_pos++], &address);
	*address = amx_ftoc(vec.fX);
	amx_GetAddr(m_AMX, m_params[m_pos++], &address);
	*address = amx_ftoc(vec.fY);
}

template <> inline void CScriptParams::Add(CVector vec)
{
	cell *address;
	amx_GetAddr(m_AMX, m_params[m_pos++], &address);
	*address = amx_ftoc(vec.fX);
	amx_GetAddr(m_AMX, m_params[m_pos++], &address);
	*address = amx_ftoc(vec.fY);
	amx_GetAddr(m_AMX, m_params[m_pos++], &address);
	*address = amx_ftoc(vec.fZ);
}

// Strings
template <> inline void CScriptParams::Add(char* szString)
{
	set_amxstring(m_AMX, m_params[m_pos], szString, m_params[m_pos + 1]);

	m_pos += 2;
}
template <> inline void CScriptParams::Add(std::string &str)
{
	set_amxstring(m_AMX, m_params[m_pos], str.c_str(), m_params[m_pos + 1]);

	m_pos += 2;
}

//----------------------------------------------------

template<typename T>
void inline CScriptParams::AddInline(T a)
{
	Add(a);
}

template<typename T, typename... Args>
void inline CScriptParams::AddInline(T a, Args... args)
{
	Add(a);
	AddInline(args...);
}

//----------------------------------------------------

// Numbers & Floats
template <class templateType> void CScriptParams::Read(templateType* var)
{
	*var = (*reinterpret_cast<const templateType*>(&static_cast<const cell&>(m_params[m_pos++])));
}

// Vectors
template <> inline void CScriptParams::Read(CVector2D* vec)
{
	vec->fX = amx_ctof(m_params[m_pos++]);
	vec->fY = amx_ctof(m_params[m_pos++]);
}
template <> inline void CScriptParams::Read(CVector* vec)
{
	vec->fX = amx_ctof(m_params[m_pos++]);
	vec->fY = amx_ctof(m_params[m_pos++]);
	vec->fZ = amx_ctof(m_params[m_pos++]);
}
// Strings
template <> inline void CScriptParams::Read(char *result)
{
	// FUCK amx_StrParam
	cell *amx_cstr;
	int amx_length;
	amx_GetAddr(m_AMX, m_params[m_pos++], &amx_cstr);
	amx_StrLen(amx_cstr, &amx_length);
	if (amx_length > 0)
	{
		amx_GetString((char*)(result), amx_cstr, sizeof(*(result)) > 1, amx_length + 1);
	}
}


//----------------------------------------------------

template<typename T>
void inline CScriptParams::ReadInline(T a)
{
	Read(a);
}

template<typename T, typename... Args>
void inline CScriptParams::ReadInline(T a, Args... args)
{
	Read(a);
	ReadInline(args...);
}
#endif
