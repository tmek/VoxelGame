#include "World/VoxelWorld.h"
#include "Chunk/Chunk.h"
#include "Chunk/ChunkKey.h"



ChunkRef VoxelWorld::GetChunk(const ChunkKey& ChunkKey)
{
    std::lock_guard<std::mutex> lock(chunksMutex);
    
    auto it = Chunks.find(ChunkKey);
    
    if (it == Chunks.end()) {
        // Allocate a new chunk and add it to the map.
        auto MyChunk = LoadOrGenerateChunk(ChunkKey);
        return Chunks.emplace(ChunkKey, MyChunk).first->second;
    }
    
    return it->second;
}

ChunkPtr VoxelWorld::LoadOrGenerateChunk(const ChunkKey& key)
{
    // allocate chunk
    auto NewChunk = std::make_shared<Chunk>();

    // Load or generate the chunk data
    

    return NewChunk; // eventually we want to return ConstChunkPtr, but this for now.
}

ChunkPtr VoxelWorld::TryGetChunk(const ChunkKey& key)
{
    std::lock_guard<std::mutex> lock(chunksMutex);
    
    auto it = Chunks.find(key);
    
    if (it == Chunks.end()) {
        return nullptr;
    }

    return it->second;
}

bool VoxelWorld::ChunkExists(const ChunkKey key) const
{
    std::lock_guard<std::mutex> lock(chunksMutex);
    
    return Chunks.find(key) != Chunks.end();
}

void VoxelWorld::RemoveChunksOutsideRadius(ChunkKey chunk_key, int chunks_wide)
{
    std::lock_guard<std::mutex> lock(chunksMutex);

    // Remove chunks outside the radius
    for (auto it = Chunks.begin(); it != Chunks.end();)
    {
        if (abs(it->first.X - chunk_key.X) > chunks_wide ||
            abs(it->first.Z - chunk_key.Z) > chunks_wide)
        {
            it = Chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
