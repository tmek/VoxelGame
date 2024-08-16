#pragma once
#include <mutex>

#include "Chunk.h"
#include "ChunkKey.h"
#include "VoxelDefines.h"

#include <unordered_map>

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
    // Get a chunk or add it if it doesn't exist
    VOXELCORE_API Chunk& GetChunk(const ChunkKey& key);

    // Get a chunk or return nullptr if it doesn't exist
    VOXELCORE_API Chunk* TryGetChunk(const ChunkKey& key);

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
    std::unordered_map<ChunkKey, Chunk, ChunkKeyHash> Chunks;
    mutable std::mutex chunksMutex; // to protect chunk access
};
