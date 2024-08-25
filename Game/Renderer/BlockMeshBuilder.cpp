#include "BlockMeshBuilder.h"

BlockMeshBuilder::BlockMeshBuilder(TMeshBuilder<VoxelGameMeshVertex>& meshBuilder)
    : meshBuilder(meshBuilder), Origin(0.0f, 0.0f, 0.0f), mins(0.0f, 0.0f, 0.0f), maxs(1.0f, 1.0f, 1.0f),
      color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

void BlockMeshBuilder::SetMeshRelativeBlockOrigin(float x, float y, float z)
{
    Origin = XMFLOAT3(x, y, z);
}

void BlockMeshBuilder::SetBounds(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
    mins = XMFLOAT3(minX, minY, minZ);
    maxs = XMFLOAT3(maxX, maxY, maxZ);
}

void BlockMeshBuilder::SetColor(float r, float g, float b, float a)
{
    color = XMFLOAT4(r, g, b, a);
}

void BlockMeshBuilder::AppendPosXFace()
{
    std::vector<VoxelGameMeshVertex> vertices = {
        {{Origin.x + maxs.x, Origin.y + mins.y, Origin.z + mins.z}, color, m_RightVector, {0.0f, 0.0f}},
        {{Origin.x + maxs.x, Origin.y + mins.y, Origin.z + maxs.z}, color, m_RightVector, {1.0f, 0.0f}},
        {{Origin.x + maxs.x, Origin.y + maxs.y, Origin.z + maxs.z}, color, m_RightVector, {1.0f, 1.0f}},
        {{Origin.x + maxs.x, Origin.y + maxs.y, Origin.z + mins.z}, color, m_RightVector, {0.0f, 1.0f}},
    };
    AppendFace(vertices);
}

void BlockMeshBuilder::AppendPosYFace()
{
    std::vector<VoxelGameMeshVertex> vertices = {
        {{Origin.x + mins.x, Origin.y + maxs.y, Origin.z + mins.z}, color, m_UpVector, {0.0f, 0.0f}},
        {{Origin.x + maxs.x, Origin.y + maxs.y, Origin.z + mins.z}, color, m_UpVector, {1.0f, 0.0f}},
        {{Origin.x + maxs.x, Origin.y + maxs.y, Origin.z + maxs.z}, color, m_UpVector, {1.0f, 1.0f}},
        {{Origin.x + mins.x, Origin.y + maxs.y, Origin.z + maxs.z}, color, m_UpVector, {0.0f, 1.0f}}
    };
    AppendFace(vertices);
}

void BlockMeshBuilder::AppendPosZFace()
{
    std::vector<VoxelGameMeshVertex> vertices = {
        {{Origin.x + mins.x, Origin.y + maxs.y, Origin.z + maxs.z}, color, m_ForwardVector, {0.0f, 1.0f}},
        {{Origin.x + maxs.x, Origin.y + maxs.y, Origin.z + maxs.z}, color, m_ForwardVector, {1.0f, 1.0f}},
        {{Origin.x + maxs.x, Origin.y + mins.y, Origin.z + maxs.z}, color, m_ForwardVector, {1.0f, 0.0f}},
        {{Origin.x + mins.x, Origin.y + mins.y, Origin.z + maxs.z}, color, m_ForwardVector, {0.0f, 0.0f}},
    };
    AppendFace(vertices);
}

void BlockMeshBuilder::AppendNegXFace()
{
    std::vector<VoxelGameMeshVertex> vertices = {
        {{Origin.x + mins.x, Origin.y + maxs.y, Origin.z + mins.z}, color, m_LeftVector, {0.0f, 1.0f}},
        {{Origin.x + mins.x, Origin.y + maxs.y, Origin.z + maxs.z}, color, m_LeftVector, {1.0f, 1.0f}},
        {{Origin.x + mins.x, Origin.y + mins.y, Origin.z + maxs.z}, color, m_LeftVector, {1.0f, 0.0f}},
        {{Origin.x + mins.x, Origin.y + mins.y, Origin.z + mins.z}, color, m_LeftVector, {0.0f, 0.0f}},
    };
    AppendFace(vertices);
}

void BlockMeshBuilder::AppendNegYFace()
{
    std::vector<VoxelGameMeshVertex> vertices = {
        {{Origin.x + mins.x, Origin.y + mins.y, Origin.z + maxs.z}, color, m_DownVector, {0.0f, 1.0f}},
        {{Origin.x + maxs.x, Origin.y + mins.y, Origin.z + maxs.z}, color, m_DownVector, {1.0f, 1.0f}},
        {{Origin.x + maxs.x, Origin.y + mins.y, Origin.z + mins.z}, color, m_DownVector, {1.0f, 0.0f}},
        {{Origin.x + mins.x, Origin.y + mins.y, Origin.z + mins.z}, color, m_DownVector, {0.0f, 0.0f}},
    };
    AppendFace(vertices);
}

void BlockMeshBuilder::AppendNegZFace()
{
    std::vector<VoxelGameMeshVertex> vertices = {
        {{Origin.x + mins.x, Origin.y + mins.y, Origin.z + mins.z}, color, m_BackVector, {0.0f, 0.0f}},
        {{Origin.x + maxs.x, Origin.y + mins.y, Origin.z + mins.z}, color, m_BackVector, {1.0f, 0.0f}},
        {{Origin.x + maxs.x, Origin.y + maxs.y, Origin.z + mins.z}, color, m_BackVector, {1.0f, 1.0f}},
        {{Origin.x + mins.x, Origin.y + maxs.y, Origin.z + mins.z}, color, m_BackVector, {0.0f, 1.0f}},
    };
    AppendFace(vertices);
}


void BlockMeshBuilder::AppendFace(const std::vector<VoxelGameMeshVertex>& vertices) const
{
    static const std::vector<uint32_t> faceIndices = {0, 1, 2, 2, 3, 0};
    meshBuilder.Append(vertices, faceIndices);
}
