#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "CoreTypes.h"
#include "PipelineState.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;


class CORE_API GraphicsDevice
{
public:

    GraphicsDevice(void* WindowHandle);

    ~GraphicsDevice();

    void Clear(const FLOAT Color[4]);

    void Present(bool EnableVSync = false) const;

    ID3D11SamplerState* CreateBaseSamplerState();

    void CreateSimpleNoiseTextureAndShaderResourceView();

    bool IsValid() const { return isValid; }

    // get device and device context
    ID3D11Device* GetDevice() { return D3D11Device_.Get(); }

    ID3D11DeviceContext* GetDeviceContext() { return GraphicsPipeline_.Get(); }

    void UpdateVertexShaderConstantBuffer(const XMMATRIX& WorldViewProjection, float TintR, float TintG, float TintB);

    void SetOpaquePassState() {};

    void SetTransparencyPassState() {};

    void SetPreDepthPrePassStates() {};

private:

    void DefineBasePassPSO();

    void InitD3D11();

    void GetWindowClientArea();

    void CreateDeviceAndContext();

    void CreateSwapChain();

    void CleanupD3D();

    void CheckHR(HRESULT HR);

private:

    // device / swapchain params
    HWND WindowHandle_;
    int WindowClientWidth_ = 800; // fallback values
    int WindowClientHeight_ = 600;
    UINT SampleCount_ = 4; // 4x MSAA
    UINT MSAAQuality_ = 0;

#pragma warning(push)
#pragma warning(disable:4251)  // hide Warning C4251 (needs dll-interface) until we have a solution

    // Device, Context, SwapChain
    ComPtr<ID3D11Device> D3D11Device_;
    ComPtr<ID3D11DeviceContext> GraphicsPipeline_;
    ComPtr<IDXGISwapChain> DxgiSwapChain_;

    // Textures
    ComPtr<ID3D11Texture2D> NoiseTexture_;

#pragma warning(pop)

    std::unique_ptr<PipelineStateManager> PipelineStateManager_;
    PipelineState InitialPipelineState_;
    PipelineState DepthPrePassPso_;
    PipelineState BasePassPso_;
    PipelineState MainTransparencyPassPso_;

    bool isValid;
};
