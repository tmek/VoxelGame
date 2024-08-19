#include "VoxelWorld.h"

ChunkOld& VoxelWorld::GetChunk(const ChunkKey& key)
{
    std::lock_guard<std::mutex> lock(chunksMutex);
    
    auto it = Chunks.find(key);
    
    if (it == Chunks.end()) {
        return Chunks.emplace(key, ChunkOld(key)).first->second;
    }
    
    return it->second;
}

ChunkOld* VoxelWorld::TryGetChunk(const ChunkKey& key)
{
    std::lock_guard<std::mutex> lock(chunksMutex);
    
    auto it = Chunks.find(key);
    
    if (it == Chunks.end()) {
        return nullptr;
    }
    
    return &it->second;
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
