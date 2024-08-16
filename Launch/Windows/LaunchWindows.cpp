#include "Hal/Windows/PlatformWindows.h"

#include "GameCore/Logging.h"
#include "HAL/PlatformWindow.h"
#include "HAL/Windows/WindowsProcess.h"


extern signed int GuardedMain( const TCHAR* CmdLine );


HINSTANCE G_hInstance;
const TCHAR* GCmdLine;

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    WindowsPlatformProcess::ShowConsole();

    VG_LOG(LogCategoryGeneral, LOG_INFO, "Application Entry Point");
    
    ::G_hInstance = hInstance;
    GCmdLine = ::GetCommandLineW();

    GuardedMain(GCmdLine);

    return 0;
}