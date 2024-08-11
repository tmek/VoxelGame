#pragma once
#include <DirectXMath.h>

#include "Core/Core.h"
#include "VoxelCore/ChunkUtils.h"
#include "WorldGen/WorldOperations.h"


struct Chunk;

class FEngineLoop
{
public:
    int32 Init();
    void DrawChunks(DirectX::XMMATRIX translationMatrix, DirectX::XMMATRIX rotationMatrix,
                    DirectX::XMMATRIX scaleMatrix,
                    DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, bool bDrawWater);
    void Tick();
    void Exit();

    static void TestBigSphere(WorldBlockCoord Center, int Radius, BlockType BlockType);

    void GeneratePerlinTerrain(int ChunksWide);
    void AddTrees(WorldOperations world, int size);
    // todo: move this to a world gen class
    void GenerateWorld();

    // todo: move to chunk mesh manager
    void RebuildChunkMeshes();
    void AddBlockTick();

    // int RenderChunk(Chunk2& chunk, DirectX::XMMATRIX translationMatrix,
    //                 DirectX::XMMATRIX rotationMatrix,
    //                 DirectX::XMMATRIX scaleMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);

private:

// determine if we're in debug mode through #ifdef
#if _DEBUG
    bool bSmallWorld = true; 
#else
    bool bSmallWorld = false;
#endif
};
