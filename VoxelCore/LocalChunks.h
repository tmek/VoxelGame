#pragma once
#include "Chunk.h"

class LocalChunks {
public:
    // Pointer to the center chunk
    const Chunk* centerChunk;

    // Neighboring Chunks (including diagonals)
    const Chunk* leftNeighbor = nullptr;
    const Chunk* rightNeighbor = nullptr;
    const Chunk* frontNeighbor = nullptr;
    const Chunk* backNeighbor = nullptr;
    const Chunk* frontLeftNeighbor = nullptr;
    const Chunk* frontRightNeighbor = nullptr;
    const Chunk* backLeftNeighbor = nullptr;
    const Chunk* backRightNeighbor = nullptr;

    // Constructor
    inline LocalChunks(const Chunk* center) : centerChunk(center) {}

    // Method to retrieve a block at a given local position
    inline BlockType GetBlock(int x, int y, int z);

    // Method to cache/set neighbor chunks
    inline void SetNeighbor(Chunk* neighbor, int offsetX, int offsetZ);
};

inline void LocalChunks::SetNeighbor(Chunk* neighbor, int offsetX, int offsetZ) {
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

    if(y<0 || y>=CHUNK_SIZE_Y)
    {
        return 0; // AIR
    }
    
    // Check if coordinates are within the center chunk
    if (x >= 0 && x < CHUNK_SIZE_X && z >= 0 && z < CHUNK_SIZE_Z) {
        BlockIndex index = x + z * CHUNK_SIZE_X + y * CHUNK_XZ_LAYER_SIZE;
        return centerChunk->GetBlockType(index);  // Access within the center chunk
    }

    // Determine which neighbor chunk to query
    const Chunk* targetChunk = centerChunk;

    if (x < 0 && z >= 0 && z < CHUNK_SIZE_Z) {
        targetChunk = leftNeighbor;
        x += CHUNK_SIZE_X;  // Adjust to local chunk space
    } else if (x >= CHUNK_SIZE_X && z >= 0 && z < CHUNK_SIZE_Z) {
        targetChunk = rightNeighbor;
        x -= CHUNK_SIZE_X;
    } else if (z < 0 && x >= 0 && x < CHUNK_SIZE_X) {
        targetChunk = backNeighbor;
        z += CHUNK_SIZE_Z;
    } else if (z >= CHUNK_SIZE_Z && x >= 0 && x < CHUNK_SIZE_X) {
        targetChunk = frontNeighbor;
        z -= CHUNK_SIZE_Z;
    } else if (x < 0 && z >= CHUNK_SIZE_Z) {
        targetChunk = frontLeftNeighbor;
        x += CHUNK_SIZE_X;
        z -= CHUNK_SIZE_Z;
    } else if (x >= CHUNK_SIZE_X && z >= CHUNK_SIZE_Z) {
        targetChunk = frontRightNeighbor;
        x -= CHUNK_SIZE_X;
        z -= CHUNK_SIZE_Z;
    } else if (x < 0 && z < 0) {
        targetChunk = backLeftNeighbor;
        x += CHUNK_SIZE_X;
        z += CHUNK_SIZE_Z;
    } else if (x >= CHUNK_SIZE_X && z < 0) {
        targetChunk = backRightNeighbor;
        x -= CHUNK_SIZE_X;
        z += CHUNK_SIZE_Z;
    }

    // todo: might want to check we haven't gone beyond the neighbor bounds
    

    // Return AIR if the neighbor chunk doesn't exist
    if (targetChunk == nullptr) {
        return 0; // AIR
    }

    // Retrieve the block from the appropriate chunk
    BlockIndex index = x + z * CHUNK_SIZE_X + y * CHUNK_XZ_LAYER_SIZE;
    return targetChunk->GetBlockType(index);
}
