#include <d3d11.h>
#include <wrl/client.h>

// Using Microsoft's WRL (Windows Runtime Library) for smart pointers
using Microsoft::WRL::ComPtr;// = TSharedRef;

class D3D11Renderer
{
public:
    D3D11Renderer(HWND hWnd);
    void RenderFrame();

private:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;

    void InitializeDeviceAndContext();
    void CreateSwapChain(HWND hWnd);
    void CreateRenderTargetView();
};

D3D11Renderer::D3D11Renderer(HWND hWnd)
{
    InitializeDeviceAndContext();
    CreateSwapChain(hWnd);
    CreateRenderTargetView();
}

void D3D11Renderer::InitializeDeviceAndContext()
{
    // Create device and context
    D3D11CreateDevice(
        nullptr, // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr, // No software rasterizer
        0, // Flags (like D3D11_CREATE_DEVICE_DEBUG)
        nullptr, // Feature levels
        0, // Number of feature levels
        D3D11_SDK_VERSION,
        &device,
        nullptr, // Feature level
        &context
    );
}

void D3D11Renderer::CreateSwapChain(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = 0; // Use window's client area dimensions
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1; // No anti-aliasing
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    ComPtr<IDXGIDevice> dxgiDevice;
    device.As(&dxgiDevice);

    ComPtr<IDXGIAdapter> adapter;
    dxgiDevice->GetAdapter(&adapter);

    ComPtr<IDXGIFactory> factory;
    adapter->GetParent(__uuidof(IDXGIFactory), &factory);

    factory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain);
}

void D3D11Renderer::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
    device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);
}

void D3D11Renderer::RenderFrame()
{
    // Clear the render target to a sky blue color
    const float clearColor[4] = { 0.529f, 0.808f, 0.922f, 1.0f }; // Sky blue color
    context->ClearRenderTargetView(renderTargetView.Get(), clearColor);

    // Present the frame
    swapChain->Present(1, 0);
}
