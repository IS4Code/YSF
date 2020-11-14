#ifndef YSF_AMXFUNCTIONS_H
#define YSF_AMXFUNCTIONS_H

#include <string>
#include <sdk/amx/amx.h>

//void Redirect(AMX * amx, char const * const from, ucell to, AMX_NATIVE * store);
cell* get_amxaddr(AMX *amx, cell amx_addr);
int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
int set_amxstring(AMX *amx, cell amx_addr, const wchar_t *source, int max);
int set_amxstring(AMX *amx, cell amx_addr, const std::string &source, int max);

#endif
