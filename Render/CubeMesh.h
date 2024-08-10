#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <DirectXMath.h>

class CubeMesh
{
public:
    CubeMesh(ID3D11Device* device, ID3D11DeviceContext* context);
    ~CubeMesh();
    bool IsValid() const { return isValid; }
    void SetShaders();
    void Select();
    void Render(const DirectX::XMMATRIX& viewProjMatrix, DirectX::XMFLOAT4 TintColor);

private:
    void InitResources();

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    bool isValid;
    unsigned int TriangleCount;

    // struct Vertex
    // {
    //     DirectX::XMFLOAT3 Pos;
    //     DirectX::XMFLOAT4 Color;
    //     DirectX::XMFLOAT3 Norm;
    //     DirectX::XMFLOAT2 UV;
    // };

    struct MyConstantBuffer
    {
        DirectX::XMMATRIX WorldViewProjection;
        DirectX::XMFLOAT4 TintColor;
    };
};
