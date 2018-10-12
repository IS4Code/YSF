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

#include "CScriptParams.h"
#include "CPlugin.h"
#include "Globals.h"

CScriptParams::CScriptParams()
{

}

//----------------------------------------------------

bool CScriptParams::Setup(size_t paramscount, std::string &&strNativeName, Flags flags, AMX* amx, cell* params, size_t start)
{
	static std::string g_NativesPrefix("Natives::");

	// Initialize variables for later use
	m_paramscount = paramscount;
	if (strNativeName.length() >= g_NativesPrefix.length() && std::equal(g_NativesPrefix.begin(), g_NativesPrefix.end(), strNativeName.begin())) // Removing "Natives::" tag from the begining of the function
	{
		m_strNativeName = std::move(strNativeName.substr(9));
	} else {
		m_strNativeName = std::move(strNativeName);
	}
	m_flags = flags;
	m_AMX = amx;
	m_params = params;
	m_pos = start;

	m_error = NONE;

	// Checking for errors
	DetectError();
	return m_error != 0;
}

//----------------------------------------------------

void CScriptParams::DetectError()
{
	if (m_flags & Flags::LOADED)
	{
		if (!CPlugin::IsInitialized())
			m_error = ISNT_LOADED;

		//logprintf("first check: %d", m_error);
		if (m_error) return;
	}
	
	if (m_flags & Flags::MORE_PARAMETER_ALLOWED)
	{
		if (m_params[0] < (m_paramscount * static_cast<int>(sizeof(cell))))
		{
			m_error = PARAMETER_COUNT_ISNT_ENOUGH;
		}
	}
	else
	{
		if (m_params[0] != (m_paramscount * static_cast<int>(sizeof(cell))))
		{
			m_error = PARAMETER_COUNT_ISNT_EQUAL;
		}
	}
}

//----------------------------------------------------

cell CScriptParams::HandleError()
{
	switch (m_error)
	{
		case PARAMETER_COUNT_ISNT_ENOUGH:
		{
			logprintf("YSF: %s: Expecting at least %d parameter(s), but found only %d", m_strNativeName.c_str(), m_paramscount, m_params[0] / sizeof(cell));
			break;
		}
		case PARAMETER_COUNT_ISNT_EQUAL:
		{
			logprintf("YSF: %s: Expecting %d parameter(s), but found %d", m_strNativeName.c_str(), m_paramscount, m_params[0] / sizeof(cell));
			break;
		}
	}
	return std::numeric_limits<int>::lowest() + (m_error - 1);
}

//----------------------------------------------------

void CScriptParams::AddInternal(float num)
{
	cell *address;
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
	{
		*address = amx_ftoc(num);
	}
}

void CScriptParams::AddInternal(double num)
{
	cell *address;
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
	{
		*address = amx_ftoc(static_cast<const float &>(num));
	}
}

// Vectors
void CScriptParams::AddInternal(CVector2D vec)
{
	cell *address;
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fX);
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fY);
}

void CScriptParams::AddInternal(CVector vec)
{
	cell *address;
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fX);
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fY);
	if (amx_GetAddr(m_AMX, ReadCell(), &address) == AMX_ERR_NONE)
		*address = amx_ftoc(vec.fZ);
}

// Strings
void CScriptParams::AddInternal(const char *str)
{
	cell addr = ReadCell();
	int maxsize = ReadInt();
	set_amxstring(m_AMX, addr, str, maxsize);
}

void CScriptParams::AddInternal(const std::string &str)
{
	cell addr = ReadCell();
	int maxsize = ReadInt();
	set_amxstring(m_AMX, addr, str, maxsize);
}

//----------------------------------------------------

void CScriptParams::ReadInternal(float &num)
{
	num = amx_ctof(ReadCell());
}

void CScriptParams::ReadInternal(double &num)
{
	num = static_cast<double>(amx_ctof(ReadCell()));
}

// Vectors
void CScriptParams::ReadInternal(CVector2D &vec)
{
	vec.fX = amx_ctof(ReadCell());
	vec.fY = amx_ctof(ReadCell());
}
void CScriptParams::ReadInternal(CVector &vec)
{
	vec.fX = amx_ctof(ReadCell());
	vec.fY = amx_ctof(ReadCell());
	vec.fZ = amx_ctof(ReadCell());
}

// Strings
void CScriptParams::ReadInternal(std::string &result)
{
	cell *addr;
	int length;

	amx_GetAddr(m_AMX, ReadCell(), &addr);
	amx_StrLen(addr, &length);

	if (length == 0)
	{
		result.clear();
		return;
	}

	result = std::string(length, '\0');
	amx_GetString(&result[0], addr, 0, length + 1);
}

void CScriptParams::ReadInternal(std::wstring &result)
{
	cell *addr;
	int length;

	amx_GetAddr(m_AMX, ReadCell(), &addr);
	amx_StrLen(addr, &length);

	if (length == 0)
	{
		result.clear();
		return;
	}

	result = std::wstring(length, '\0');
	amx_GetString(reinterpret_cast<char*>(&result[0]), addr, 1, length + 1);
}
