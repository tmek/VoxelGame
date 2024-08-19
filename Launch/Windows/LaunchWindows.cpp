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
    VG_LOG(LogCategoryGeneral, LOG_INFO, "Windows Platform Entry Point (WinMain)");

    // name main thread
    VG_LOG(LogCategoryGeneral, LOG_INFO, "Setting main thread name to 'MainThread'");
    WindowsPlatformProcess::SetThreadName(L"MainThread"); // todo: this should happen very early (e.g. in static intialization somewhere)

    // set main thread priority
    VG_LOG(LogCategoryGeneral, LOG_INFO, "Setting main thread priority to THREAD_PRIORITY_ABOVE_NORMAL");
    ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_NORMAL);

    
    ::G_hInstance = hInstance;
    GCmdLine = ::GetCommandLineW();

    GuardedMain(GCmdLine);

    return 0;
}