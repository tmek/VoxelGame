#pragma once

#include "Platform.h"
#include "Windows/PlatformWindowsMinimal.h"

/*
 * Represents an OS window for the platform.
 */
class PlatformWindow 
{
public:
    PlatformWindow();

    void ProcessMessageQueue();
    HWND GetHandle() const { return hwnd; }
    void SetTitle(const char* title);
    void ForceRepaint();

private:
    static constexpr TCHAR ClassName[] = L"VoxelGameWindow";
    HWND hwnd;
    
    void RegisterWindowClass();
    int32 Create();

};

// todo: you could have something external that maps PlatformWindow to hwnd to remove the dependency on windef.h
