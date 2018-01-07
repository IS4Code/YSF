#include "../main.h"
#include "../Natives.h"

// native IsMenuDisabled(Menu:menuid);
AMX_DECLARE_NATIVE(Natives::IsMenuDisabled)
{
	CHECK_PARAMS(1, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return !!(!pMenu->interaction.Menu);
}

// native IsMenuRowDisabled(Menu:menuid, row);
AMX_DECLARE_NATIVE(Natives::IsMenuRowDisabled)
{
	CHECK_PARAMS(2, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	const int itemid = CScriptParams::Get()->ReadInt();
	if (itemid < 0 || itemid >= 12) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return !!(!pMenu->interaction.Row[itemid]);
}

// native GetMenuColumns(menuid);
AMX_DECLARE_NATIVE(Natives::GetMenuColumns)
{
	CHECK_PARAMS(1, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return pMenu->byteColumnsNumber;
}

// native GetMenuItems(menuid, column);
AMX_DECLARE_NATIVE(Natives::GetMenuItems)
{
	CHECK_PARAMS(2, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	const int column = CScriptParams::Get()->ReadInt();
	if (menuid < 0 || menuid > 2) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	return pMenu->byteItemsCount[column];
}

// native GetMenuPos(menuid, &Float:fX, &Float:fY);
AMX_DECLARE_NATIVE(Natives::GetMenuPos)
{
	CHECK_PARAMS(3, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	CScriptParams::Get()->Add(pMenu->vecPos);
	return 1;
}

// native GetMenuColumnWidth(menuid, &Float:fColumn1, &Float:fColumn2);
AMX_DECLARE_NATIVE(Natives::GetMenuColumnWidth)
{
	CHECK_PARAMS(4, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	CScriptParams::Get()->Add(pMenu->fColumn1Width, pMenu->fColumn2Width);
	return 1;
}

// native GetMenuColumnHeader(menuid, column, header[], len = sizeof(header));
AMX_DECLARE_NATIVE(Natives::GetMenuColumnHeader)
{
	CHECK_PARAMS(4, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	const int column = CScriptParams::Get()->ReadInt();
	if (menuid < 0 || menuid > 2) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	CScriptParams::Get()->Add(&pMenu->szHeaders[column][0]);
	return 1;
}

// native GetMenuItem(menuid, column, itemid, item[], len = sizeof(item));
AMX_DECLARE_NATIVE(Natives::GetMenuItem)
{
	CHECK_PARAMS(5, LOADED);

	const int menuid = CScriptParams::Get()->ReadInt();
	if (menuid < 1 || menuid >= MAX_MENUS) return 0;

	const int column = CScriptParams::Get()->ReadInt();
	if (menuid < 0 || menuid > 2) return 0;

	const int itemid = CScriptParams::Get()->ReadInt();
	if (itemid < 0 || itemid >= 12) return 0;

	if (!pNetGame->pMenuPool->bIsCreated[menuid]) return 0;
	CMenu *pMenu = pNetGame->pMenuPool->pMenu[menuid];

	CScriptParams::Get()->Add(&pMenu->szItems[itemid][column][0]);
	return 1;
}