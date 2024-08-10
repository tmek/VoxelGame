#pragma once
#include "ChunkKey.h"
#include "VoxelDefines.h"

struct WorldBlockCoord;


// A block's index into a BlockStates and LightLevels arrays.
typedef int BlockIndex;

// A block's local coordinates within its chunk as well as the chunk's key.
struct LocalBlockCoord
{
    ChunkKey chunkKey;
    BlockIndex Index;  // blocks are stored in XZY order (X increases fastest, Y increases slowest)

    // todo: it's possible these and index can be set out of sync. 
    int X;
    int Y;
    int Z;
};

struct ChunkRegion
{
    LocalBlockCoord startingBlock; // The starting block within the chunk
    int width; // The width of the sub-volume within the chunk
    int height; // The height of the sub-volume within the chunk
    int depth; // The depth of the sub-volume within the chunk
};

// A block's coordinates in the world
// Integers used because blocks are discrete.
struct WorldBlockCoord
{
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