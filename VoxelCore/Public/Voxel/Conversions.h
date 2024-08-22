// Copyright

#pragma once

#include "VoxelDefines.h"

#include "CoreTypes.h" 
#include "Block/Block.h"
#include "Chunk/ChunkConstants.h"
#include "Chunk/ChunkKey.h"

#include "Misc/AssertionMacros.h"

/** Using the following naming conventions:
 *  - WorldPosition: A block coordinate position in world space.
 *  - BlockOffset: A block coordinate offset in chunk space.
 *  - BlockIndex: A linear index into a chunk's block array.
 *  - ChunkKey: A chunk's cx,cz coordinate in the world's chunk grid.
 */


/*
    // chunkkey <--> worldposition
    ChunkKeyToWorldPosition
    WorldPositionToChunkKey

    // blockindex <--> blockoffset
    BlockIndexToBlockOffset
    BlockOffsetToBlockIndex

    // worldposition <--> chunk local variables (chunkkey, blockoffset, blockindex)
    WorldPositionToBlockOffset (omits the chunk key, should this exist? it is computable without intermediate chunk key)

    WorldPositionToChunkKeyBlockOffsetAndBlockIndex

    ChunkKeyAndBlockOffsetToWorldPosition
    ChunkKeyAndBlockIndexToWorldPosition
*/

#define CheckWorldPosition(BlockPosition) check((BlockPosition).X >= MinPlayerWorldPosition && (BlockPosition).X < MaxPlayerWorldPosition); \
    check((BlockPosition).Y >= MinBlockHeight && (BlockPosition).Y <= MaxBlockHeight); \
    check((BlockPosition).Z >= MinPlayerWorldPosition && (BlockPosition).Z < MaxPlayerWorldPosition)

#define CheckBlockIndex(BlockIndex) check((BlockIndex) < ChunkSize && (BlockIndex) >= 0)

#define CheckBlockOffset(BlockOffset) check((BlockOffset).X >= 0 && (BlockOffset).X < ChunkWidth); \
    check((BlockOffset).Y >= 0 && (BlockOffset).Y < ChunkHeight); \
    check((BlockOffset).Z >= 0 && (BlockOffset).Z < ChunkDepth)

#define CheckChunkKey(ChunkKey) check((ChunkKey).X >= WorldChunkMin && (ChunkKey).X < WorldChunkMax); \
    check((ChunkKey).Z >= WorldChunkMin && (ChunkKey).Z < WorldChunkMax)


/** Convert chunk key to chunk origin in world space. */
constexpr inline void ChunkKeyToWorldPosition(
    const ChunkKey& ChunkKeyIn,
    BlockCoordinate& ChunkWorldPositionOut) noexcept
{
    CheckChunkKey(ChunkKeyIn);
    
    ChunkWorldPositionOut.X = ChunkKeyIn.X * ChunkWidth;
    ChunkWorldPositionOut.Y = 0;
    ChunkWorldPositionOut.Z = ChunkKeyIn.Z * ChunkDepth;

    CheckWorldPosition(ChunkWorldPositionOut);
}


/** Convert world position to chunk key. */
constexpr inline void WorldPositionToChunkKey(const BlockCoordinate& WorldPositionIn,
                                              ChunkKey& ChunkKeyOut) noexcept
{
    CheckWorldPosition(WorldPositionIn);

    const int32 SignMaskX = WorldPositionIn.X >> 31; // Extract the sign bit (0 if positive, -1 if negative)
    const int32 SignMaskZ = WorldPositionIn.Z >> 31; // Extract the sign bit (0 if positive, -1 if negative)
    ChunkKeyOut.X = (WorldPositionIn.X + SignMaskX) >> 4; // Equivalent to dividing by ChunkWidth (16)
    ChunkKeyOut.Z = (WorldPositionIn.Z + SignMaskZ) >> 4; // Equivalent to dividing by ChunkDepth (16)

    CheckChunkKey(ChunkKeyOut);
}
static_assert(ChunkWidth == 16, "ChunkWidth must be 16 for bitwise version");
static_assert(ChunkDepth == 16, "ChunkDepth must be 16 for bitwise version");


/** Convert world position to block offset. */
constexpr inline void WorldPositionToBlockOffset(
    const BlockCoordinate& WorldPositionIn, BlockCoordinate& BlockOffsetOut) noexcept
{
    CheckWorldPosition(WorldPositionIn);

    // get local coordinates within the chunk and account for negative values
    // to do this we add 1 to the negative worldIn value and then mod by the chunk size
    // otherwise we just mod by the chunk size
    BlockOffsetOut.X = WorldPositionIn.X < 0
                           ? (WorldPositionIn.X + 1) % ChunkWidth + ChunkWidth - 1
                           : WorldPositionIn.X % ChunkWidth;
    BlockOffsetOut.Y = WorldPositionIn.Y;
    BlockOffsetOut.Z = WorldPositionIn.Z < 0
                           ? (WorldPositionIn.Z + 1) % ChunkDepth + ChunkDepth - 1
                           : WorldPositionIn.Z % ChunkDepth;

    CheckBlockOffset(BlockOffsetOut);
}


/** Convert block index to block offset. */
constexpr inline void BlockIndexToBlockOffset(const BlockIndex& BlockIndexIn, BlockCoordinate& BlockOffsetOut) noexcept
{
    CheckBlockIndex(BlockIndexIn);

    const int32 Int32Index = static_cast<int32>(BlockIndexIn);

    // get the block offset using block index in YZX order (x increases fastest, y increases slowest)
    BlockOffsetOut.Y = Int32Index / ChunkLayerSize;
    BlockOffsetOut.Z = (Int32Index / ChunkWidth) % ChunkDepth;
    BlockOffsetOut.X = Int32Index % ChunkWidth;

    CheckBlockOffset(BlockOffsetOut);
}


/** Convert block offset to block index. */
constexpr inline void BlockOffsetToBlockIndex(const BlockCoordinate& BlockOffsetIn, BlockIndex& BlockIndexOut) noexcept
{
    CheckBlockOffset(BlockOffsetIn);

    // get the block index within chunk in YZX order (x increases fastest, y increases slowest)
    BlockIndexOut =
        BlockOffsetIn.Y * ChunkLayerSize +
        BlockOffsetIn.Z * ChunkWidth +
        BlockOffsetIn.X;

    CheckBlockIndex(BlockIndexOut);
}



/** Convert world position to chunk key, block offset, and block index. */
constexpr inline void WorldPositionToChunkKeyBlockOffsetAndBlockIndex(
    const BlockCoordinate& BlockWorldPositionIn,
    ChunkKey& ChunkKeyOut, BlockCoordinate& BlockOffsetOut, BlockIndex& BlockIndexOut) noexcept
{
    WorldPositionToChunkKey(BlockWorldPositionIn, ChunkKeyOut);
    WorldPositionToBlockOffset(BlockWorldPositionIn, BlockOffsetOut);
    BlockOffsetToBlockIndex(BlockOffsetOut, BlockIndexOut);
}





/** Convert chunk key and block offset to world position */
constexpr inline void ChunkKeyAndBlockOffsetToWorldPosition(
    const ChunkKey& ChunkKeyIn, const BlockCoordinate& BlockOffsetIn,
    BlockCoordinate& WorldPositionOut) noexcept
{
    // get the world position of the chunk origin
    ChunkKeyToWorldPosition(ChunkKeyIn, WorldPositionOut);

    // offset the chunk origin by the block offset
    WorldPositionOut.X += BlockOffsetIn.X;
    WorldPositionOut.Y += BlockOffsetIn.Y;
    WorldPositionOut.Z += BlockOffsetIn.Z;

    CheckWorldPosition(WorldPositionOut);
}


/** Convert chunk key and block index to world position */
constexpr inline void ChunkKeyAndBlockIndexToWorldPosition(
    const ChunkKey& ChunkKeyIn, const BlockIndex& BlockIndexIn,
    BlockCoordinate& BlockWorldPositionOut) noexcept
{
    BlockCoordinate BlockOffset;
    BlockIndexToBlockOffset(BlockIndexIn, BlockOffset);
    ChunkKeyAndBlockOffsetToWorldPosition(ChunkKeyIn, BlockOffset, BlockWorldPositionOut);
}


// /** Get the chunk key for a world block coordinate */
// constexpr inline void WorldPositionToChunkKeyBranching(const BlockCoordinate& BlockWorldPostionIn,
//     ChunkKey& ChunkKeyOut) noexcept
// {
//     // get chunk key accounting for negative worldIn coordinates
//     ChunkKeyOut.X = BlockWorldPostionIn.X < 0 ? (BlockWorldPostionIn.X + 1) / ChunkWidth - 1 : BlockWorldPostionIn.X / ChunkWidth;
//     ChunkKeyOut.Z = BlockWorldPostionIn.Z < 0 ? (BlockWorldPostionIn.Z + 1) / ChunkDepth - 1 : BlockWorldPostionIn.Z / ChunkDepth;
// }
//
//
// /** Get the chunk key for a world block coordinate. Non-branching version */
// constexpr inline void WorldPositionToChunkKeyNonBranching(const BlockCoordinate& BlockWorldPostionIn,
//     ChunkKey& ChunkKeyOut) noexcept
// {
//     int OneIfNegX = (BlockWorldPostionIn.X < 0);
//     int OneIfNegZ = (BlockWorldPostionIn.Z < 0);
//     ChunkKeyOut.X = (BlockWorldPostionIn.X + OneIfNegX) / ChunkWidth - OneIfNegX;
//     ChunkKeyOut.Z = (BlockWorldPostionIn.Z + OneIfNegZ) / ChunkDepth - OneIfNegZ;
// }
