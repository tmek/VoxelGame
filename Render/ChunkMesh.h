#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Mesh.h"
#include "MeshBuilder.h"
#include "MeshVertex.h"

struct Vertex;
class Chunk;

using namespace DirectX;


struct MyConstantBuffer
{
    DirectX::XMMATRIX WorldViewProjection;
    DirectX::XMFLOAT4 TintColor;
};


class ChunkMesh {
public:
    ChunkMesh();
    ~ChunkMesh();

    void BuildMesh(Chunk& chunk, ID3D11Device* device);
    void Render(const XMMATRIX& viewProjMatrix, ID3D11DeviceContext* context);

    ID3D11Buffer* GetVertexBuffer() const { return m_mesh.vertexBuffer.Get(); }
    ID3D11Buffer* GetIndexBuffer() const { return m_mesh.indexBuffer.Get(); }
    UINT GetIndexCount() const { return m_mesh.indexCount; }


private:
    void AddBlock(int xi, int yi, int zi, int blockType, Chunk& chunk, MeshBuilder<MeshVertex>& builder);

    // std::vector<Vertex> m_vertices;
    // std::vector<UINT> m_indices;
    // ID3D11Buffer* m_vertexBuffer;
    // ID3D11Buffer* m_indexBuffer;
    // UINT m_indexCount;
    Mesh m_mesh;
    
    ID3D11Buffer* constantBuffer;
    float TintR;
    float TintG;
    float TintB;

    void CreateBuffers(ID3D11Device* device);
};
