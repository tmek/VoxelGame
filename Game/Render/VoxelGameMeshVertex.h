#pragma once
#include <DirectXMath.h>

using namespace DirectX;

// generalized vertex format for voxel game
struct VoxelGameMeshVertex {
    XMFLOAT3 Position;
    XMFLOAT4 Color;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;

    // todo: layout defined in CubeMesh.cpp
    
};

