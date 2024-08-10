
#include "WorldOperations.h"

#include "VoxelCore/Chunk.h"
#include "PerlinNoise.h"

void WorldOperations::DrawSphere(int centerX, int centerY, int centerZ, int radius, BlockType blockType)
{
    // Calculate the sphere's bounds
    WorldRegion region;
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
    IterateBlocks(region, [&](Chunk* const chunk, const LocalBlockCoord& localBlock, const WorldBlockCoord& worldBlock)
    {
        // Calculate the squared distance from the center of the sphere to the current block
        int dx = worldBlock.X - centerX;
        int dy = worldBlock.Y - centerY;
        int dz = worldBlock.Z - centerZ;
        int distanceSquared = dx * dx + dy * dy + dz * dz;

        // Check if the block is within the sphere
        // todo: add radius bias back in to remove those unwanted single blocks at axes.
        if (distanceSquared <= radiusSquared - bias)
        {
            // Set the block type
            chunk->SetBlockType(localBlock.Index, blockType);
        }
    });
}

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
            double noiseValue = perlinNoise.noise(x * scale, 0.0, z * scale);
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


BlockType WorldOperations::GetBlockType(const WorldBlockCoord& worldBlock) const
{
    // Get the chunk
    LocalBlockCoord localBlock;
    WorldToLocal(worldBlock, localBlock);
    Chunk& chunk = world_.GetChunk(localBlock.chunkKey);

    // Get the block type
    return chunk.GetBlockType(localBlock.Index);
}


void WorldOperations::SetBlockType(const WorldBlockCoord& worldBlock, const BlockType blockType) const
{
    // Get the chunk
    LocalBlockCoord localBlock;
    WorldToLocal(worldBlock, localBlock);
    Chunk& chunk = world_.GetChunk(localBlock.chunkKey);

    // Set the block type
    chunk.SetBlockType(localBlock.Index, blockType);
}


int WorldOperations::GetHighestBlockHeightAt(int x, int z)
{
    // Get the chunk
    WorldBlockCoord worldBlock = {x, CHUNK_SIZE_Y-1, z};
    LocalBlockCoord localBlock;
    WorldToLocal(worldBlock, localBlock);
    Chunk& chunk = world_.GetChunk(localBlock.chunkKey);

    return chunk.GetHighestBlockHeightAt(localBlock.Index);
}

// Is there air? YOU DON'T KNOW!! (v=kcPouxFqUFA)
bool WorldOperations::IsAirBlock(int x, int y, int z) const
{
    LocalBlockCoord localBlock;
    WorldToLocal({x, y, z}, localBlock);

    Chunk* chunk = world_.TryGetChunk(localBlock.chunkKey);
    if(chunk == nullptr) 
    {
        return true; // since there's no chunk, it's air
    }
    
    return chunk->IsAirBlock(localBlock.Index);
}
