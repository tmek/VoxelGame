#pragma once

#include "Windows/WindowsHWrapper.h"

#include <DirectXMath.h>
#include <vector>

#include "CoreTypes.h"
#include "IEngineLoop.h"

#include "Chunk/Chunk.h"
#include "Chunk/ChunkKey.h"

#include "WorldGen/WorldOperations.h"


struct ChunkOld;
class FOldEngineLoop;

extern FOldEngineLoop GOldEngineLoop;

class FOldEngineLoop : public IEngineLoop
{
public:
    void ClearPendingCleanupObjects() override;

    int32 Init() override;
    
    void DrawChunks(DirectX::XMMATRIX TranslationMatrix, DirectX::XMMATRIX RotationMatrix,
                    DirectX::XMMATRIX ScaleMatrix,
                    DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, bool bDrawWater);
    
    void ComputeChunkVisibility(DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix);
    void Tick() override;
    void Exit();
    void GenerateChunk(ChunkRef Chunk, ChunkKey Key);

    void SetBlocksInFrontOfPlayer();

    void GenerateChunksInBackground(const std::vector<ChunkKey>& ChunkKeys);
    void GenerateChunksAroundCamera(int RadiusInChunks);
    void AddTrees(WorldOperations World, int Size);

    // todo: move this to a world gen class
    void GenerateWorld();

    // todo: move to chunk mesh manager
    //void RebuildChunkMeshes();
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


