#pragma once
#include <mutex>
#include <unordered_map>

#include "Chunk/Chunk.h"

#include "RHI/Mesh.h"
#include "Chunk/ChunkKey.h"
#include "Chunk/ChunkKeyHash.h"

struct Chunk;

struct ID3D11Device;

class ChunkMeshManager
{
public:
    Mesh* GetChunkMesh(const ChunkKey& key);

    void RebuildChunkMesh(const ChunkKey& key, ChunkRef, ID3D11Device* device);

    static ChunkMeshManager& GetInstance();
    
private:
    // todo: need memory management to delete meshes that are no longer needed
    std::unordered_map<ChunkKey, Mesh, ChunkKeyHash> ChunkMeshes;
    mutable std::mutex chunkMeshesMutex; // protect access to ChunkMeshes
};
