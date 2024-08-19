#include "ChunkMeshManager.h"

#include "Render/ChunkMeshBuilder.h"

extern bool GIsRequestingExit;

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


void ChunkMeshManager::RebuildChunkMesh(const ChunkKey& key, const ChunkOld& chunk, ID3D11Device* device)
{
    if (!device || GIsRequestingExit)
    {
        return;
    }

    Mesh newMesh = ChunkMeshBuilder::Build(key, chunk, device);

    // lock the unordered_map and store the mesh
    std::lock_guard<std::mutex> lock(chunkMeshesMutex); // Lock the mutex

    if(newMesh.vertexBuffer == nullptr) // todo: need a better way to check if mesh is valid.
    {
        return;
    }
    
    ChunkMeshes[key] = std::move(newMesh);
}

ChunkMeshManager& ChunkMeshManager::GetInstance()
{
    static ChunkMeshManager instance;
    return instance;
}
