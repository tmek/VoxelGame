#pragma once
#include <memory>
#include "Platform.h"

#ifndef TEXT
    #ifdef _UNICODE
        #define TEXT(x) L##x
    #else
        #define TEXT(x) x
    #endif
#endif


/** Represents a generic platform window */
class GenericWindow
{
public:
    
    /** Options for creating a window */
    struct WindowOptions
    {
        const TCHAR* Title = TEXT("Voxel Game");
        const TCHAR* ClassName = TEXT("VoxelGameWindow");
        int32 Width = 1920;
        int32 Height = 1080;
    };

    /** Create a window with the given options or defaults. */
    GenericWindow(const WindowOptions& options = WindowOptions());

    // rule of five
    GenericWindow(const GenericWindow&) = delete;
    GenericWindow& operator=(const GenericWindow&) = delete;
    GenericWindow(GenericWindow&&) = default;
    GenericWindow& operator=(GenericWindow&&) = default;
    ~GenericWindow();

    void SetTitle(const TCHAR* title) const;

    void ForceRepaint() const;

    void ProcessMessageQueue() const;

    /** Returns the OS window handle cast to a void pointer */
    void* GetHandle() const;

private:
    struct Impl;

    /** Pimpl idiom to hide Windows-specific details */
    std::unique_ptr<Impl> pImpl; 

    void RegisterWindowClass();
    
    int32 Create();
};
