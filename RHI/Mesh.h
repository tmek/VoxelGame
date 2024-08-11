#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct SubMesh
{
    UINT indexCount = 0;
    UINT startIndexLocation = 0;
    int baseVertexLocation = 0;
};

// Represents a 3D mesh with vertex and index buffers.
// It is not dependent on any specific vertex format or struct.
struct Mesh
{
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;
    UINT vertexCount = 0;
    UINT indexCount = 0;
    UINT vertexStride = 0;

    std::vector<SubMesh> SubMeshes;

    // text tag for debugging
    std::shared_ptr<std::string> DebugTag;

    DirectX::XMFLOAT3 DebugColor = {1.0f, 1.0f, 1.0f};

    Mesh()
    {
        // generate random debug color
        DebugColor.x = (rand() % 255) / 255.0f;
        DebugColor.y = (rand() % 255) / 255.0f;
        DebugColor.z = (rand() % 255) / 255.0f;
    }

    UINT GetByteWidth() const { return vertexCount * vertexStride; }
    bool IsValid() const { return vertexBuffer.Get() != nullptr && indexBuffer.Get() != nullptr; }

    void BindToDeviceContext(ID3D11DeviceContext* deviceContext) const;
    void Draw(ID3D11DeviceContext* deviceContext) const;
    void DrawSubMesh(ID3D11DeviceContext* deviceContext, UINT subMeshIndex) const;

    // ComPtr will automatically release the resources

    // todo: make class immutable with readonly properties.
    // todo: something like: mesh.indexFormat = DXGI_FORMAT_R32_UINT;
    // todo: something like: mesh.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    // todo: something like: mesh.boundingBox = BoundingBox::CreateFromPoints(vertexList.size(), vertexList.data(), sizeof(VertexType));
    // todo: something like: mesh.boundingSphere = BoundingSphere::CreateFromPoints(vertexList.size(), vertexList.data(), sizeof(VertexType));
    // todo: something like: mesh.material = material;
};
