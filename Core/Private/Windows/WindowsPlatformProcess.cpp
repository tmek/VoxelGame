
#include <cstdio>

#include "Logging/LogMacros.h"

#include "Windows/WindowsIncludes.h"
#include "Windows/WindowsPlatformProcess.h"

void WindowsPlatformProcess::SleepMS(int32 Milliseconds)
{
    ::Sleep(Milliseconds);
}

void WindowsPlatformProcess::ShowConsole()
{
    auto hwnd = GetConsoleWindow();
    if (hwnd)
    {
        TE_LOG(LogConsole, Log, "Showing existing Console.");
        ShowWindow(hwnd, SW_SHOW);
        return;
    }
    
    if (!AllocConsole())
    {
        // Use GetLastError to get the error code
        auto ErrorCode = GetLastError();
        
        TE_LOG(LogConsole, Error, "Error allocating console: %d", ErrorCode);
        return;
    }

    FILE* file_out = nullptr;
    if (freopen_s(&file_out, "CONOUT$", "w", stdout) != 0)
    {
        TE_LOG(LogConsole, Error, "Error redirecting stdout");
        return;
    }

    FILE* file_err = nullptr;
    if (freopen_s(&file_err, "CONOUT$", "w", stderr) != 0)
    {
        TE_LOG(LogConsole, Error, "Error redirecting stderr");
        return;
    }

    FILE* file_in = nullptr;
    if (freopen_s(&file_in, "CONIN$", "r", stdin) != 0)
    {
        TE_LOG(LogConsole, Error, "Error redirecting stdin");
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
        SetWindowPos(consoleWindow, NULL, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
    else
    {
       TE_LOG(LogConsole, Error, "Error getting monitor info");
    }

    TE_LOG(LogConsole, Log, "Console initialized successfully!");
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
