// copyright

#include <cstdlib>

#include "HAL/Platform.h"
#include "Windows/WindowsHWrapper.h"

extern signed int GuardedMain( const TCHAR* CmdLine );

/**
 * The command-line invocation string, processed using the standard Windows CommandLineToArgvW implementation.
 * This need to be a static global to avoid object unwinding errors in WinMain when SEH is enabled.
 */
static TCHAR* GSavedCommandLine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    ::GSavedCommandLine = ::GetCommandLineW();
    GuardedMain(GSavedCommandLine);

    return 0;
}