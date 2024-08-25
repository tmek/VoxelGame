/*
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <iostream>

using namespace DirectX;
using namespace Microsoft::WRL;

// Structures for vertex and constant buffer
struct Vertex {
    XMFLOAT3 position;
    XMFLOAT4 color;
};

struct ConstantBuffer {
    XMMATRIX mvpMatrix;
};

// Globals
ComPtr<ID3D12Device> device;
ComPtr<ID3D12CommandQueue> commandQueue;
ComPtr<ID3D12CommandAllocator> commandAllocator;
ComPtr<ID3D12GraphicsCommandList> commandList;
ComPtr<ID3D12PipelineState> pipelineState;
ComPtr<ID3D12RootSignature> rootSignature;
ComPtr<ID3D12Resource> vertexBuffer;
ComPtr<ID3D12Resource> indexBuffer;
ComPtr<ID3D12Resource> constantBuffer;
D3D12_VIEWPORT viewport;
D3D12_RECT scissorRect;

// Cube vertices and indices
Vertex vertices[] = {
    { { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    { { 1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    { { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    { { -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
    { { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
    { { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    { { 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } }
};

uint16_t indices[] = {
    0, 1, 2, 2, 1, 3,
    4, 5, 6, 6, 5, 7,
    0, 1, 4, 4, 1, 5,
    2, 3, 6, 6, 3, 7,
    0, 2, 4, 4, 2, 6,
    1, 3, 5, 5, 3, 7
};

void InitializeD3D12(HWND hwnd) {
    // Step 1: Create Device and Command Queue
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

    // Step 2: Create Command Allocator and Command List
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
    device->CreateGraphicsCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));

    // Step 3: Create Root Signature
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.pParameters = nullptr;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

    // Step 4: Create Pipeline State Object (PSO)
    // In actual code, shaders would be loaded and compiled here
    // For brevity, assume pVertexShader and pPixelShader are valid shader blobs

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = { pVertexShader->GetBufferPointer(), pVertexShader->GetBufferSize() };
    psoDesc.PS = { pPixelShader->GetBufferPointer(), pPixelShader->GetBufferSize() };
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.NumRenderTargets = 1;
    
    device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));

    // Step 5: Create Vertex Buffer
    const UINT vertexBufferSize = sizeof(vertices);
    device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer));

    // Copy vertex data to vertex buffer.
    void* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    vertexBuffer->Map(0, &readRange, &pVertexDataBegin);
    memcpy(pVertexDataBegin, vertices, sizeof(vertices));
    vertexBuffer->Unmap(0, nullptr);

    // Step 6: Create Index Buffer
    const UINT indexBufferSize = sizeof(indices);
    device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuffer));

    // Copy index data to index buffer.
    void* pIndexDataBegin;
    indexBuffer->Map(0, &readRange, &pIndexDataBegin);
    memcpy(pIndexDataBegin, indices, sizeof(indices));
    indexBuffer->Unmap(0, nullptr);

    // Step 7: Set Viewport and Scissor Rect
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(800);
    viewport.Height = static_cast<float>(600);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = static_cast<LONG>(800);
    scissorRect.bottom = static_cast<LONG>(600);
}
*/