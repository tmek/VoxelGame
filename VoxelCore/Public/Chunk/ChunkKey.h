// Copyright

#pragma once

#include "Voxel/VoxelTypes.h"

/** Uniquely identifies a chunk in the world. */
struct ChunkKey final
{
    int32 X;
    int32 Z;

    // Generates a 64-bit raw representation of the ChunkKey (for operator== and ChunkKeyHash)
    constexpr uint64 ToRawData() const noexcept 
    {
        return (static_cast<uint64>(X) << 32) | static_cast<uint32>(Z);
    }

    constexpr bool operator==(const ChunkKey& Other) const noexcept 
    { 
        return ToRawData() == Other.ToRawData(); 
    }
};
