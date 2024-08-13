#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
using namespace DirectX;


class GraphicsDevice
{
public:
    GraphicsDevice(HWND hWnd);
    ~GraphicsDevice();

    void Clear(const FLOAT color[4]);
    void Present(bool vsync = false);
    HRESULT CreateCheckerboardTexture(ComPtr<ID3D11Device> g_pd3dDevice,
                                      ComPtr<ID3D11DeviceContext> g_pImmediateContext,
                                      ComPtr<ID3D11Texture2D>& g_pTexture,
                                      ComPtr<ID3D11ShaderResourceView>& g_pTextureView);

    bool IsValid() const { return isValid; }

    // get device and device context
    ID3D11Device* GetDevice() { return device.Get(); }
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext.Get(); }

    void CreateConstantBuffer();
    //todo: don't leave constant buffer here
    void SetConstants(const XMMATRIX& viewProjMatrix, float TintR, float TintG, float TintB);
    void EnableDepthWrite();
    void DisableDepthWrite();

private:
    void InitD3D(HWND hWnd);
    void CleanupD3D();

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    ComPtr<ID3D11DepthStencilView> depthStencilView;
    ComPtr<ID3D11SamplerState> g_pSamplerLinear;

    ComPtr<ID3D11Texture2D> g_pTexture;
    ComPtr<ID3D11ShaderResourceView> g_pTextureView;

    ComPtr<ID3D11SamplerState> g_pSamplerPoint;


    D3D11_VIEWPORT viewport;
    bool isValid;

    // ----- temporary tech debt ----
    ID3D11Buffer* constantBuffer;

};