// Copyright

#pragma once

#include "CoreTypes.h"
#include "Math/IntVector.h"


/** Represents a unique index of a block within a chunk. */
using BlockIndex = uint32;

/** Represents a 3D coordinate of a block, either in world or chunk space. */   
using BlockCoordinate = IntVector;

/** Defines a region of blocks in world or chunk space, bounded by Min and Max coordinates. */
struct BlockRegion
{
    BlockCoordinate Min;
    BlockCoordinate Max;
};


// could use the following subtypes to distinguish between world and chunk space

// Represents a block's position in world space
using WorldBlockCoordinate = BlockCoordinate;

// Represents a block's position in chunk space
using ChunkBlockCoordinate = BlockCoordinate;
