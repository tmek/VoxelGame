#include "ChunkMeshManager.h"

#include "Render/ChunkMeshBuilder.h"

Mesh* ChunkMeshManager::GetChunkMesh(const ChunkKey& key)
{
    // get mesh from unordered_map
    std::lock_guard<std::mutex> lock(chunkMeshesMutex); // Lock the mutex
    auto it = ChunkMeshes.find(key);
    if (it != ChunkMeshes.end())
    {
        return &it->second;
    }
    else
    {
        return nullptr;
    }
}


void ChunkMeshManager::RebuildChunkMesh(const ChunkKey& key, const Chunk& chunk, ID3D11Device* device)
{
    Mesh newMesh = ChunkMeshBuilder::Build(key, chunk, device);
    
    // store the mesh in the unordered_map
    std::lock_guard<std::mutex> lock(chunkMeshesMutex); // Lock the mutex
    ChunkMeshes[key] = std::move(newMesh);
}

ChunkMeshManager& ChunkMeshManager::GetInstance()
{
    static ChunkMeshManager instance;
    return instance;
}
