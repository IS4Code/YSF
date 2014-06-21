#ifndef __YSF_HOOKS
#define __YSF_HOOKS

// Hopefully this will be optimised out
// Unfortunately I can't get ANY direct cast to work
// pointer_to_member seems like a very special structure
// Edit: Seems like it is optimised out, good, so this
// just bypasses the compilers checks to do what I want
#define POINTER_TO_MEMBER(m,n,o) temp = n, m = *(o *)&temp

bool Unlock(void *address, size_t len);
void AssemblySwap(char * addr, char * dat, int len);
void AssemblyRedirect(void * from, void * to, char * ret);

void Redirect(AMX * amx, char const * const from, ucell to, AMX_NATIVE * store);

#ifdef WIN32
DWORD FindPattern(char *pattern, char *mask);
#endif

void GetAddresses();
void InstallPreHooks();

#endif
