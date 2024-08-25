#pragma once
#include <vector>
#include <DirectXMath.h>

#include "VoxelGameMeshVertex.h"

#include "RHI/TMeshBuilder.h"

using namespace DirectX;
using std::vector;

// Constructs vertices and indices for the six faces of a block mesh.
// Can change offset, bounds, and color for the block.
// Caller can choose which faces to include or exclude.
class BlockMeshBuilder {
public:
    BlockMeshBuilder(TMeshBuilder<VoxelGameMeshVertex>& meshBuilder);

    void SetMeshRelativeBlockOrigin(float x, float y, float z);
    void SetBounds(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
    void SetColor(float r, float g, float b, float a);

    void AppendNegXFace();
    void AppendPosXFace();
    void AppendPosYFace();
    void AppendNegYFace();
    void AppendPosZFace();
    void AppendNegZFace();

private:
    void AppendFace(const std::vector<VoxelGameMeshVertex>& vertices) const;

    TMeshBuilder<VoxelGameMeshVertex>& meshBuilder;
    XMFLOAT3 Origin;
    XMFLOAT3 mins;
    XMFLOAT3 maxs;
    XMFLOAT4 color;
    
    XMFLOAT3 m_RightVector =  {1.0f, 0.0f, 0.0f};
    XMFLOAT3 m_LeftVector =   {-1.0f, 0.0f, 0.0f};
    XMFLOAT3 m_UpVector =     {0.0f, 1.0f, 0.0f};
    XMFLOAT3 m_DownVector =   {0.0f, -1.0f, 0.0f};
    XMFLOAT3 m_ForwardVector ={0.0f, 0.0f, 1.0f};
    XMFLOAT3 m_BackVector =   {0.0f, 0.0f, -1.0f};
};

