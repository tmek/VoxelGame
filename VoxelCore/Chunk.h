#pragma once
#include <cstdint>

#include "Coords2.h"
#include "VoxelDefines.h"

struct Chunk
{
    // --- chunk data ---
    BlockState blockStates[CHUNK_TOTAL_BLOCKS]; // Lower 12-bits for block type, upper 4-bits for block metadata
    BlockLightLevel lightLevels[CHUNK_TOTAL_BLOCKS]; // Lower 4-bits for torchlight, Upper 4-bits for sunlight
    // --- end data ---

    // non copyable
    Chunk() = default;
    ~Chunk() = default;

    // Copy constructor - deleted to prevent copying
    Chunk(const Chunk&) = delete;
    // Copy assignment operator - deleted to prevent copying
    Chunk& operator=(const Chunk&) = delete;
    // Move constructor
    Chunk(Chunk&&) noexcept = default;
    // Move assignment operator
    Chunk& operator=(Chunk&&) noexcept = default;
    
    // --- chunk ops ---
    inline BlockState GetBlockState(BlockIndex index) const;
    inline BlockType GetBlockType(BlockIndex index) const;
    inline BlockMetadata GetBlockMetadata(BlockIndex index) const;
    inline BlockLightLevel GetBlockLightLevel(BlockIndex index) const;
    inline BlockLightLevel GetBlockSkyLight(BlockIndex index) const;

    inline void SetBlockState(BlockIndex index, BlockState state);
    inline void SetBlockType(BlockIndex index, BlockType blockType);
    inline void SetBlockMetadata(BlockIndex index , BlockMetadata metadata);
    inline void SetBlockLightLevel(BlockIndex index, BlockLightLevel lightLevel);
    inline void SetBlockSkyLight(BlockIndex index, BlockLightLevel lightLevel);

    inline bool IsAirBlock(BlockIndex index) const;

    int GetHighestBlockHeightAt(BlockIndex index) const;
};

void Chunk::SetBlockType(BlockIndex index, BlockType blockType)
{
    // break if index is out of bounds
    // todo: this needs to be a check macro or something that gets removed in test/shipping .
    if (index < 0 || index >= CHUNK_TOTAL_BLOCKS)
    {
        __debugbreak();
    }
     
    
    blockStates[index] = (blockStates[index] & 0xF000) | (blockType & 0x0FFF);
}