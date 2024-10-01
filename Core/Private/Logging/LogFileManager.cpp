#include "LogFileManager.h"

// Definition of static members
std::wofstream LogFileManager::LogFile;
std::mutex LogFileManager::LogMutex;

// Global instance of the LogFileManager, initialized before main()
LogFileManager GLogManager(L"LogOutput.txt");

