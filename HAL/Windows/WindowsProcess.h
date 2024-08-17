#pragma once

#include "GameCore/Core.h"

struct WindowsPlatformProcess
{
    static void SetThreadName(const wchar_t* name);
    static void SleepMS(int32 Milliseconds);
    static void ShowConsole();

    // function to get platform name
    static const char* GetPlatformName();
};

typedef WindowsPlatformProcess PlatformProcess;
