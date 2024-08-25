// Copyright Voxel Games, Inc. All Rights Reserved.

#include "HAL/HackPlatformDefines.h"

#include "CoreMinimal.h"

#include "Windows/WindowsHWrapper.h"

#include "Launch/LaunchOldEngineLoop.h"

#include <cstdio>
#include <DirectXColors.h>
#include <unordered_set>

#include "RHI/Mesh.h"
#include "RHI/GraphicsDevice.h"

#include "HAL/PlatformWindow.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"

#include "Renderer/CubeMesh.h"

#include "Input/InputManager.h"

#include "Misc/Application.h"
#include "Misc/FrameTiming.h"
#include "Misc/PlayerController.h"
#include "Misc/ThreadPool.h"

#include "Renderer/BoxMeshBuilder.h"

#include "Utils/ChunkUtils.h"

#include "World/VoxelWorld.h"
#include "VoxelMisc/ChunkMeshManager.h"
#include "WorldGen/BlockTypes.h"
#include "WorldGen/TerrainGenerators.h"
#include "WorldGen/WorldOperations.h"

#include <algorithm>
#include <execution>

#include "Renderer/Camera.h"


// use static initialization to show console early in app startup
static bool ShowConsoleOnInit = []()
{
    WindowsPlatformProcess::ShowConsole();
    return true;
}();


PlatformWindow* GWindow = nullptr;
GraphicsDevice* GGraphicsDevice = nullptr;
CubeMesh* GCubeMesh = nullptr;
Mesh GBoxMesh;
vector<ChunkKey> VisibleChunks;

VoxelWorld GWorld;

#if 1
#include "WorldGen/ForrestBeachGenerator.h"
ForestBeachBiomeGenerator biome;
#else
#include "WorldGen/MountainsGenerator.h"
MountainsGenerator biome;
#endif

// if debug mode, only generate a small world using #if
#if _DEBUG
int ChunkDrawRadius = 32;
#else
int ChunkDrawRadius = 32;
#endif


PlayerController GPlayerController;
float InitialCameraHeight = static_cast<float>(-(ChunkDepth + 2) * ChunkDrawRadius);
Vector InitialCameraPosition = {0.0f, 100.0f, InitialCameraHeight};

Camera GCamera = Camera(InitialCameraPosition,
                        Vector(0.0f, 0.0f, 1.0f),
                        90.0f,
                        16.0f / 9.0f,
                        0.1f,
                        1000.0f);

int WorldBlockCounter = 0;

ThreadPool GThreadPool(std::thread::hardware_concurrency());

// keep a hashed list of chunk keys that have been queued for generation
// so we don't queue the same chunk multiple times.
std::unordered_set<ChunkKey, ChunkKeyHash> ChunkLoadQueue;

void FOldEngineLoop::SetBlocksInFrontOfPlayer()
{

    // place blocks at player position if keys 1-5 are pressed.
    // if (true)
    // {
    //     BlockType NewBlockType = 0;
    //     bool KeyPressed = false;
    //     for (char key = VK_NUMPAD0; key <= VK_NUMPAD5; ++key)
    //     {
    //         if (InputManager::Get().IsKeyHeld(key))
    //         {
    //             KeyPressed = true;
    //             NewBlockType = key - VK_NUMPAD0;
    //             if (NewBlockType == 4) NewBlockType = WATER;
    //             break;
    //         }
    //     }
    //
    //     if (KeyPressed)
    //     {
    //         // // put the block 10 units in front of the camera
    //         // XMVECTOR CameraForward = GPlayerController.GetForwardVector();
    //         // //XMVector3Rotate(
    //         // //     XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
    //         // //     XMQuaternionRotationRollPitchYaw(
    //         // //         XMConvertToRadians(GPlayerController.Pitch),
    //         // //         XMConvertToRadians(GPlayerController.Yaw), 0.0f));
    //         //
    //         // XMVECTOR CameraPosition = GCameraPosition;
    //         // XMVECTOR BlockPosition = XMVectorAdd(CameraPosition, CameraForward * 10.0f);
    //         //
    //         //
    //         // XMVECTOR BlockPositionRounded = XMVectorRound(BlockPosition);
    //         //
    //         // BlockCoordinate blockCursor;
    //         // blockCursor.X = static_cast<int>(XMVectorGetX(BlockPositionRounded));
    //         // blockCursor.Y = static_cast<int>(XMVectorGetY(BlockPositionRounded));
    //         // blockCursor.Z = static_cast<int>(XMVectorGetZ(BlockPositionRounded));
    //         //
    //         //
    //         // ChunkKey centerChunkKey;
    //         // WorldPositionToChunkKey(blockCursor, centerChunkKey);
    //
    //         // if (0)
    //         // {
    //         //     // // stale code
    //         //     // auto Coords = GWorld.GetChunkCoordAndLocal(worldX, worldZ);
    //         //     // world.SetBlock(worldX, worldY, worldZ, NewBlockType);
    //         //     // GWorld.GetChunk(Coords.ChunkX, Coords.ChunkZ).RebuildMesh(GGraphicsDevice->GetDevice());
    //         // }
    //         // else
    //         // {
    //         //     //world.DrawSphere(worldX, worldY, worldZ, 4, NewBlockType);
    //         //     //TestBigSphere(blockCursor, 4, NewBlockType);
    //         //
    //         //     // rebuild chunk and surrounding chunks.
    //         //     // todo: ideally it should only rebuild the chunks that have been modified.
    //         //     // for (int dx = -1; dx <= 1; dx++)
    //         //     // {
    //         //     //     for (int dz = -1; dz <= 1; dz++)
    //         //     //     {
    //         //     //         ChunkKey currentChunkKey = {centerChunkKey.X + dx, centerChunkKey.Z + dz};
    //         //     //         auto& currentChunk = GWorld.GetChunk(currentChunkKey);
    //         //     //
    //         //     //         // todo: ideally should add chunks to a rebuild queue and update a few per frame.
    //         //     //         ChunkMeshManager::GetInstance().RebuildChunkMesh(
    //         //     //             currentChunkKey, currentChunk, GGraphicsDevice->GetDevice());
    //         //     //     }
    //         //     // }
    //         // }
    //     }
    // }
}

void FOldEngineLoop::GenerateChunksInBackground(const std::vector<ChunkKey>& ChunkKeys)
{
    for (const auto& key : ChunkKeys)
    {
        GThreadPool.Enqueue([this,key]()
        {
            //VG_LOG(LogCategoryGeneral, LOG_INFO, "Generating chunk: %d, %d", key.X, key.Z);

            // Get or create the chunk
            ChunkRef chunk = GWorld.GetChunk(key);

            // generate the chunk
            GenerateChunk(chunk, key);

            // build its mesh
            ChunkMeshManager::GetInstance().RebuildChunkMesh(key, chunk, GGraphicsDevice->GetDevice());
        });
    }
}

void FOldEngineLoop::GenerateChunksAroundCamera(int RadiusInChunks /*= 32*/)
{
    // get camera position

    BlockCoordinate cameraBlock;
    cameraBlock.X = static_cast<int>(GCamera.GetPosition().X);
    cameraBlock.Y = static_cast<int>(GCamera.GetPosition().Y);
    cameraBlock.Z = static_cast<int>(GCamera.GetPosition().Z);

    ChunkKey cameraChunkKey;
    WorldPositionToChunkKey(cameraBlock, cameraChunkKey);

    // log camera chunk key
    //VG_LOG(LogCategoryGeneral, LOG_INFO, "Camera Chunk Key: %d, %d", cameraChunkKey.X, cameraChunkKey.Z);

    // make list of chunks to generate, centered around the camera
    // do an actual radius not just a square


    std::vector<ChunkKey> chunkKeys;
    int radius = RadiusInChunks;
    float bias = 0; //0.5f ; // removes unwanted single chunk at the edge of the radius along x & z axes
    int radiusSquared = radius * radius;

    TE_LOG(LogTemp, Log, TEXT("Radius = %d"), radius);
    for (int dx = 0; dx <= radius; dx++)
    {
        for (int dz = 0; dz <= radius; dz++)
        {
            int distanceSquared = (dx * dx) + (dz * dz);
            //if (distanceSquared <= radiusSquared - bias)
            {
                // Generate for the +,+ quadrant
                ChunkKey chunkKey1 = {cameraChunkKey.X + dx, cameraChunkKey.Z + dz};
                if (ChunkLoadQueue.find(chunkKey1) == ChunkLoadQueue.end())
                {
                    ChunkLoadQueue.insert(chunkKey1);
                    chunkKeys.push_back(chunkKey1);
                }

                // Generate for the +,- quadrant
                ChunkKey chunkKey2 = {cameraChunkKey.X + dx, cameraChunkKey.Z - dz};
                if (ChunkLoadQueue.find(chunkKey2) == ChunkLoadQueue.end())
                {
                    ChunkLoadQueue.insert(chunkKey2);
                    chunkKeys.push_back(chunkKey2);
                }

                // Generate for the -,+ quadrant
                ChunkKey chunkKey3 = {cameraChunkKey.X - dx, cameraChunkKey.Z + dz};
                if (ChunkLoadQueue.find(chunkKey3) == ChunkLoadQueue.end())
                {
                    ChunkLoadQueue.insert(chunkKey3);
                    chunkKeys.push_back(chunkKey3);
                }

                // Generate for the -,- quadrant
                ChunkKey chunkKey4 = {cameraChunkKey.X - dx, cameraChunkKey.Z - dz};
                if (ChunkLoadQueue.find(chunkKey4) == ChunkLoadQueue.end())
                {
                    ChunkLoadQueue.insert(chunkKey4);
                    chunkKeys.push_back(chunkKey4);
                }
            }
        }
    }

    TE_LOG(LogTemp, Log, TEXT("Chunks Submitted For Background Generation: %d"), chunkKeys.size());

    GenerateChunksInBackground(chunkKeys);

    // remove chunks outside nearby radius
    //GWorld.RemoveChunksOutsideRadius(cameraChunkKey, ChunksWide);


    return;
    /*
    // generate some perlin terrain
    int size = RadiusInChunks * ChunkWidth;
    int startX = -size / 2;
    int startZ = -size / 2;
    int width = size;
    int depth = size;
    float scale = 0.025f;
    float amplitude = 20.0f;
    uint8_t blockType = 3;


    
    // create chunk subvolumes for the perlin noise terrain
    int maxheight = 384;
    BlockRegion terrainRegion = {
        {startX, 0, startZ}, // min coordinates
        {startX + width, maxheight, startZ + depth} // max coordinates
    };

    auto chunksAndSubVolumes = ChunkVolumeMapper::GetChunksAndSubVolumes(terrainRegion);

    PerlinNoise perlinNoise; // must be initiated once before using

    // Parallel for_each using execution policy
    // todo: i think i can hit a racecondition or deadlock here. I need to make sure VoxelWorld is thread safe.

    // get total # chunks to generate
    size_t totalChunks = chunksAndSubVolumes.size();
    StopWatch timer;
    timer.Start();

    std::for_each(std::execution::par, chunksAndSubVolumes.begin(), chunksAndSubVolumes.end(),
                  [&](const auto& chunkSubVolume)
                  {
                      VG_LOG(LogCategoryGeneral, LOG_INFO, "Generating terrain for chunk: %d, %d",
                             chunkSubVolume.startingBlock.chunkKey.X, chunkSubVolume.startingBlock.chunkKey.Z);

                      const auto& startingBlock = chunkSubVolume.startingBlock;

                      BlockCoordinate startingBlockWorldCoords;
                      LocalToWorld(startingBlock, startingBlockWorldCoords);

                      // get chunk data
                      auto& chunk = GWorld.GetChunk(chunkSubVolume.startingBlock.chunkKey);

                      //GenerateChunk(
                  });


    
    timer.Stop();

    // log how many chunks were generated and time
    VG_LOG(LogCategoryGeneral, LOG_INFO, "Chunk Generation time: %f", timer.GetElapsedSeconds());
    // log avg milliseconds per chunk
    double avgMsPerChunk = timer.GetElapsedSeconds() / totalChunks * 1000;
    VG_LOG(LogCategoryGeneral, LOG_INFO, "Avg ms per chunk: %f ms", avgMsPerChunk);
    // chunks per second
    double chunksPerSecond = totalChunks / timer.GetElapsedSeconds();
    VG_LOG(LogCategoryGeneral, LOG_INFO, "Chunks per second: %f", chunksPerSecond);

    VG_LOG(LogCategoryGeneral, LOG_INFO, "Generated %d chunks", totalChunks);
    */
}

//
// void FOldEngineLoop::GeneratePerlinTerrain2(int ChunksWide = 32)
// {
//     // generate some perlin terrain
//     int size = ChunksWide * CHUNK_SIZE_X;
//     int startX = -size / 2;
//     int startZ = -size / 2;
//     int width = size;
//     int depth = size;
//     float scale = 0.025f;
//     float amplitude = 20.0f;
//     uint8_t blockType = 3;
//
//     // create chunk subvolumes for the perlin noise terrain
//     int maxheight = 384;
//     WorldRegion terrainRegion = {
//         {startX, 0, startZ}, // min coordinates
//         {startX + width, maxheight, startZ + depth} // max coordinates
//     };
//
//
//     auto chunksAndSubVolumes = ChunkVolumeMapper::GetChunksAndSubVolumes(terrainRegion);
//
//     PerlinNoise perlinNoise; // must be initiated once before using
//
//     for (const auto& chunkSubVolume : chunksAndSubVolumes)
//     {
//         VG_LOG(LogCategoryGeneral, LOG_INFO, "Generating terrain for chunk: %d, %d",
//                chunkSubVolume.startingBlock.chunkKey.X, chunkSubVolume.startingBlock.chunkKey.Z);
//
//         const auto& startingBlock = chunkSubVolume.startingBlock;
//
//         WorldBlockCoord startingBlockWorldCoords;
//         LocalToWorld(startingBlock, startingBlockWorldCoords);
//
//         // get chunk data
//         auto& chunk = GWorld.GetChunk(chunkSubVolume.startingBlock.chunkKey);
//         auto chunkData = chunk.GetBlockData()->blockStates;
//
//         ChunkUtils::TFillChunkSubvolume(chunkData,
//                                         chunkSubVolume.startingBlock.Index,
//                                         chunkSubVolume.width, chunkSubVolume.height, chunkSubVolume.depth,
//                                         startingBlockWorldCoords.X, startingBlockWorldCoords.Y,
//                                         startingBlockWorldCoords.Z,
//                                         [&](auto* block_states, int index, int world_x, int world_y, int world_z)
//                                         {
//                                             BlockType block_type1 = biome.GetBlock(world_x, world_y, world_z);
//
//                                             if (block_type1 != AIR)
//                                             {
//                                                 block_states[index] = block_type1;
//                                             }
//                                             return;
//      
//                                         });
//     }
// }

void FOldEngineLoop::AddTrees(WorldOperations World, int Size)
{
    int chunksWide = Size / ChunkWidth;

    int treesPerChunk = 3;
    int treeCount = treesPerChunk * chunksWide * chunksWide;
    for (int i = 0; i < treeCount; i++)
    {
        // random x, z
        int x = rand() % Size - Size / 2;
        int z = rand() % Size - Size / 2;

        int y = World.GetHighestBlockHeightAt(x, z);

        // check ground is not water block
        const Block ground = World.GetBlockRef({x, y, z});
        if (ground == WATER || ground == STONE || ground == SAND || ground == ICE || ground == OAK_LEAVES)
        {
            continue;
        }

        check(y >= 0 && y < ChunkHeight);


        // determine leaves and trunk type
        BlockType trunkType = rand() % 10 ? OAK_LOG : BIRCH_LOG;
        BlockType leafType = rand() % 2 ? OAK_LEAVES : DARK_OAK_LEAVES;


        // make trunk height between 6 and 12
        int trunkheight = rand() % 4 + 6;
        for (int j = 0; j < trunkheight; j++)
        {
            World.SetBlockType({x, y + 1 + j, z}, trunkType);
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
                    World.SetBlockType({x + dx, y + trunkheight + 1 + dy, z + dz}, leafType);
                }
            }
        }
    }
}

void FOldEngineLoop::GenerateWorld()
{
    return;
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

    int size = ChunkDrawRadius * ChunkWidth;
    GenerateChunksAroundCamera(ChunkDrawRadius);

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

// void FOldEngineLoop::RebuildChunkMeshes()
// {
//     for (auto& chunkEntry : GWorld)
//     {
//         const auto& chunkKey = chunkEntry.first;
//
//         if(ChunkPtr chunk = GWorld.TryGetChunk(chunkKey))
//         {
//             ChunkMeshManager::GetInstance().RebuildChunkMesh(chunkKey, chunk, GGraphicsDevice->GetDevice());
//         }
//     }
// }


double GTime;

void FOldEngineLoop::AddBlockTick()
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
            BlockCoordinate worldBlock = {WorldBlockCounter, 0, 1};
            world.SetBlockType(worldBlock, 1);

            // rebuild chunk and surrounding chunks.
            ChunkKey chunkKey;
            WorldPositionToChunkKey(worldBlock, chunkKey);
            auto chunk = GWorld.GetChunk(chunkKey);
            ChunkMeshManager::GetInstance().RebuildChunkMesh(chunkKey, chunk, GGraphicsDevice->GetDevice());
        }
    }
}

void FOldEngineLoop::ClearPendingCleanupObjects()
{
}

int32 FOldEngineLoop::Init()
{
    TE_LOG(LogTemp, Log, TEXT("FOldEngineLoop::Init()"));

    InputManager::Get().ShowCursor(false);

    GTime = PlatformTime::Seconds();

    GWindow = new PlatformWindow();

    GGraphicsDevice = new GraphicsDevice(GWindow->GetHandle());
    assert(GGraphicsDevice->IsValid());

    BoxMeshBuilder boxMeshBuilder;
    GBoxMesh = boxMeshBuilder.Build(GGraphicsDevice->GetDevice());

    GCubeMesh = new CubeMesh(GGraphicsDevice->GetDevice(), GGraphicsDevice->GetDeviceContext());
    //assert(GMesh->IsValid());

    GCubeMesh->SetShaders();
    GCubeMesh->Select();

    GenerateChunksAroundCamera(ChunkDrawRadius);

    // {
    //     ScopedTimer timer("GenerateWorld");
    //     GenerateWorld();
    // }
    //
    // {
    //     ScopedTimer timer("RebuildChunkMeshes");
    //     RebuildChunkMeshes();
    // }

    return 0;
}

// this was so slow because it was rendering each block individually and doing massive memory reads.
// it had to scan through # of chunks * 16 * 16 * 384 blocks which was several megabytes of memory.

// int FOldEngineLoop::RenderChunk(Chunk& chunk, XMMATRIX translationMatrix, XMMATRIX rotationMatrix,
//                              XMMATRIX scaleMatrix, XMMATRIX viewMatrix,
//                              XMMATRIX projectionMatrix)
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
//                     translationMatrix = XMMatrixTranslation(BlockX, BlockY, BlockZ);
//
//                     XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
//                     XMMATRIX WorldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
//                     XMMATRIX TransposedWVPMatrix = XMMatrixTranspose(WorldViewProjectionMatrix);
//
//                     // create an array of 5 tint colors
//                     static XMFLOAT4 TintColors[] = {
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


struct Plane
{
    XMFLOAT3 Normal;
    float Distance;
};

void ExtractFrustumPlanes(Plane planes[6], XMMATRIX& viewProjMatrix)
{
    XMFLOAT4X4 m;
    XMStoreFloat4x4(&m, viewProjMatrix);

    // Left plane
    planes[0].Normal = {m._14 + m._11, m._24 + m._21, m._34 + m._31};
    planes[0].Distance = m._44 + m._41;

    // Right plane
    planes[1].Normal = {m._14 - m._11, m._24 - m._21, m._34 - m._31};
    planes[1].Distance = m._44 - m._41;

    // Top plane
    planes[2].Normal = {m._14 - m._12, m._24 - m._22, m._34 - m._32};
    planes[2].Distance = m._44 - m._42;

    // Bottom plane
    planes[3].Normal = {m._14 + m._12, m._24 + m._22, m._34 + m._32};
    planes[3].Distance = m._44 + m._42;

    // Near plane
    planes[4].Normal = {m._13, m._23, m._33};
    planes[4].Distance = m._43;

    // Far plane
    planes[5].Normal = {m._14 - m._13, m._24 - m._23, m._34 - m._33};
    planes[5].Distance = m._44 - m._43;

    // Normalize the planes
    for (int i = 0; i < 6; i++)
    {
        float length = sqrtf(planes[i].Normal.x * planes[i].Normal.x +
            planes[i].Normal.y * planes[i].Normal.y +
            planes[i].Normal.z * planes[i].Normal.z);
        planes[i].Normal.x /= length;
        planes[i].Normal.y /= length;
        planes[i].Normal.z /= length;
        planes[i].Distance /= length;
    }
}


constexpr inline bool IsBoxInFrustum(const Plane planes[6], const XMFLOAT3& minPoint, const XMFLOAT3& maxPoint) noexcept
{
    for (int i = 0; i < 6; i++)
    {
        // Find the point in the AABB that is most opposite to the plane normal (i.e., "furthest" from the plane)
        XMFLOAT3 farthestPoint = minPoint;

        if (planes[i].Normal.x >= 0) farthestPoint.x = maxPoint.x;
        if (planes[i].Normal.y >= 0) farthestPoint.y = maxPoint.y;
        if (planes[i].Normal.z >= 0) farthestPoint.z = maxPoint.z;

        // If this point is outside the plane, then the box is outside the frustum
        float distance = planes[i].Normal.x * farthestPoint.x +
        planes[i].Normal.y * farthestPoint.y +
        planes[i].Normal.z * farthestPoint.z + planes[i].Distance;

        if (distance < 0)
        {
            return false; // Box is outside this plane
        }
    }

    return true; // Box is inside or intersecting the frustum
}

int GTotalDrawCalls;

void FOldEngineLoop::DrawChunks(XMMATRIX TranslationMatrix, XMMATRIX RotationMatrix,
                                XMMATRIX ScaleMatrix, XMMATRIX ViewMatrix,
                                XMMATRIX ProjectionMatrix, bool BdrawWater)
{
    // if (0)
    // {
    //     XMMATRIX WorldViewProjectionMatrix = XMMatrixIdentity() * viewMatrix * projectionMatrix;
    //     XMMATRIX TransposedWVPMatrix = XMMatrixTranspose(WorldViewProjectionMatrix);
    //
    //     GGraphicsDevice->SetConstants(TransposedWVPMatrix, 1, 1, 1);
    //     GBoxMesh.BindToDeviceContext(GGraphicsDevice->GetDeviceContext());
    //     GBoxMesh.Draw(GGraphicsDevice->GetDeviceContext());
    //
    //     return;
    // }



    // iterate world's chunks and render
    for (ChunkKey& chunkKey : VisibleChunks)
    {
        BlockCoordinate chunkOrigin;
        ChunkKeyToWorldPosition(chunkKey, chunkOrigin);

        TranslationMatrix = XMMatrixTranslation(
            static_cast<float>(chunkOrigin.X), 0.0f, static_cast<float>(chunkOrigin.Z));

        XMMATRIX worldMatrix = ScaleMatrix * RotationMatrix * TranslationMatrix;
        XMMATRIX WorldViewProjectionMatrix = worldMatrix * ViewMatrix * ProjectionMatrix;
        XMMATRIX TransposedWVPMatrix = XMMatrixTranspose(WorldViewProjectionMatrix);


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

            if (BdrawWater)
            {
                GGraphicsDevice->DisableDepthWrite();
                const SubMesh& subMesh = mesh->SubMeshes[1];
                if (subMesh.indexCount == 0)
                {
                    continue;
                }
                mesh->DrawSubMesh(dc, 1);
            }
            else
            {
                GGraphicsDevice->EnableDepthWrite();
                mesh->DrawSubMesh(dc, 0);
            }
            GTotalDrawCalls++;
        }
    }
}


void FOldEngineLoop::ComputeChunkVisibility(XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix)
{
    // build list of visible chunks
    VisibleChunks.clear();
    // get view frustum planes
    Plane frustumPlanes[6];
    auto viewProjMatrix = ViewMatrix * ProjectionMatrix;
    ExtractFrustumPlanes(frustumPlanes, viewProjMatrix);

    // loop through chunk keys and check if they are in view frustum    
    for (auto& chunkEntry : GWorld)
    {
        const auto& chunkKey = chunkEntry.first;

        BlockCoordinate chunkOrigin;
        ChunkKeyToWorldPosition(chunkKey, chunkOrigin);

        // get chunk mins and maxs
        XMFLOAT3 BoundingBoxMin = {
            static_cast<float>(chunkOrigin.X),
            0.0f,
            static_cast<float>(chunkOrigin.Z)};
        XMFLOAT3 BoundingBoxMax = {
            static_cast<float>(chunkOrigin.X + ChunkWidth),
            static_cast<float>(ChunkHeight),
            static_cast<float>(chunkOrigin.Z + ChunkDepth)
        };

        // check if chunk is within view frustum
        if (IsBoxInFrustum(frustumPlanes, BoundingBoxMin, BoundingBoxMax))
        {
            VisibleChunks.push_back(chunkKey);
        }
    }
}

void FOldEngineLoop::Tick()
{
    GFrameNumber++;

    WorldOperations world(GWorld);

    // update timing
    double DeltaTime = Application::GetDeltaTime();
    GTime = Application::GetCurrentTime();
    FrameTiming::Update(DeltaTime);

    // message pump
    GWindow->ProcessMessageQueue();

    // toggle chunk debug visualization
    if (InputManager::Get().IsKeyPressed(' '))
    {
        InputManager::Get().ClearKeyPressed(' '); // todo: may need a better way to 'consume' input
        GShouldRenderTintColor = !GShouldRenderTintColor;
        TE_LOG(LogTemp, Log, TEXT("GShouldRenderTintColor: %d"), GShouldRenderTintColor);
    }


    //auto Color = Colors::Red;
    constexpr float MinecraftSkyColor[] = {0.4706f, 0.6549f, 1.0f, 1.0f};
    constexpr float ClearColor1[] = {0.4706f, 0.6549f, 1.f, 1.0f}; // minecraft sky
    constexpr float ClearColor2[] = {0.4f, 0.2f, 0.0f, 1.0f};

    // set clear color
    static bool bColor = true;

    GGraphicsDevice->Clear(bColor ? MinecraftSkyColor : ClearColor2);

    // movement
    GPlayerController.Tick(static_cast<float>(DeltaTime));
    constexpr auto MinecraftSprintSpeed = 5.612f;
    
    static float PlayerSpeed = MinecraftSprintSpeed;
    constexpr float IncreaseSpeedMultiplier = 2;

    if (InputManager::Get().IsKeyPressed('R'))
    {
        TE_LOG(LogTemp, Log, TEXT("Increase speed"));
        PlayerSpeed = PlayerSpeed * IncreaseSpeedMultiplier;
    }
    if (InputManager::Get().IsKeyPressed('F'))
    {
        TE_LOG(LogTemp, Log, TEXT("Decrease speed"));
        PlayerSpeed = PlayerSpeed / IncreaseSpeedMultiplier;
    }
    InputManager::Get().ClearKeyPressed('R');
    InputManager::Get().ClearKeyPressed('F');


    if (PlayerSpeed < MinecraftSprintSpeed)
    {
        PlayerSpeed = MinecraftSprintSpeed;
    }

    Vector PlayerVelocity = GPlayerController.GetMovementVector() * PlayerSpeed;  
    

    GCamera.SetPosition(GCamera.GetPosition() + PlayerVelocity * static_cast<float>(DeltaTime));
    GCamera.SetTarget(GCamera.GetPosition() + GPlayerController.GetForwardVector());

    // camera matrix (view)
    const float AnimatedSin = static_cast<float>(sin(GTime));

    const FXMVECTOR EyePosition = XMVectorSet(GCamera.GetPosition().X, GCamera.GetPosition().Y, GCamera.GetPosition().Z, 0.0f);
    const FXMVECTOR FocusPosition = XMVectorSet(GCamera.GetTarget().X, GCamera.GetTarget().Y, GCamera.GetTarget().Z, 0.0f);
    const FXMVECTOR UpVector = XMVectorSet(GCamera.GetUp().X, GCamera.GetUp().Y, GCamera.GetUp().Z, 0.0f);

    // update matrix
    XMMATRIX TranslationMatrix = XMMatrixIdentity();
    XMMATRIX RotationMatrix = XMMatrixIdentity();
    XMMATRIX ScaleMatrix = XMMatrixIdentity();


    constexpr float WindowWidth = 1280.0f;
    constexpr float WindowHeight = 720.0f;

    // projection
    constexpr float FovAngleY = XMConvertToRadians(70.0f);
    constexpr float AspectRatio = WindowWidth / WindowHeight;
    constexpr float NearZ = 0.1f;
    constexpr float FarZ = 10000.0f;

    const XMMATRIX ViewMatrix = XMMatrixLookAtLH(EyePosition, FocusPosition, UpVector);
    const XMMATRIX ProjectionMatrix = XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);


    // hotkey for clearing the world
    if (InputManager::Get().IsKeyPressed('C'))
    {
        TE_LOG(LogTemp, Log, TEXT("Clearing world"));
        GWorld.Clear(); // todo: have to clear meshes too.
        ChunkLoadQueue.clear();
        //GeneratePerlinTerrain(ChunkGenerationRadius);
        //RebuildChunkMeshes();
    }


    static bool FreezeFrustumCulling = false;
    if (InputManager::Get().IsKeyPressed('G')) // toggle freeze culling 
    {
        TE_LOG(LogTemp, Log, TEXT("Toggling freeze frustum culling"));
        FreezeFrustumCulling = !FreezeFrustumCulling;
        InputManager::Get().ClearKeyPressed('G');
    }

    if (!FreezeFrustumCulling)
    {
        ComputeChunkVisibility(ViewMatrix, ProjectionMatrix);
    }




    // draw chunks: solid blocks first, then water blocks.
    GTotalDrawCalls = 0;
    DrawChunks(TranslationMatrix, RotationMatrix, ScaleMatrix, ViewMatrix, ProjectionMatrix, false /* solid */);
    DrawChunks(TranslationMatrix, RotationMatrix, ScaleMatrix, ViewMatrix, ProjectionMatrix, true /* water */);

    // device present
    GGraphicsDevice->Present(GEnableVSync);

    if (!GEnableVSync)
    {
        // yield to other threads once per frame
        //std::this_thread::yield();
    }

    // update window title
    constexpr float TimeBetweenUpdates = 0.01f;
    static float UpdateTimeRemaining = TimeBetweenUpdates;
    UpdateTimeRemaining -= (float)DeltaTime;

    if (UpdateTimeRemaining <= 0)
    {
        UpdateTimeRemaining = TimeBetweenUpdates;
        int ChunkCount = static_cast<int>(GWorld.GetChunkCount());
        int MouseX;
        int MouseY;
        InputManager::Get().GetMousePosition(MouseX, MouseY);
        bool MouseDown = InputManager::Get().IsMouseButtonHeld(VK_LBUTTON);
        constexpr int buffer_count = 256;
        static WIDECHAR TempBuffer[buffer_count];

        // printf to temp buffer
        auto FPS = FrameTiming::GetFPS();
        swprintf(TempBuffer, buffer_count,
                 L"Voxel Game: FPS=%.2f, Chunks=%d, DrawCalls=%d, Mouse={%d, %d}, MouseDown=%d",
                 FPS, ChunkCount, GTotalDrawCalls, MouseX, MouseY, MouseDown);

        GWindow->SetTitle(TempBuffer);
    }
}

void FOldEngineLoop::Exit()
{
    TE_LOG(LogTemp, Log, TEXT("FOldEngineLoop::Exit()"));
}

void FOldEngineLoop::GenerateChunk(ChunkRef Chunk, ChunkKey Key)
{
    ChunkUtils::TFillChunkSubvolume(Chunk->Blocks,
                                    0,
                                    ChunkWidth, ChunkHeight, ChunkDepth,
                                    Key.X * ChunkWidth, 0, Key.Z * ChunkDepth,
                                    [&](auto* block_states, int index, int world_x, int world_y,
                                        int world_z)
                                    {
                                        const Block block_type1 = biome.GetBlock(
                                            world_x, world_y, world_z);

                                        // set only non-air blocks (to prevent drawing air blocks over something that was drawn to the chunk previously)
                                        if (!block_type1.IsAir())
                                        {
                                            block_states[index] = block_type1;
                                        }
                                    });
}

FOldEngineLoop GOldEngineLoop;
