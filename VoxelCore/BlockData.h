#pragma once
#include "VoxelDefines.h"

struct BlockData
{
    // Struct of Arrays instead of Array of Structs for cache efficiency.
    BlockState blockStates[CHUNK_TOTAL_BLOCKS];
    BlockLightLevel lightLevels[CHUNK_TOTAL_BLOCKS];
};
