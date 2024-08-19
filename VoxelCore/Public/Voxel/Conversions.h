// Copyright

#pragma once

#include "VoxelDefines.h"

#include "Chunk/ChunkConstants.h"
#include "Chunk/ChunkKey.h"


/** Get the chunk key for a world block coordinate */
constexpr inline void WorldToLocalBranching(const BlockCoordinate& BlockWorldPostionIn,
    ChunkKey& ChunkKeyOut) noexcept
{
    // get chunk key accounting for negative worldIn coordinates
    ChunkKeyOut.X = BlockWorldPostionIn.X < 0 ? (BlockWorldPostionIn.X + 1) / ChunkWidth - 1 : BlockWorldPostionIn.X / ChunkWidth;
    ChunkKeyOut.Z = BlockWorldPostionIn.Z < 0 ? (BlockWorldPostionIn.Z + 1) / ChunkDepth - 1 : BlockWorldPostionIn.Z / ChunkDepth;
}


/** Get the chunk key for a world block coordinate. Non-branching version */
constexpr inline void WorldToLocalNonBranching(const BlockCoordinate& BlockWorldPostionIn,
    ChunkKey& ChunkKeyOut) noexcept
{
    int OneIfNegX = (BlockWorldPostionIn.X < 0);
    int OneIfNegZ = (BlockWorldPostionIn.Z < 0);
    ChunkKeyOut.X = (BlockWorldPostionIn.X + OneIfNegX) / ChunkWidth - OneIfNegX;
    ChunkKeyOut.Z = (BlockWorldPostionIn.Z + OneIfNegZ) / ChunkDepth - OneIfNegZ;
}


/** Get the chunk key for a world block coordinate. Bitwise version */
constexpr inline void WorldToLocal(const BlockCoordinate& BlockWorldPostionIn,
    ChunkKey& ChunkKeyOut) noexcept
{
    const int32 SignMaskX = BlockWorldPostionIn.X >> 31; // Extract the sign bit (0 if positive, -1 if negative)
    const int32 SignMaskZ = BlockWorldPostionIn.Z >> 31; // Extract the sign bit (0 if positive, -1 if negative)
    ChunkKeyOut.X = (BlockWorldPostionIn.X + SignMaskX) >> 4; // Equivalent to dividing by ChunkWidth (16)
    ChunkKeyOut.Z = (BlockWorldPostionIn.Z + SignMaskZ) >> 4; // Equivalent to dividing by ChunkDepth (16)
}
static_assert(ChunkWidth == 16, "ChunkWidth must be 16 for bitwise version");
static_assert(ChunkDepth == 16, "ChunkDepth must be 16 for bitwise version");


/** Convert a world block position to a chunk key, block offset and block index. */
constexpr inline void WorldToLocal(
    const BlockCoordinate& BlockWorldPositionIn,
    ChunkKey& ChunkKeyOut, BlockCoordinate& BlockOffsetOut, BlockIndex& BlockIndexOut) noexcept
{
    // get chunk key accounting for negative worldIn coordinates
    //localOut.chunkKey.X = worldIn.X < 0 ? (worldIn.X + 1) / CHUNK_SIZE_X - 1 : worldIn.X / CHUNK_SIZE_X;
    //localOut.chunkKey.Z = worldIn.Z < 0 ? (worldIn.Z + 1) / CHUNK_SIZE_Z - 1 : worldIn.Z / CHUNK_SIZE_Z;
    WorldToLocal(BlockWorldPositionIn, ChunkKeyOut);

    // get local coordinates within the chunk and account for negative values
    // to do this we add 1 to the negative worldIn value and then mod by the chunk size
    // otherwise we just mod by the chunk size
    BlockOffsetOut.X = BlockWorldPositionIn.X<0 ? (BlockWorldPositionIn.X + 1) % ChunkWidth + ChunkWidth - 1 : BlockWorldPositionIn.X % ChunkWidth;
    BlockOffsetOut.Y = BlockWorldPositionIn.Y;
    BlockOffsetOut.Z = BlockWorldPositionIn.Z<0 ? (BlockWorldPositionIn.Z + 1) % ChunkDepth + ChunkDepth - 1 : BlockWorldPositionIn.Z % ChunkDepth;

    // clamp the y value between 0 and CHUNK_SIZE_Y (for now. we may want the callers to handle this and let check fail)
    if (BlockOffsetOut.Y < 0)
    {
        BlockOffsetOut.Y = 0;
    }
    else if (BlockOffsetOut.Y >= ChunkHeight)
    {
        BlockOffsetOut.Y = ChunkHeight - 1;
    }

    // get the 1D index of the block within the chunk in YZX order (x increases fastest, y increases slowest)
    BlockIndexOut =
        BlockOffsetOut.Y * ChunkLayerSize +
        BlockOffsetOut.Z * ChunkWidth +
        BlockOffsetOut.X;

    // check local coordinates are within the chunk
    check(BlockOffsetOut.X >= 0 && BlockOffsetOut.X < ChunkWidth);
    check(BlockOffsetOut.Y >= 0 && BlockOffsetOut.Y < ChunkHeight);
    check(BlockOffsetOut.Z >= 0 && BlockOffsetOut.Z < ChunkDepth);

    // check 1D index is within the chunk
    check(BlockIndexOut < ChunkSize);
}


/** Convert a chunk key to the block world position at its origin */
constexpr inline void ChunkToWorld(
    const ChunkKey& ChunkKeyIn,
    BlockCoordinate& ChunkWorldPositionOut) noexcept
{
    ChunkWorldPositionOut.X = ChunkKeyIn.X * ChunkWidth;
    ChunkWorldPositionOut.Y = 0;
    ChunkWorldPositionOut.Z = ChunkKeyIn.Z * ChunkDepth;
}


/** Convert a chunk key block offset to a block world position */
constexpr inline void LocalToWorld(
    const ChunkKey& ChunkKeyIn, const BlockCoordinate& BlockChunkOffsetIn,
    BlockCoordinate& BlockWorldPositionOut) noexcept
{
    ChunkToWorld(ChunkKeyIn, BlockWorldPositionOut);
    BlockWorldPositionOut.X += BlockChunkOffsetIn.X;
    BlockWorldPositionOut.Y = BlockChunkOffsetIn.Y;
    BlockWorldPositionOut.Z += BlockChunkOffsetIn.Z;
}
