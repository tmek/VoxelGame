// Copyright Voxel Games, Inc. All Rights Reserved.

#include "Launch/LaunchOldEngineLoop.h"

#include <algorithm>
#include <execution>
#include <unordered_set>

#include "CoreMinimal.h"
#include "DirectXMath.h"

#include "HAL/HackPlatformDefines.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformWindow.h"

#include "Input/InputManager.h"

#include "Math/Math.h"
#include "Math/Matrix.h"

#include "Misc/Application.h"
#include "Misc/FrameTiming.h"
#include "Misc/PlayerController.h"
#include "Misc/ThreadPool.h"

#include "Renderer/Camera.h"
#include "Renderer/VoxelWorldRenderer.h"


#include "Utils/ChunkUtils.h"

#include "VoxelMisc/ChunkMeshManager.h"

#include "Windows/WindowsHWrapper.h"

#include "World/VoxelWorld.h"

#include "WorldGen/BlockTypes.h"
#include "WorldGen/TerrainGenerators.h"

using namespace DirectX;

// Static Variables Initialization
static bool ShowConsoleOnInit = []()
{
    WindowsPlatformProcess::ShowConsole();
    return true;
}();


// Constants
constexpr auto MinecraftSprintSpeed = 5.612f;
constexpr float IncreaseSpeedMultiplier = 2;
constexpr int ChunkDrawRadius = 32;


// globals
ThreadPool GThreadPool(std::thread::hardware_concurrency());

PlatformWindow* GWindow = nullptr;
VoxelWorldRenderer GRenderer;

// GraphicsDevice* GGraphicsDevice = nullptr;
// CubeMesh* GCubeMesh = nullptr;
// Mesh GBoxMesh;
// TArray<ChunkKey> VisibleChunks;


std::shared_ptr<VoxelWorld> GWorld = std::make_unique<VoxelWorld>();

std::unordered_set<ChunkKey, ChunkKeyHash> ChunkLoadQueue;

float               GPlayerSpeed = MinecraftSprintSpeed;
PlayerController    GPlayerController;
Camera              GCamera = Camera(Vector(0.0f, 100.0f, -1152.0f),
                        Vector::ForwardVector,
                        70.0f,
                        16.0f / 10.0f,
                        0.1f,
                        10000.0f);


// Biome generator
#define USE_FORREST_BIOME 1
#if USE_FORREST_BIOME
#include "WorldGen/ForrestBeachGenerator.h"
ForestBeachBiomeGenerator GBiome;
#else
    #include "WorldGen/MountainsGenerator.h"
    MountainsGenerator biome;
#endif


void FOldEngineLoop::InitializeGraphics()
{
     GWindow = new PlatformWindow();
     GRenderer.Initialize(GWindow->GetHandle());
    
    // GGraphicsDevice = new GraphicsDevice(GWindow->GetHandle());
    // assert(GGraphicsDevice->IsValid());
    //
    // BoxMeshBuilder boxMeshBuilder;
    // GBoxMesh = boxMeshBuilder.Build(GGraphicsDevice->GetDevice());
    //
    // GCubeMesh = new CubeMesh(GGraphicsDevice->GetDevice(), GGraphicsDevice->GetDeviceContext());
    //
    // GCubeMesh->SetShaders(); // sets general shaders
    // GCubeMesh->Select();     // sets general input layout and topology
}

void FOldEngineLoop::GenerateChunksInBackground(const TArray<ChunkKey>& ChunkKeys)
{
    for (const auto& GeneratedChunkKey : ChunkKeys)
    {
        GThreadPool.Enqueue([GeneratedChunkKey]()
        {
            // GWorld can be destroyed before the thread finishes.
            // todo: may need a more graceful way to handle this.
            // todo: may need to add a mutex to GWorld, just checking GWorld == nullptr is not enough.
            if(GIsRequestingExit)
            {
                return;
            }
            
            ChunkRef GeneratedChunk = GWorld->GetChunk(GeneratedChunkKey);
            ChunkUtils::TFillChunkSubvolume(GeneratedChunk->Blocks, 0,
                                            ChunkWidth, ChunkHeight, ChunkDepth,
                                            GeneratedChunkKey.X * ChunkWidth, 0, GeneratedChunkKey.Z * ChunkDepth,
                                            [&](auto* block_states, int index, int world_x, int world_y, int world_z)
                                            {
                                                const Block block_type1 = GBiome.GetBlock(world_x, world_y, world_z);
                                                if (!block_type1.IsAir())
                                                {
                                                    block_states[index] = block_type1;
                                                }
                                            });

            //ChunkMeshManager::GetInstance().RebuildChunkMesh(key, chunk, GGraphicsDevice->GetDevice());
            GRenderer.CreateChunkMesh(GeneratedChunkKey, GeneratedChunk);
        });
    }
}

void FOldEngineLoop::GenerateChunksAroundCamera(const int RadiusInChunks)
{
    BlockCoordinate cameraBlock = {
        static_cast<int>(GCamera.GetPosition().X),
        static_cast<int>(GCamera.GetPosition().Y),
        static_cast<int>(GCamera.GetPosition().Z)
    };

    ChunkKey CameraChunkKey;
    WorldPositionToChunkKey(cameraBlock, CameraChunkKey);

    TArray<ChunkKey> ChunksAroundCamera;
    int radiusSquared = RadiusInChunks * RadiusInChunks;

    for (int dx = 0; dx <= RadiusInChunks; dx++)
    {
        for (int dz = 0; dz <= RadiusInChunks; dz++)
        {
            int distanceSquared = (dx * dx) + (dz * dz);
            if (distanceSquared <= radiusSquared)
            {
                // add chunks from all four +/- quadrants
                const ChunkKey PosPosQuadrant = {CameraChunkKey.X + dx, CameraChunkKey.Z + dz};
                const ChunkKey PosNegQuadrant = {CameraChunkKey.X + dx, CameraChunkKey.Z - dz};
                const ChunkKey NegPosQuadrant = {CameraChunkKey.X - dx, CameraChunkKey.Z + dz};
                const ChunkKey NegNegQuadrant = {CameraChunkKey.X - dx, CameraChunkKey.Z - dz};
                for (const ChunkKey& chunkKey : {PosPosQuadrant, PosNegQuadrant, NegPosQuadrant, NegNegQuadrant})
                {
                    if (ChunkLoadQueue.find(chunkKey) == ChunkLoadQueue.end())
                    {
                        ChunkLoadQueue.insert(chunkKey);
                        ChunksAroundCamera.push_back(chunkKey);
                    }
                }
            }
        }
    }

    GenerateChunksInBackground(ChunksAroundCamera);
}


void FOldEngineLoop::UpdateChunkVisibility(const Matrix& ViewMatrix, const Matrix& ProjectionMatrix)
{
    // if (GFreezeFrustumCulling) { return; }
    //
    // //VisibleChunks.clear();
    // Plane FrustumPlanes[6];
    //
    // Matrix ViewProjMatrix = ViewMatrix * ProjectionMatrix;
    //
    // Math::ExtractFrustumPlanes(ViewProjMatrix, FrustumPlanes);
    //
    // for (auto& ChunkEntry : *GWorld)
    // {
    //     const auto& ChunkKey = ChunkEntry.first;
    //
    //     BlockCoordinate ChunkOrigin;
    //     ChunkKeyToWorldPosition(ChunkKey, ChunkOrigin);
    //
    //     Vector BoundsMin = {
    //         static_cast<float>(ChunkOrigin.X),
    //         0.0f,
    //         static_cast<float>(ChunkOrigin.Z)
    //     };
    //     Vector BoundsMax = { 
    //         static_cast<float>(ChunkOrigin.X + ChunkWidth),
    //         static_cast<float>(ChunkHeight),
    //         static_cast<float>(ChunkOrigin.Z + ChunkDepth)};
    //
    //     if (Math::IsBoxInFrustum(FrustumPlanes, BoundsMin, BoundsMax))
    //     {
    //         VisibleChunks.push_back(ChunkKey);
    //     }
    // }
}

void FOldEngineLoop::HandleInput(const double DeltaTime)
{
    GWindow->ProcessMessageQueue();
    
    GPlayerController.Tick(static_cast<float>(DeltaTime));

    if (InputManager::Get().IsKeyPressed('C'))
    {
        InputManager::Get().ClearKeyPressed('C');
        TE_LOG(LogTemp, Log, TEXT("Clearing world"));
        GWorld->Clear();
        ChunkLoadQueue.clear();
    }

    if (InputManager::Get().IsKeyPressed('G'))
    {
        InputManager::Get().ClearKeyPressed('G');
        TE_LOG(LogTemp, Log, TEXT("Toggling freeze frustum culling"));
        GFreezeFrustumCulling = !GFreezeFrustumCulling;
    }

    if (InputManager::Get().IsKeyPressed('R'))
    {
        InputManager::Get().ClearKeyPressed('R');
        TE_LOG(LogTemp, Log, TEXT("Increase speed"));
        GPlayerSpeed = GPlayerSpeed * IncreaseSpeedMultiplier;
    }
    if (InputManager::Get().IsKeyPressed('F'))
    {
        InputManager::Get().ClearKeyPressed('F');
        TE_LOG(LogTemp, Log, TEXT("Decrease speed"));
        GPlayerSpeed = GPlayerSpeed / IncreaseSpeedMultiplier;
    }

    // toggle chunk debug visualization
    if (InputManager::Get().IsKeyPressed(' '))
    {
        InputManager::Get().ClearKeyPressed(' '); // todo: may need a better way to 'consume' input
        GEnableDebugTint = !GEnableDebugTint;
        TE_LOG(LogTemp, Log, TEXT("GShouldRenderTintColor: %d"), GEnableDebugTint);
    }
}

void FOldEngineLoop::UpdateCamera(const double DeltaTime)
{
    if (GPlayerSpeed < MinecraftSprintSpeed)
    {
        GPlayerSpeed = MinecraftSprintSpeed;
    }

    Vector PlayerVelocity = GPlayerController.GetMovementVector() * GPlayerSpeed;
    GCamera.SetPosition(GCamera.GetPosition() + PlayerVelocity * static_cast<float>(DeltaTime));
    GCamera.SetTarget(GCamera.GetPosition() + GPlayerController.GetForwardVector());
}

Matrix FOldEngineLoop::CreateViewMatrix()
{
    XMVECTOR Eye = XMVectorSet(GCamera.GetPosition().X, GCamera.GetPosition().Y, GCamera.GetPosition().Z, 0.0f);
    XMVECTOR At = XMVectorSet(GCamera.GetFocusPosition().X, GCamera.GetFocusPosition().Y, GCamera.GetFocusPosition().Z, 0.0f);
    XMVECTOR Up = XMVectorSet(GCamera.GetUpDirection().X, GCamera.GetUpDirection().Y, GCamera.GetUpDirection().Z, 0.0f);
    
    XMMATRIX View = XMMatrixLookAtLH(Eye, At, Up);

    return *reinterpret_cast<Matrix*>(&View);
}

Matrix FOldEngineLoop::CreateProjectionMatrix()
{
    XMMATRIX Projection = XMMatrixPerspectiveFovLH(
        Math::ConvertToRadians(GCamera.GetFieldOfViewY()),
        GCamera.GetAspectRatio(),
        GCamera.GetNearPlane(),
        GCamera.GetFarPlane());

    // return std::bit_cast<Matrix>(Projection);
    return *reinterpret_cast<Matrix*>(&Projection);
}


void FOldEngineLoop::UpdateWindowTitle(const double DeltaTime)
{
    constexpr float TimeBetweenUpdates = 0.01f;
    static float UpdateTimeRemaining = TimeBetweenUpdates;
    UpdateTimeRemaining -= static_cast<float>(DeltaTime);

    if (UpdateTimeRemaining <= 0)
    {
        UpdateTimeRemaining = TimeBetweenUpdates;
        int ChunkCount = static_cast<int>(GWorld->GetChunkCount());
        int MouseX;
        int MouseY;
        InputManager::Get().GetMousePosition(MouseX, MouseY);
        bool MouseDown = InputManager::Get().IsMouseButtonHeld(VK_LBUTTON);
        constexpr int buffer_count = 256;
        static WIDECHAR TempBuffer[buffer_count];

        auto FPS = FrameTiming::GetFPS();
        swprintf(TempBuffer, buffer_count,
                 L"Voxel Game: FPS=%.2f, Chunks=%d, DrawCalls=%d, Mouse={%d, %d}, MouseDown=%d",
                 FPS, ChunkCount, GTotalDrawCalls, MouseX, MouseY, MouseDown);

        GWindow->SetTitle(TempBuffer);
    }
}


int32 FOldEngineLoop::Init()
{
    TE_LOG(LogTemp, Log, TEXT("FOldEngineLoop::Init()"));

    InputManager::Get().ShowCursor(false);

    InitializeGraphics();

    GenerateChunksAroundCamera(ChunkDrawRadius);

    return 0;
}

void FOldEngineLoop::Tick()
{
    // update timing and framecounts
    GFrameNumber++;
    double DeltaTime = Application::GetDeltaTime();
    FrameTiming::Update(DeltaTime);

    HandleInput(DeltaTime);
    UpdateCamera(DeltaTime);

    // const Matrix ViewMatrix = CreateViewMatrix();
    // const Matrix ProjectionMatrix = CreateProjectionMatrix();

//    UpdateChunkVisibility(ViewMatrix, ProjectionMatrix);

    // set to minecraft sky blue
    // constexpr float MinecraftSkyColor[] = {0.4706f, 0.6549f, 1.0f, 1.0f};
    // GGraphicsDevice->Clear(MinecraftSkyColor);
    //
    // GTotalDrawCalls = 0;
    // DrawChunks(ViewMatrix, ProjectionMatrix, false);
    // DrawChunks(ViewMatrix, ProjectionMatrix, true);

    //GGraphicsDevice->Present(GEnableVSync);

    GRenderer.RenderScene(GWorld.get(), GCamera);

    UpdateWindowTitle(DeltaTime);
}

void FOldEngineLoop::Exit()
{
    TE_LOG(LogTemp, Log, TEXT("FOldEngineLoop::Exit()"));
}

void FOldEngineLoop::DrawChunks(const Matrix& ViewMatrix, const Matrix& ProjectionMatrix, const bool bDrawWater)
{
    // for (ChunkKey& chunkKey : VisibleChunks)
    // {
    //     BlockCoordinate ChunkOrigin;
    //     ChunkKeyToWorldPosition(chunkKey, ChunkOrigin);
    //
    //     Matrix TranslationMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
    //                                  0.0f, 1.0f, 0.0f, 0.0f,
    //                                  0.0f, 0.0f, 1.0f, 0.0f,
    //                                  static_cast<float>(ChunkOrigin.X), 0.0f, static_cast<float>(ChunkOrigin.Z), 1.0f };
    //
    //     const Matrix WorldMatrix = TranslationMatrix;
    //     const Matrix WorldViewProjectionMatrix = WorldMatrix * ViewMatrix * ProjectionMatrix;
    //     Matrix TransposedWVPMatrix = WorldViewProjectionMatrix.Transpose();
    //
    //     if (Mesh* ChunkMesh = ChunkMeshManager::GetInstance().GetChunkMesh(chunkKey))
    //     {
    //         XMMATRIX XMTransposedWVPMatrix = *reinterpret_cast<XMMATRIX*>(&TransposedWVPMatrix);
    //         
    //         GGraphicsDevice->SetConstants(XMTransposedWVPMatrix, ChunkMesh->DebugColor.x, ChunkMesh->DebugColor.y, ChunkMesh->DebugColor.z);
    //         ChunkMesh->BindToDeviceContext(GGraphicsDevice->GetDeviceContext());
    //
    //         constexpr int SolidBlocksSubMesh = 0;
    //         constexpr int WaterBlocksSubMesh = 1;
    //
    //         if (bDrawWater)
    //         {
    //             GGraphicsDevice->DisableDepthWrite();
    //             if (ChunkMesh->SubMeshes[WaterBlocksSubMesh].indexCount > 0)
    //             {
    //                 ChunkMesh->DrawSubMesh(GGraphicsDevice->GetDeviceContext(), WaterBlocksSubMesh);
    //             }
    //         }
    //         else
    //         {
    //             GGraphicsDevice->EnableDepthWrite();
    //             ChunkMesh->DrawSubMesh(GGraphicsDevice->GetDeviceContext(), SolidBlocksSubMesh);
    //         }
    //
    //         GTotalDrawCalls++;
    //     }
    // }
}


// void FOldEngineLoop::GenerateChunk(const ChunkKey Key, ChunkRef Chunk)
// {
//     ChunkUtils::TFillChunkSubvolume(Chunk->Blocks,
//                                     0,
//                                     ChunkWidth, ChunkHeight, ChunkDepth,
//                                     Key.X * ChunkWidth, 0, Key.Z * ChunkDepth,
//                                     [&](auto* block_states, int index, int world_x, int world_y,
//                                         int world_z)
//                                     {
//                                         const Block block_type1 = biome.GetBlock(
//                                             world_x, world_y, world_z);
//
//                                         // set only non-air blocks (to prevent drawing air blocks over something that was drawn to the chunk previously)
//                                         if (!block_type1.IsAir())
//                                         {
//                                             block_states[index] = block_type1;
//                                         }
//                                     });
// }

FOldEngineLoop GOldEngineLoop;
