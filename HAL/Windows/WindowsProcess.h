#pragma once

#include "GameCore/Core.h"

struct WindowsProcess
{
    static void Sleep(int32 Milliseconds);
    static void ShowConsole();

    // function to get platform name
    static const char* GetPlatformName();
};

typedef WindowsProcess PlatformProcess;
