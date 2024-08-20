// Copyright

#pragma once

#include "Block/Block.h"
#include "Chunk/ChunkConstants.h"
#include <memory>

/** A fixed-size 3D 'chunk' of blocks at a particular location in the world. */
struct alignas(64) VOXELCORE_API Chunk final // cache-line aligned
{
    Block Blocks[ChunkSize];
};


/** Shared pointer to a chunk */
using ChunkRef = std::shared_ptr<Chunk>; // todo: need an actual shared_ref<T> type like Unreal's TSharedRef<T>
using ChunkPtr = std::shared_ptr<Chunk>;
using ChunkConstPtr = std::shared_ptr<const Chunk>;
using ChunkConstRef = std::shared_ptr<const Chunk>;

