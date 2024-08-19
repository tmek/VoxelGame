// Copyright

#pragma once

#include "Voxel/PrimitiveTypes.h"


/** Chunk dimensions */
constexpr int32 ChunkWidth =  16;  // X 
constexpr int32 ChunkHeight = 384; // Y 
constexpr int32 ChunkDepth =  16;  // Z 

/** Chunk sizes */
constexpr int32 ChunkLayerSize = ChunkWidth * ChunkDepth;   // Number of blocks in a layer (X * Z) 
constexpr int32 ChunkSize = ChunkLayerSize * ChunkHeight;   // Total number of blocks in a chunk (X * Y * Z)

/** Utility */
constexpr int32 MaxBlockHeight = ChunkHeight - 1;           // Maximum Y height of the topmost block layer in a chunk
constexpr int32 MinBlockHeight = 0;                         // Minimum Y height of the bottommost block layer in a chunk

