#ifndef __YSF_HOOKS
#define __YSF_HOOKS

// Hopefully this will be optimised out
// Unfortunately I can't get ANY direct cast to work
// pointer_to_member seems like a very special structure
// Edit: Seems like it is optimised out, good, so this
// just bypasses the compilers checks to do what I want
#define POINTER_TO_MEMBER(m,n,o) temp = n, m = *(o *)&temp

bool Unlock(void *address, int len);
void GetAddresses();
void InstallPreHooks();
void InstallPostHooks();

#endif
