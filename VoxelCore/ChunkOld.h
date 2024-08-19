#pragma once
#include "VoxelDefines.h"
#include "Coordinates.h"
#include "BlockData.h"

#include <memory>

#include "HAL/Platform.h"

struct ChunkOld
{
    // Constructor
    ChunkOld(ChunkKey key)
        : blockData(std::make_unique<ChunkBlockData>()), key(key)
    {
    }

    // non copyable
    ChunkOld(const ChunkOld&) = delete;
    ChunkOld& operator=(const ChunkOld&) = delete;

    // move constructor
    ChunkOld(ChunkOld&& other) noexcept
        : blockData(std::move(other.blockData))
    {
    }

    // move assignment
    ChunkOld& operator=(ChunkOld&& other) noexcept
    {
        if (this != &other)
        {
            blockData = std::move(other.blockData);
        }
        return *this;
    }

    ChunkBlockData* GetBlockData() const { return blockData.get(); }
    
    // --- chunk ops ---
    inline BlockState GetBlockState(ChunkBlockIndex index) const;
    VOXELCORE_API inline BlockType GetBlockType(ChunkBlockIndex index) const;
    inline BlockMetadata GetBlockMetadata(ChunkBlockIndex index) const;
    inline BlockLightLevels GetBlockLightLevel(ChunkBlockIndex index) const;
    inline BlockLightLevels GetBlockSkyLight(ChunkBlockIndex index) const;

    inline void SetBlockState(ChunkBlockIndex index, BlockState state);
    inline void SetBlockType(ChunkBlockIndex index, BlockType blockType);
    inline void SetBlockMetadata(ChunkBlockIndex index, BlockMetadata metadata);
    inline void SetBlockLightLevel(ChunkBlockIndex index, BlockLightLevels lightLevel);
    inline void SetBlockSkyLight(ChunkBlockIndex index, BlockLightLevels lightLevel);

    VOXELCORE_API inline bool IsAirBlock(ChunkBlockIndex index) const;
    VOXELCORE_API inline bool IsWaterBlock(ChunkBlockIndex index) const;

    VOXELCORE_API int GetHighestBlockHeightAt(ChunkBlockIndex index) const;

private:
    std::unique_ptr<ChunkBlockData> blockData;     
    ChunkKey key;
};

inline void ChunkOld::SetBlockType(ChunkBlockIndex index, BlockType blockType)
{
    // break if index is out of bounds
    // todo: this needs to be a check macro or something that gets removed in test/shipping .
    if (index < 0 || index >= ChunkSize)
    {
        PLATFORM_BREAK();
    }

    blockData->blockStates[index] = (blockData->blockStates[index] & 0xF000) | (blockType & 0x0FFF);
}
