#include "../Natives.h"
#include "../CPlugin.h"
#include "../CScriptParams.h"
#include "../CFunctions.h"
#include "../Globals.h"
#include "../Utils.h"
#include "../RPCs.h"

namespace Natives
{
	// native IsValidTextDraw(Text:textdrawid);
	AMX_DECLARE_NATIVE(IsValidTextDraw)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		return pNetGame->pTextDrawPool->bSlotState[textdrawid];
	}

	// native IsTextDrawVisibleForPlayer(playerid, Text:textdrawid);
	AMX_DECLARE_NATIVE(IsTextDrawVisibleForPlayer)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;

		return pNetGame->pTextDrawPool->bHasText[textdrawid][playerid];
	}

	// native TextDrawGetString(Text:textdrawid, text[], len = sizeof(text));
	AMX_DECLARE_NATIVE(TextDrawGetString)
	{
		CHECK_PARAMS(3, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		const char *szText = (pNetGame->pTextDrawPool->bSlotState[textdrawid]) ? pNetGame->pTextDrawPool->szFontText[textdrawid] : "";
		return set_amxstring(amx, params[2], szText, params[3]);
	}

	// native TextDrawSetPos(Text:textdrawid, Float:fX, Float:fY);
	AMX_DECLARE_NATIVE(TextDrawSetPos)
	{
		CHECK_PARAMS(3, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		CScriptParams::Get()->Read(pTD->vecPos);
		return 1;
	}

	// native TextDrawGetLetterSize(Text:textdrawid, &Float:fX, &Float:fY);
	AMX_DECLARE_NATIVE(TextDrawGetLetterSize)
	{
		CHECK_PARAMS(3, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecLetter);
		return 1;
	}

	// native TextDrawGetTextSize(Text:textdrawid, &Float:fX, &Float:fY);
	AMX_DECLARE_NATIVE(TextDrawGetTextSize)
	{
		CHECK_PARAMS(3, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecLine);
		return 1;
	}

	// native TextDrawGetPos(Text:textdrawid, &Float:fX, &Float:fY);
	AMX_DECLARE_NATIVE(TextDrawGetPos)
	{
		CHECK_PARAMS(3, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecPos);
		return 1;
	}

	// native TextDrawGetColor(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetColor)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return ABGR_RGBA(pTD->dwLetterColor);
	}

	// native TextDrawGetBoxColor(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetBoxColor)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return ABGR_RGBA(pTD->dwBoxColor);
	}

	// native TextDrawGetBackgroundColor(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetBackgroundColor)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return ABGR_RGBA(pTD->dwBackgroundColor);
	}

	// native TextDrawGetShadow(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetShadow)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return pTD->byteShadow;
	}

	// native TextDrawGetOutline(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetOutline)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return pTD->byteOutline;
	}

	// native TextDrawGetFont(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetFont)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return pTD->byteStyle;
	}

	// native TextDrawIsBox(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawIsBox)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return pTD->byteBox;
	}

	// native TextDrawIsProportional(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawIsProportional)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return pTD->byteProportional;
	}

	// native TextDrawIsSelectable(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawIsSelectable)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return pTD->byteSelectable;
	}

	// native TextDrawGetAlignment(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetAlignment)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		BYTE ret = 0;

		if (pTD->byteCenter) ret = 2;
		else if (pTD->byteLeft) ret = 1;
		else if (pTD->byteRight) ret = 3;
		return ret;
	}

	// native TextDrawGetPreviewModel(Text:textdrawid);
	AMX_DECLARE_NATIVE(TextDrawGetPreviewModel)
	{
		CHECK_PARAMS(1, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		return pTD->dwModelIndex;
	}

	// native TextDrawGetPreviewRot(Text:textdrawid, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fZoom);
	AMX_DECLARE_NATIVE(TextDrawGetPreviewRot)
	{
		CHECK_PARAMS(5, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecRot, pTD->fZoom);
		return 1;
	}

	// native TextDrawGetPreviewVehCol(Text:textdrawid, &color1, &color2);
	AMX_DECLARE_NATIVE(TextDrawGetPreviewVehCol)
	{
		CHECK_PARAMS(3, LOADED);

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->color1, pTD->color2);
		return 1;
	}

	// native TextDrawSetStringForPlayer(Text:textdrawid, playerid, const string[], {Float,_}:...);
	AMX_DECLARE_NATIVE(TextDrawSetStringForPlayer)
	{
		if (CScriptParams::Get()->Setup(3, __FUNCTION__, static_cast<CScriptParams::Flags>(CScriptParams::Flags::LOADED | CScriptParams::Flags::MORE_PARAMETER_ALLOWED), amx, params)) return CScriptParams::Get()->HandleError();

		const int textdrawid = CScriptParams::Get()->ReadInt();
		if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

		const int playerid = CScriptParams::Get()->ReadInt();
		if (!IsPlayerConnected(playerid)) return 0;

		if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
		CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

		int len;
		char* szMessage = CSAMPFunctions::format_amxstring(amx, params, 3, len);
		if (!szMessage) return 0;

		RakNet::BitStream bs;
		bs.Write((WORD)textdrawid);
		bs.Write((WORD)len);
		bs.Write(szMessage, len + 1);

		CSAMPFunctions::RPC(&RPC_SetTextDrawString, &bs, HIGH_PRIORITY, RELIABLE, 0, CSAMPFunctions::GetPlayerIDFromIndex(playerid), false, false);
		return 1;
	}

	// Per-Player textdraws
	// native IsValidPlayerTextDraw(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(IsValidPlayerTextDraw)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid];
	}

	// native IsPlayerTextDrawVisible(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(IsPlayerTextDrawVisible)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		return pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bHasText[textdrawid];
	}

	// native PlayerTextDrawGetString(playerid, PlayerText:textdrawid, text[], len = sizeof(text));
	AMX_DECLARE_NATIVE(PlayerTextDrawGetString)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;

		bool bIsValid = static_cast<int>(pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) != 0;
		if (!bIsValid) return 0;

		const char *szText = (bIsValid) ? pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->szFontText[textdrawid] : "";
		return set_amxstring(amx, params[3], szText, params[4]);
	}

	// native PlayerTextDrawSetPos(playerid, PlayerText:textdrawid, Float:fX, Float:fY);
	AMX_DECLARE_NATIVE(PlayerTextDrawSetPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

		CScriptParams::Get()->Read(pTD->vecPos);
		return 1;
	}

	// native PlayerTextDrawGetLetterSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetLetterSize)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecLetter);
		return 1;
	}

	// native PlayerTextDrawGetTextSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetTextSize)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecLine);
		return 1;
	}

	// native PlayerTextDrawGetPos(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPos)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecPos);
		return 1;
	}

	// native PlayerTextDrawGetColor(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetColor)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return ABGR_RGBA(pTD->dwLetterColor);
	}

	// native PlayerTextDrawGetBoxColor(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetBoxColor)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return ABGR_RGBA(pTD->dwBoxColor);
	}

	// native PlayerTextDrawGetBackgroundCol(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetBackgroundCol)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return ABGR_RGBA(pTD->dwBackgroundColor);
	}

	// native PlayerTextDrawGetShadow(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetShadow)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return pTD->byteShadow;
	}

	// native PlayerTextDrawGetOutline(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetOutline)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return pTD->byteOutline;
	}

	// native PlayerTextDrawGetFont(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetFont)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return pTD->byteStyle;
	}

	// native PlayerTextDrawIsBox(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawIsBox)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return pTD->byteBox;
	}

	// native PlayerTextDrawIsProportional(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawIsProportional)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return pTD->byteProportional;
	}

	// native PlayerTextDrawIsSelectable(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawIsSelectable)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return pTD->byteSelectable;
	}

	// native PlayerTextDrawGetAlignment(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetAlignment)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		BYTE ret = 0;

		if (pTD->byteCenter) ret = 2;
		else if (pTD->byteLeft) ret = 1;
		else if (pTD->byteRight) ret = 3;
		return ret;
	}

	// native PlayerTextDrawGetPreviewModel(playerid, PlayerText:textdrawid);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPreviewModel)
	{
		CHECK_PARAMS(2, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];
		return pTD->dwModelIndex;
	}

	// native PlayerTextDrawGetPreviewRot(playerid, PlayerText:textdrawid, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fZoom);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPreviewRot)
	{
		CHECK_PARAMS(6, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->vecRot, pTD->fZoom);
		return 1;
	}

	// native PlayerTextDrawGetPreviewVehCol(playerid, PlayerText:textdrawid, &color1, &color2);
	AMX_DECLARE_NATIVE(PlayerTextDrawGetPreviewVehCol)
	{
		CHECK_PARAMS(4, LOADED);

		const int playerid = CScriptParams::Get()->ReadInt();
		const int textdrawid = CScriptParams::Get()->ReadInt();

		if (!IsPlayerConnected(playerid)) return 0;
		if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
		if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

		CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

		CScriptParams::Get()->Add(pTD->color1, pTD->color2);
		return 1;
	}
}

static AMX_NATIVE_INFO native_list[] =
{
	// Textdraw functions
	AMX_DEFINE_NATIVE(IsValidTextDraw)
	AMX_DEFINE_NATIVE(IsTextDrawVisibleForPlayer)
	AMX_DEFINE_NATIVE(TextDrawGetString)
	AMX_DEFINE_NATIVE(TextDrawSetPos)
	AMX_DEFINE_NATIVE(TextDrawGetLetterSize)
	AMX_DEFINE_NATIVE(TextDrawGetTextSize)
	AMX_DEFINE_NATIVE(TextDrawGetPos)
	AMX_DEFINE_NATIVE(TextDrawGetColor)
	AMX_DEFINE_NATIVE(TextDrawGetBoxColor)
	AMX_DEFINE_NATIVE(TextDrawGetBackgroundColor)
	AMX_DEFINE_NATIVE(TextDrawGetShadow)
	AMX_DEFINE_NATIVE(TextDrawGetOutline)
	AMX_DEFINE_NATIVE(TextDrawGetFont)
	AMX_DEFINE_NATIVE(TextDrawIsBox)
	AMX_DEFINE_NATIVE(TextDrawIsProportional)
	AMX_DEFINE_NATIVE(TextDrawIsSelectable) // R6
	AMX_DEFINE_NATIVE(TextDrawGetAlignment)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewModel)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewRot)
	AMX_DEFINE_NATIVE(TextDrawGetPreviewVehCol)
	AMX_DEFINE_NATIVE(TextDrawSetStringForPlayer) // R19

	// Per-Player Textdraw functions - R4
	AMX_DEFINE_NATIVE(IsValidPlayerTextDraw)
	AMX_DEFINE_NATIVE(IsPlayerTextDrawVisible)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetString)
	AMX_DEFINE_NATIVE(PlayerTextDrawSetPos)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetLetterSize)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetTextSize)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPos)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetColor)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetBoxColor)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetBackgroundCol)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetShadow)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetOutline)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetFont)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsBox)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsProportional)
	AMX_DEFINE_NATIVE(PlayerTextDrawIsSelectable) // R6
	AMX_DEFINE_NATIVE(PlayerTextDrawGetAlignment)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewModel)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewRot)
	AMX_DEFINE_NATIVE(PlayerTextDrawGetPreviewVehCol)
	AMX_DEFINE_NATIVE(TextDrawSetStringForPlayer) // R19
};

void TextDrawsLoadNatives()
{
	RegisterNatives(native_list);
}
