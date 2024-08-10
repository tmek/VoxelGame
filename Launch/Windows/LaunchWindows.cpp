#include <windows.h>
#include <stdio.h>
#include "Core/Logging.h"
#include "HAL/PlatformWindow.h"
#include "Input/InputManager.h"


extern signed int GuardedMain( const TCHAR* CmdLine );


void HelloWorldMsgBox()
{
    // create a "Hello World" message box using MessageBox()
    MessageBox(NULL,
               L"Hello World!",
               L"Just another Hello World program!",
               MB_ICONEXCLAMATION | MB_OK);
}

HINSTANCE G_hInstance;
const TCHAR* GCmdLine;

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    VG_LOG(LOG_CATEGORY_GENERAL, LOG_INFO, "Application Entry Point");
    
    ::G_hInstance = hInstance;
    GCmdLine = ::GetCommandLineW();

    GuardedMain(GCmdLine);

    return 0;
}