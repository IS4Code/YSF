#ifndef YSF_PLATFORM_H
#define YSF_PLATFORM_H

#include <stdlib.h>
#include <string.h>
#include "types.h"

#ifdef _WIN32
/*#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>*/

inline void LoadTickCount() {}
#else
DWORD GetTickCount();
void LoadTickCount();
#endif

#endif