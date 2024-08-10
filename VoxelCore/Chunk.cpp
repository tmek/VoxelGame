#include "Chunk.h"



void Chunk::SetBlockState(BlockIndex index, uint16_t state)
{
    blockStates[index] = state;
}

uint16_t Chunk::GetBlockState(BlockIndex index) const
{
    return blockStates[index];
}

uint16_t Chunk::GetBlockType(BlockIndex index) const
{
    return blockStates[index] & 0x0FFF;
}

void Chunk::SetBlockMetadata(BlockIndex index, uint8_t metadata)
{
    blockStates[index] = (blockStates[index] & 0x0FFF) | ((metadata & 0x0F) << 12);
}

uint8_t Chunk::GetBlockMetadata(BlockIndex index) const
{
    return (blockStates[index] >> 12) & 0x0F;
}

void Chunk::SetBlockLightLevel(BlockIndex index, uint8_t light)
{
    lightLevels[index] = (lightLevels[index] & 0xF0) | (light & 0x0F);
}

uint8_t Chunk::GetBlockLightLevel(BlockIndex index) const
{
    return lightLevels[index] & 0x0F;
}

void Chunk::SetBlockSkyLight(BlockIndex index, uint8_t light)
{
    lightLevels[index] = (lightLevels[index] & 0x0F) | ((light & 0x0F) << 4);
}

uint8_t Chunk::GetBlockSkyLight(BlockIndex index) const
{
    return (lightLevels[index] >> 4) & 0x0F;
}

bool Chunk::IsAirBlock(BlockIndex index) const
{
    // todo: IsAirBlock should only be public from the world class.
    // because we don't want to have to create a new chunk just to check if a block is air. (bad performance)
    // (if there's no chunk, it's air.)
    
    return GetBlockType(index) == 0;
}

int Chunk::GetHighestBlockHeightAt(BlockIndex index) const // todo: this should accept a LocalBlock (maybe) 
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
            int y = index / CHUNK_XZ_LAYER_SIZE;
            return y;
        }
        
        index -= CHUNK_XZ_LAYER_SIZE;
    }

    return 0;    
}