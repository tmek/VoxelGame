#pragma once

#include "CoreTypes.h"

struct CORE_API WindowsPlatformProcess
{
    static void SleepMS(int32 Milliseconds);
    static void ShowConsole();

    // function to get platform name
    static const char* GetPlatformName();
};

