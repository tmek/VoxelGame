// Copyright

#pragma once

#include "Block/Block.h"
#include "Chunk/ChunkConstants.h"
#include <memory>

/** A 3D array of blocks */
struct alignas(64) VOXELCORE_API Chunk final // cache-line aligned
{
    Block Blocks[ChunkSize];
};


/** Shared pointer to a chunk */
using ChunkRef = std::shared_ptr<Chunk>;
using ChunkConstRef = std::shared_ptr<const Chunk>;

