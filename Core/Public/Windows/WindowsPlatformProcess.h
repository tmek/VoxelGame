#pragma once

#include "CoreTypes.h"

struct CORE_API WindowsPlatformProcess
{
    static void SetThreadName(const wchar_t* name);
    static void SleepMS(int32 Milliseconds);
    static void ShowConsole();

    // function to get platform name
    static const TCHAR* GetPlatformName();
};

typedef CORE_API WindowsPlatformProcess PlatformProcess;
