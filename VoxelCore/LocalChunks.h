#pragma once
#include "ChunkOld.h"

class LocalChunks {
public:
    // Pointer to the center chunk
    const ChunkOld* centerChunk;

    // Neighboring Chunks (including diagonals)
    const ChunkOld* leftNeighbor = nullptr;
    const ChunkOld* rightNeighbor = nullptr;
    const ChunkOld* frontNeighbor = nullptr;
    const ChunkOld* backNeighbor = nullptr;
    const ChunkOld* frontLeftNeighbor = nullptr;
    const ChunkOld* frontRightNeighbor = nullptr;
    const ChunkOld* backLeftNeighbor = nullptr;
    const ChunkOld* backRightNeighbor = nullptr;

    // Constructor
    inline LocalChunks(const ChunkOld* center) : centerChunk(center) {}

    // Method to retrieve a block at a given local position
    inline BlockType GetBlock(int x, int y, int z);

    // Method to cache/set neighbor chunks
    inline void SetNeighbor(ChunkOld* neighbor, int offsetX, int offsetZ);
};

inline void LocalChunks::SetNeighbor(ChunkOld* neighbor, int offsetX, int offsetZ) {
    if (offsetX == -1 && offsetZ == 0) leftNeighbor = neighbor;
    else if (offsetX == 1 && offsetZ == 0) rightNeighbor = neighbor;
    else if (offsetX == 0 && offsetZ == -1) backNeighbor = neighbor;
    else if (offsetX == 0 && offsetZ == 1) frontNeighbor = neighbor;
    else if (offsetX == -1 && offsetZ == 1) frontLeftNeighbor = neighbor;
    else if (offsetX == 1 && offsetZ == 1) frontRightNeighbor = neighbor;
    else if (offsetX == -1 && offsetZ == -1) backLeftNeighbor = neighbor;
    else if (offsetX == 1 && offsetZ == -1) backRightNeighbor = neighbor;
}

inline BlockType LocalChunks::GetBlock(int x, int y, int z) {

    if(y<0 || y>=ChunkHeight)
    {
        return 0; // AIR
    }
    
    // Check if coordinates are within the center chunk
    if (x >= 0 && x < ChunkWidth && z >= 0 && z < ChunkDepth) {
        ChunkBlockIndex index = x + z * ChunkWidth + y * ChunkLayerSize;
        return centerChunk->GetBlockType(index);  // Access within the center chunk
    }

    // Determine which neighbor chunk to query
    const ChunkOld* targetChunk = centerChunk;

    if (x < 0 && z >= 0 && z < ChunkDepth) {
        targetChunk = leftNeighbor;
        x += ChunkWidth;  // Adjust to local chunk space
    } else if (x >= ChunkWidth && z >= 0 && z < ChunkDepth) {
        targetChunk = rightNeighbor;
        x -= ChunkWidth;
    } else if (z < 0 && x >= 0 && x < ChunkWidth) {
        targetChunk = backNeighbor;
        z += ChunkDepth;
    } else if (z >= ChunkDepth && x >= 0 && x < ChunkWidth) {
        targetChunk = frontNeighbor;
        z -= ChunkDepth;
    } else if (x < 0 && z >= ChunkDepth) {
        targetChunk = frontLeftNeighbor;
        x += ChunkWidth;
        z -= ChunkDepth;
    } else if (x >= ChunkWidth && z >= ChunkDepth) {
        targetChunk = frontRightNeighbor;
        x -= ChunkWidth;
        z -= ChunkDepth;
    } else if (x < 0 && z < 0) {
        targetChunk = backLeftNeighbor;
        x += ChunkWidth;
        z += ChunkDepth;
    } else if (x >= ChunkWidth && z < 0) {
        targetChunk = backRightNeighbor;
        x -= ChunkWidth;
        z += ChunkDepth;
    }

    // todo: might want to check we haven't gone beyond the neighbor bounds
    

    // Return AIR if the neighbor chunk doesn't exist
    if (targetChunk == nullptr) {
        return 0; // AIR
    }

    // Retrieve the block from the appropriate chunk
    ChunkBlockIndex index = x + z * ChunkWidth + y * ChunkLayerSize;
    return targetChunk->GetBlockType(index);
}
