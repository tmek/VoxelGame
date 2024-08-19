#include <d3d11.h>
#include <dxgi.h>
#include <iostream>
#include <vector>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

void PrintD3D11DeviceInfo(ID3D11Device* device, ID3D11DeviceContext* context, UINT creationFlags)
{
    if (!device || !context)
    {
        std::cerr << "Invalid device or context!" << std::endl;
        return;
    }

    // Adapter description
    ComPtr<IDXGIDevice> dxgiDevice;
    if (SUCCEEDED(device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf()))))
    {
        ComPtr<IDXGIAdapter> adapter;
        if (SUCCEEDED(dxgiDevice->GetAdapter(adapter.GetAddressOf())))
        {
            DXGI_ADAPTER_DESC adapterDesc;
            adapter->GetDesc(&adapterDesc);
            
            std::wcout << L"Adapter Description: " << adapterDesc.Description << std::endl;
            std::wcout << L"VRAM: " << adapterDesc.DedicatedVideoMemory / (1024 * 1024) << L" MB" << std::endl;
            // print shared system memory, etc.
            std::wcout << L"Shared GPU Memory: " << adapterDesc.SharedSystemMemory / (1024 * 1024) << L" MB" <<
                std::endl;
        }
    }

    // Feature level
    D3D_FEATURE_LEVEL featureLevel = device->GetFeatureLevel();
    std::cout << "Feature Level: " << std::hex << featureLevel << std::endl;

    // Device creation flags
    std::cout << "Device Creation Flags: " << std::hex << creationFlags << std::endl;

    // Shader model
    std::cout << "Highest Shader Model Supported: ";
    switch (featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_1: std::cout << "5.1";
        break;
    case D3D_FEATURE_LEVEL_11_0: std::cout << "5.0";
        break;
    case D3D_FEATURE_LEVEL_10_1: std::cout << "4.1";
        break;
    case D3D_FEATURE_LEVEL_10_0: std::cout << "4.0";
        break;
    case D3D_FEATURE_LEVEL_9_3: std::cout << "3.0";
        break;
    default: std::cout << "Unknown";
        break;
    }
    std::cout << std::endl;

    // Multisample quality levels
    UINT numQualityLevels = 0;
    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
    std::cout << "Multisample Quality Levels for 4x MSAA: " << numQualityLevels << std::endl;

    // Context type
    D3D11_DEVICE_CONTEXT_TYPE contextType = context->GetType();
    std::cout << "Context Type: ";
    switch (contextType)
    {
    case D3D11_DEVICE_CONTEXT_IMMEDIATE: std::cout << "Immediate";
        break;
    case D3D11_DEVICE_CONTEXT_DEFERRED: std::cout << "Deferred";
        break;
    default: std::cout << "Unknown";
        break;
    }
    std::cout << std::endl;

    // Current Render Targets
    ID3D11RenderTargetView* rtvs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
    context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, rtvs, nullptr);
    std::cout << "Bound Render Targets: " << std::endl;
    for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
    {
        if (rtvs[i])
        {
            std::cout << "  Render Target " << i << ": Bound" << std::endl;
            rtvs[i]->Release();
        }
        else
        {
            // output a line indicating that the render target is not bound
            std::cout << "  Render Target " << i << ": Not Bound" << std::endl;
        }
        
    }

    // Bound Shaders (simplified example, checking only vertex and pixel shaders)
    ID3D11VertexShader* vs = nullptr;
    ID3D11PixelShader* ps = nullptr;
    context->VSGetShader(&vs, nullptr, nullptr);
    context->PSGetShader(&ps, nullptr, nullptr);

    std::cout << "Bound Vertex Shader: " << (vs ? "Yes" : "No") << std::endl;
    std::cout << "Bound Pixel Shader: " << (ps ? "Yes" : "No") << std::endl;

    if (vs) vs->Release();
    if (ps) ps->Release();
}


void PrintDirectXVersion(D3D_FEATURE_LEVEL featureLevel) {
    std::cout << "Direct3D Version: ";
    switch (featureLevel) {
    case D3D_FEATURE_LEVEL_12_1:
        std::cout << "Direct3D 12.1" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_12_0:
        std::cout << "Direct3D 12.0" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_11_1:
        std::cout << "Direct3D 11.1" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_11_0:
        std::cout << "Direct3D 11.0" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_10_1:
        std::cout << "Direct3D 10.1" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_10_0:
        std::cout << "Direct3D 10.0" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_9_3:
        std::cout << "Direct3D 9.3" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_9_2:
        std::cout << "Direct3D 9.2" << std::endl;
        break;
    case D3D_FEATURE_LEVEL_9_1:
        std::cout << "Direct3D 9.1" << std::endl;
        break;
    default:
        std::cout << "Unknown or unsupported Direct3D version" << std::endl;
        break;
    }
}


void QueryAdaptersAndOutputs() {
    ComPtr<IDXGIFactory> factory;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf())))) {
        std::cerr << "Failed to create DXGIFactory!" << std::endl;
        return;
    }

    ComPtr<IDXGIAdapter> adapter;
    UINT adapterIndex = 0;

    while (factory->EnumAdapters(adapterIndex, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC adapterDesc;
        adapter->GetDesc(&adapterDesc);
        std::wcout << L"Adapter " << adapterIndex << L": " << adapterDesc.Description << std::endl;
        std::wcout << L"  VRAM: " << adapterDesc.DedicatedVideoMemory / (1024 * 1024) << L" MB" << std::endl;
        std::wcout << L"  Vendor ID: " << std::hex << adapterDesc.VendorId << std::endl;
        
        // Reset to decimal for following outputs
        std::wcout << std::dec;

        // Enumerate outputs (monitors)
        ComPtr<IDXGIOutput> output;
        UINT outputIndex = 0;
        while (adapter->EnumOutputs(outputIndex, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
            DXGI_OUTPUT_DESC outputDesc;
            output->GetDesc(&outputDesc);
            std::wcout << L"  Output " << outputIndex << L": " << outputDesc.DeviceName << std::endl;

            // Query display modes
            UINT numModes = 0;
            output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr);
            std::vector<DXGI_MODE_DESC> displayModes(numModes);
            output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, displayModes.data());

            std::wcout << L"  Supported display modes:" << std::endl;
            for (const DXGI_MODE_DESC& mode : displayModes) {
                std::wcout << L"    " << mode.Width << L"x" << mode.Height << L"@" << mode.RefreshRate.Numerator / mode.RefreshRate.Denominator << L"Hz" << " - " << mode.Format << " - " << mode.ScanlineOrdering << " - " << mode.Scaling <<  std::endl;
            }
            outputIndex++;
        }

        adapterIndex++;
    }
}
