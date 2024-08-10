#pragma once

#include "Core/Core.h"
#include "Windows.h"
//#include "minwindef.h"

/*
 * Represents an OS window for the platform.
 */
class PlatformWindow 
{
public:
    PlatformWindow();

    void ProcessMessageQueue();
    HWND GetHandle() const { return hwnd; }
    void SetTitle(const char* title) { SetWindowTextA(hwnd, title); }
    void ForceRepaint();

private:
    void RegisterWindowClass();
    int32 Create();

    HWND hwnd;

    static constexpr TCHAR ClassName[] = L"VoxelGameWindow";
};

// todo: you could have something external that maps PlatformWindow to hwnd to remove the dependency on windef.h
