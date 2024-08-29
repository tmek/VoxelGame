#pragma once

#include "Windows/WindowsHWrapper.h"

#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

#include "DynamicRHI.h"

#include "RHI/Mesh.h"
#include "RHI/TMeshBuilder.h"

using Microsoft::WRL::ComPtr;

class CORE_API MeshAssembler
{
public:

    MeshAssembler(ID3D11Device* device);

    template <class VertexType>
    Mesh AssembleMesh(const std::vector<TMeshBuilder<VertexType>>& builders, const char* DebugName);


private:

#pragma warning(push)
#pragma warning(disable:4251)
    ComPtr<ID3D11Device> device;


    template <typename VertexType>
    ComPtr<ID3D11Buffer> CreateVertexBuffer(const std::vector<VertexType>& vertices);

    ComPtr<ID3D11Buffer> CreateIndexBuffer(const std::vector<UINT>& indices);
#pragma warning(pop)
};


template <typename VertexType>
Mesh MeshAssembler::AssembleMesh(const std::vector<TMeshBuilder<VertexType>>& builders, const char* DebugName)
{
    Mesh mesh;
    std::vector<VertexType> combinedVertices;
    std::vector<UINT> combinedIndices;
    UINT vertexOffset = 0;
    UINT indexOffset = 0;

    for (const auto& builder : builders)
    {
        SubMesh subMesh;
        const auto& vertices = builder.GetVertices();
        const auto& indices = builder.GetIndices();

        subMesh.indexCount = static_cast<UINT>(indices.size());
        subMesh.startIndexLocation = indexOffset;
        subMesh.baseVertexLocation = vertexOffset;

        combinedVertices.insert(combinedVertices.end(), vertices.begin(), vertices.end());
        for (UINT index : indices)
        {
            combinedIndices.push_back(index + vertexOffset);
        }

        vertexOffset += static_cast<UINT>(vertices.size());
        indexOffset += static_cast<UINT>(indices.size());

        mesh.SubMeshes.push_back(subMesh);
    }

    mesh.vertexBuffer = CreateVertexBuffer(combinedVertices);
    mesh.indexBuffer = CreateIndexBuffer(combinedIndices);
    DynamicRHI::SetDebugName(mesh.vertexBuffer.Get(), "%s:VB", DebugName);
    DynamicRHI::SetDebugName(mesh.indexBuffer.Get(), "%s:IB", DebugName);

    mesh.vertexCount = vertexOffset;
    mesh.indexCount = indexOffset;
    mesh.vertexStride = sizeof(VertexType);

    return mesh;
}

template <typename VertexType>
ComPtr<ID3D11Buffer> MeshAssembler::CreateVertexBuffer(const std::vector<VertexType>& vertices)
{
    if (vertices.empty())
    {
        return nullptr;
    }

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(VertexType));
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    ComPtr<ID3D11Buffer> buffer;
    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf());
    assert(SUCCEEDED(hr));

    return buffer;
}
