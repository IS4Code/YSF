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

#include "main.h"

CScriptParams::CScriptParams()
{

}

//----------------------------------------------------

bool CScriptParams::Setup(size_t paramscount, std::string &&strNativeName, Flags flags, AMX* amx, cell* params, size_t start)
{
	// Initialize variables for later use
	m_paramscount = paramscount;
	m_strNativeName = std::move(strNativeName.substr(9)); // Removing "Natives::" tag from the begining of the function
	m_flags = flags;
	m_AMX = amx;
	m_params = params;
	m_pos = start;

	m_error = NONE;

	// Checking for errors
	DetectError();
	return static_cast<int>(m_error != 0);
}

//----------------------------------------------------

void CScriptParams::DetectError()
{
	if (m_flags & Flags::LOADED)
	{
		if (!CServer::Get()->IsInitialized())
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
