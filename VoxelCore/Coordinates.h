#pragma once
#include "ChunkKey.h"
#include "VoxelDefines.h"

struct WorldBlockCoord;

// A block's index into a BlockStates and LightLevels arrays.
// Blocks are stored in XZY order (when iterating linearly X increases fastest, Y increases slowest).
// todo: this may have been a mistake lol, because light and high checks scan vertically and are common.
//       so checking one block up or down means skipping 256 blocks in the cache. :(|
//       so maybe YZX order would be better.
typedef int BlockIndex;

// A block's local coordinates within its chunk as well as the chunk's key.
struct LocalBlockCoord
{
    ChunkKey chunkKey;
    BlockIndex Index;  

    // todo: it's possible these and index can be set out of sync. need them for now.
    int X;
    int Y;
    int Z;
};

// A 3D region of blocks in a chunk
struct ChunkRegion
{
    LocalBlockCoord startingBlock; 
    int width; 
    int height;
    int depth; 
};

// A block's coordinates in the world. Integers used because blocks are discrete.
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