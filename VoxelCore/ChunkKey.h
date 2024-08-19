// Copyright

#pragma once

#include <cstdint>
#include <algorithm>
#include  "VoxelDefines.h"

// Coordinates of a chunk within the world
struct VOXELCORE_API ChunkKey
{
    int32_t X;
    int32_t Z;

    inline ChunkKey() : X(0), Z(0) {}
    inline ChunkKey(int32_t XIn, int32_t ZIn) : X(XIn), Z(ZIn) {}

    inline bool operator==(const ChunkKey& Other) const { return X == Other.X && Z == Other.Z; }
    inline bool operator!=(const ChunkKey& Other) const { return !(*this == Other); }
};


