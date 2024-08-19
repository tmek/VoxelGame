// Copyright

#pragma once

#include "Chunk/Chunk.h"
#include "Chunk/ChunkKey.h"
#include "Chunk/ChunkMap.h"


#include <mutex>

/*
    todo: Need to think more about const for Chunks and ChunkMap.
    
    This class would like shared pointers to const chunks.
    but the ChunkMap currently stores shared pointers to non-const chunks.
    Ideally each chunk should be created as a temporary, be generated, and finally to map as const.

    I guess the "world" can be responsible for loading or generating chunks and adding them to its ChunkMap.

    For now I'll just leave them non-const until it builds and runs again.
*/

class VoxelWorld
{
public:
    // construct
    VOXELCORE_API VoxelWorld() = default;

    // copy
    VoxelWorld(const VoxelWorld&) = delete;
    VoxelWorld& operator=(const VoxelWorld&) = delete;
    
    // move
    VOXELCORE_API VoxelWorld(VoxelWorld&&) = default;
    VOXELCORE_API VoxelWorld& operator=(VoxelWorld&&) = default;

    // destruct
    VOXELCORE_API ~VoxelWorld() = default; // Destructor

public:
    // Get a chunk or loads/generates it if it doesn't exist
    VOXELCORE_API ChunkRef GetChunk(const ChunkKey& ChunkKey);

    ChunkPtr LoadOrGenerateChunk(const ChunkKey& key); 

    
    // Get a chunk or return nullptr if it doesn't exist
    VOXELCORE_API ChunkPtr TryGetChunk(const ChunkKey& key);

    // Check if a chunk exists
    VOXELCORE_API bool ChunkExists(const ChunkKey key) const;

    //Iterators for the chunks
    VOXELCORE_API auto begin() { return Chunks.begin(); }
    VOXELCORE_API auto end() { return Chunks.end(); }
    VOXELCORE_API auto begin() const { return Chunks.begin(); }
    VOXELCORE_API auto end() const { return Chunks.end(); }

    VOXELCORE_API size_t GetChunkCount() const
    {
        return Chunks.size();
    }

    VOXELCORE_API void RemoveChunksOutsideRadius(ChunkKey chunk_key, int chunks_wide);

    VOXELCORE_API void Clear()
    {
        std::lock_guard<std::mutex> lock(chunksMutex);
        Chunks.clear();
    }


private:
    ChunkMap Chunks;
    mutable std::mutex chunksMutex; // to protect chunk access
};
