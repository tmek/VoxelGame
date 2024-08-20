// copyright

#include "Hal/Windows/PlatformWindows.h"

extern signed int GuardedMain( const TCHAR* CmdLine );

// globals
HINSTANCE GInstanceHandle;
const TCHAR* GCommandLine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    ::GInstanceHandle = hInstance;
    ::GCommandLine = ::GetCommandLineW();
    
    GuardedMain(GCommandLine);

    return 0;
}