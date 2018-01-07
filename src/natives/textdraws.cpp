#include "../main.h"
#include "../Natives.h"

// native IsValidTextDraw(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::IsValidTextDraw)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	return pNetGame->pTextDrawPool->bSlotState[textdrawid];
}

// native IsTextDrawVisibleForPlayer(playerid, Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::IsTextDrawVisibleForPlayer)
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
AMX_DECLARE_NATIVE(Natives::TextDrawGetString)
{
	CHECK_PARAMS(3, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	const char *szText = (pNetGame->pTextDrawPool->bSlotState[textdrawid]) ? pNetGame->pTextDrawPool->szFontText[textdrawid] : '\0';
	return set_amxstring(amx, params[2], szText, params[3]);
}

// native TextDrawSetPos(Text:textdrawid, Float:fX, Float:fY);
AMX_DECLARE_NATIVE(Natives::TextDrawSetPos)
{
	CHECK_PARAMS(3, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	CScriptParams::Get()->Read(&pTD->vecPos);
	return 1;
}

// native TextDrawGetLetterSize(Text:textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::TextDrawGetLetterSize)
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
AMX_DECLARE_NATIVE(Natives::TextDrawGetTextSize)
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
AMX_DECLARE_NATIVE(Natives::TextDrawGetPos)
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
AMX_DECLARE_NATIVE(Natives::TextDrawGetColor)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return ABGR_RGBA(pTD->dwLetterColor);
}

// native TextDrawGetBoxColor(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetBoxColor)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return ABGR_RGBA(pTD->dwBoxColor);
}

// native TextDrawGetBackgroundColor(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetBackgroundColor)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return ABGR_RGBA(pTD->dwBackgroundColor);
}

// native TextDrawGetShadow(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetShadow)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteShadow;
}

// native TextDrawGetOutline(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetOutline)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteOutline;
}

// native TextDrawGetFont(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetFont)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteStyle;
}

// native TextDrawIsBox(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawIsBox)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteBox;
}

// native TextDrawIsProportional(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawIsProportional)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteProportional;
}

// native TextDrawIsSelectable(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawIsSelectable)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->byteSelectable;
}

// native TextDrawGetAlignment(Text:textdrawid);
AMX_DECLARE_NATIVE(Natives::TextDrawGetAlignment)
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
AMX_DECLARE_NATIVE(Natives::TextDrawGetPreviewModel)
{
	CHECK_PARAMS(1, LOADED);

	const int textdrawid = CScriptParams::Get()->ReadInt();
	if (textdrawid < 0 || textdrawid >= MAX_TEXT_DRAWS) return 0;

	if (!pNetGame->pTextDrawPool->bSlotState[textdrawid]) return 0;
	CTextdraw *pTD = pNetGame->pTextDrawPool->TextDraw[textdrawid];

	return pTD->dwModelIndex;
}

// native TextDrawGetPreviewRot(Text:textdrawid, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fZoom);
AMX_DECLARE_NATIVE(Natives::TextDrawGetPreviewRot)
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
AMX_DECLARE_NATIVE(Natives::TextDrawGetPreviewVehCol)
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
AMX_DECLARE_NATIVE(Natives::TextDrawSetStringForPlayer)
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
AMX_DECLARE_NATIVE(Natives::IsValidPlayerTextDraw)
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
AMX_DECLARE_NATIVE(Natives::IsPlayerTextDrawVisible)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetString)
{
	CHECK_PARAMS(4, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int textdrawid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;
	if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;

	bool bIsValid = static_cast<int>(pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) != 0;
	if (!bIsValid) return 0;

	const char *szText = (bIsValid) ? pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->szFontText[textdrawid] : '\0';
	return set_amxstring(amx, params[3], szText, params[4]);
}

// native PlayerTextDrawSetPos(playerid, PlayerText:textdrawid, Float:fX, Float:fY);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawSetPos)
{
	CHECK_PARAMS(4, LOADED);

	const int playerid = CScriptParams::Get()->ReadInt();
	const int textdrawid = CScriptParams::Get()->ReadInt();

	if (!IsPlayerConnected(playerid)) return 0;
	if (textdrawid >= MAX_PLAYER_TEXT_DRAWS) return 0;
	if (!pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->bSlotState[textdrawid]) return 0;

	CTextdraw *pTD = pNetGame->pPlayerPool->pPlayer[playerid]->pTextdraw->TextDraw[textdrawid];

	CScriptParams::Get()->Read(&pTD->vecPos);
	return 1;
}

// native PlayerTextDrawGetLetterSize(playerid, PlayerText:textdrawid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetLetterSize)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetTextSize)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPos)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetColor)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetBoxColor)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetBackgroundCol)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetShadow)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetOutline)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetFont)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawIsBox)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawIsProportional)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawIsSelectable)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetAlignment)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPreviewModel)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPreviewRot)
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
AMX_DECLARE_NATIVE(Natives::PlayerTextDrawGetPreviewVehCol)
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
