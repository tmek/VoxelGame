#pragma once

#include "Windows/WindowsHWrapper.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "CoreTypes.h"
#include "PipelineState.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;


class CORE_API DynamicRHI
{
public:

    static void SetDebugName(ID3D11DeviceChild* Resource, const char* Format, ...);

    void BeginPass(const wchar_t* Name);

    void EndPass();

    const PipelineState& GetCurrentPSO() const;

    DynamicRHI(void* WindowHandle);

    ~DynamicRHI();

    void Clear(const float ColorClear[4], float DepthClear = 1.0f, UINT8 StencilClear = 0) const;

    void Present(bool EnableVSync = false) const;

    void CreateSimpleNoiseTextureAndShaderResourceView();

    bool IsValid() const { return isValid; }

    ID3D11Device* GetDevice() { return GraphicsFactory_.Get(); }

    ID3D11DeviceContext* GetDeviceContext() { return GraphicsPipeline_.Get(); }

    void UpdateVertexShaderConstantBuffer(const XMMATRIX& WorldViewProjection, float TintR, float TintG, float TintB);

    void SetOpaquePassState()
    {
        PipelineStateManager_->ApplyPipelineState(OpaquePassPSO_);
    };

    void SetTransparencyPassState()
    {
        PipelineStateManager_->ApplyPipelineState(TransparencyPassPSO_);
    };

    void SetPreDepthPrePassStates()
    {
        PipelineStateManager_->ApplyPipelineState(DepthPrePassPSO_);
    };

    void ApplyPSO(PipelineState PSO)
    {
        PipelineStateManager_->ApplyPipelineState(PSO);
    }

private:

    void DefineOpaquePassPSO();

    void DefineTransparencyPassPSO();

    void DefineDepthPrePassPSO();

    void InitD3D11();

    void GetWindowClientArea();

    void CreateDeviceAndContext();

    void CreateSwapChain();

    void SetDebugName(ComPtr<ID3D11DeviceChild> resource, const char* format, ...);


    void CleanupD3D();

    void CheckHR(HRESULT HR);

private:

    // device / swapchain params
    HWND WindowHandle_;
    int WindowClientWidth_ = 800; // fallback values
    int WindowClientHeight_ = 600;
    UINT SampleCount_ = 1; // 4x MSAA
    UINT MSAAQuality_ = 0;

#pragma warning(push)
#pragma warning(disable:4251)  // hide Warning C4251 (needs dll-interface) until we have a solution

    // Device, Context, SwapChain
    ComPtr<ID3D11Device> GraphicsFactory_;
    ComPtr<ID3D11DeviceContext> GraphicsPipeline_;
    ComPtr<IDXGISwapChain> DxgiSwapChain_;

    // Textures
    ComPtr<ID3D11Texture2D> NoiseTexture_;

    std::unique_ptr<PipelineStateManager> PipelineStateManager_;
#pragma warning(pop)

    PipelineState InitialPipelineState_;
    PipelineState DepthPrePassPSO_;
    PipelineState OpaquePassPSO_;
    PipelineState TransparencyPassPSO_;

    bool isValid;
};
