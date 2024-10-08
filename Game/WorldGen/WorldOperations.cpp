﻿
#include "WorldOperations.h"

#include "BlockTypes.h"

#include "Block/Block.h"

#include "Chunk/Chunk.h"
#include "Chunk/ChunkKey.h"

#include "Math/Noise/PerlinNoise.h"

#include "Voxel/Conversions.h"


/*
void WorldOperations::DrawSphere_deprecated(int centerX, int centerY, int centerZ, int radius, BlockType blockType)
{
    // Calculate the sphere's bounds
    BlockRegion region;
    region.Min.X = centerX - radius;
    region.Min.Y = centerY - radius;
    region.Min.Z = centerZ - radius;
    region.Max.X = centerX + radius;
    region.Max.Y = centerY + radius;
    region.Max.Z = centerZ + radius;

    // Calculate the squared radius once
    int radiusSquared = radius * radius;

    // Define a small bias factor
    int bias = 1; // You can adjust this value to fine-tune the effect

    // Iterate over all blocks within the sphere's bounds
    // IterateBlocks(region, [&](Chunk* const chunk, const LocalBlockCoord& localBlock, const WorldBlockCoord& worldBlock)
    // {
    //     // Calculate the squared distance from the center of the sphere to the current block
    //     int dx = worldBlock.X - centerX;
    //     int dy = worldBlock.Y - centerY;
    //     int dz = worldBlock.Z - centerZ;
    //     int distanceSquared = dx * dx + dy * dy + dz * dz;
    //
    //     // Check if the block is within the sphere
    //     // todo: add radius bias back in to remove those unwanted single blocks at axes.
    //     if (distanceSquared <= radiusSquared - bias)
    //     {
    //         // Set the block type
    //         chunk->SetBlockType(localBlock.Index, blockType);
    //     }
    // });
}
*/

static void CalculateHeightRange(float amplitude, int& minY, int& maxY)
{
    minY = static_cast<int>(-amplitude / 2);
    maxY = static_cast<int>((3 * amplitude) / 2);
}

// static void WorldOperations::GenerateHills(int startX, int startZ, int width, int depth, float amplitude, uint16_t type)
// {
//     int minY, maxY;
//     CalculateHeightRange(amplitude, minY, maxY);
//      
// }

void WorldOperations::GeneratePerlinNoiseTerrain(int startX, int startZ, int width, int depth, float scale, float amplitude,
                                           uint16_t type)
{
    PerlinNoise perlinNoise;

    // todo: upgrade this to use the new IterateBlocks function

    for (int x = startX; x < startX + width; ++x)
    {
        for (int z = startZ; z < startZ + depth; ++z)
        {
            double noiseValue = perlinNoise.sample3D(x * scale, 0.0, z * scale);
            const int y = static_cast<int>(noiseValue * amplitude + amplitude / 2);

            // set blocks at the bottom of the vertical range to water
            int bottom = 32 - static_cast<int>(amplitude) / 2;
            if (y == 0)
            {
                SetBlockType({x, y, z}, 4); // water                
            }
            else
            {
                SetBlockType({x, y, z}, type);
            }

            // fill in the area below the terrain with dirt
            // todo: this should be a world or chunk function for efficiency
            for (int i = y - 1; i >= 0; --i)
            {
                SetBlockType({x, i, z}, 2);
            }
        }
    }
}


Block& WorldOperations::GetBlockRef(const BlockCoordinate& WorldBlockPosition) const
{
    // Get the chunk
    ChunkKey ChunkKey;
    BlockCoordinate BlockOffset;
    BlockIndex BlockIndex;
    WorldPositionToChunkKeyBlockOffsetAndBlockIndex(WorldBlockPosition, ChunkKey, BlockOffset, BlockIndex);

    ChunkPtr chunk = world_.TryGetChunk(ChunkKey);

    if(!chunk)
    {
        return const_cast<Block&>(Block::Empty);
    }
    
    // Get the block type
    return chunk->Blocks[BlockIndex];
}


void WorldOperations::SetBlockType(const BlockCoordinate& worldBlock, const BlockType blockType) const
{
    // TODO: currently disabling this temporarily, this means we can draw trees or single blocks in the world.
    // // Get the chunk
    // LocalBlockCoord localBlock;
    // WorldToLocal(worldBlock, localBlock);
    // Chunk& chunk = world_.GetChunk(localBlock.chunkKey);
    //
    // // Set the block type
    // chunk.SetBlockType(localBlock.Index, blockType);
}


int WorldOperations::GetHighestBlockHeightAt(int x, int z)
{
    // Get the chunk
    BlockCoordinate WorldBlockPosition = {x, MaxBlockY, z};

    ChunkKey ChunkKey;
    BlockCoordinate HighestBlockOffset;
    BlockIndex HighestBlockIndex;
    WorldPositionToChunkKeyBlockOffsetAndBlockIndex(WorldBlockPosition, ChunkKey, HighestBlockOffset, HighestBlockIndex);
    
    // if the block index is invalid, return bottom layer Y height
    if (HighestBlockIndex <= 0 || HighestBlockIndex >= ChunkSize)
    {
        return MinBlockY;
    }

    // try to get the chunk
    ChunkPtr Chunk = world_.TryGetChunk(ChunkKey);
    if(!Chunk)
    {
        return MaxBlockY; 
    }


    // start at the top of the chunk and move down until we find a non-air block or hit the bottom
    while (HighestBlockIndex > 0)
    {
        const bool IsAir = Chunk->Blocks[HighestBlockIndex].IsAir();
        if (!IsAir)
        {
            return HighestBlockIndex / ChunkLayerSize;
        }

        // move down one layer
        HighestBlockIndex -= ChunkLayerSize;
    }

    // we went all the way to the bottom and found nothing
    return 0;     
}

// Is there air? YOU DON'T KNOW!! (v=kcPouxFqUFA)
bool WorldOperations::IsAirBlock(int x, int y, int z) const
{
    ChunkKey ChunkKey;
    BlockCoordinate localBlock;
    BlockIndex index;
    WorldPositionToChunkKeyBlockOffsetAndBlockIndex({x, y, z}, ChunkKey, localBlock, index);

    ChunkPtr chunk = world_.TryGetChunk(ChunkKey);
    if(chunk == nullptr) 
    {
        return true; // since there's no chunk, it's air
    }
    
    return chunk->Blocks[index].IsAir();
}

bool WorldOperations::IsWaterBlock(int x, int y, int z) const
{
    ChunkKey ChunkKey;
    BlockCoordinate localBlock;
    BlockIndex index;
    WorldPositionToChunkKeyBlockOffsetAndBlockIndex({x, y, z}, ChunkKey, localBlock, index);

    ChunkPtr chunk = world_.TryGetChunk(ChunkKey);
    if(chunk == nullptr) 
    {
        return false; // since there's no chunk, it's not water
    }
    
    return chunk->Blocks[index].IsWater();
}
