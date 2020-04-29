#ifndef YSF_CCONFIG_H
#define YSF_CCONFIG_H

#include "CSingleton.h"

class CConfig : public CSingleton<CConfig>
{
public:
	CConfig();

	bool m_bPickupProtection : 1;
	bool m_bDeathProtection : 1;
	bool m_bDialogProtection : 1;
	bool m_bUseCustomSpawn : 1;
	bool m_bUsePerPlayerGangZones : 1;
	bool m_bAllowRemoteRCONWithBannedIPs : 1;
	bool m_bIncreaseRakNetInternalPlayers : 1;
	int m_iRakNetInternalSleepTime;
	int m_iAttachObjectDelay;
	bool m_bStorePlayerObjectsMaterial : 1;
	bool m_bGroupPlayerObjects : 1;
};

#endif
