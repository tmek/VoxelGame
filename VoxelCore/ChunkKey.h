#pragma once
#include <cstdint>
#include <algorithm>

#include  "VoxelDefines.h"

// Coordinates of a chunk within the world
struct VOXELCORE_API ChunkKey
{
    int32_t X;
    int32_t Z;

    ChunkKey() : X(0), Z(0) {}
    ChunkKey(int32_t x, int32_t z) : X(x), Z(z) {}

    bool operator==(const ChunkKey& other) const
    {
        return X == other.X && Z == other.Z;
    }
    bool operator!=(const ChunkKey& chunk_key) const
    {
        return !(*this == chunk_key);
    }
};


// Hash function for ChunkKey
#if defined(_WIN64) // Hash for 64-bit systems (perfect hash)
struct VOXELCORE_API ChunkKeyHash
{
    size_t operator()(const ChunkKey& key) const
    {
        // Combine the two 32-bit integers into a single 64-bit integer.
        // Should be a perfect hash function for 64-bit systems.
        // will need different hash function for 32-bit systems
        size_t hash = key.X;
        hash = hash << 32;
        hash |= key.Z;
        return hash;
    }
};
#else // Hash for 32-bit systems (simple hash)
struct ChunkKeyHash
{
    size_t operator()(const ChunkKey& key) const
    {
        const size_t hx = std::hash<int32_t>()(key.X);
        const size_t hz = std::hash<int32_t>()(key.X);
        return hx ^ (hz << 1); // Combine the hashes with a bit shift to reduce collisions
    }
};
#endif
