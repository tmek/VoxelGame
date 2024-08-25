// Copyright

#pragma once

#include <memory>
#include "Platform.h"


/** Represents a generic platform window */
class CORE_API PlatformWindow
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
    PlatformWindow(const WindowOptions& options = WindowOptions());

    // rule of five
    PlatformWindow(const PlatformWindow&) = delete;
    PlatformWindow& operator=(const PlatformWindow&) = delete;
    PlatformWindow(PlatformWindow&&) = default;
    PlatformWindow& operator=(PlatformWindow&&) = default;
    ~PlatformWindow();

    void SetTitle(const TCHAR* title) const;

    void ForceRepaint() const;

    void ProcessMessageQueue() const;

    /** Returns the OS window handle cast to a void pointer */
    void* GetHandle() const;

private:
    struct Impl;

#pragma warning(push)
#pragma warning(disable:4251)
    /** Pimpl idiom to hide Windows-specific details */
    std::unique_ptr<Impl> pImpl;
#pragma warning(pop)

    void RegisterWindowClass();
    
    int32 Create();
};
