#pragma once

#include "PrimitiveTypes.h"

// Chunk dimensions
constexpr int32 ChunkWidth =  16;  // X dimension of a chunk
constexpr int32 ChunkHeight = 384; // Y dimension of a chunk
constexpr int32 ChunkDepth =  16;  // Z dimension of a chunk

// Chunk sizes
constexpr int32 ChunkLayerSize = ChunkWidth * ChunkDepth;                // Number of blocks in a layer (X * Z)
constexpr int32 ChunkSize = ChunkWidth * ChunkHeight * ChunkDepth;  // Total number of blocks in a chunk (X * Y * Z)

// Utility
constexpr int32 MaxBlockHeight = ChunkHeight - 1; // Maximum Y height of the topmost block layer in a chunk

