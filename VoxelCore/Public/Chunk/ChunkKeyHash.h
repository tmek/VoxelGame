// Copyright

#pragma once

#include "ChunkKey.h"
#include <bit>


/** Computes a hash for a ChunkKey */
#if defined(_WIN64)

    /** Hash for 64-bit systems (this 64-bit hash will have no collisions) */
    struct VOXELCORE_API ChunkKeyHash final
    {
        constexpr inline size_t operator()(const ChunkKey& Key) const noexcept
        {
            // returns a 64-bit size_t with X in the upper 32 bits and Z in the lower 32 bits
            return std::bit_cast<size_t>(Key);
        }
    };
    static_assert(sizeof(ChunkKey) == sizeof(size_t), "ChunkKey must be the same size as size_t.");

#else

    /** Hash for 32-bit systems (simple hash) */
    struct ChunkKeyHash final
    {
        constexpr inline size_t operator()(const ChunkKey& key) const noexcept
        {
            // Prime number multiplication combined with bitwise operations for better distribution
            const size_t hx = static_cast<size_t>(key.X);
            const size_t hz = static_cast<size_t>(key.Z);
            return (hx * 31) + hz;
        }
    };

#endif 
