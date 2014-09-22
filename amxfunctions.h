#ifndef __AMXFUNCTIONS_H
#define __AMXFUNCTIONS_H

#include "SDK\plugin.h"

void Redirect(AMX * amx, char const * const from, ucell to, AMX_NATIVE * store);
int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);

#endif
