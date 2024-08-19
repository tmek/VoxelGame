// Copyright

#pragma once

#include "ChunkKey.h"


/** Compares two chunk keys for equality. */
constexpr inline bool operator==(const ChunkKey& LHS, const ChunkKey& RHS) noexcept
{
    return LHS.X == RHS.X && LHS.Z == RHS.Z;
}

/** Compares two chunk keys for inequality. */
constexpr inline bool operator!=(const ChunkKey& LHS, const ChunkKey& RHS) noexcept 
{
    return !(LHS == RHS);
}
