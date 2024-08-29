// Copyright

#pragma once

#include "CoreTypes.h"



/** Chunk dimensions */
constexpr int32 ChunkWidth =  16;  // X 
constexpr int32 ChunkHeight = 384; // Y 
constexpr int32 ChunkDepth =  16;  // Z

// Verify chunk width and depth are powers of 2
#define IS_POWER_OF_TWO(x) (((x) > 0) && ((x) & ((x) - 1)) == 0)
static_assert(IS_POWER_OF_TWO(ChunkWidth), "ChunkWidth must be a power of 2");
static_assert(IS_POWER_OF_TWO(ChunkDepth), "ChunkDepth must be a power of 2");
/** Chunk sizes */
constexpr int32 ChunkRowSize = ChunkWidth;
constexpr int32 ChunkLayerSize = ChunkWidth * ChunkDepth;   // Number of blocks in a layer (X * Z) 
constexpr int32 ChunkSize = ChunkLayerSize * ChunkHeight;   // Total number of blocks in a chunk (X * Y * Z)

// Log2 sizes (for using bit manipulation instead of division and modulo)  
constexpr int32 ChunkLayerSizeLog2 = 8; // log2(ChunkLayerSize);
constexpr int32 ChunkWidthLog2 = 4;   // log2(ChunkWidth);
constexpr int32 ChunkDepthLog2 = 4;   // log2(ChunkDepth);

/** World limits */
constexpr int32 MaxPlayerWorldPosition = 30000000;
constexpr int32 MinPlayerWorldPosition = -MaxPlayerWorldPosition;
constexpr int32 MaxWorldGenerationDistance =  MaxPlayerWorldPosition + 128 * ChunkWidth;
constexpr int32 MinWorldGenerationDistance =  MinPlayerWorldPosition - 128 * ChunkWidth;
constexpr int32 WorldChunkMax = MaxWorldGenerationDistance / ChunkWidth;
constexpr int32 WorldChunkMin = MinWorldGenerationDistance / ChunkWidth;
/** Utility */
constexpr int32 MaxBlockX = ChunkWidth - 1;     // Maximum X of right most block of chunk
constexpr int32 MaxBlockY = ChunkHeight - 1;    // Maximum Y of topmost block layer in a chunk
constexpr int32 MaxBlockZ = ChunkDepth - 1;     // Maximum Z of far most block of chunk
constexpr int32 MinBlockX = 0;
constexpr int32 MinBlockY = 0;
constexpr int32 MinBlockZ = 0;                  

