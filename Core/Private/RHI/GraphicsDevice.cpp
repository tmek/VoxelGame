// copyright

#include "RHI/GraphicsDevice.h"

#include <vector>

#include <wrl/client.h>

#include "CoreMinimal.h"

#include "RHI/ImageUtil.h"

using Microsoft::WRL::ComPtr;


struct MyConstantBuffer // todo: this shouldn't be defined here.
{
    DirectX::XMMATRIX WorldViewProjection;
    DirectX::XMFLOAT4 TintColor;
};

GraphicsDevice::GraphicsDevice(void* WindowHandle)
{
    WindowHandle_ = static_cast<HWND>(WindowHandle);
    InitD3D11();
}

GraphicsDevice::~GraphicsDevice()
{
    CleanupD3D();
}

void GraphicsDevice::UpdateVertexShaderConstantBuffer(const XMMATRIX& WorldViewProjection, float TintR, float TintG, float TintB)
{
    // update constant buffer
    MyConstantBuffer VertexShaderConstants;

    VertexShaderConstants.WorldViewProjection = WorldViewProjection;

    XMFLOAT4 TintColor =  {TintR, TintG, TintB, 1.0f};
    XMFLOAT4 BlackColor = {0.0f, 0.0f, 0.0f, 1.0f};
    VertexShaderConstants.TintColor = (GEnableDebugTint) ? TintColor : BlackColor ;  

    // using map/unmap with write_discard to update constant buffers (should be faster than updatesubresource.
    D3D11_MAPPED_SUBRESOURCE MappedConstantBuffer;

    ID3D11Resource* Buffer = BasePassPso_.VertexConstantBuffers[0].Get();
    HRESULT hr = GraphicsPipeline_->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedConstantBuffer);
    if (SUCCEEDED(hr))
    {
        // Copy the new data into the constant buffer
        memcpy(MappedConstantBuffer.pData, &VertexShaderConstants, sizeof(VertexShaderConstants));

        // Unmap the buffer to indicate that the data is ready to be used
        GraphicsPipeline_->Unmap(Buffer, 0);
    }
}

void GraphicsDevice::CheckHR(HRESULT HR)
{
    if (FAILED(HR))
    {
        TE_LOG(LogGraphics, Error, TEXT("Graphics Device Error. HRESULT=%x"), HR);
    }
}


void GraphicsDevice::GetWindowClientArea()
{
    RECT clientRect;
    if (GetClientRect(WindowHandle_, &clientRect))
    {
        WindowClientWidth_ = clientRect.right - clientRect.left;
        WindowClientHeight_ = clientRect.bottom - clientRect.top;
    }
}

void GraphicsDevice::CreateDeviceAndContext()
{
    HRESULT HR = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        D3D11Device_.GetAddressOf(),
        nullptr,
        GraphicsPipeline_.GetAddressOf()
    );
    CheckHR(HR);
}

void GraphicsDevice::CreateSwapChain()
{
    // Get the IDXGIFactory
    IDXGIDevice* DXGIDevice = nullptr;
    CheckHR(D3D11Device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DXGIDevice)));

    IDXGIAdapter* DXGIAdapter = nullptr;
    CheckHR(DXGIDevice->GetAdapter(&DXGIAdapter));

    IDXGIFactory* DXGIFactory = nullptr;
    CheckHR(DXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&DXGIFactory)));

    // Describe Swap Chain
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = WindowClientWidth_;
    sd.BufferDesc.Height = WindowClientHeight_;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = WindowHandle_;
    sd.Windowed = true;

    // Check MSAA Support

    D3D11Device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, SampleCount_, &MSAAQuality_);
    if (MSAAQuality_ > 0)
    {
        sd.SampleDesc.Count = SampleCount_;
        sd.SampleDesc.Quality = MSAAQuality_ - 1; // Use the highest quality level
    }
    else
    {
        sd.SampleDesc.Count = 1; // No MSAA support, fallback
        sd.SampleDesc.Quality = 0;
    }

    CheckHR(DXGIFactory->CreateSwapChain(DXGIDevice, &sd, &DxgiSwapChain_));

    // Cleanup
    DXGIDevice->Release();
    DXGIAdapter->Release();
    DXGIFactory->Release();
}





void GraphicsDevice::InitD3D11()
{
    isValid = false;

    GetWindowClientArea();

    CreateDeviceAndContext();
    CreateSwapChain();

    PipelineStateManager_ = std::make_unique<PipelineStateManager>(GraphicsPipeline_);
    PipelineStateManager_->Initialize();
    InitialPipelineState_ = PipelineStateManager_->GetCurrentState();
    BasePassPso_ = InitialPipelineState_;

    DefineBasePassPSO();
    
    CreateSimpleNoiseTextureAndShaderResourceView();
    
    // apply base state
    PipelineStateManager_->ApplyPipelineState(BasePassPso_);

    // Success
    TE_LOG(LogGraphics, Log, TEXT("Graphics device created"));
    isValid = true;
}


void GraphicsDevice::CleanupD3D()
{
    if (GraphicsPipeline_) GraphicsPipeline_->ClearState();
}

void GraphicsDevice::Clear(const FLOAT color[4])
{
    // TODO: we shouldn't be referencing base pass PSO, but rather a "current" pass PSO
    GraphicsPipeline_->ClearRenderTargetView(BasePassPso_.RenderTargetViews[0].Get(), color);
    GraphicsPipeline_->ClearDepthStencilView(BasePassPso_.DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Present(bool EnableVSync) const
{
    uint32 SyncInterval = EnableVSync ? 1 : 0;

    DxgiSwapChain_->Present(SyncInterval, 0);
}




void GraphicsDevice::DefineBasePassPSO()
{
    // render target from swap chain back buffer
    ComPtr<ID3D11Texture2D> SwapChainBackBuffer;
    CheckHR(DxgiSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), &SwapChainBackBuffer));
    CheckHR(D3D11Device_->CreateRenderTargetView(SwapChainBackBuffer.Get(), nullptr, BasePassPso_.RenderTargetViews[0].GetAddressOf()));


    // depth/stencil texture and DSV
    D3D11_TEXTURE2D_DESC DepthStencilTextureDesc = {};
    DepthStencilTextureDesc.Width = WindowClientWidth_; // Set the width of the texture
    DepthStencilTextureDesc.Height = WindowClientHeight_; // Set the height of the texture
    DepthStencilTextureDesc.MipLevels = 1; // We don't need mipmaps for the depth buffer
    DepthStencilTextureDesc.ArraySize = 1; // We need only one texture
    DepthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Use a 24-bit depth buffer and 8-bit stencil buffer
    DepthStencilTextureDesc.SampleDesc.Count = SampleCount_;
    DepthStencilTextureDesc.SampleDesc.Quality = MSAAQuality_ - 1;
    DepthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
    DepthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // Bind as a depth stencil texture
    DepthStencilTextureDesc.CPUAccessFlags = 0; // CPU does not need access
    DepthStencilTextureDesc.MiscFlags = 0; // No additional flags
    ComPtr<ID3D11Texture2D> DepthStencilTexture;
    CheckHR(D3D11Device_->CreateTexture2D(&DepthStencilTextureDesc, nullptr, DepthStencilTexture.GetAddressOf()));
    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DepthStencilTextureDesc.Format; // Same format as the depth stencil texture
    DSVDesc.ViewDimension = (SampleCount_ > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0; // Mipmap slice index
    CheckHR(D3D11Device_->CreateDepthStencilView(DepthStencilTexture.Get(), &DSVDesc, BasePassPso_.DepthStencilView.GetAddressOf()));

    // constant buffer
    D3D11_BUFFER_DESC BufferDesc = {};
    BufferDesc.Usage = D3D11_USAGE_DYNAMIC; // usage dynamic required for map/unmap
    BufferDesc.ByteWidth = sizeof(MyConstantBuffer);
    BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    CheckHR(D3D11Device_->CreateBuffer(&BufferDesc, nullptr, BasePassPso_.VertexConstantBuffers[0].GetAddressOf()));
    

    // viewport
    D3D11_VIEWPORT Viewport;
    Viewport.Width = static_cast<FLOAT>(WindowClientWidth_);
    Viewport.Height = static_cast<FLOAT>(WindowClientHeight_);
    Viewport.TopLeftX = 0;
    Viewport.TopLeftY = 0;
    Viewport.MinDepth = 0.0f;
    Viewport.MaxDepth = 1.0f;
    BasePassPso_.Viewports.push_back(Viewport); // todo: should probably use a map or c array.


    // rasterizer state
    D3D11_RASTERIZER_DESC RasterizerDesc = {};
    RasterizerDesc.FillMode = D3D11_FILL_SOLID;
    RasterizerDesc.CullMode = D3D11_CULL_BACK;
    RasterizerDesc.FrontCounterClockwise = TRUE; // Use counter-clockwise vertices as the front face
    RasterizerDesc.DepthClipEnable = TRUE;
    CheckHR(D3D11Device_->CreateRasterizerState(&RasterizerDesc, BasePassPso_.RasterizerState.GetAddressOf()));

    
    // create blend state
    D3D11_BLEND_DESC BlendStateDesc = {};
    // BlendStateDesc.IndependentBlendEnable = 1;
    // BlendStateDesc.AlphaToCoverageEnable = 1;
    BlendStateDesc.RenderTarget[0].BlendEnable = 1;
    BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    CheckHR(D3D11Device_->CreateBlendState(&BlendStateDesc, BasePassPso_.BlendState.GetAddressOf()));

    // normal read/write depth/stencil state
    D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc = {};
    DepthStencilStateDesc.DepthEnable = TRUE; // Enable depth testing
    DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // Allow writing to the depth buffer
    DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS; // Use standard depth test (less is closer)
    DepthStencilStateDesc.StencilEnable = FALSE; // Disable stencil testing for now
    CheckHR(D3D11Device_->CreateDepthStencilState(&DepthStencilStateDesc, BasePassPso_.DepthStencilState.GetAddressOf()));

    // creates 'Minecraft' style pixelated texture sampler
    D3D11_SAMPLER_DESC BlockTextureSamplerState = {};
    BlockTextureSamplerState.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
    BlockTextureSamplerState.Filter = D3D11_FILTER_ANISOTROPIC;
    BlockTextureSamplerState.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    BlockTextureSamplerState.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    BlockTextureSamplerState.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    BlockTextureSamplerState.MipLODBias = 0.0f; // No additional LOD bias
    BlockTextureSamplerState.MaxAnisotropy = 8; // Max anisotropy level for best quality
    BlockTextureSamplerState.ComparisonFunc = D3D11_COMPARISON_NEVER;
    BlockTextureSamplerState.MinLOD = 0; // Start at the base mip level
    BlockTextureSamplerState.MaxLOD = D3D11_FLOAT32_MAX; // Use all mip levels
    CheckHR(D3D11Device_->CreateSamplerState(&BlockTextureSamplerState, BasePassPso_.PixelSamplerStates[0].GetAddressOf()));
}

void GraphicsDevice::CreateSimpleNoiseTextureAndShaderResourceView()
{
    UINT ImageWidth = 16;
    UINT ImageHeight = 16;

    // Create an RGBA image and fill it with noise.
    std::vector<UINT> RGBAImage;
    ImageUtil::FillImageWithNoise(ImageWidth, ImageHeight, RGBAImage);

    // Texture description
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = ImageWidth;
    desc.Height = ImageHeight;
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    // Create texture with no data, we'll update resource and generate mips later.
    CheckHR(D3D11Device_->CreateTexture2D(&desc, nullptr, NoiseTexture_.GetAddressOf()));

    // Update texture data
    GraphicsPipeline_->UpdateSubresource(NoiseTexture_.Get(), 0, nullptr, RGBAImage.data(), ImageWidth * sizeof(UINT), 0);

    // Shader resource view description
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1; // Use all mipmap levels
    CheckHR(D3D11Device_->CreateShaderResourceView(NoiseTexture_.Get(), &srvDesc, BasePassPso_.PixelShaderResourceViews[0].GetAddressOf()));

    // Generate mips
    GraphicsPipeline_->GenerateMips(BasePassPso_.PixelShaderResourceViews[0].Get());
}
