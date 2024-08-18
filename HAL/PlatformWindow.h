#pragma once
#include <memory>
#include "Platform.h"

class PlatformWindow
{
public:
    struct WindowOptions
    {
        const WIDECHAR* Title = L"Voxel Game";
        int32 Width = 1920;
        int32 Height = 1080;
    };

    PlatformWindow(const WindowOptions& options = WindowOptions());
    ~PlatformWindow();

    void SetTitle(const WIDECHAR& title) const;
    void ForceRepaint() const;
    void ProcessMessageQueue() const;

    void* GetHandle() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl; // Pimpl idiom to hide Windows-specific details

    void RegisterWindowClass();
    int32 Create();
};
