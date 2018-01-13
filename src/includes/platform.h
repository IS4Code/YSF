#ifndef YSF_PLATFORM_INCLUDED
#define YSF_PLATFORM_INCLUDED

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline void LoadTickCount() {}
#else
#define MAX_PATH 260
#define NULL 0
typedef unsigned long DWORD;
typedef const char *PCHAR;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef float FLOAT;

DWORD GetTickCount();
void LoadTickCount();
#endif

#endif