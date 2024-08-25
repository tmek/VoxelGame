// // Copyright (c) 2024 Thomas. All rights reserved.

#pragma once

#include "Chunk\ChunkKey.h"
#include "Chunk\Chunk.h"
#include "World\VoxelWorld.h"
#include "Camera.h"

class VoxelWorldRenderer
{
public:
    // Initializes the renderer.
    void Initialize();

    // Creates a chunk mesh based on the given ChunkKey and Chunk.
    void CreateChunkMesh(const ChunkKey& Key, const Chunk& Chunk);

    // Deletes a chunk mesh based on the given ChunkKey.
    void DeleteChunkMesh(const ChunkKey& Key);

    // Renders the entire scene using the VoxelWorld and Camera.
    void RenderScene(const VoxelWorld& World, const Camera& Camera);

private:
    // Updates the visibility of objects in the scene.
    void UpdateVisibility();

    // Begins a new frame, typically involves clearing the device.
    void BeginFrame();

    // Sets the current camera, updating the constant buffer.
    void SetCamera(const Camera& camera);

    // Draws all chunks, first opaque then translucent meshes.
    void DrawChunks();

    // Ends the current frame, typically involves presenting the device.
    void EndFrame();
};
