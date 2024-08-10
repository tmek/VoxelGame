#include "ChunkMeshManager.h"

#include "Render/ChunkMeshBuilder.h"

Mesh* ChunkMeshManager::GetChunkMesh(const ChunkKey& key)
{
    // get mesh from unordered_map
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
    // store the mesh in the unordered_map
    ChunkMeshes[key] = ChunkMeshBuilder::Build(key, chunk, device);
}

ChunkMeshManager& ChunkMeshManager::GetInstance()
{
    static ChunkMeshManager instance;
    return instance;
}
