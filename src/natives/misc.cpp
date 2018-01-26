#include <string>

#include "../includes/platform.h"
#include "../System.h"
#include "../Natives.h"
#include "../CScriptParams.h"

namespace Natives
{
	// native execute(const command[], saveoutput=0, index=0);
	AMX_DECLARE_NATIVE(execute)
	{
		CHECK_PARAMS(3, NO_FLAGS);

		std::string command;
		int saveoutput, index;
		CScriptParams::Get()->Read(command, saveoutput, index);

		ExecuteCommand(command, saveoutput != 0, index);
		return 1;
	}

	// native ffind(const pattern[], filename[], len, &idx);
	AMX_DECLARE_NATIVE(ffind)
	{
		// Find a file, idx determines which one of a number of matches to use
		CHECK_PARAMS(4, NO_FLAGS);
		cell *cptr;
		char *szSearch;
		// Get the search pattern
		amx_StrParam(amx, params[1], szSearch);
		if (szSearch)
		{
			// Get associated search information
			amx_GetAddr(amx, params[4], &cptr);
			cell count = *cptr;

			std::string result;
			if (FindFile(szSearch, result, count))
			{
				(*cptr)++;
				amx_GetAddr(amx, params[2], &cptr);
				amx_SetString(cptr, result.c_str(), 0, 0, params[3]);
				return 1;
			}
		}
		return 0;
	}

	// native dfind(const pattern[], filename[], len, &idx);
	AMX_DECLARE_NATIVE(dfind)
	{
		// Find a directory, idx determines which one of a number of matches to use
		// Identical to ffind in all but 1 line
		CHECK_PARAMS(4, NO_FLAGS);
		cell *cptr;
		char *szSearch;
		// Get the search pattern
		amx_StrParam(amx, params[1], szSearch);
		if (szSearch)
		{
			// Get associated search information
			amx_GetAddr(amx, params[4], &cptr);
			cell count = *cptr;

			std::string result;
			if (FindDirectory(szSearch, result, count))
			{
				(*cptr)++;
				amx_GetAddr(amx, params[2], &cptr);
				amx_SetString(cptr, result.c_str(), 0, 0, params[3]);
				return 1;
			}
		}
		return 0;
	}

	// native dcreate(const name[]);
	AMX_DECLARE_NATIVE(dcreate)
	{
		// Creates a directory
		CHECK_PARAMS(1, NO_FLAGS);
		char
			*szSearch;
		// Get the search pattern
		amx_StrParam(amx, params[1], szSearch);
		if (szSearch)
		{
			return static_cast<cell>(CreateNewDirectory(szSearch));
		}
		return 0;
	}

	// native frename(const oldname[], const newname[]);
	AMX_DECLARE_NATIVE(frename)
	{
		// Creates a directory
		CHECK_PARAMS(2, NO_FLAGS);
		char
			*szOld,
			*szNew;
		// Get the search pattern
		amx_StrParam(amx, params[1], szOld);
		amx_StrParam(amx, params[2], szNew);
		if (szOld && szNew)
		{
			char
				*szO = (char *)alloca(strlen(szOld) + 16),
				*szN = (char *)alloca(strlen(szNew) + 16);
			strcpy(szO, "./scriptfiles/");
			strcpy(szO + 14, szOld);
			strcpy(szN, "./scriptfiles/");
			strcpy(szN + 14, szNew);
			return (cell)rename(szO, szN);
		}
		return 0;
	}

	// native drename(const oldname[], const newname[]);
	AMX_DECLARE_NATIVE(drename)
	{
		// Creates a directory
		CHECK_PARAMS(2, NO_FLAGS);
		char
			*szOld,
			*szNew;
		// Get the search pattern
		amx_StrParam(amx, params[1], szOld);
		amx_StrParam(amx, params[2], szNew);
		if (szOld && szNew)
		{
			char
				*szO = (char *)alloca(strlen(szOld) + 16),
				*szN = (char *)alloca(strlen(szNew) + 16);
			strcpy(szO, "./scriptfiles/");
			strcpy(szO + 14, szOld);
			int
				end;
			end = strlen(szO);
			if (szO[end - 1] != '/')
			{
				szO[end] = '/';
				szO[end + 1] = '\0';
			}
			strcpy(szN, "./scriptfiles/");
			strcpy(szN + 14, szNew);
			end = strlen(szN);
			if (szN[end - 1] != '/')
			{
				szN[end] = '/';
				szN[end + 1] = '\0';
			}
			return (cell)rename(szO, szN);
		}
		return 0;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// Execute
	AMX_DEFINE_NATIVE(execute)

	// File
	AMX_DEFINE_NATIVE(ffind)
	AMX_DEFINE_NATIVE(frename)

	// Directory
	AMX_DEFINE_NATIVE(dfind)
	AMX_DEFINE_NATIVE(dcreate)
	AMX_DEFINE_NATIVE(drename)
};

void MiscLoadNatives()
{
	RegisterNatives(native_list);
}
