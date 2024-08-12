// Copyright Voxel Games, Inc. All Rights Reserved.

#include "LaunchEngineLoop.h"

#include "Core/Core.h"
#include <cstdio>
#include <DirectXColors.h>

#include "RHI/GraphicsDevice.h"
#include "RHI/Mesh.h"

#include "HAL/PlatformWindow.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"

#include "Render/CubeMesh.h"

#include "Core/FrameTiming.h"
#include "Core/Macros.h"
#include "Core/PlayerController.h"
#include "Core/StopWatch.h"
#include "HAL/Windows/WindowsProcess.h"

#include "Input/InputManager.h"
#include "VoxelCore/ChunkUtils.h"

#include "VoxelCore/VoxelWorld.h"
#include "VoxelMisc/ChunkMeshManager.h"
#include "WorldGen/BlockTypes.h"
#include "WorldGen/ForrestBeachGenerator.h"
#include "WorldGen/PerlinNoise.h"
#include "WorldGen/TerrainGenerators.h"
#include "WorldGen/WorldOperations.h"

#include "WorldGen/MountainsGenerator.h"

extern bool GIsRequestingExit;

PlatformWindow* GWindow = nullptr;
GraphicsDevice* GGraphicsDevice = nullptr;
CubeMesh* GMesh = nullptr;

VoxelWorld GWorld;

double GTime = 0.0;

int32 GFrameNumber = 0;
bool vsync = true;

#if 1
ForestBeachBiomeGenerator biome;
#else
MountainsGenerator biome;
#endif

int WorldChunks = 24;


PlayerController GPlayerController;
DirectX::XMVECTOR GCameraPosition = DirectX::XMVectorSet(0.0f, 10.0f, -10.0f, 0.0f);

bool GShouldRenderTintColor;

int WorldBlockCounter = 0;

void FEngineLoop::GeneratePerlinTerrain(int ChunksWide = 32)
{
    // generate some perlin terrain
    int size = ChunksWide * CHUNK_SIZE_X;
    int startX = -size / 2;
    int startZ = -size / 2;
    int width = size;
    int depth = size;
    float scale = 0.025f;
    float amplitude = 20.0f;
    uint8_t blockType = 3;

    // create chunk subvolumes for the perlin noise terrain
    int maxheight = 384;
    WorldRegion terrainRegion = {
        {startX, 0, startZ}, // min coordinates
        {startX + width, maxheight, startZ + depth} // max coordinates
    };


    auto chunksAndSubVolumes = ChunkVolumeMapper::GetChunksAndSubVolumes(terrainRegion);

    PerlinNoise perlinNoise; // must be initiated once before using

    for (const auto& chunkSubVolume : chunksAndSubVolumes)
    {
        VG_LOG(LOG_CATEGORY_GENERAL, LOG_INFO, "Generating terrain for chunk: %d, %d",
               chunkSubVolume.startingBlock.chunkKey.X, chunkSubVolume.startingBlock.chunkKey.Z);

        const auto& startingBlock = chunkSubVolume.startingBlock;

        WorldBlockCoord startingBlockWorldCoords;
        LocalToWorld(startingBlock, startingBlockWorldCoords);

        // get chunk data
        auto& chunk = GWorld.GetChunk(chunkSubVolume.startingBlock.chunkKey);
        auto chunkData = chunk.GetBlockData()->blockStates;

        ChunkUtils::TFillChunkSubvolume(chunkData,
                                        chunkSubVolume.startingBlock.Index,
                                        chunkSubVolume.width, chunkSubVolume.height, chunkSubVolume.depth,
                                        startingBlockWorldCoords.X, startingBlockWorldCoords.Y,
                                        startingBlockWorldCoords.Z,
                                        [&](auto* block_states, int index, int world_x, int world_y, int world_z)
                                        {
                                            BlockType block_type1 = biome.GetBlock(world_x, world_y, world_z);

                                            if (block_type1 != AIR)
                                            {
                                                block_states[index] = block_type1;
                                            }
                                            return;
                                            // as we get farther from world origin, increase amplitude
                                            float distance = (float)sqrt(world_x * world_x + world_z * world_z);
                                            float distanceFactor = distance / 100.0f;
                                            float newAmplitude = amplitude + amplitude * distanceFactor;

                                            // compute noise value
                                            double noiseValue = perlinNoise.
                                                sample3D(world_x * scale, 0.0, world_z * scale);

                                            // compute ground level
                                            const int ground_level = static_cast<int>(noiseValue * newAmplitude +
                                                amplitude
                                                / 2);

                                            int block_type = 0; // air default
                                            if (world_y == ground_level)
                                            {
                                                block_type = 3;
                                            }
                                            if (world_y < ground_level)
                                            {
                                                block_type = 2;
                                            }
                                            if (world_y < ground_level - 3)
                                            {
                                                // 1% chance of bedrock vs stone
                                                block_type = rand() % 100 == 0 ? 5 : 1;
                                            }

                                            if (world_y < 4)
                                            {
                                                block_type = 4; // water
                                            }

                                            if (world_y == 0)
                                            {
                                                block_type = 5; // bedrock
                                            }

                                            if (world_y <= ground_level)
                                            {
                                                if (world_y > 40)
                                                {
                                                    // random chance of stone the higher we go
                                                    block_type = rand() % 100 < 80 ? 1 : block_type;
                                                }

                                                if (world_y > 50)
                                                {
                                                    // random chance of snow the higher we go
                                                    block_type = rand() % 100 < 80 ? 7 : block_type;
                                                }
                                            }

                                            if (block_type) // don't set air blocks
                                            {
                                                block_states[index] = block_type;
                                            }
                                        });
    }
}

void FEngineLoop::AddTrees(WorldOperations world, int size)
{
    int chunksWide = size / CHUNK_SIZE_X;

    int treesPerChunk = 3;
    int treeCount = treesPerChunk * chunksWide * chunksWide;
    for (int i = 0; i < treeCount; i++)
    {
        // random x, z
        int x = rand() % size - size / 2;
        int z = rand() % size - size / 2;

        int y = world.GetHighestBlockHeightAt(x, z);

        // check ground is not water block
        auto ground = world.GetBlockType({x, y, z});
        if (ground == WATER || ground == STONE || ground == SAND || ground == ICE || ground == OAK_LEAVES)
        {
            continue;
        }

        check(y >= 0 && y < CHUNK_SIZE_Y);

        // make trunk height between 6 and 12
        BlockType trunkType = rand() % 10 ? OAK_LOG : BIRCH_LOG;
        int trunkheight = rand() % 4 + 6;
        for (int j = 0; j < trunkheight; j++)
        {
            world.SetBlockType({x, y + 1 + j, z}, trunkType);
        }

        // add leaves
        int leafsize = trunkheight / 3;

        for (int dx = -leafsize; dx <= leafsize; dx++)
        {
            for (int dz = -leafsize; dz <= leafsize; dz++)
            {
                for (int dy = -leafsize / 2; dy <= leafsize; dy++)
                {
                    // randomly skip some blocks
                    if (rand() % 2 == 0)
                    {
                        continue;
                    }
                    world.SetBlockType({x + dx, y + trunkheight + 1 + dy, z + dz}, OAK_LEAVES);
                }
            }
        }
    }
}

void FEngineLoop::GenerateWorld()
{
    WorldOperations world(GWorld);

    //world.DrawSphere(0, 0, 0, 8, 1);

    //TestBigSphere({0, 32, 0}, 16, 1);
    //TestBigSphere({100, 0, 0}, 64, 99); // 99 = random block type

    int range = 1000;
    int max_radius = 32;
    // draw a bunch of random spheres, keep y coord between 0 and 384, x and z between -range and range
    for (int i = 0; i < 100; i++)
    {
        int x = rand() % (range * 2) - range;
        int y = rand() % 384;
        int z = rand() % (range * 2) - range;

        int radius = rand() % max_radius + 1;
        int blockType = rand() % 5 + 1;

        //TestBigSphere({x, y, z}, radius, blockType);
    }

    // origin block
    world.SetBlockType({0, 0, 0}, 5);

    int size = WorldChunks * CHUNK_SIZE_X;
    GeneratePerlinTerrain(WorldChunks);

    // add some "trees"
    //AddTrees(world, 64);

    AddTrees(world, size);
}

/*

    world.SetBlockType({0, 0, 0}, 5);

    world.SetBlockType({16 * 0 + 0, 0, 0}, 1);
    world.SetBlockType({16 * 1 + 1, 0, 0}, 2);
    world.SetBlockType({16 * 2 + 2, 0, 0}, 3);
    world.SetBlockType({16 * 3 + 3, 0, 0}, 4);


    for (int i = 0; i < 5; i++)
    {
        world.SetBlockType({i, 0, 0}, i + 1);
    }

    // debug block at max height
    world.SetBlockType({0, 383, 0}, 1);

    world.DrawSphere(8, 8, 8, 1, 1);
    world.DrawSphere(0, 20, 0, 5, 1);
    world.DrawSphere(50, 16, 128, bSmallWorld ? 8 : 32, 1);
    world.DrawSphere(-8, 8, 0, 5, 1);

    // Generate Perlin noise terrain 
    int ChunksWide = bSmallWorld ? 8 : 64;
    int size = ChunksWide * 16;
    int startX = -size / 2;
    int startZ = -size / 2;
    int width = size;
    int depth = size;
    float scale = 0.025f;
    float amplitude = 20.0f;
    uint8_t blockType = 3;

    world.GeneratePerlinNoiseTerrain(startX, startZ, width, depth, scale, amplitude, blockType);

    // add some "trees"
    int treeCount = bSmallWorld ? 50 : 2000;
    for (int i = 0; i < treeCount; i++)
    {
        // random x, z
        int x = rand() % size - size / 2;
        int z = rand() % size - size / 2;

        int y = world.GetHighestBlockHeightAt(x, z);

        check(y >= 0 && y < CHUNK_SIZE_Y);

        int trunkheight = 5;
        for (int j = 0; j < trunkheight; j++)
        {
            world.SetBlockType({x, y + 1 + j, z}, 1);
        }

        // add leaves
        for (int dx = -2; dx <= 2; dx++)
        {
            for (int dz = -2; dz <= 2; dz++)
            {
                for (int dy = 0; dy <= 2; dy++)
                {
                    // randomly skip some blocks
                    if (rand() % 2 == 0)
                    {
                        continue;
                    }
                    world.SetBlockType({x + dx, y + trunkheight + 1 + dy, z + dz}, 2);
                }
            }
        }
    }
}
*/

void FEngineLoop::RebuildChunkMeshes()
{
    for (auto& chunkEntry : GWorld)
    {
        // get chunk from key
        const auto& chunkKey = chunkEntry.first;
        auto& chunk = GWorld.GetChunk(chunkKey);

        ChunkMeshManager::GetInstance().RebuildChunkMesh(chunkKey, chunk, GGraphicsDevice->GetDevice());
    }
}


void FEngineLoop::AddBlockTick()
{
    return;
    static double NextTimeToAddBlock = 0.0f;

    WorldOperations world(GWorld);
    if (NextTimeToAddBlock <= GTime)
    {
        NextTimeToAddBlock = GTime + 0.1f;
        WorldBlockCounter++;
        if (WorldBlockCounter < 1000)
        {
            WorldBlockCoord worldBlock = {WorldBlockCounter, 0, 1};
            world.SetBlockType(worldBlock, 1);

            // rebuild chunk and surrounding chunks.
            ChunkKey chunkKey;
            WorldToLocal(worldBlock, chunkKey);
            auto& chunk = GWorld.GetChunk(chunkKey);
            ChunkMeshManager::GetInstance().RebuildChunkMesh(chunkKey, chunk, GGraphicsDevice->GetDevice());
        }
    }
}

int32 FEngineLoop::Init()
{
    VG_LOG(LOG_CATEGORY_GENERAL, LOG_INFO, "FEngineLoop::Init()");

    GTime = PlatformTime::GetTimeInSeconds();

    GWindow = new PlatformWindow();

    GGraphicsDevice = new GraphicsDevice(GWindow->GetHandle());
    assert(GGraphicsDevice->IsValid());

    GMesh = new CubeMesh(GGraphicsDevice->GetDevice(), GGraphicsDevice->GetDeviceContext());
    assert(GMesh->IsValid());

    GMesh->SetShaders();
    GMesh->Select();

    {
        ScopedTimer timer("GenerateWorld");
        GenerateWorld();
    }

    {
        ScopedTimer timer("RebuildChunkMeshes");
        RebuildChunkMeshes();
    }


    return 0;
}

// this was so slow because it was rendering each block individually and doing massive memory reads.
// it had to scan through # of chunks * 16 * 16 * 384 blocks which was several megabytes of memory.

// int FEngineLoop::RenderChunk(Chunk& chunk, DirectX::XMMATRIX translationMatrix, DirectX::XMMATRIX rotationMatrix,
//                              DirectX::XMMATRIX scaleMatrix, DirectX::XMMATRIX viewMatrix,
//                              DirectX::XMMATRIX projectionMatrix)
// {
//     bool ShouldDraw = !InputManager::Get().IsKeyHeld('R');
//     if(!ShouldDraw) return 1;
//     
//     int DrawCalls = 0;
//     
//     int32 WorldOffsetX = chunk.GetWorldOffsetX();
//     int32 WorldOffsetZ = chunk.GetWorldOffsetZ();
//
//     // draw chunk
//     for (int32 i = 0; i < Chunk::Width; i++)
//         for (int32 j = 0; j < Chunk::Height; j++)
//             for (int32 k = 0; k < Chunk::Depth; k++)
//             {
//                 Block& CurrentBlock = chunk.GetBlock(i, j, k);
//                 if (CurrentBlock.Type == 0)
//                 {
//                     continue;
//                 }
//
//                 DrawCalls++;
//
//                 if(ShouldDraw)
//                 {
//                     float BlockX = static_cast<float>(i) + WorldOffsetX;
//                     float BlockZ = static_cast<float>(k) + WorldOffsetZ;
//                     float BlockY = static_cast<float>(j);
//
//                     translationMatrix = DirectX::XMMatrixTranslation(BlockX, BlockY, BlockZ);
//
//                     DirectX::XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
//                     DirectX::XMMATRIX WorldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
//                     DirectX::XMMATRIX TransposedWVPMatrix = DirectX::XMMatrixTranspose(WorldViewProjectionMatrix);
//
//                     // create an array of 5 tint colors
//                     static DirectX::XMFLOAT4 TintColors[] = {
//                         {0.5f, 0.5f, 0.5f, 1.0f}, // stone
//                         {0.55f, 0.27f, 0.07f, 1.0f}, // dirt
//                         {0.3f, 0.6f, 0.2f, 1.0f}, // grass
//                         {0.0f, 0.5f, 1.0f, 1.0f}, // water
//                         {0.2f, 0.2f, 0.2f, 1.0f} // bedrock
//                     };
//
//                     int ColorIndex = (CurrentBlock.Type - 1) % 5; // subtract 1 to make the block type 0-based 
//                     auto BlockColor = TintColors[ColorIndex];
//
//                     GMesh->Render(TransposedWVPMatrix, BlockColor);
//                 }
//             }
//     return DrawCalls;
// }

void FEngineLoop::DrawChunks(DirectX::XMMATRIX translationMatrix, DirectX::XMMATRIX rotationMatrix,
                             DirectX::XMMATRIX scaleMatrix, DirectX::XMMATRIX viewMatrix,
                             DirectX::XMMATRIX projectionMatrix, bool bDrawWater)
{
    // iterate world's chunks and render
    int TotalDrawCalls = 0;
    for (auto& chunkEntry : GWorld)
    {
        const auto& chunkKey = chunkEntry.first;

        //Chunk& chunk = chunkEntry.second;
        // if (chunk.GetWorldOffsetX() == 0 && chunk.GetWorldOffsetZ() == 0)
        // {
        //     // skip the chunk at 0,0
        //     //continue;
        // }

        WorldBlockCoord chunkOrigin;
        ChunkToWorld(chunkKey, chunkOrigin);

        translationMatrix = DirectX::XMMatrixTranslation(
            static_cast<float>(chunkOrigin.X), 0.0f, static_cast<float>(chunkOrigin.Z));

        DirectX::XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
        DirectX::XMMATRIX WorldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
        DirectX::XMMATRIX TransposedWVPMatrix = DirectX::XMMatrixTranspose(WorldViewProjectionMatrix);

        Mesh* mesh = ChunkMeshManager::GetInstance().GetChunkMesh(chunkKey);
        if (mesh)
        {
            float TintR = mesh->DebugColor.x;
            float TintG = mesh->DebugColor.y;
            float TintB = mesh->DebugColor.z;

            GGraphicsDevice->SetConstants(TransposedWVPMatrix, TintR, TintG, TintB);

            mesh->BindToDeviceContext(GGraphicsDevice->GetDeviceContext());

            constexpr int SolidBlocksSubMesh = 0;
            constexpr int WaterBlocksSubMesh = 1;

            auto dc = GGraphicsDevice->GetDeviceContext();

            if(bDrawWater)
            {
                GGraphicsDevice->DisableDepthWrite();
                mesh->DrawSubMesh(dc, 1);
            }
            else
            {
                GGraphicsDevice->EnableDepthWrite();
                mesh->DrawSubMesh(dc, 0);
            }
        }
    }
}

void FEngineLoop::Tick()
{
    GFrameNumber++;

    WorldOperations world(GWorld);

    double PreviousTime = GTime;
    GTime = PlatformTime::GetTimeInSeconds();
    float DeltaTime = static_cast<float>(GTime - PreviousTime);

    FrameTiming::Update(DeltaTime);

    GWindow->ProcessMessageQueue();

    if (InputManager::Get().IsKeyPressed(' '))
    {
        GShouldRenderTintColor = !GShouldRenderTintColor;
    }

    AddBlockTick();

    // set clear color
    static bool bColor = true;

    //auto Color = DirectX::Colors::Red;
    float MinecraftSkyColor[] = {0.4706f, 0.6549f, 1.0f, 1.0f};
    float ClearColor1[] = {0.4706f, 0.6549f, 1.f, 1.0f}; // minecraft sky
    float ClearColor2[] = {0.4f, 0.2f, 0.0f, 1.0f};
    GGraphicsDevice->Clear(bColor ? MinecraftSkyColor : ClearColor2);

    // update matrix
    float yaw = static_cast<float>(GTime);
    float pitch = yaw * 0.5f;
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixIdentity();
    // DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();


    // movement
    GPlayerController.ProcessInput(static_cast<float>(DeltaTime));
    auto MinecraftSprintSpeed = 5.612f;
    static float DistancePerSecond = MinecraftSprintSpeed;
    float increase = 2;

    if (InputManager::Get().IsKeyPressed('R'))
    {
        VG_LOG(LOG_CATEGORY_GENERAL, LOG_INFO, "Increase speed");
        DistancePerSecond = DistancePerSecond * increase;
    }
    if (InputManager::Get().IsKeyPressed('F'))
    {
        VG_LOG(LOG_CATEGORY_GENERAL, LOG_INFO, "Decrease speed");
        DistancePerSecond = DistancePerSecond / increase;
    }
    InputManager::Get().ClearKeyPressed('R');
    InputManager::Get().ClearKeyPressed('F');


    if (DistancePerSecond < MinecraftSprintSpeed) DistancePerSecond = MinecraftSprintSpeed;


    float MovementScale = static_cast<float>(DeltaTime) * DistancePerSecond;
    DirectX::FXMVECTOR MoveVector = DirectX::XMVectorMultiply(GPlayerController.MovementVector,
                                                              DirectX::XMVectorSet(
                                                                  MovementScale, MovementScale, MovementScale,
                                                                  0.0f));
    DirectX::XMVector3Normalize(MoveVector);
    GCameraPosition = DirectX::XMVectorAdd(GCameraPosition, MoveVector);

    // using player controller yaw AND pitch, create a lookat point in front of the GCameraPosition
    DirectX::XMVECTOR LookAtPoint = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    LookAtPoint = GPlayerController.GetForwardVector();
        // DirectX::XMVector3Rotate(LookAtPoint,
        //                                    DirectX::XMQuaternionRotationRollPitchYaw(
        //                                        DirectX::XMConvertToRadians(GPlayerController.Pitch),
        //                                        DirectX::XMConvertToRadians(GPlayerController.Yaw), 0.0f));
    LookAtPoint = DirectX::XMVectorAdd(GCameraPosition, LookAtPoint);

    // camera matrix (view)
    float AnimatedSin = static_cast<float>(sin(GTime));
    DirectX::FXMVECTOR eye = GCameraPosition; // eye 
    DirectX::FXMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // look at origin
    DirectX::FXMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // y-up
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye, LookAtPoint, up);

    // projection
    float NearZ = 0.1f;
    float FarZ = 10000.0f;
    float WindowWidth = 1280.0f;
    float WindowHeight = 720.0f;
    float AspectRatio = WindowWidth / WindowHeight;
    float FovAngleYRadians = DirectX::XMConvertToRadians(70.0f);
    DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
        FovAngleYRadians, AspectRatio, NearZ, FarZ);

    // place blocks at player position if keys 1-5 are pressed.
    if (true)
    {
        BlockType NewBlockType = 0;
        bool KeyPressed = false;
        for (char key = VK_NUMPAD0; key <= VK_NUMPAD5; ++key)
        {
            if (InputManager::Get().IsKeyHeld(key))
            {
                KeyPressed = true;
                NewBlockType = key - VK_NUMPAD0;
                if(NewBlockType == 4) NewBlockType = WATER; 
                break;
            }
        }

        if (KeyPressed)
        {
            // put the block 10 units in front of the camera
             DirectX::XMVECTOR CameraForward = GPlayerController.GetForwardVector();
                 //DirectX::XMVector3Rotate(
            //     DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
            //     DirectX::XMQuaternionRotationRollPitchYaw(
            //         DirectX::XMConvertToRadians(GPlayerController.Pitch),
            //         DirectX::XMConvertToRadians(GPlayerController.Yaw), 0.0f));

            DirectX::XMVECTOR CameraPosition = GCameraPosition;
            DirectX::XMVECTOR BlockPosition = DirectX::XMVectorAdd(CameraPosition, CameraForward * 10.0f);


            DirectX::XMVECTOR BlockPositionRounded = DirectX::XMVectorRound(BlockPosition);

            WorldBlockCoord blockCursor;
            blockCursor.X = static_cast<int>(DirectX::XMVectorGetX(BlockPositionRounded));
            blockCursor.Y = static_cast<int>(DirectX::XMVectorGetY(BlockPositionRounded));
            blockCursor.Z = static_cast<int>(DirectX::XMVectorGetZ(BlockPositionRounded));


            ChunkKey centerChunkKey;
            WorldToLocal(blockCursor, centerChunkKey);

            if (0)
            {
                // // stale code
                // auto Coords = GWorld.GetChunkCoordAndLocal(worldX, worldZ);
                // world.SetBlock(worldX, worldY, worldZ, NewBlockType);
                // GWorld.GetChunk(Coords.ChunkX, Coords.ChunkZ).RebuildMesh(GGraphicsDevice->GetDevice());
            }
            else
            {
                //world.DrawSphere(worldX, worldY, worldZ, 4, NewBlockType);
                TestBigSphere(blockCursor, 4, NewBlockType);

                // rebuild chunk and surrounding chunks.
                // todo: ideally it should only rebuild the chunks that have been modified.
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dz = -1; dz <= 1; dz++)
                    {
                        ChunkKey currentChunkKey = {centerChunkKey.X + dx, centerChunkKey.Z + dz};
                        auto& currentChunk = GWorld.GetChunk(currentChunkKey);

                        // todo: ideally should add chunks to a rebuild queue and update a few per frame.
                        ChunkMeshManager::GetInstance().RebuildChunkMesh(
                            currentChunkKey, currentChunk, GGraphicsDevice->GetDevice());
                    }
                }
            }
        }
    }

    DrawChunks(translationMatrix, rotationMatrix, scaleMatrix, viewMatrix, projectionMatrix, false);
    DrawChunks(translationMatrix, rotationMatrix, scaleMatrix, viewMatrix, projectionMatrix, true);

    // device present
    GGraphicsDevice->Present(vsync);

    // update window title
    constexpr float TimeBetweenUpdates = 0.01f;
    static float UpdateTimeRemaining = TimeBetweenUpdates;
    UpdateTimeRemaining -= DeltaTime;

    if (UpdateTimeRemaining <= 0)
    {
        UpdateTimeRemaining = TimeBetweenUpdates;
        int ChunkCount = static_cast<int>(GWorld.GetChunkCount());
        int MouseX;
        int MouseY;
        InputManager::Get().GetMousePosition(MouseX, MouseY);
        bool MouseDown = InputManager::Get().IsMouseButtonHeld(VK_LBUTTON);
        constexpr int buffer_count = 256;
        static char TempBuffer[buffer_count];
        (void)sprintf_s(TempBuffer, buffer_count,
                        "FPS: %f, MS: %f, MouseXY(%d, %d), MouseDown: %d, WorldChunks: %d, Platform: %s",
                        FrameTiming::GetFPS(), FrameTiming::GetFrameTimeInMS(), MouseX, MouseY, MouseDown,
                        ChunkCount, PlatformProcess::GetPlatformName()
        );
        GWindow->SetTitle(TempBuffer);
    }
}

void FEngineLoop::Exit()
{
    VG_LOG(LOG_CATEGORY_GENERAL, LOG_INFO, "FEngineLoop::Exit()");
}


void FEngineLoop::TestBigSphere(WorldBlockCoord Center, int Radius, BlockType BlockType)
{
    // Define the world region to map
    WorldRegion sphereWorldRegion = {
        {Center.X - Radius, Center.Y - Radius, Center.Z - Radius}, // Min coordinates
        {Center.X + Radius, Center.Y + Radius, Center.Z + Radius} // Max coordinates
    };

    auto chunksAndSubVolumes = ChunkVolumeMapper::GetChunksAndSubVolumes(sphereWorldRegion);

    // process each chunk and its sub-volume starting block and dimensions
    for (const auto& chunkSubVolume : chunksAndSubVolumes)
    {
        const auto& startingBlock = chunkSubVolume.startingBlock;

        WorldBlockCoord startingBlockWorldCoords;
        LocalToWorld(startingBlock, startingBlockWorldCoords);

        // get chunk data
        auto& chunk = GWorld.GetChunk(chunkSubVolume.startingBlock.chunkKey);
        auto chunkData = chunk.GetBlockData()->blockStates;

        ChunkUtils::TFillChunkSubvolume(chunkData,
                                        chunkSubVolume.startingBlock.Index,
                                        chunkSubVolume.width, chunkSubVolume.height, chunkSubVolume.depth,
                                        startingBlockWorldCoords.X, startingBlockWorldCoords.Y,
                                        startingBlockWorldCoords.Z,
                                        // [&](auto* block_states, int index, int world_x, int world_y, int world_z)
                                        // {
                                        //     if (ChunkUtils::IsPointInSphere(
                                        //         world_x, world_y, world_z, Center.X, Center.Y, Center.Z, Radius))
                                        //     {
                                        //         block_states[index] = BlockType;
                                        //     }
                                        // });


                                        // world origin of chunk block 0
                                        [&](auto* block_states, int index, int world_x, int world_y, int world_z)
                                        {
                                            if (ChunkUtils::IsPointInSphere(
                                                world_x, world_y, world_z, Center.X, Center.Y, Center.Z, Radius))
                                            {
                                                int block_type = BlockType;

                                                // get random block type between 1 and 7       
                                                int rand_block_type = rand() % 7 + 1;

                                                if (block_type == 99)
                                                {
                                                    block_type = rand_block_type;
                                                }

                                                if (world_y == 0) block_type = 5; // bedrock

                                                block_states[index] = block_type;
                                            }
                                        });
    }
}
