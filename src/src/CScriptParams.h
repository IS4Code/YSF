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

	enum Flags
	{
		NO_FLAGS = 0,
		LOADED = 1,
		MORE_PARAMETER_ALLOWED = 2,
	};

	bool Setup(size_t paramscount, std::string &strNativeName, Flags flags, AMX* amx, cell* params, size_t start = 1);
	cell HandleError();
	
	template<typename T> void Add(T a);
	template<typename T1, typename T2> void Add(T1 a, T2 b);
	template<typename T1, typename T2, typename T3> void Add(T1 a, T2 b, T3 c);
	template<typename T1, typename T2, typename T3, typename T4> void Add(T1 a, T2 b, T3 c, T4 d);
	template<typename T1, typename T2, typename T3, typename T4, typename T5> void Add(T1 a, T2 b, T3 c, T4 d, T5 e);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6> void Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7> void Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8> void Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9> void Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10> void Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i, T10 j);
	
	template<typename T> void Read(T a);
	template<typename T1, typename T2> void Read(T1 a, T2 b);
	template<typename T1, typename T2, typename T3> void Read(T1 a, T2 b, T3 c);
	template<typename T1, typename T2, typename T3, typename T4> void Read(T1 a, T2 b, T3 c, T4 d);
	template<typename T1, typename T2, typename T3, typename T4, typename T5> void Read(T1 a, T2 b, T3 c, T4 d, T5 e);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6> void Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7> void Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8> void Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9> void Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i);
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10> void Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i, T10 j);

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
		if (typeid(templateType) == typeid(float) || typeid(templateType) == typeid(double))
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

	if (amx_length > 0 && temp != NULL)
	{
		amx_GetString(temp, amx_cstr, 0, amx_length + 1);
		result->append(temp);
	}
	
	delete[] temp;
	temp = NULL;
}


//----------------------------------------------------

template<typename T>
void inline CScriptParams::Read(T a)
{
	ReadInternal(a);
}

template<typename T1, typename T2>
void inline CScriptParams::Read(T1 a, T2 b)
{
	ReadInternal(a);
	ReadInternal(b);
}
template<typename T1, typename T2, typename T3>
void inline CScriptParams::Read(T1 a, T2 b, T3 c)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
}
template<typename T1, typename T2, typename T3, typename T4>
void inline CScriptParams::Read(T1 a, T2 b, T3 c, T4 d)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
	ReadInternal(d);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
void inline CScriptParams::Read(T1 a, T2 b, T3 c, T4 d, T5 e)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
	ReadInternal(d);
	ReadInternal(e);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void inline CScriptParams::Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
	ReadInternal(d);
	ReadInternal(e);
	ReadInternal(f);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void inline CScriptParams::Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
	ReadInternal(d);
	ReadInternal(e);
	ReadInternal(f);
	ReadInternal(g);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void inline CScriptParams::Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
	ReadInternal(d);
	ReadInternal(e);
	ReadInternal(f);
	ReadInternal(g);
	ReadInternal(h);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void inline CScriptParams::Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
	ReadInternal(d);
	ReadInternal(e);
	ReadInternal(f);
	ReadInternal(g);
	ReadInternal(h);
	ReadInternal(i);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
void inline CScriptParams::Read(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i, T10 j)
{
	ReadInternal(a);
	ReadInternal(b);
	ReadInternal(c);
	ReadInternal(d);
	ReadInternal(e);
	ReadInternal(f);
	ReadInternal(g);
	ReadInternal(h);
	ReadInternal(i);
	ReadInternal(j);
}

//----------------------------------------------------

template<typename T>
void inline CScriptParams::Add(T a)
{
	AddInternal(a);
}

template<typename T1, typename T2>
void inline CScriptParams::Add(T1 a, T2 b)
{
	AddInternal(a);
	AddInternal(b);
}
template<typename T1, typename T2, typename T3>
void inline CScriptParams::Add(T1 a, T2 b, T3 c)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
}
template<typename T1, typename T2, typename T3, typename T4>
void inline CScriptParams::Add(T1 a, T2 b, T3 c, T4 d)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
	AddInternal(d);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
void inline CScriptParams::Add(T1 a, T2 b, T3 c, T4 d, T5 e)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
	AddInternal(d);
	AddInternal(e);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void inline CScriptParams::Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
	AddInternal(d);
	AddInternal(e);
	AddInternal(f);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void inline CScriptParams::Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
	AddInternal(d);
	AddInternal(e);
	AddInternal(f);
	AddInternal(g);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void inline CScriptParams::Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
	AddInternal(d);
	AddInternal(e);
	AddInternal(f);
	AddInternal(g);
	AddInternal(h);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void inline CScriptParams::Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
	AddInternal(d);
	AddInternal(e);
	AddInternal(f);
	AddInternal(g);
	AddInternal(h);
	AddInternal(i);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
void inline CScriptParams::Add(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h, T9 i, T10 j)
{
	AddInternal(a);
	AddInternal(b);
	AddInternal(c);
	AddInternal(d);
	AddInternal(e);
	AddInternal(f);
	AddInternal(g);
	AddInternal(h);
	AddInternal(i);
	AddInternal(j);
}

#endif
