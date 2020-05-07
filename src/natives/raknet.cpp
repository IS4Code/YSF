#include "../Natives.h"
#include "../CPlugin.h"
#include "../CAddresses.h"
#include "../CScriptParams.h"
#include "../Utils.h"

#include "raknet/NetworkTypes.h"

#include <cstring>

namespace Natives
{
	// native ClearBanList();
	AMX_DECLARE_NATIVE(ClearBanList)
	{
		if (!CPlugin::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		CSAMPFunctions::ClearBanList();
		CPlugin::Get()->ClearBans();
		return 1;
	}

	// native IsBanned(ipaddress[]);
	AMX_DECLARE_NATIVE(IsBanned)
	{
		CHECK_PARAMS(1, LOADED);

		char *ip;
		amx_StrParam(amx, params[1], ip);
		return (ip) ? CPlugin::Get()->IsBanned(ip) : 0;
	}

	// native SetTimeoutTime(playerid, time);
	AMX_DECLARE_NATIVE(SetTimeoutTime)
	{
		CHECK_PARAMS(2, LOADED);

		const PlayerID playerId = CSAMPFunctions::GetPlayerIDFromIndex(CScriptParams::Get()->ReadInt());
		if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress || !IsPlayerConnected(static_cast<int>(params[1])))
			return 0;

		CSAMPFunctions::SetTimeoutTime(static_cast<RakNetTime>(CScriptParams::Get()->ReadInt()), playerId);
		return 1;
	}

	// native GetLocalIP(index, localip[], len = sizeof(localip));
	AMX_DECLARE_NATIVE(GetLocalIP)
	{
		CHECK_PARAMS(3, LOADED);

		return set_amxstring(amx, params[2], CSAMPFunctions::GetLocalIP(CScriptParams::Get()->ReadInt()), params[3]);
	}

	// native SendRPC(playerid, RPC, {Float,_}:...)
	AMX_DECLARE_NATIVE(SendRPC)
	{
		if (!CPlugin::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		const bool bBroadcast = static_cast<int>(params[1]) == -1;
		BYTE rpcid = static_cast<BYTE>(params[2]);

		PlayerID playerId = bBroadcast ? UNASSIGNED_PLAYER_ID : CSAMPFunctions::GetPlayerIDFromIndex(static_cast<int>(params[1]));

		if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress && !bBroadcast)
			return 0;

		RakNet::BitStream bs;
		cell *type = (cell*)0;
		cell *data = (cell*)0;

		for (int i = 0; i < (int)((params[0] / sizeof(cell)) - 2); i += 2)
		{
			amx_GetAddr(amx, params[i + 3], &type);
			amx_GetAddr(amx, params[i + 4], &data);

			if (type && data)
			{
				switch (*type)
				{
				case BS_BOOL:
					bs.Write((bool)(*data != 0));
					break;
				case BS_CHAR:
					bs.Write(*(char*)data);
					break;
				case BS_UNSIGNEDCHAR:
					bs.Write(*(unsigned char*)data);
					break;
				case BS_SHORT:
					bs.Write(*(short*)data);
					break;
				case BS_UNSIGNEDSHORT:
					bs.Write(*(unsigned short*)data);
					break;
				case BS_INT:
					bs.Write(*(int*)data);
					break;
				case BS_UNSIGNEDINT:
					bs.Write(*(unsigned int*)data);
					break;
				case BS_FLOAT:
					bs.Write(*(float*)data);
					break;
				case BS_STRING:
				{
					int len;
					amx_StrLen(data, &len);
					len++;
					char* str = new char[len];
					amx_GetString(str, data, 0, len);
					bs.Write(str, len - 1);
					//logprintf("str: %s", str);
					delete[] str;
				}
				break;
				}
			}
		}

		if (bBroadcast)
		{
			CSAMPFunctions::RPC(&rpcid, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true, 0);
		}
		else
		{
			CSAMPFunctions::RPC(&rpcid, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, playerId, 0, 0);
		}
		return 1;
	}

	// native SendData(playerid, {Float,_}:...)
	AMX_DECLARE_NATIVE(SendData)
	{
		if (!CPlugin::Get()->IsInitialized()) return std::numeric_limits<int>::lowest(); // If unknown server version

		const bool bBroadcast = static_cast<int>(params[1]) == -1;
		const PlayerID playerId = bBroadcast ? UNASSIGNED_PLAYER_ID : CSAMPFunctions::GetPlayerIDFromIndex(static_cast<int>(params[1]));

		if (playerId.binaryAddress == UNASSIGNED_PLAYER_ID.binaryAddress && !bBroadcast)
			return 0;

		RakNet::BitStream bs;
		cell *type = (cell*)0;
		cell *data = (cell*)0;

		for (int i = 0; i < (int)((params[0] / sizeof(cell)) - 2); i += 2)
		{
			amx_GetAddr(amx, params[i + 2], &type);
			amx_GetAddr(amx, params[i + 3], &data);

			if (type && data)
			{
				switch (*type)
				{
				case BS_BOOL:
					bs.Write((bool)(*data != 0));
					break;
				case BS_CHAR:
					bs.Write(*(char*)data);
					break;
				case BS_UNSIGNEDCHAR:
					bs.Write(*(unsigned char*)data);
					break;
				case BS_SHORT:
					bs.Write(*(short*)data);
					break;
				case BS_UNSIGNEDSHORT:
					bs.Write(*(unsigned short*)data);
					break;
				case BS_INT:
					bs.Write(*(int*)data);
					break;
				case BS_UNSIGNEDINT:
					bs.Write(*(unsigned int*)data);
					break;
				case BS_FLOAT:
					bs.Write(*(float*)data);
					break;
				case BS_STRING:
				{
					int len;
					amx_StrLen(data, &len);
					len++;
					char* str = new char[len];
					amx_GetString(str, data, 0, len);
					bs.Write(str, len - 1);
					//logprintf("str: %s", str);
					delete[] str;
				}
				break;
				}
			}
		}

		if (bBroadcast)
		{
			CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
		}
		else
		{
			CSAMPFunctions::Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, playerId, 0);
		}
		return 1;
	}

	// native ToggleCloseConnectionFix(bool:toggle);
	AMX_DECLARE_NATIVE(ToggleCloseConnectionFix)
	{
		CHECK_PARAMS(1, LOADED);

		auto &arr = *CAddress::ADDR_WrongPacketIDBranch;
		static bool toggled = false;
		static unsigned char backup[sizeof(arr)];

		bool toggle = CScriptParams::Get()->ReadBool();
		if(toggle != toggled)
		{
			toggled = toggle;
			if(toggle)
			{
				std::memcpy(backup, arr, sizeof(arr));
				std::memset(arr, 0x90, sizeof(arr));
			}else{
				std::memcpy(arr, backup, sizeof(arr));
			}
		}
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// RakServer functions
	AMX_DEFINE_NATIVE(ClearBanList)
	AMX_DEFINE_NATIVE(IsBanned)

	AMX_DEFINE_NATIVE(SetTimeoutTime)
	AMX_DEFINE_NATIVE(GetLocalIP)

	AMX_DEFINE_NATIVE(SendRPC)
	AMX_DEFINE_NATIVE(SendData)

	AMX_DEFINE_NATIVE(ToggleCloseConnectionFix)
};

void RakNetLoadNatives()
{
	RegisterNatives(native_list);
}
