#pragma once
#include "VoxelDefines.h"

struct BlockData
{
    BlockState blockStates[CHUNK_TOTAL_BLOCKS];
    BlockLightLevel lightLevels[CHUNK_TOTAL_BLOCKS];
};



