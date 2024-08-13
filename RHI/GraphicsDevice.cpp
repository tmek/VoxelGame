#include "GraphicsDevice.h"

#include <vector>

#include "Render/ImageUtil.h"
#include "GameCore/Logging.h"
#include "GameCore/Macros.h"
#include "VoxelCore/VoxelDefines.h"

using Microsoft::WRL::ComPtr;


GraphicsDevice::GraphicsDevice(HWND hWnd)
{
    InitD3D(hWnd);
}

GraphicsDevice::~GraphicsDevice()
{
    CleanupD3D();
}

struct MyConstantBuffer
{
    DirectX::XMMATRIX WorldViewProjection;
    DirectX::XMFLOAT4 TintColor;
};

extern bool GShouldRenderTintColor;

void GraphicsDevice::CreateConstantBuffer()
{
    // Create constant buffer
    // todo: belongs elsewhere
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(MyConstantBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
    check(hr == S_OK); 
}

void GraphicsDevice::SetConstants(const XMMATRIX& viewProjMatrix, float TintR, float TintG, float TintB)
{
    // update constant buffer
    MyConstantBuffer cb;
    cb.WorldViewProjection = viewProjMatrix;
    if (GShouldRenderTintColor)
    {
        cb.TintColor = {TintR, TintG, TintB, 1.0f};
    }
    else
    {
        cb.TintColor = {0.0f, 0.0f, 0.0f, 1.0f};
    }
    
    deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);    
}

void GraphicsDevice::EnableDepthWrite()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    
    // Enable depth testing and depth writes
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = FALSE;

    ComPtr<ID3D11DepthStencilState> depthStencilState;
    HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
    if (SUCCEEDED(hr)) {
        deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    } else {
        check(hr);
    }
}

void GraphicsDevice::DisableDepthWrite()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    
    // Disable depth writes but keep depth testing
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = FALSE;

    ComPtr<ID3D11DepthStencilState> depthStencilState;
    HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
    if (SUCCEEDED(hr)) {
        deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    } else {
        check(hr);
    }
}

void GraphicsDevice::InitD3D(HWND hWnd)
{
    isValid = false;

    // get hwnd client area
    RECT clientRect;
    int WindowClientWidth = 800; // fallback values
    int WindowClientHeight = 600;
    if (GetClientRect(hWnd, &clientRect))
    {
        WindowClientWidth = clientRect.right - clientRect.left;
        WindowClientHeight = clientRect.bottom - clientRect.top;
    }

    // set swap chain description struct
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = WindowClientWidth; // Set to the window's width
    swapChainDesc.BufferDesc.Height = WindowClientHeight; // Set to the window's height
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    // create device, context and swap chain 
    // it is possible to create a device and context separately from the swapchain, but this is the most common way)
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        swapChain.GetAddressOf(),
        device.GetAddressOf(),
        nullptr,
        deviceContext.GetAddressOf()
    );

    // Creating the Direct3D device.
    // VERIFYD3D11RESULT(D3D11CreateDevice(
    //     Adapter.DXGIAdapter,
    //     DriverType,
    //     NULL,
    //     DeviceFlags,
    //     &FeatureLevel,
    //     1,
    //     D3D11_SDK_VERSION,
    //     Direct3DDevice.GetInitReference(),
    //     &ActualFeatureLevel,
    //     Direct3DDeviceIMContext.GetInitReference()
    // ));
    
    // at this point we have: a device, context and swap chain

    if (FAILED(hr)) {
        VG_LOG(LOG_CATEGORY_GRAPHICS, LOG_ERROR, "Failed to create device and swap chain: %x", hr);
        return;
    }

    // get backBuffer 
    ComPtr<ID3D11Texture2D> backBuffer;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
    if (FAILED(hr)) {
        VG_LOG(LOG_CATEGORY_GRAPHICS, LOG_ERROR, "Failed to get back buffer: %x", hr);
        return;
    }

    // create render target view of back buffer
    hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        VG_LOG(LOG_CATEGORY_GRAPHICS, LOG_ERROR, "Failed to create render target view: %x", hr);
        return;
    }

    viewport.Width = static_cast<FLOAT>(swapChainDesc.BufferDesc.Width);
    viewport.Height = static_cast<FLOAT>(swapChainDesc.BufferDesc.Height);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    //deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
    deviceContext->RSSetViewports(1, &viewport);

    // Define the depth stencil texture description
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = WindowClientWidth; // Set the width of the texture
    depthStencilDesc.Height = WindowClientHeight; // Set the height of the texture
    depthStencilDesc.MipLevels = 1; // We don't need mipmaps for the depth buffer
    depthStencilDesc.ArraySize = 1; // We need only one texture
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Use a 24-bit depth buffer and 8-bit stencil buffer
    depthStencilDesc.SampleDesc.Count = 1; // Use no multi-sampling
    depthStencilDesc.SampleDesc.Quality = 0; // Quality level of the multi-sampling
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // Bind as a depth stencil texture
    depthStencilDesc.CPUAccessFlags = 0; // CPU does not need access
    depthStencilDesc.MiscFlags = 0; // No additional flags

    // Create the depth stencil texture
    ID3D11Texture2D* depthStencilBuffer;
    hr = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
    if (FAILED(hr)) {
        VG_LOG(LOG_CATEGORY_GRAPHICS, LOG_ERROR, "Failed to create depth stencil texture: %x", hr);
        return;
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc2 = {};
    depthStencilDesc2.DepthEnable = TRUE;
    depthStencilDesc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // Disable depth writing
    depthStencilDesc2.DepthFunc = D3D11_COMPARISON_LESS;
    ID3D11DepthStencilState* pDepthStencilState = nullptr;
    device->CreateDepthStencilState(&depthStencilDesc2, &pDepthStencilState);
    deviceContext->OMSetDepthStencilState(pDepthStencilState, 0);
    

    // Define the depth stencil view description
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = depthStencilDesc.Format; // Same format as the depth stencil texture
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // Use 2D texture
    depthStencilViewDesc.Texture2D.MipSlice = 0; // Mipmap slice index

    // Create the depth stencil view
    hr = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
    if (FAILED(hr)) {
        VG_LOG(LOG_CATEGORY_GRAPHICS, LOG_ERROR, "Failed to create depth stencil view: %x", hr);
        return;
    }

    // Release the depth stencil buffer since it's now referenced by the view
    depthStencilBuffer->Release();

    // Define the depth stencil state description
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    depthStencilStateDesc.DepthEnable = TRUE; // Enable depth testing
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // Allow writing to the depth buffer
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS; // Use standard depth test (less is closer)
    depthStencilStateDesc.StencilEnable = FALSE; // Disable stencil testing for now

    // Create the depth stencil state
    ID3D11DepthStencilState* depthStencilState;
    hr = device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
    if (FAILED(hr)) {
        VG_LOG(LOG_CATEGORY_GRAPHICS, LOG_ERROR, "Failed to create depth stencil view: %x", hr);
        return;
    }

    // Set the depth stencil state
    deviceContext->OMSetDepthStencilState(depthStencilState, 1);

    // Bind the Depth Stencil View to the Output Merger Stage
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = TRUE; // Use counter-clockwise vertices as the front face
    rasterDesc.DepthClipEnable = TRUE;

    ID3D11RasterizerState* rasterState;
    device->CreateRasterizerState(&rasterDesc, &rasterState);
    deviceContext->RSSetState(rasterState);

    // create texture
    CreateCheckerboardTexture(device, deviceContext, g_pTexture, g_pTextureView);
    
    // Bind the texture and sampler state
    deviceContext->PSSetShaderResources(0, 1, g_pTextureView.GetAddressOf());
    //deviceContext->PSSetSamplers(0, 1, g_pSamplerLinear.GetAddressOf());    

    CreateConstantBuffer();

    // setup alpha blending
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* pBlendState = nullptr;
    device->CreateBlendState(&blendDesc, &pBlendState);

    // Set the blend state
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    deviceContext->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);    
    
    // Success
    VG_LOG(LOG_CATEGORY_GRAPHICS, LOG_INFO, "Graphics device created");
    isValid = true;
}

void GraphicsDevice::CleanupD3D()
{
    if (deviceContext) deviceContext->ClearState();
}

void GraphicsDevice::Clear(const FLOAT color[4])
{
    deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
    deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Present(bool vsync)
{
    swapChain->Present(vsync, 0);
}

HRESULT GraphicsDevice::CreateCheckerboardTexture(ComPtr<ID3D11Device> g_pd3dDevice, ComPtr<ID3D11DeviceContext> g_pImmediateContext, ComPtr<ID3D11Texture2D>& g_pTexture, ComPtr<ID3D11ShaderResourceView>& g_pTextureView)
{
    UINT width = 16;
    UINT height = 16;

    // Fill the checkerboard pattern data
    std::vector<UINT> Image32;
    //ImageUtil::CreateCheckerboardPattern(width, height, Image);
    ImageUtil::CreateNoisePattern(width, height, Image32);

    // Texture description
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;// | D3D11_BIND_RENDER_TARGET;
    //desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    
    // Subresource data
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = Image32.data();
    initData.SysMemPitch = width * sizeof(UINT);

    // Create texture
    HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &initData, &g_pTexture);
    if (FAILED(hr))
        return hr;

    // Shader resource view description
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1; // Use all mipmap levels

    // Create shader resource view
    hr = g_pd3dDevice->CreateShaderResourceView(g_pTexture.Get(), &srvDesc, &g_pTextureView);

    // Generate mipmaps
    g_pImmediateContext->UpdateSubresource(g_pTexture.Get(), 0, nullptr, Image32.data(), width * sizeof(UINT), 0);
    g_pImmediateContext->GenerateMips(g_pTextureView.Get());

    // void CreateSamplerState()
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerPoint);

    g_pImmediateContext->PSSetSamplers(0, 1, g_pSamplerPoint.GetAddressOf());
    
    return hr;
}
