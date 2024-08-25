// Copyright

#pragma once

#include "Chunk/Chunk.h"

#include "Misc/AssertionMacros.h"


/* A cluster of chunks surrounding a center chunk.
 * Used for efficient access of a chunks blocks and the blocks immediately surrounding it. */
class ChunkCluster
{
    // Pointer to the center chunk
    Chunk& CenterChunk;

    // Neighboring Chunks (including diagonals)
    ChunkPtr LeftNeighbor = nullptr;
    ChunkPtr RightNeighbor = nullptr;
    ChunkPtr FrontNeighbor = nullptr;
    ChunkPtr BackNeighbor = nullptr;
    ChunkPtr FrontLeftNeighbor = nullptr;
    ChunkPtr FrontRightNeighbor = nullptr;
    ChunkPtr BackLeftNeighbor = nullptr;
    ChunkPtr BackRightNeighbor = nullptr;

public:
    // Constructor
    inline ChunkCluster(Chunk& center) : CenterChunk(center)
    {
    }

    // Method to cache/set neighbor chunks
    inline void SetNeighbor(ChunkPtr neighbor, int offsetX, int offsetZ);

    // Method to retrieve a block within the cluster at a given block offset relative to the center chunk. 
    inline BlockRef GetBlockFromCluster(int x, int y, int z) const;
};

inline BlockRef ChunkCluster::GetBlockFromCluster(int x, int y, int z) const
{
    //check(y >= 0 && y < ChunkHeight);

    // return empty block (AIR) if y is out of bounds
    if(y < 0 || y >= ChunkHeight)
    {
        return const_cast<Block&>(Block::Empty);
    }

    // coordinates within the center chunk?
    if (x >= 0 && x < ChunkWidth && z >= 0 && z < ChunkDepth)
    {
        return CenterChunk.Blocks[x + z * ChunkWidth + y * ChunkLayerSize];
    }

    // Determine which neighbor chunk to query
    ChunkPtr targetChunk = nullptr; 

    if (x < 0 && z >= 0 && z < ChunkDepth)
    {
        targetChunk = LeftNeighbor;
        x += ChunkWidth; // Adjust to local chunk space
    }
    else if (x >= ChunkWidth && z >= 0 && z < ChunkDepth)
    {
        targetChunk = RightNeighbor;
        x -= ChunkWidth;
    }
    else if (z < 0 && x >= 0 && x < ChunkWidth)
    {
        targetChunk = BackNeighbor;
        z += ChunkDepth;
    }
    else if (z >= ChunkDepth && x >= 0 && x < ChunkWidth)
    {
        targetChunk = FrontNeighbor;
        z -= ChunkDepth;
    }
    else if (x < 0 && z >= ChunkDepth)
    {
        targetChunk = FrontLeftNeighbor;
        x += ChunkWidth;
        z -= ChunkDepth;
    }
    else if (x >= ChunkWidth && z >= ChunkDepth)
    {
        targetChunk = FrontRightNeighbor;
        x -= ChunkWidth;
        z -= ChunkDepth;
    }
    else if (x < 0 && z < 0)
    {
        targetChunk = BackLeftNeighbor;
        x += ChunkWidth;
        z += ChunkDepth;
    }
    else if (x >= ChunkWidth && z < 0)
    {
        targetChunk = BackRightNeighbor;
        x -= ChunkWidth;
        z += ChunkDepth;
    }

    // check we haven't accidentally gone outside the cluster's bounds
    check(x >= -ChunkWidth && x < 2 * ChunkWidth);
    check(z >= -ChunkDepth && z < 2 * ChunkDepth);
    
    // Return empty block (AIR) if the neighbor chunk doesn't exist
    if (targetChunk == nullptr)
    {
        return const_cast<Block&>(Block::Empty);
    }

    // Retrieve the block from the appropriate chunk
    BlockIndex index = x + z * ChunkWidth + y * ChunkLayerSize;
    return targetChunk->Blocks[index];
}

void ChunkCluster::SetNeighbor(ChunkPtr neighbor, int offsetX, int offsetZ)
{
    if (offsetX == -1 && offsetZ == 0) LeftNeighbor = neighbor;
    else if (offsetX == 1 && offsetZ == 0) RightNeighbor = neighbor;
    else if (offsetX == 0 && offsetZ == -1) BackNeighbor = neighbor;
    else if (offsetX == 0 && offsetZ == 1) FrontNeighbor = neighbor;
    else if (offsetX == -1 && offsetZ == 1) FrontLeftNeighbor = neighbor;
    else if (offsetX == 1 && offsetZ == 1) FrontRightNeighbor = neighbor;
    else if (offsetX == -1 && offsetZ == -1) BackLeftNeighbor = neighbor;
    else if (offsetX == 1 && offsetZ == -1) BackRightNeighbor = neighbor;
}