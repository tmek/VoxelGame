#include "CubeMesh.h"
#include <d3dcompiler.h>
#include <iostream>
#include <DirectXColors.h>
#include "RHI/GraphicsDevice.h"

#include "Input/InputManager.h"

#include "Renderer/VoxelGameMeshVertex.h"

#define MAX_OF_TWO_NUMBERS(a, b) ((a) > (b) ? (a) : (b))

CubeMesh::CubeMesh(ID3D11Device* device, ID3D11DeviceContext* context)
    : device(device), context(context)
{
    isValid = false;
    InitResources();
}

CubeMesh::~CubeMesh()
{
}

using namespace DirectX;

float CalculateLightIntensity(XMFLOAT3 normal, XMFLOAT3 lightDir)
{
    XMVECTOR normalVec = XMLoadFloat3(&normal);
    XMVECTOR lightDirVec = XMLoadFloat3(&lightDir);


    float dotProduct = XMVectorGetX(XMVector3Dot(normalVec, lightDirVec));
    return MAX_OF_TWO_NUMBERS(0.0f, dotProduct); // Clamp to [0, 1]
}

XMFLOAT4 GetShadedColor(float intensity)
{
    float shade = intensity * 255.0f; // Convert to [0, 255] range
    return XMFLOAT4(shade / 255.0f, shade / 255.0f, shade / 255.0f, 1.0f); // Grayscale color with alpha = 1.0f
}

void CubeMesh::InitResources()
{
    // setup some simulated lighting
    XMFLOAT3 lightDirection = XMFLOAT3(-.5f, 1.0f, 0.2f);
    // normalize the light direction
    XMVECTOR LightDir = XMVector3Normalize(XMLoadFloat3(&lightDirection));
    XMStoreFloat3(&lightDirection, LightDir);

    XMFLOAT3 frontNormal = XMFLOAT3(0.0f, 0.0f, 1.0f);
    XMFLOAT3 backNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    XMFLOAT3 leftNormal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
    XMFLOAT3 rightNormal = XMFLOAT3(1.0f, 0.0f, 0.0f);
    XMFLOAT3 topNormal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    XMFLOAT3 bottomNormal = XMFLOAT3(0.0f, -1.0f, 0.0f);

    XMFLOAT4 frontColor = GetShadedColor(CalculateLightIntensity(frontNormal, lightDirection));
    XMFLOAT4 backColor = GetShadedColor(CalculateLightIntensity(backNormal, lightDirection));
    XMFLOAT4 leftColor = GetShadedColor(CalculateLightIntensity(leftNormal, lightDirection));
    XMFLOAT4 rightColor = GetShadedColor(CalculateLightIntensity(rightNormal, lightDirection));
    XMFLOAT4 topColor = GetShadedColor(CalculateLightIntensity(topNormal, lightDirection));
    XMFLOAT4 bottomColor = GetShadedColor(CalculateLightIntensity(bottomNormal, lightDirection));

    // Define vertices of a triangle
    VoxelGameMeshVertex vertices[] =
    {
        // Front face
        {DirectX::XMFLOAT3(0, 0, 0), frontColor},
        {DirectX::XMFLOAT3(0, 1, 0), frontColor},
        {DirectX::XMFLOAT3(1, 1, 0), frontColor},
        {DirectX::XMFLOAT3(0, 0, 0), frontColor},
        {DirectX::XMFLOAT3(1, 1, 0), frontColor},
        {DirectX::XMFLOAT3(1, 0, 0), frontColor},

        // Back face
        {DirectX::XMFLOAT3(0, 0, 1), backColor},
        {DirectX::XMFLOAT3(1, 0, 1), backColor},
        {DirectX::XMFLOAT3(1, 1, 1), backColor},
        {DirectX::XMFLOAT3(0, 0, 1), backColor},
        {DirectX::XMFLOAT3(1, 1, 1), backColor},
        {DirectX::XMFLOAT3(0, 1, 1), backColor},

        // Left face

        {DirectX::XMFLOAT3(0, 1, 1), leftColor},
        {DirectX::XMFLOAT3(0, 1, 0), leftColor},
        {DirectX::XMFLOAT3(0, 0, 0), leftColor},
        {DirectX::XMFLOAT3(0, 0, 1), leftColor},
        {DirectX::XMFLOAT3(0, 1, 1), leftColor},
        {DirectX::XMFLOAT3(0, 0, 0), leftColor},

        // Right face
        {DirectX::XMFLOAT3(1, 0, 0), rightColor},
        {DirectX::XMFLOAT3(1, 1, 0), rightColor},
        {DirectX::XMFLOAT3(1, 1, 1), rightColor},
        {DirectX::XMFLOAT3(1, 0, 0), rightColor},
        {DirectX::XMFLOAT3(1, 1, 1), rightColor},
        {DirectX::XMFLOAT3(1, 0, 1), rightColor},

        // Top face
        {DirectX::XMFLOAT3(1, 1, 1), topColor},
        {DirectX::XMFLOAT3(1, 1, 0), topColor},
        {DirectX::XMFLOAT3(0, 1, 0), topColor},
        {DirectX::XMFLOAT3(0, 1, 1), topColor},
        {DirectX::XMFLOAT3(1, 1, 1), topColor},
        {DirectX::XMFLOAT3(0, 1, 0), topColor},

        // Bottom face
        {DirectX::XMFLOAT3(0, 0, 0), bottomColor},
        {DirectX::XMFLOAT3(1, 0, 0), bottomColor},
        {DirectX::XMFLOAT3(1, 0, 1), bottomColor},
        {DirectX::XMFLOAT3(0, 0, 0), bottomColor},
        {DirectX::XMFLOAT3(1, 0, 1), bottomColor},
        {DirectX::XMFLOAT3(0, 0, 1), bottomColor}
    };

    // Vertex vertices[] =
    // {
    //     {DirectX::XMFLOAT3(0, 0, 0), Red},
    //     {DirectX::XMFLOAT3(0, 1, 0), Green},
    //     {DirectX::XMFLOAT3(1, 1, 0), Blue},
    //
    //     {DirectX::XMFLOAT3(1, 1, 0), Magenta},
    //     {DirectX::XMFLOAT3(1, 0, 0), Yellow},
    //     {DirectX::XMFLOAT3(0, 0, 0), White}
    // };

    TriangleCount = sizeof(vertices) / sizeof(VoxelGameMeshVertex);

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create vertex buffer: " << std::hex << hr << std::endl;
        return;
    }

    // Load and compile shaders

    // const variables to hold shader source file path
    const TCHAR* VertexShaderPath = L"Shaders/VertexShader.hlsl";
    const TCHAR* pixelShaderPath = L"Shaders/PixelShader.hlsl";

    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

    hr = D3DCompileFromFile(pixelShaderPath, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            char* errorMessage = (char*)errorBlob->GetBufferPointer();
            std::cerr << "Pixel Shader Compile Error: " << errorMessage << std::endl;
        }
        return;
    }

    hr = D3DCompileFromFile(VertexShaderPath, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            char* errorMessage = (char*)errorBlob->GetBufferPointer();
            std::cerr << "Vertex Shader Compile Error: " << errorMessage << std::endl;
        }
        return;
    }

    hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr,
                                    &vertexShader);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create vertex shader: " << std::hex << hr << std::endl;
        return;
    }
    hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create pixel shader: " << std::hex << hr << std::endl;
        return;
    }

    // Define input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // todo: be sure to use offsetof([VertexType], [PropertyName]) instead of hardcoding
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VoxelGameMeshVertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VoxelGameMeshVertex, Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VoxelGameMeshVertex, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VoxelGameMeshVertex, TexCoord), D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    
    hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(),
                                   vsBlob->GetBufferSize(),
                                   &inputLayout);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create input layout: " << std::hex << hr << std::endl;
        return;
    }

    // Create constant buffer
    // bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    // bufferDesc.ByteWidth = sizeof(ConstantBuffer);
    // bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    // bufferDesc.CPUAccessFlags = 0;
    // hr = device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
    // if (FAILED(hr)) {
    //     std::cerr << "Failed to create constant buffer: " << std::hex << hr << std::endl;
    //     return;
    // }

    // Create constant buffer
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(MyConstantBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    hr = device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create constant buffer: " << std::hex << hr << std::endl;
        return;
    }

    isValid = true;
}

void CubeMesh::Select()
{
    // select geometry
    UINT stride = sizeof(VoxelGameMeshVertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetInputLayout(inputLayout.Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CubeMesh::SetShaders()
{
    // select shaders
    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);
}

void CubeMesh::Render(const DirectX::XMMATRIX& viewProjMatrix, DirectX::XMFLOAT4 TintColor)
{
    // update constant buffer
    MyConstantBuffer cb;
    cb.WorldViewProjection = viewProjMatrix;
    cb.TintColor = TintColor;
    context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);
    context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    // draw
    context->Draw(TriangleCount, 0);
}
