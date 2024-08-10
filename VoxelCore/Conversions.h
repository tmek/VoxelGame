#pragma once
#include "Coords2.h"


inline void WorldToLocal(const WorldBlockCoord& worldIn, ChunkKey& chunkKey)
{
    // get chunk key accounting for negative worldIn coordinates
    chunkKey.X = worldIn.X < 0 ? (worldIn.X + 1) / CHUNK_SIZE_X - 1 : worldIn.X / CHUNK_SIZE_X;
    chunkKey.Z = worldIn.Z < 0 ? (worldIn.Z + 1) / CHUNK_SIZE_Z - 1 : worldIn.Z / CHUNK_SIZE_Z;
}

inline void WorldToLocal(const WorldBlockCoord& worldIn, LocalBlockCoord& localOut)
{
    // get chunk key accounting for negative worldIn coordinates
    //localOut.chunkKey.X = worldIn.X < 0 ? (worldIn.X + 1) / CHUNK_SIZE_X - 1 : worldIn.X / CHUNK_SIZE_X;
    //localOut.chunkKey.Z = worldIn.Z < 0 ? (worldIn.Z + 1) / CHUNK_SIZE_Z - 1 : worldIn.Z / CHUNK_SIZE_Z;
    WorldToLocal(worldIn, localOut.chunkKey);

    // get local coordinates within the chunk and account for negative values
    // to do this we add 1 to the negative worldIn value and then mod by the chunk size
    // otherwise we just mod by the chunk size
    localOut.X = worldIn.X < 0 ? (worldIn.X + 1) % CHUNK_SIZE_X + CHUNK_SIZE_X - 1 : worldIn.X % CHUNK_SIZE_X;
    localOut.Y = worldIn.Y;
    localOut.Z = worldIn.Z < 0 ? (worldIn.Z + 1) % CHUNK_SIZE_Z + CHUNK_SIZE_Z - 1 : worldIn.Z % CHUNK_SIZE_Z;

    // clamp the y value between 0 and CHUNK_SIZE_Y (for now. we may want the callers to handle this and let check fail)
    if (localOut.Y < 0)
    {
        localOut.Y = 0;
    }
    else if (localOut.Y >= CHUNK_SIZE_Y)
    {
        localOut.Y = CHUNK_SIZE_Y - 1;
    }
    
    // get the 1D index of the block within the chunk in YZX order (x increases fastest, y increases slowest)
    localOut.Index =
        localOut.Y * CHUNK_XZ_LAYER_SIZE +
        localOut.Z * CHUNK_SIZE_X +
        localOut.X;

    // check local coordinates are within the chunk
    check(localOut.X >= 0 && localOut.X < CHUNK_SIZE_X);
    check(localOut.Y >= 0 && localOut.Y < CHUNK_SIZE_Y);
    check(localOut.Z >= 0 && localOut.Z < CHUNK_SIZE_Z);

    // check 1D index is within the chunk
    check(localOut.Index >= 0 && localOut.Index < CHUNK_TOTAL_BLOCKS);
}

inline void ChunkToWorld(const ChunkKey& chunkKeyIn, WorldBlockCoord& chunkOriginOut)
{
    chunkOriginOut.X = chunkKeyIn.X * CHUNK_SIZE_X;
    chunkOriginOut.Y = 0;
    chunkOriginOut.Z = chunkKeyIn.Z * CHUNK_SIZE_Z;
}


inline void LocalToWorld(const LocalBlockCoord& local_block, WorldBlockCoord& world_block)
{
    ChunkToWorld(local_block.chunkKey, world_block);
    world_block.X += local_block.X;
    world_block.Y = local_block.Y;
    world_block.Z += local_block.Z;
}