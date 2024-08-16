#pragma once

#include "GameCore/Core.h"

struct WindowsPlatformProcess
{
    static void SleepMS(int32 Milliseconds);
    static void ShowConsole();

    // function to get platform name
    static const char* GetPlatformName();
};

typedef WindowsPlatformProcess PlatformProcess;
