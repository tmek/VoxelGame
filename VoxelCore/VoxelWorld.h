#pragma once
#include "Chunk.h"
#include "ChunkKey.h"
#include "VoxelDefines.h"

#include <unordered_map>

class VOXELCORE_API VoxelWorld {
private:
    // --- begin data ---
#pragma warning(push)
#pragma warning(disable: 4251)
    std::unordered_map<ChunkKey, Chunk, ChunkKeyHash> Chunks;
#pragma warning(pop)
    //std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash> Chunks;
    // --- end data ---

public:
    // construct
    VoxelWorld() = default;

    // copy
    VoxelWorld(const VoxelWorld&) = delete;
    VoxelWorld& operator=(const VoxelWorld&) = delete;

    // move
    VoxelWorld(VoxelWorld&&) = default;
    VoxelWorld& operator=(VoxelWorld&&) = default;

    // destruct
    ~VoxelWorld() = default; // Destructor
    
public:
    // Get a chunk or add it if it doesn't exist
    Chunk& GetChunk(const ChunkKey& key);

    // Get a chunk or return nullptr if it doesn't exist
    Chunk* TryGetChunk(const ChunkKey& key);

    // Check if a chunk exists
    bool ChunkExists(const ChunkKey key) const;

    //Iterators for the chunks
    auto begin() { return Chunks.begin(); }
    auto end() { return Chunks.end(); }
    auto begin() const { return Chunks.begin(); }
    auto end() const { return Chunks.end(); }

    auto GetChunkCount() const
    //{ return Chunks.size(); }
    { return 0; }
};
