// // Copyright (c) 2024 Thomas. All rights reserved.

#pragma once

#include "Camera.h"
#include "Chunk\Chunk.h"
#include "Chunk\ChunkKey.h"
#include "World\VoxelWorld.h"

#include "Containers/Array.h"

#include "RHI/Mesh.h"

class GraphicsDevice;
class CubeMesh;
struct Mesh;

class VoxelWorldRenderer
{
public:

    // Initializes the renderer.
    void Initialize(void* WindowHandle);

    // Creates a chunk mesh based on the given ChunkKey and Chunk.
    void CreateChunkMesh(const ChunkKey& Key, const ChunkRef Chunk);

    // Deletes a chunk mesh based on the given ChunkKey.
    void DeleteChunkMesh(const ChunkKey& Key);


    // Renders the entire scene using the VoxelWorld and Camera.
    void RenderScene(const VoxelWorld* World, const Camera& Camera);

private:

    // Begins a new frame, typically involves clearing the device.
    void BeginFrame();

    // Updates the visibility of objects in the scene.
    void UpdateVisibility();

    void RenderDepthPrePass();

    void RenderMainPass();

    void RenderOpaqueGeometry();

    void RenderTransparentGeometry();

    // Draws all chunks, first opaque then translucent meshes.
    void DrawChunks(int PassIndex);

    // Ends the current frame, typically involves presenting the device.
    void EndFrame();

private:

    GraphicsDevice* GraphicsDevice_ = nullptr;
    CubeMesh* CubeMesh_ = nullptr;
    Mesh BoxMesh_;
    TArray<ChunkKey> VisibleChunks_;

    const VoxelWorld* CurrentWorld_ = nullptr;
    Camera CurrentCamera_;
};
