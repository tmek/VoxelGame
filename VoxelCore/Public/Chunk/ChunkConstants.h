// Copyright

#pragma once

#include "Voxel/VoxelTypes.h"


/** Chunk dimensions */
constexpr int32 ChunkWidth =  16;  // X 
constexpr int32 ChunkHeight = 384; // Y 
constexpr int32 ChunkDepth =  16;  // Z

/** Chunk sizes */
constexpr int32 ChunkLayerSize = ChunkWidth * ChunkDepth;   // Number of blocks in a layer (X * Z) 
constexpr int32 ChunkSize = ChunkLayerSize * ChunkHeight;   // Total number of blocks in a chunk (X * Y * Z)

/** World limits */
constexpr int32 MaxPlayerWorldPosition = 30000000;
constexpr int32 MinPlayerWorldPosition = -MaxPlayerWorldPosition;
constexpr int32 MaxWorldGenerationDistance =  MaxPlayerWorldPosition + 128 * ChunkWidth;
constexpr int32 MinWorldGenerationDistance =  MinPlayerWorldPosition - 128 * ChunkWidth;
constexpr int32 WorldChunkMax = MaxWorldGenerationDistance / ChunkWidth;
constexpr int32 WorldChunkMin = MinWorldGenerationDistance / ChunkWidth;
/** Utility */
constexpr int32 MaxBlockHeight = ChunkHeight - 1;           // Maximum Y height of the topmost block layer in a chunk
constexpr int32 MinBlockHeight = 0;                         // Minimum Y height of the bottommost block layer in a chunk

