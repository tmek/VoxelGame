#include "HAL/Windows/PlatformWindows.h"
#include "PlatformWindow.h" 
#include "Input/InputManager.h"


extern HINSTANCE G_hInstance;
extern bool GIsRequestingExit;

LRESULT CALLBACK ProcessWindowMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

PlatformWindow::PlatformWindow()
{
    RegisterWindowClass();
    Create();   
}

void PlatformWindow::RegisterWindowClass()
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = ProcessWindowMessage;
    wc.hInstance = G_hInstance;
    wc.lpszClassName = ClassName;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_MENUTEXT);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClass(&wc);
}

int32 PlatformWindow::Create()
{
    constexpr LONG CLIENT_WIDTH = 1920;
    constexpr LONG CLIENT_HEIGHT = 1080;
    // Calculate the required window size
    RECT windowRect = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;
    
    // Create and show the main window
    hwnd = CreateWindowEx(
        0,
        ClassName,
        L"Voxel Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
        nullptr,
        nullptr,
        G_hInstance, 
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, 10);
    UpdateWindow(hwnd);

    return 0;    
}

void PlatformWindow::SetTitle(const char* title)
{ SetWindowTextA(hwnd, title); }

void PlatformWindow::ForceRepaint()
{
    InvalidateRect(hwnd, nullptr, TRUE);
    UpdateWindow(hwnd);
}

void PlatformWindow::ProcessMessageQueue()
{
    static MSG msg = {};

    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }    
}

LRESULT CALLBACK ProcessWindowMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

            // LPCWSTR text = L"Hello, World!";
            // static char TextBuffer[100];
            // int count = sprintf_s(TextBuffer, "WindowProc(): FrameNumber=%d\n", GFrameNumber);
            // TextOutA(hdc, 50, 50, TextBuffer, static_cast<int32>(strlen(TextBuffer)));

            EndPaint(hwnd, &ps);
            break;
        }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}