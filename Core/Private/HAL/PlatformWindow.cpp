

#include <stdexcept>

#include "Input/InputManager.h"
#include "HAL/PlatformWindow.h"

#include "Windows/WindowsHWrapper.h"


struct PlatformWindow::Impl
{
    inline static const WIDECHAR* WindowClassName = L"VoxelGameWindow"; 
    
    HWND Hwnd;
    WindowOptions Options;
    HINSTANCE InstanceHandle;
    
    Impl(const WindowOptions& options)
        : Hwnd(nullptr), Options(options)
    {
        InstanceHandle = GetModuleHandle(nullptr);
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        InputManager::Get().Update(uMsg, wParam, lParam);

        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            GIsRequestingExit = true;
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Placeholder for custom painting code

            EndPaint(hwnd, &ps);
            break;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        return 0;
    }
};

PlatformWindow::PlatformWindow(const WindowOptions& options)
    : pImpl(std::make_unique<Impl>(options))
{
    RegisterWindowClass();
    Create();
}

PlatformWindow::~PlatformWindow()
{
    if (pImpl->Hwnd)
    {
        DestroyWindow(pImpl->Hwnd);
    }
    UnregisterClass(Impl::WindowClassName, pImpl->InstanceHandle);
}

void PlatformWindow::RegisterWindowClass()
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = Impl::WindowProc;  // Use the static method in Impl
    wc.hInstance = pImpl->InstanceHandle;
    wc.lpszClassName = Impl::WindowClassName;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_MENUTEXT);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClass(&wc))
    {
        throw std::runtime_error("Failed to register window class");
    }
}

int32_t PlatformWindow::Create()
{
    RECT windowRect = { 0, 0, pImpl->Options.Width, pImpl->Options.Height };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    pImpl->Hwnd = CreateWindowEx(
        0,
        Impl::WindowClassName,
        pImpl->Options.Title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
        nullptr,
        nullptr,
        pImpl->InstanceHandle,
        nullptr
    );

    if (!pImpl->Hwnd)
    {
        throw std::runtime_error("Failed to create window");
    }

    ShowWindow(pImpl->Hwnd, SW_SHOWDEFAULT);
    UpdateWindow(pImpl->Hwnd);

    return 0;
}

void PlatformWindow::SetTitle(const TCHAR* title) const
{
#ifdef UNICODE
    SetWindowText(pImpl->Hwnd, title);
#else
    SetWindowTextW(pImpl->Hwnd, &title);
#endif
}

void PlatformWindow::ForceRepaint() const
{
    InvalidateRect(pImpl->Hwnd, nullptr, TRUE);
    UpdateWindow(pImpl->Hwnd);
}

void PlatformWindow::ProcessMessageQueue() const
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void* PlatformWindow::GetHandle() const
{
    return pImpl->Hwnd;
}

/*
// todo: if we want these back in going to have to come up with platform agnostic way to do them. (mirror enums, bitflags, structs, etc)
int32 Left = DefaultPosition;
int32 Top = DefaultPosition;
uint32 WindowStyle = DefaultWindowStyle;
int ShowCommand = DefaultShowCommand;

private:
// Constants to hide Windows-specific values (todo: will have to create platform-agnostic versions)
static constexpr std::int32_t DefaultPosition = -1; // Custom default to represent CW_USEDEFAULT
static constexpr std::uint32_t DefaultWindowStyle = 0x00CF0000L; // WS_OVERLAPPEDWINDOW
static constexpr std::int32_t DefaultShowCommand = 10; // SW_SHOWDEFAULT
*/