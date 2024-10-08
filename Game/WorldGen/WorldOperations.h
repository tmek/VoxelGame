﻿// Copyright

#pragma once

#include "World/VoxelWorld.h"


// class to perform world operations
class WorldOperations
{
public:
    // Constructor
    WorldOperations(VoxelWorld& world) : world_(world)
    {
    }

    // Template function to iterate over a 3D volume and apply a given .
    // It keeps track of the current chunk and only fetches it when the chunk key changes.
    // Being templated allows the operation to be a lambda function or a function object.
    // Performance implications are minimal due to inlining of the blockOperation.
    // todo: it would be amazing if we could do range-based for loops with this
    template <typename Func>
    void IterateBlocks_deprecated(BlockRegion& region, Func blockOperation);
    // todo: move to it's own file and keep the editing functions here

    // Function to draw a sphere in the world using a specific operation
    void DrawSphere_deprecated(int centerX, int centerY, int centerZ, int radius, BlockType blockType);

    // Method to generate terrain with Perlin noise
    void GeneratePerlinNoiseTerrain(int startX, int startZ, int width, int depth, float scale, float amplitude,
                                    uint16_t type);
    
    Block& GetBlockRef(const BlockCoordinate& WorldBlockPosition) const;

    void SetBlockType(const BlockCoordinate& worldBlock, const BlockType blockType) const;

    int GetHighestBlockHeightAt(int x, int z);

    bool IsAirBlock(int x, int y, int z) const;
    bool IsWaterBlock(int x, int y, int z) const;

private:
    VoxelWorld& world_; // todo: shared pointer or something
};

//
// template <typename Func>
// void WorldOperations::IterateBlocks_deprecated(BlockRegion& region, Func blockOperation)
// {
//     ChunkPtr activeChunk = nullptr;
//     ChunkKey activeChunkKey;
//
//     BlockCoordinate BlockOffset;
//     
//     // Iterate over all blocks within the 3D region in XZY order
//     for (int y = region.Min.Y; y <= region.Max.Y; ++y)
//     {
//         for (int z = region.Min.Z; z <= region.Max.Z; ++z)
//         {
//             for (int x = region.Min.X; x <= region.Max.X; ++x)
//             {
//                 BlockCoordinate worldBlock = {x, y, z};
//                 WorldPositionToBlockOffset(worldBlock, BlockOffset);
//
//                 // retrieve chunk if needed
//                 bool ShouldGetChunk = BlockOffset.ChunkKey != activeChunkKey || activeChunk == nullptr;
//                 if (ShouldGetChunk)
//                 {
//                     activeChunkKey = localBlock.ChunkKey;
//                     activeChunk = &world_.GetChunk(activeChunkKey);
//                 }
//
//                 check(activeChunk != nullptr);
//                 
//                 // Apply operation to block
//                 blockOperation(activeChunk, localBlock, worldBlock);
//             }
//         }
//     }
// }
