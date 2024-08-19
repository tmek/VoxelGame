#pragma once

#include "RHI/Mesh.h"

struct ID3D11Device;
struct Chunk;
struct ChunkKey;

class ChunkMeshBuilder
{
public:
    static Mesh Build(const ChunkKey& chunkKey, Chunk& chunk, ID3D11Device* device);
};
