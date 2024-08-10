#include "VoxelWorld.h"

Chunk& VoxelWorld::GetChunk(const ChunkKey& key)
{
    auto it = Chunks.find(key);

    if (it == Chunks.end()) {
        auto emplaceResult = Chunks.emplace(key, Chunk());
        it = emplaceResult.first;
    }

    return it->second;
}

Chunk* VoxelWorld::TryGetChunk(const ChunkKey& key)
{
    auto it = Chunks.find(key);

    if (it == Chunks.end()) {
        return nullptr;
    }

    return &it->second;
}

bool VoxelWorld::ChunkExists(const ChunkKey key) const
{
    return Chunks.find(key) != Chunks.end();
}
