#pragma once
#include "Coordinates.h"
#include "DataTypes/ChunkConstants.h"


inline void WorldToLocal(const BlockWorldCoordinate& WorldIn, ChunkKey& ChunkKeyOut)
{
    // get chunk key accounting for negative worldIn coordinates
    ChunkKeyOut.X = WorldIn.X < 0 ? (WorldIn.X + 1) / ChunkWidth - 1 : WorldIn.X / ChunkWidth;
    ChunkKeyOut.Z = WorldIn.Z < 0 ? (WorldIn.Z + 1) / ChunkDepth - 1 : WorldIn.Z / ChunkDepth;
}

inline void WorldToLocal(const BlockWorldCoordinate& worldIn, LocalBlockPosition& localOut)
{
    // get chunk key accounting for negative worldIn coordinates
    //localOut.chunkKey.X = worldIn.X < 0 ? (worldIn.X + 1) / CHUNK_SIZE_X - 1 : worldIn.X / CHUNK_SIZE_X;
    //localOut.chunkKey.Z = worldIn.Z < 0 ? (worldIn.Z + 1) / CHUNK_SIZE_Z - 1 : worldIn.Z / CHUNK_SIZE_Z;
    WorldToLocal(worldIn, localOut.ChunkKey);

    // get local coordinates within the chunk and account for negative values
    // to do this we add 1 to the negative worldIn value and then mod by the chunk size
    // otherwise we just mod by the chunk size
    localOut.X = worldIn.X < 0 ? (worldIn.X + 1) % ChunkWidth + ChunkWidth - 1 : worldIn.X % ChunkWidth;
    localOut.Y = worldIn.Y;
    localOut.Z = worldIn.Z < 0 ? (worldIn.Z + 1) % ChunkDepth + ChunkDepth - 1 : worldIn.Z % ChunkDepth;

    // clamp the y value between 0 and CHUNK_SIZE_Y (for now. we may want the callers to handle this and let check fail)
    if (localOut.Y < 0)
    {
        localOut.Y = 0;
    }
    else if (localOut.Y >= ChunkHeight)
    {
        localOut.Y = ChunkHeight - 1;
    }
    
    // get the 1D index of the block within the chunk in YZX order (x increases fastest, y increases slowest)
    localOut.Index =
        localOut.Y * ChunkLayerSize +
        localOut.Z * ChunkWidth +
        localOut.X;

    // check local coordinates are within the chunk
    check(localOut.X >= 0 && localOut.X < ChunkWidth);
    check(localOut.Y >= 0 && localOut.Y < ChunkHeight);
    check(localOut.Z >= 0 && localOut.Z < ChunkDepth);

    // check 1D index is within the chunk
    check(localOut.Index >= 0 && localOut.Index < ChunkSize);
}

inline void ChunkToWorld(const ChunkKey& chunkKeyIn, BlockWorldCoordinate& chunkOriginOut)
{
    chunkOriginOut.X = chunkKeyIn.X * ChunkWidth;
    chunkOriginOut.Y = 0;
    chunkOriginOut.Z = chunkKeyIn.Z * ChunkDepth;
}


inline void LocalToWorld(const LocalBlockPosition& local_block, BlockWorldCoordinate& world_block)
{
    ChunkToWorld(local_block.ChunkKey, world_block);
    world_block.X += local_block.X;
    world_block.Y = local_block.Y;
    world_block.Z += local_block.Z;
}