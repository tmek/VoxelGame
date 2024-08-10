#pragma once

constexpr int32_t CHUNK_SIZE_X = 16; // Width of a chunk (X dimension)
constexpr int32_t CHUNK_SIZE_Y = 384; // Height of a chunk (Y dimension)
constexpr int32_t CHUNK_SIZE_Z = 16; // Depth of a chunk (Z dimension)
constexpr int32_t CHUNK_XZ_LAYER_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Z; // Number of blocks in one horizontal slice of a chunk
constexpr int32_t CHUNK_TOTAL_BLOCKS = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z; // Total number of blocks in a chunk

#define check(expr) if(!(expr)) __debugbreak()

typedef unsigned short BlockState;
typedef unsigned short BlockType;
typedef unsigned char BlockMetadata;
typedef unsigned char BlockLightLevel;