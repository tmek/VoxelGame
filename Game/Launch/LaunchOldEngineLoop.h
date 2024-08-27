#pragma once

#include <vector>

#include "CoreTypes.h"
#include "IEngineLoop.h"

#include "Chunk/Chunk.h"
#include "Chunk/ChunkKey.h"

template <typename T>
struct TMatrix;
using Matrix = TMatrix<float>;

template <typename T>
using TArray = std::vector<T>;


class FOldEngineLoop : public IEngineLoop
{
public:

    int32 Init() override;

    void InitializeGraphics();

    void GenerateChunksAroundCamera(const int RadiusInChunks);

    void GenerateChunksInBackground(const TArray<ChunkKey>& ChunkKeys);

    //void GenerateChunk(const ChunkKey Key, ChunkRef Chunk);
    
public:
    
    void Tick() override;

    void HandleInput(double DeltaTime);

    void UpdateCamera(double DeltaTime);
    
    Matrix CreateViewMatrix();

    Matrix CreateProjectionMatrix();

    void UpdateChunkVisibility(const Matrix& ViewMatrix, const Matrix& ProjectionMatrix);
    
    void DrawChunks(const Matrix& ViewMatrix, const Matrix& ProjectionMatrix, const bool bDrawWater);

    void UpdateWindowTitle(const double DeltaTime);

public:

    void Exit();
    
    void ClearPendingCleanupObjects() override
    {
    };

private:

// determine if we're in debug mode through #ifdef
#if _DEBUG
    bool bSmallWorld = true; 
#else
    bool bSmallWorld = false;
#endif
};

extern FOldEngineLoop GOldEngineLoop;
