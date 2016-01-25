#ifndef YSF_HOOKS_H
#define YSF_HOOKS_H

#include <sampgdk/sampgdk.h>

// Hopefully this will be optimised out
// Unfortunately I can't get ANY direct cast to work
// pointer_to_member seems like a very special structure
// Edit: Seems like it is optimised out, good, so this
// just bypasses the compilers checks to do what I want
#define POINTER_TO_MEMBER(m,n,o) temp = n, m = *(o *)&temp

extern AMX_NATIVE pDestroyObject, pDestroyPlayerObject, pTogglePlayerControllable, pSetPlayerWorldBounds, pSetPlayerTeam, pSetPlayerSkin, pSetPlayerFightingStyle, pSetPlayerName, pSetVehicleToRespawn, pChangeVehicleColor, pDestroyVehicle, pAttachObjectToPlayer;

void InstallPreHooks();
void InstallPostHooks();
void UninstallHooks();

#endif
