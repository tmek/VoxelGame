#include "WindowsProcess.h"

#include <stdio.h>
#include "HAL/Windows/PlatformWindows.h"

void WindowsPlatformProcess::SleepMS(int32 Milliseconds)
{
    ::Sleep(Milliseconds);
}

void WindowsPlatformProcess::ShowConsole()
{
    if (!AllocConsole())
    {
        fprintf(stderr, "Error allocating console\n");
        return;
    }

    FILE* file_out = nullptr;
    if (freopen_s(&file_out, "CONOUT$", "w", stdout) != 0)
    {
        fprintf(stderr, "Error redirecting stdout\n");
        return;
    }

    FILE* file_err = nullptr;
    if (freopen_s(&file_err, "CONOUT$", "w", stderr) != 0)
    {
        fprintf(stderr, "Error redirecting stderr\n");
        return;
    }

    FILE* file_in = nullptr;
    if (freopen_s(&file_in, "CONIN$", "r", stdin) != 0)
    {
        fprintf(stderr, "Error redirecting stdin\n");
        return;
    }

    // Get the handle to the console window
    HWND consoleWindow = GetConsoleWindow();

    // Get the monitor information for the secondary monitor
    POINT pt = {0, 0};
    HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfo(hMonitor, &monitorInfo))
    {
        // Calculate the position for the console window on the secondary monitor
        int posX = monitorInfo.rcMonitor.left;
        int posY = monitorInfo.rcMonitor.top;

        // Move the console window to the desired position
        //SetWindowPos(consoleWindow, NULL, posX-1024, posY+100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        int offsetX = -1024;
        int offsetY = 100;
        SetWindowPos(consoleWindow, NULL, posX+offsetX, posY+offsetY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
    else
    {
        fprintf(stderr, "Error getting monitor info\n");
    }

    // Test printing to console
    printf("Console initialized successfully!\n");
    fflush(stdout);
}

const char* WindowsPlatformProcess::GetPlatformName()
{
    // want to know if we are running on 32 or 64 bit and if release or debug
#if defined(_WIN64)
    #if defined(_DEBUG)
                return "Windows 64-bit Debug";
    #else
                return "Windows 64-bit Release";
    #endif
#else
    #if defined(_DEBUG)
                return "Windows 32-bit Debug";
    #else
                return "Windows 32-bit Release";
    #endif
#endif
}
