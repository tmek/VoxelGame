// copyright

#pragma once

#include "VoxelTypes.h"

#include "Misc/AssertionMacros.h"

/** Using the following naming conventions:
 *  - WorldPosition: A block coordinate explicitly in world space.
 *  - BlockOffset: A block coordinate explicitly in chunk space.
 *  - BlockIndex: A block's index in a chunk.
 *  - ChunkKey: A chunk's cx,cz coordinate in the world's chunk grid.
 */
#define CheckBlockIndex(BlockIndex) check((BlockIndex) < ChunkSize && (BlockIndex) >= 0)

#define CheckWorldPosition(BlockPosition) \
check((BlockPosition).X >= MinPlayerWorldPosition && (BlockPosition).X < MaxPlayerWorldPosition); \
check((BlockPosition).Y >= 0 && (BlockPosition).Y < ChunkHeight); \
check((BlockPosition).Z >= MinPlayerWorldPosition && (BlockPosition).Z < MaxPlayerWorldPosition)

#define CheckBlockXYZ(X,Y,Z) \
check((X) >= 0 && (X) < ChunkWidth); \
check((Y) >= 0 && (Y) < ChunkHeight); \
check((Z) >= 0 && (Z) < ChunkDepth)

#define CheckBlockOffset(BlockOffset) CheckBlockXYZ((BlockOffset).X, (BlockOffset).Y, (BlockOffset).Z)

#define CheckChunkKey(ChunkKey) \
check((ChunkKey).X >= WorldChunkMin && (ChunkKey).X < WorldChunkMax); \
check((ChunkKey).Z >= WorldChunkMin && (ChunkKey).Z < WorldChunkMax)

#define IsBlockInChunk(X, Y, Z) \
     ((X) >= 0 && (X) < ChunkWidth && \
      (Y) >= 0 && (Y) < ChunkHeight && \
      (Z) >= 0 && (Z) < ChunkDepth)
