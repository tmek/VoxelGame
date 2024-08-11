#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "Render/MeshBuilder.h"
#include "RHI/Mesh.h"

using Microsoft::WRL::ComPtr;

class MeshAssembler {
public:
    MeshAssembler(ID3D11Device* device);

    template <typename VertexType>
    Mesh AssembleMesh(const std::vector<MeshBuilder<VertexType>>& builders);

private:
    ComPtr<ID3D11Device> device;

    template <typename VertexType>
    ComPtr<ID3D11Buffer> CreateVertexBuffer(const std::vector<VertexType>& vertices);
    ComPtr<ID3D11Buffer> CreateIndexBuffer(const std::vector<UINT>& indices);
};


template <typename VertexType>
Mesh MeshAssembler::AssembleMesh(const std::vector<MeshBuilder<VertexType>>& builders) {
    Mesh mesh;
    std::vector<VertexType> combinedVertices;
    std::vector<UINT> combinedIndices;
    UINT vertexOffset = 0;
    UINT indexOffset = 0;

    for (const auto& builder : builders) {
        SubMesh subMesh;
        const auto& vertices = builder.GetVertices();
        const auto& indices = builder.GetIndices();

        subMesh.indexCount = static_cast<UINT>(indices.size());
        subMesh.startIndexLocation = indexOffset;
        subMesh.baseVertexLocation = vertexOffset;

        combinedVertices.insert(combinedVertices.end(), vertices.begin(), vertices.end());
        for (UINT index : indices) {
            combinedIndices.push_back(index + vertexOffset);
        }

        vertexOffset += static_cast<UINT>(vertices.size());
        indexOffset += static_cast<UINT>(indices.size());

        mesh.SubMeshes.push_back(subMesh);
    }

    mesh.vertexBuffer = CreateVertexBuffer(combinedVertices);
    mesh.indexBuffer = CreateIndexBuffer(combinedIndices);
    mesh.vertexCount = vertexOffset;
    mesh.indexCount = indexOffset;
    mesh.vertexStride = sizeof(VertexType);

    return mesh;
}

template <typename VertexType>
ComPtr<ID3D11Buffer> MeshAssembler::CreateVertexBuffer(const std::vector<VertexType>& vertices) {
    if (vertices.empty()) {
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

