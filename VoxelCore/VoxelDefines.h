#pragma once
#include <cstdint>
#include <intrin.h>

constexpr int32_t CHUNK_SIZE_X = 16; // Width of a chunk
constexpr int32_t CHUNK_SIZE_Y = 384; // Height of a chunk
constexpr int32_t CHUNK_SIZE_Z = 16; // Depth of a chunk
constexpr int32_t CHUNK_XZ_LAYER_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Z;
constexpr int32_t CHUNK_TOTAL_BLOCKS = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z; // Total blocks in a chunk

typedef unsigned short BlockState;
typedef unsigned short BlockType;
typedef unsigned char BlockMetadata;
typedef unsigned char BlockLightLevel;


#ifdef CPPDYNAMICLIBRARYTEMPLATE_EXPORTS
#define VOXELCORE_API __declspec(dllexport)
#else
#define VOXELCORE_API __declspec(dllimport)
#endif


// Core module stuff below:
// todo: need to extract "Core" from "Game\Core" into its own module and move this there.

// Q: Why is there a __nop() before __debugbreak()?
// A: VS' debug engine has a bug where it will silently swallow explicit
// breakpoint interrupts when single-step debugging either line-by-line or
// over call instructions. This can hide legitimate reasons to trap. Asserts
// for example, which can appear as if the did not fire, leaving a programmer
// unknowingly debugging an undefined process.
#define PLATFORM_BREAK() (__nop(), __debugbreak())

#ifndef check
#define check(expr) if(!(expr)) PLATFORM_BREAK()
#endif