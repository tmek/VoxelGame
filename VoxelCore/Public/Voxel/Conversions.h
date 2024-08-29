// Copyright

#pragma once

#include "CoreTypes.h"
#include "Chunk/ChunkConstants.h"
#include "Chunk/ChunkKey.h"
#include "VoxelDefines.h"

#undef TE_ENABLE_CONVERSION_OPTIMIZATIONS

/* Optimization Notes:

    bitwise '&' can be used like a wrap-around modulo that works as desired with negative numbers:
        a & (b-1)
        - Requires b is positive and a power of 2
        - Gives the same result as = (a % b + b) % b 

    bitwise shift >> can be used as a fast division by a power of 2 that works as desired with negative numbers:
        a >> log2(b)
        - Requires b is positive and a power of 2
        - Gives the same result as (a >= 0) ? (a / b) : ((a - (b - 1)) / b);

    bitwise shift << can be used as a fast multiplication by a power of 2: 
        a << log2(b)
        - Requires b is positive and a power of 2
        - Gives the same result as a * b 
*/


/** Convert world coordinate to chunk grid coordinate. */
inline void WorldToChunkGrid(const int32& WorldCoordinateIn, int32& ChunkCoordinateOut) noexcept
{
    // requires division that rounds towards negative infinity
#ifdef TE_ENABLE_CONVERSION_OPTIMIZATIONS
    ChunkCoordinateOut = WorldCoordinateIn >> ChunkWidthLog2;
#else
    ChunkCoordinateOut = (WorldCoordinateIn >= 0)
                             ? (WorldCoordinateIn / ChunkWidth)
                             : ((WorldCoordinateIn - (ChunkWidth - 1)) / ChunkWidth);
#endif
}


/** Convert world position to chunk key. */
inline void WorldToChunkKey(const BlockCoordinate& WorldPositionIn, ChunkKey& ChunkKeyOut) noexcept
{
    CheckWorldPosition(WorldPositionIn);

    WorldToChunkGrid(WorldPositionIn.X, ChunkKeyOut.X);
    WorldToChunkGrid(WorldPositionIn.Z, ChunkKeyOut.Z);

    CheckChunkKey(ChunkKeyOut);
}

/** Convert chunk key to chunk origin in world space. */
inline void ChunkKeyToWorld(const ChunkKey& ChunkKeyIn, BlockCoordinate& ChunkWorldPositionOut) noexcept
{
    CheckChunkKey(ChunkKeyIn);

#ifdef TE_ENABLE_CONVERSION_OPTIMIZATIONS
    ChunkWorldPositionOut.X = ChunkKeyIn.X << ChunkWidthLog2;
    ChunkWorldPositionOut.Y = 0;
    ChunkWorldPositionOut.Z = ChunkKeyIn.Z << ChunkDepthLog2;
#else
    ChunkWorldPositionOut.X = ChunkKeyIn.X * ChunkWidth;
    ChunkWorldPositionOut.Y = 0;
    ChunkWorldPositionOut.Z = ChunkKeyIn.Z * ChunkDepth;
#endif

    CheckWorldPosition(ChunkWorldPositionOut);
}


/** Convert world coordinate to local coordinate within a chunk. */
inline void WorldToLocal(const int32& WorldCoordinateIn, int32& LocalCoordinateOut) noexcept
{
#ifdef TE_ENABLE_CONVERSION_OPTIMIZATIONS
    LocalCoordinateOut = WorldCoordinateIn & MaxBlockX;
#else
    LocalCoordinateOut = (WorldCoordinateIn % ChunkWidth + ChunkWidth) % ChunkWidth;
#endif
}


/** Convert world coordinates to block local coordinates within a chunk. */
inline void WorldToLocal(const BlockCoordinate& WorldPositionIn, BlockCoordinate& BlockOffsetOut) noexcept
{
    CheckWorldPosition(WorldPositionIn);

    WorldToLocal(WorldPositionIn.X, BlockOffsetOut.X);
    BlockOffsetOut.Y = WorldPositionIn.Y;
    WorldToLocal(WorldPositionIn.Z, BlockOffsetOut.Z);

    CheckBlockOffset(BlockOffsetOut);
}


inline void IndexToLocal(const BlockIndex& BlockIndexIn, int& X, int& Y, int& Z) noexcept
{
    CheckBlockIndex(BlockIndexIn);

    const int32& Int32Index = static_cast<int32>(BlockIndexIn);

#ifdef TE_ENABLE_CONVERSION_OPTIMIZATIONS
    X = Int32Index & MaxBlockX;
    Y = Int32Index >> ChunkLayerSizeLog2;
    Z = (Int32Index >> ChunkWidthLog2) & MaxBlockZ;
#else
    X = Int32Index % ChunkWidth;
    Y = Int32Index / ChunkLayerSize;
    Z = (Int32Index / ChunkDepth) % ChunkDepth;
#endif

    CheckBlockXYZ(X, Y, Z);
}

/** Convert block index to local coordinates within a chunk (in XZY order: x increases fastest, y increases slowest). */

inline void IndexToLocal(const BlockIndex& BlockIndexIn, BlockCoordinate& BlockOffsetOut) noexcept
{
    CheckBlockIndex(BlockIndexIn);

    const int32& Int32Index = static_cast<int32>(BlockIndexIn);

#ifdef TE_ENABLE_CONVERSION_OPTIMIZATIONS
    BlockOffsetOut.X = Int32Index & MaxBlockX;
    BlockOffsetOut.Y = Int32Index >> ChunkLayerSizeLog2;
    BlockOffsetOut.Z = (Int32Index >> ChunkDepthLog2) & MaxBlockZ;
#else
    BlockOffsetOut.X = Int32Index % ChunkWidth;
    BlockOffsetOut.Y = Int32Index / ChunkLayerSize;
    BlockOffsetOut.Z = (Int32Index / ChunkDepth) % ChunkDepth;
#endif

    CheckBlockOffset(BlockOffsetOut);
}


inline void LocalToIndex(const int& X, const int& Y, const int& Z, BlockIndex& BlockIndexOut) noexcept
{
    CheckBlockXYZ(X, Y, Z);

#ifdef TE_ENABLE_CONVERSION_OPTIMIZATIONS
    BlockIndexOut =
    X +
    (Y << ChunkLayerSizeLog2) +
    (Z << ChunkWidthLog2);
#else
    BlockIndexOut =
    X +
    Y * ChunkLayerSize +
    Z * ChunkWidth;
#endif

    CheckBlockIndex(BlockIndexOut);
}

/** Convert block local coordinates to block index within chunk (in XZY order: x increases fastest, y increases slowest). */
inline void LocalToIndex(const BlockCoordinate& BlockOffsetIn, BlockIndex& BlockIndexOut) noexcept
{
    LocalToIndex(BlockOffsetIn.X, BlockOffsetIn.Y, BlockOffsetIn.Z, BlockIndexOut);
}

/** Convert world position to chunk key, block offset, and block index. */
inline void WorldPositionToChunkKeyBlockOffsetAndBlockIndex(
    const BlockCoordinate& BlockWorldPositionIn,
    ChunkKey& ChunkKeyOut, BlockCoordinate& BlockOffsetOut, BlockIndex& BlockIndexOut) noexcept
{
    WorldToChunkKey(BlockWorldPositionIn, ChunkKeyOut);
    WorldToLocal(BlockWorldPositionIn, BlockOffsetOut);
    LocalToIndex(BlockOffsetOut, BlockIndexOut);
}

/** Convert chunk key and block offset to world position */
inline void ChunkKeyAndBlockOffsetToWorldPosition(
    const ChunkKey& ChunkKeyIn, const BlockCoordinate& BlockOffsetIn,
    BlockCoordinate& WorldPositionOut) noexcept
{
    ChunkKeyToWorld(ChunkKeyIn, WorldPositionOut);

    CheckBlockOffset(BlockOffsetIn);

    // offset the chunk origin by the block offset
    WorldPositionOut.X += BlockOffsetIn.X;
    WorldPositionOut.Y += BlockOffsetIn.Y;
    WorldPositionOut.Z += BlockOffsetIn.Z;

    CheckWorldPosition(WorldPositionOut);
}


/** Convert chunk key and block index to world position */
inline void ChunkKeyAndBlockIndexToWorldPosition(
    const ChunkKey& ChunkKeyIn, const BlockIndex& BlockIndexIn,
    BlockCoordinate& BlockWorldPositionOut) noexcept
{
    BlockCoordinate BlockOffset;
    IndexToLocal(BlockIndexIn, BlockOffset);
    ChunkKeyAndBlockOffsetToWorldPosition(ChunkKeyIn, BlockOffset, BlockWorldPositionOut);
}
