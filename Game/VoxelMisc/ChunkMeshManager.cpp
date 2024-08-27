// copyright

#include "ChunkMeshManager.h"

#include "CoreGlobals.h"
#include "Chunk/Chunk.h"
#include "HAL/PlatformWindow.h"
#include "Renderer/ChunkMeshBuilder.h"



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


void ChunkMeshManager::RebuildChunkMesh(const ChunkKey& key, const ChunkRef chunk, ID3D11Device* device)
{
    if (!device || GIsRequestingExit)
    {
        return;
    }

    Mesh newMesh = ChunkMeshBuilder::Build(key, *chunk, device);

    // lock the unordered_map and store the mesh
    std::lock_guard<std::mutex> lock(chunkMeshesMutex); // Lock the mutex

    if(newMesh.vertexBuffer == nullptr) // todo: need a better way to check if mesh is valid.
    {
        return;
    }
    
    ChunkMeshes[key] = std::move(newMesh);
}

void ChunkMeshManager::DeleteChunkMesh(ChunkKey Key)
{
    // lock the unordered_map and delete the mesh
    std::lock_guard<std::mutex> lock(chunkMeshesMutex); // Lock the mutex
    ChunkMeshes.erase(Key);
}

ChunkMeshManager& ChunkMeshManager::GetInstance()
{
    static ChunkMeshManager instance;
    return instance;
}
