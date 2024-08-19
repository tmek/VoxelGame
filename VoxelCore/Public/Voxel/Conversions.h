// Copyright

#pragma once

#include "VoxelDefines.h"

#include "Block/Block.h"

#include "Chunk/ChunkConstants.h"
#include "Chunk/ChunkKey.h"


/** Get the chunk key for a world block coordinate */
constexpr inline void WorldPositionToChunkKeyBranching(const BlockCoordinate& BlockWorldPostionIn,
    ChunkKey& ChunkKeyOut) noexcept
{
    // get chunk key accounting for negative worldIn coordinates
    ChunkKeyOut.X = BlockWorldPostionIn.X < 0 ? (BlockWorldPostionIn.X + 1) / ChunkWidth - 1 : BlockWorldPostionIn.X / ChunkWidth;
    ChunkKeyOut.Z = BlockWorldPostionIn.Z < 0 ? (BlockWorldPostionIn.Z + 1) / ChunkDepth - 1 : BlockWorldPostionIn.Z / ChunkDepth;
}


/** Get the chunk key for a world block coordinate. Non-branching version */
constexpr inline void WorldPositionToChunkKeyNonBranching(const BlockCoordinate& BlockWorldPostionIn,
    ChunkKey& ChunkKeyOut) noexcept
{
    int OneIfNegX = (BlockWorldPostionIn.X < 0);
    int OneIfNegZ = (BlockWorldPostionIn.Z < 0);
    ChunkKeyOut.X = (BlockWorldPostionIn.X + OneIfNegX) / ChunkWidth - OneIfNegX;
    ChunkKeyOut.Z = (BlockWorldPostionIn.Z + OneIfNegZ) / ChunkDepth - OneIfNegZ;
}


/** Get the chunk key for a world block coordinate. Bitwise version */
constexpr inline void WorldPositionToChunkKey(const BlockCoordinate& BlockWorldPositionIn,
    ChunkKey& ChunkKeyOut) noexcept
{
    check(BlockWorldPositionIn.Y >= 0 && BlockWorldPositionIn.Y < ChunkHeight);
    
    const int32 SignMaskX = BlockWorldPositionIn.X >> 31; // Extract the sign bit (0 if positive, -1 if negative)
    const int32 SignMaskZ = BlockWorldPositionIn.Z >> 31; // Extract the sign bit (0 if positive, -1 if negative)
    ChunkKeyOut.X = (BlockWorldPositionIn.X + SignMaskX) >> 4; // Equivalent to dividing by ChunkWidth (16)
    ChunkKeyOut.Z = (BlockWorldPositionIn.Z + SignMaskZ) >> 4; // Equivalent to dividing by ChunkDepth (16)
}
static_assert(ChunkWidth == 16, "ChunkWidth must be 16 for bitwise version");
static_assert(ChunkDepth == 16, "ChunkDepth must be 16 for bitwise version");


/** Convert a block world position to the block offset in its chunk. */
constexpr inline void WorldPositionToBlockOffset(
    const BlockCoordinate& BlockWorldPositionIn, BlockCoordinate& BlockOffsetOut) noexcept
{
    check(BlockWorldPositionIn.Y >= 0 && BlockWorldPositionIn.Y < ChunkHeight);
    
    // get local coordinates within the chunk and account for negative values
    // to do this we add 1 to the negative worldIn value and then mod by the chunk size
    // otherwise we just mod by the chunk size
    BlockOffsetOut.X = BlockWorldPositionIn.X<0 ? (BlockWorldPositionIn.X + 1) % ChunkWidth + ChunkWidth - 1 : BlockWorldPositionIn.X % ChunkWidth;
    BlockOffsetOut.Y = BlockWorldPositionIn.Y;
    BlockOffsetOut.Z = BlockWorldPositionIn.Z<0 ? (BlockWorldPositionIn.Z + 1) % ChunkDepth + ChunkDepth - 1 : BlockWorldPositionIn.Z % ChunkDepth;
}

/** Convert a block index to a block offset. */
constexpr inline void BlockIndexToBlockOffset(const BlockIndex& BlockIndexIn, BlockCoordinate& BlockOffsetOut) noexcept
{
    // get the 3D coordinates of the block within the chunk
    int Int32Index = static_cast<int32>(BlockIndexIn);
    BlockOffsetOut.X = Int32Index % ChunkWidth;
    BlockOffsetOut.Y = Int32Index / ChunkLayerSize;
    BlockOffsetOut.Z = (Int32Index / ChunkWidth) % ChunkDepth;

    // check 1D index is within the chunk
    check(BlockIndexIn < ChunkSize);
}

/** Convert a block offset to a block index. */
constexpr inline void BlockOffsetToBlockIndex(const BlockCoordinate& BlockOffsetIn, BlockIndex& BlockIndexOut) noexcept
{
    // get the 1D index of the block within the chunk in YZX order (x increases fastest, y increases slowest)
    BlockIndexOut =
        BlockOffsetIn.Y * ChunkLayerSize +
        BlockOffsetIn.Z * ChunkWidth +
        BlockOffsetIn.X;

    // check local coordinates are within the chunk
    check(BlockOffsetIn.X >= 0 && BlockOffsetIn.X < ChunkWidth);
    check(BlockOffsetIn.Y >= 0 && BlockOffsetIn.Y < ChunkHeight);
    check(BlockOffsetIn.Z >= 0 && BlockOffsetIn.Z < ChunkDepth);

    // check 1D index is within the chunk
    check(BlockIndexOut < ChunkSize);    
}

/** Convert a world block position to a chunk key + block offset + block index. */
constexpr inline void WorldToLocal(
    const BlockCoordinate& BlockWorldPositionIn,
    ChunkKey& ChunkKeyOut, BlockCoordinate& BlockOffsetOut, BlockIndex& BlockIndexOut) noexcept
{
    WorldPositionToChunkKey(BlockWorldPositionIn, ChunkKeyOut);
    WorldPositionToBlockOffset(BlockWorldPositionIn, BlockOffsetOut);
    BlockOffsetToBlockIndex(BlockOffsetOut, BlockIndexOut);
}


/** Convert a chunk key to the block world position at its origin */
constexpr inline void ChunkKeyToWorldPosition(
    const ChunkKey& ChunkKeyIn,
    BlockCoordinate& ChunkWorldPositionOut) noexcept
{
    ChunkWorldPositionOut.X = ChunkKeyIn.X * ChunkWidth;
    ChunkWorldPositionOut.Y = 0;
    ChunkWorldPositionOut.Z = ChunkKeyIn.Z * ChunkDepth;
}


/** Convert a chunk key + block offset to the block world position */
constexpr inline void LocalToWorld(
    const ChunkKey& ChunkKeyIn, const BlockCoordinate& BlockOffsetIn,
    BlockCoordinate& BlockWorldPositionOut) noexcept
{
    ChunkKeyToWorldPosition(ChunkKeyIn, BlockWorldPositionOut);
    BlockWorldPositionOut.X += BlockOffsetIn.X;
    BlockWorldPositionOut.Y = BlockOffsetIn.Y;
    BlockWorldPositionOut.Z += BlockOffsetIn.Z;
}


/** Convert a chunk key + block index to the block world position */
constexpr inline void LocalToWorld(
    const ChunkKey& ChunkKeyIn, const BlockIndex& BlockIndexIn,
    BlockCoordinate& BlockWorldPositionOut) noexcept
{
    BlockCoordinate BlockOffset;
    BlockIndexToBlockOffset(BlockIndexIn, BlockOffset);
    LocalToWorld(ChunkKeyIn, BlockOffset, BlockWorldPositionOut);
}
