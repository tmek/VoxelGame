#pragma once
#include <cstdint>

constexpr int32_t CHUNK_SIZE_X = 16; // Width of a chunk
constexpr int32_t CHUNK_SIZE_Y = 384; // Height of a chunk
constexpr int32_t CHUNK_SIZE_Z = 16; // Depth of a chunk
constexpr int32_t CHUNK_XZ_LAYER_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Z;
constexpr int32_t CHUNK_TOTAL_BLOCKS = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z; // Total blocks in a chunk

typedef unsigned short BlockState;
typedef unsigned short BlockType;
typedef unsigned char BlockMetadata;
typedef unsigned char BlockLightLevel;

#ifndef check
#define check(expr) if(!(expr)) __debugbreak()
#endif

#ifdef CPPDYNAMICLIBRARYTEMPLATE_EXPORTS
#define VOXELCORE_API __declspec(dllexport)
#else
#define VOXELCORE_API __declspec(dllimport)
#endif

