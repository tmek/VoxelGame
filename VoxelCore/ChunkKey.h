#pragma once
#include <cstdint>

// Coordinates of a chunk within the world
struct ChunkKey
{
    int32_t X;
    int32_t Z;

    // Constructors
    ChunkKey() : X(0), Z(0) {}
    ChunkKey(int32_t x, int32_t z) : X(x), Z(z) {}

    // Equality Operators
    bool operator==(const ChunkKey& other) const
    {
        return X == other.X && Z == other.Z;
    }
    bool operator!=(const ChunkKey& chunk_key) const
    {
        return !(*this == chunk_key);
    }
};

struct ChunkKeyHash
{
    size_t operator()(const ChunkKey& key) const
    {
        // Combine the two 32-bit integers into a single 64-bit integer.
        // This is a perfect hash function for 64-bit systems.
        size_t hash = key.X;
        hash = hash << 32;
        hash |= key.Z;
        return hash;
    }
};


// inline void GetChunkDebugColor(ChunkKey chunk_key, float& r, float& g, float& b)
// {
//     // using chunk key x,y as a hash, generate a psuedo random color for debugging
//     // this is not a good hash function, but it's good enough for debugging
//     // todo: replace with a better hash function for color generation (one from ue?)
//     int32_t hash = chunk_key.X * 49157 + chunk_key.Z * 98317;
//     r = (hash & 0xFF) / 255.0f;
//     g = ((hash >> 8) & 0xFF) / 255.0f;
//     b = ((hash >> 16) & 0xFF) / 255.0f;
// }

//
// // Hash function for ChunkKey
// #if defined(_WIN64) // Hash for 64-bit systems (perfect hash)
//     
//
// #else // Hash for 32-bit systems (simple hash)
//     struct ChunkKeyHash
//     {
//         std::size_t operator()(const ChunkKey& key) const
//         {
//             const std::size_t hx = std::hash<int32_t>()(key.X);
//             const std::size_t hz = std::hash<int32_t>()(key.X);
//             return hx ^ (hz << 1); // Combine the hashes with a bit shift to reduce collisions
//         }
//     };
// #endif
