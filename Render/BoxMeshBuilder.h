﻿#pragma once
#include "MeshBuilder.h"
#include "VoxelGameMeshVertex.h"
#include "RHI/Mesh.h"

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

struct Vec3
{
    float x, y, z;
};

class BoxMeshBuilder
{

    
public:
    Mesh Build(ID3D11Device* device);

    void BuildFaceFromUVWAxes(const int UAxis, const int VAxis, const int WAxis,
                              const int localX, const int localY, const int localZ, MeshBuilder<VoxelGameMeshVertex>& meshBuilder, XMFLOAT4 color);
};
