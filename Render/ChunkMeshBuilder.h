#pragma once
#include <memory>
#include "RHI/Mesh.h"

struct Chunk;
struct ChunkKey;

class ChunkMeshBuilder
{
public:
    static Mesh Build(const ChunkKey& chunkKey, const Chunk& chunk, ID3D11Device* device);
};
