#ifndef YSF_HOOKS_H
#define YSF_HOOKS_H

#include "includes/types.h"

extern char gRecordingDataPath[MAX_PATH];

void InstallPreHooks();
void InstallPostHooks();
void UninstallHooks();

#endif
