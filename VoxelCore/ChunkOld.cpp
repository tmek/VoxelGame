#include "ChunkOld.h"

#include "../WorldGen/BlockTypes.h"


void ChunkOld::SetBlockState(ChunkBlockIndex index, uint16_t state)
{
    blockData->blockStates[index] = state;
}

uint16_t ChunkOld::GetBlockState(ChunkBlockIndex index) const
{
    return blockData->blockStates[index];
}

uint16_t ChunkOld::GetBlockType(ChunkBlockIndex index) const
{
    return blockData->blockStates[index] & 0x0FFF;
}

void ChunkOld::SetBlockMetadata(ChunkBlockIndex index, uint8_t metadata)
{
    blockData->blockStates[index] = (blockData->blockStates[index] & 0x0FFF) | ((metadata & 0x0F) << 12);
}

uint8_t ChunkOld::GetBlockMetadata(ChunkBlockIndex index) const
{
    return (blockData->blockStates[index] >> 12) & 0x0F;
}

void ChunkOld::SetBlockLightLevel(ChunkBlockIndex index, uint8_t light)
{
    blockData->lightLevels[index] = (blockData->lightLevels[index] & 0xF0) | (light & 0x0F);
}

uint8_t ChunkOld::GetBlockLightLevel(ChunkBlockIndex index) const
{
    return blockData->lightLevels[index] & 0x0F;
}

void ChunkOld::SetBlockSkyLight(ChunkBlockIndex index, uint8_t light)
{
    blockData->lightLevels[index] = (blockData->lightLevels[index] & 0x0F) | ((light & 0x0F) << 4);
}

uint8_t ChunkOld::GetBlockSkyLight(ChunkBlockIndex index) const
{
    return (blockData->lightLevels[index] >> 4) & 0x0F;
}

bool ChunkOld::IsAirBlock(ChunkBlockIndex index) const
{
    // todo: IsAirBlock should only be public from the world class.
    // because we don't want to have to create a new chunk just to check if a block is air. (bad performance)
    // (if there's no chunk, it's air.)
    
    return GetBlockType(index) == 0;
}

bool ChunkOld::IsWaterBlock(ChunkBlockIndex index) const
{
    // todo: IsAirBlock should only be public from the world class.
    // because we don't want to have to create a new chunk just to check if a block is air. (bad performance)
    // (if there's no chunk, it's air.)
    
    return GetBlockType(index) == 13;
}


int ChunkOld::GetHighestBlockHeightAt(ChunkBlockIndex index) const // todo: this should accept a LocalBlock (maybe) 
{
    // if the block index is invalid (outside chunk), return 0
    if (index == -1)
    {
        return 0;
    }

    // start at the top of the chunk and move down until we find a non-air block or hit the bottom
    while (index > 0)
    {
        //check(BlockIndex >= 0 && BlockIndex < Width * Height * Depth);

        if (!IsAirBlock(index))
        {
            int y = index / ChunkLayerSize;
            return y;
        }
        
        index -= ChunkLayerSize;
    }

    return 0;    
}