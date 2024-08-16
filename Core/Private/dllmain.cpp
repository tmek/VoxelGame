#include <cstdio>
#include <windows.h>

#include "Logging/LogMacros.h"

#include "HAL/Platform.h"
#include "HAL/PlatformProcess.h"

using namespace ELogVerbosity;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        TE_LOG(LogTemp, Log, "[%s] DllMain: DLL_PROCESS_ATTACH", ModuleName);
        break;
    case DLL_THREAD_ATTACH:
        TE_LOG(LogTemp, Log, "[%s] DllMain: DLL_THREAD_ATTACH", ModuleName);
        break;
    case DLL_THREAD_DETACH:
        TE_LOG(LogTemp, Log, "[%s] DllMain: DLL_THREAD_DETACH", ModuleName);
        break;
    case DLL_PROCESS_DETACH:
        TE_LOG(LogTemp, Log, "[%s] DllMain: DLL_PROCESS_DETACH", ModuleName);
        break;
    default:
        TE_LOG(LogTemp, Log, "[%s] DllMain: default", ModuleName);
        break;
    }

    return TRUE;
}
