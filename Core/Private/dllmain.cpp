#include <cstdio>
#include <windows.h>

#include "Logging/LogMacros.h"

#include "HAL/Platform.h"
#include "HAL/PlatformProcess.h"

using namespace ELogVerbosity;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    const char* ReasonString = nullptr;
    
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ReasonString = "DLL_PROCESS_ATTACH";
        break;
    case DLL_THREAD_ATTACH:
        ReasonString = "DLL_THREAD_ATTACH";
        break;
    case DLL_THREAD_DETACH:
        ReasonString = "DLL_THREAD_DETACH";
        break;
    case DLL_PROCESS_DETACH:
        ReasonString = "DLL_PROCESS_DETACH";
        break;
    default:
        ReasonString = "Reason Unknown";
    }

    //TE_LOG(LogTemp, Log, "%s::DllMain: %s", ModuleName, ReasonString);
    
    return TRUE;
}
