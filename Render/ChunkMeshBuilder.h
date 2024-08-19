#pragma once
#include <memory>
#include "RHI/Mesh.h"

struct ChunkOld;
struct ChunkKey;

class ChunkMeshBuilder
{
public:
    static Mesh Build(const ChunkKey& chunkKey, const ChunkOld& chunk, ID3D11Device* device);
};
