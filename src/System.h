#ifndef YSF_SYSTEM_H
#define YSF_SYSTEM_H

#include <string>

char* GetNPCCommandLine(int processId);
int FindNPCProcessID(const char *name);
void ExecuteCommand(const std::string &cmd, bool saveOutput, int index);
bool FindFile(const char *szSearch, std::string &result, int count);
bool FindDirectory(const char *szSearch, std::string &result, int count);
bool CreateNewDirectory(const char *name);

#endif