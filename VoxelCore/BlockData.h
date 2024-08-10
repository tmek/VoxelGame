#pragma once
#include "VoxelDefines.h"

// trying to keep this as clean and simple as possible.
// Data-oriented design: Struct of Arrays instead of Array of Structs for cache efficiency. 

struct BlockData
{
    BlockState blockStates[CHUNK_TOTAL_BLOCKS];
    BlockLightLevel lightLevels[CHUNK_TOTAL_BLOCKS];
};
