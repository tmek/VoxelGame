#pragma once
#include "ChunkKey.h"
#include "VoxelDefines.h"

// A block's coordinates in the world
// Integer coordinates are used to represent blocks because they are discrete.
struct WorldBlockCoord
{
    int X;
    int Y;
    int Z;
};

// A block's index into a chunk's BlockStates and LightLevels arrays.
typedef int BlockIndex;

// A block's local coordinates within its chunk as well as the chunk's key.
struct LocalBlockCoord
{
    ChunkKey chunkKey;
    BlockIndex Index;  // blocks are stored in XZY order (X increases fastest, Y increases slowest)

    // we need to compute these to compute the index, so storing them, but generally not used.
    // todo: consider removing these and just computing them when needed, technically they can get out of sync with the index
    int X;
    int Y;
    int Z;
};


// A 3D region of blocks in the world
struct WorldRegion
{
    WorldBlockCoord Min;
    WorldBlockCoord Max;
};