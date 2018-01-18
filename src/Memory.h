#ifndef YSF_MEMORY_H
#define YSF_MEMORY_H

#include "includes/types.h"

bool Unlock(void *address, size_t len);

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask);
DWORD FindPattern(char *pattern, char *mask);
void InstallJump(unsigned long addr, void *func);

#endif