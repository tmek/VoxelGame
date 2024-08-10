#pragma once
#include <cassert>
#include <vector>
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include "RHI/Mesh.h"
#include "Core/Macros.h"
#include "VoxelCore/VoxelDefines.h"

using Microsoft::WRL::ComPtr;

template <typename VertexType>
class MeshBuilder {
public:
    MeshBuilder(ID3D11Device* device);

    void Clear();
    void Append(const std::vector<VertexType>& vertices, const std::vector<UINT>& indices);
    Mesh ToMesh();

    UINT GetVertexCount() const { return static_cast<UINT>(vertexList.size()); }
    UINT GetIndexCount() const { return static_cast<UINT>(indexList.size()); }
    
private:
    std::vector<VertexType> vertexList;
    std::vector<UINT> indexList;
    ComPtr<ID3D11Device> device;

    ComPtr<ID3D11Buffer> CreateVertexBuffer();
    ComPtr<ID3D11Buffer> CreateIndexBuffer(); 
};

template <typename VertexType>
MeshBuilder<VertexType>::MeshBuilder(ID3D11Device* device) : device(device) {}

template <typename VertexType>
void MeshBuilder<VertexType>::Clear() {
    vertexList.clear();
    indexList.clear();
}

template <typename VertexType>
void MeshBuilder<VertexType>::Append(const std::vector<VertexType>& vertices, const std::vector<UINT>& indices) {
    UINT indexOffset = static_cast<UINT>(vertexList.size());
    vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());
    for (UINT index : indices) {
        indexList.push_back(indexOffset + index);
    }
}

template <typename VertexType>
Mesh MeshBuilder<VertexType>::ToMesh() {

    // validate data: we should not have any indices that point to a vertex that does not exist
    for (UINT index : indexList) {
        check(index < vertexList.size());
    }
    
    Mesh mesh;// = std::make_shared<Mesh>();
    mesh.vertexBuffer = CreateVertexBuffer();
    mesh.indexBuffer = CreateIndexBuffer();
    mesh.vertexCount = static_cast<UINT>(vertexList.size());
    mesh.indexCount = static_cast<UINT>(indexList.size());
    mesh.vertexStride = sizeof(VertexType);
    return mesh;
}

template <typename VertexType>
ComPtr<ID3D11Buffer> MeshBuilder<VertexType>::CreateVertexBuffer() {
    if(vertexList.empty()) {
        return nullptr;
    }
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<UINT>(vertexList.size() * sizeof(VertexType));
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertexList.data();

    ComPtr<ID3D11Buffer> buffer;
    HRESULT HR = device->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf());
    assert(SUCCEEDED(HR));
    
    return buffer;
}

template <typename VertexType>
ComPtr<ID3D11Buffer> MeshBuilder<VertexType>::CreateIndexBuffer() {
    if(vertexList.empty()) {
        return nullptr;
    }
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<UINT>(indexList.size() * sizeof(UINT));
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indexList.data();

    ComPtr<ID3D11Buffer> buffer;
    HRESULT HR = device->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf());
    assert(SUCCEEDED(HR));
    return buffer;
}
