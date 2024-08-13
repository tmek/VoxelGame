#pragma once
#include "VoxelDefines.h"
#include "Coordinates.h"
#include "BlockData.h"

#include <memory>

#include "HAL/Platform.h"

struct VOXELCORE_API Chunk
{
private:
#pragma warning(push)
#pragma warning(disable: 4251) // 'Chunk::blockData': 'std::unique_ptr<BlockData,std::default_delete<BlockData>>' needs to have dll-interface to be used by clients of 'Chunk' 
    std::unique_ptr<BlockData> blockData;     
#pragma warning(pop)
    ChunkKey key;
    // --- end data ---
public:
    
    // Constructor
    Chunk(ChunkKey key)
        : blockData(std::make_unique<BlockData>()), key(key)
    {
    }

    // non copyable
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    // move constructor
    Chunk(Chunk&& other) noexcept
        : blockData(std::move(other.blockData))
    {
    }

    // move assignment
    Chunk& operator=(Chunk&& other) noexcept
    {
        if (this != &other)
        {
            blockData = std::move(other.blockData);
        }
        return *this;
    }

    BlockData* GetBlockData() const { return blockData.get(); }
    
    // --- chunk ops ---
    inline BlockState GetBlockState(BlockIndex index) const;
    inline BlockType GetBlockType(BlockIndex index) const;
    inline BlockMetadata GetBlockMetadata(BlockIndex index) const;
    inline BlockLightLevel GetBlockLightLevel(BlockIndex index) const;
    inline BlockLightLevel GetBlockSkyLight(BlockIndex index) const;

    inline void SetBlockState(BlockIndex index, BlockState state);
    inline void SetBlockType(BlockIndex index, BlockType blockType);
    inline void SetBlockMetadata(BlockIndex index, BlockMetadata metadata);
    inline void SetBlockLightLevel(BlockIndex index, BlockLightLevel lightLevel);
    inline void SetBlockSkyLight(BlockIndex index, BlockLightLevel lightLevel);

    inline bool IsAirBlock(BlockIndex index) const;
    inline bool IsWaterBlock(BlockIndex index) const;

    int GetHighestBlockHeightAt(BlockIndex index) const;
};

inline void Chunk::SetBlockType(BlockIndex index, BlockType blockType)
{
    // break if index is out of bounds
    // todo: this needs to be a check macro or something that gets removed in test/shipping .
    if (index < 0 || index >= CHUNK_TOTAL_BLOCKS)
    {
        PLATFORM_BREAK();
    }

    blockData->blockStates[index] = (blockData->blockStates[index] & 0xF000) | (blockType & 0x0FFF);
}
