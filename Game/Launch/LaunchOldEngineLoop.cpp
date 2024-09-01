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
constexpr auto MinecraftSprintSpeedInMetersPerSecond = 5.612f;
constexpr float IncreaseSpeedMultiplier = 2;
constexpr int ChunkDrawRadius = 32;
Vector PlayerVelocity = Vector::ZeroVector;

// globals
ThreadPool GThreadPool(std::thread::hardware_concurrency() / 2);

PlatformWindow* GWindow = nullptr;
VoxelWorldRenderer GRenderer;

// GraphicsDevice* GGraphicsDevice = nullptr;
// CubeMesh* GCubeMesh = nullptr;
// Mesh GBoxMesh;
// TArray<ChunkKey> VisibleChunks;


std::shared_ptr<VoxelWorld> GWorld = std::make_unique<VoxelWorld>();

std::unordered_set<ChunkKey, ChunkKeyHash> ChunkLoadQueue;

float GPlayerSpeed = MinecraftSprintSpeedInMetersPerSecond;
PlayerController GPlayerController;
Camera StartingCameraState =  Camera(Vector(0.0f, 100.0f, 0.0f),
                        Vector::ForwardVector,
                        70.0f,
                        16.0f / 10.0f,
                        0.1f,
                        10000.0f);

Camera GCamera = StartingCameraState;


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
    PlatformWindow::WindowOptions options;
    options.Width = 800;
    options.Height = 600;
    GWindow = new PlatformWindow();
    GRenderer.Initialize(GWindow->GetHandle());
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
            if (GIsRequestingExit)
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
    WorldToChunkKey(cameraBlock, CameraChunkKey);

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

void FOldEngineLoop::HandleInput(const double DeltaTime)
{
    GWindow->ProcessMessageQueue();

    GPlayerController.Tick(static_cast<float>(DeltaTime));

    if (InputManager::Get().IsKeyPressed('O'))
    {
        InputManager::Get().ClearKeyPressed('O');
        GCamera = StartingCameraState;
        TE_LOG(LogTemp, Log, TEXT("Camera reset to world origin"));
    }

    if (InputManager::Get().IsKeyPressed('V'))
    {
        InputManager::Get().ClearKeyPressed('V');
        GEnableVSync = !GEnableVSync;
        TE_LOG(LogTemp, Log, TEXT("Toggle VSync"));
    }

    
    // if (InputManager::Get().IsKeyPressed('L'))
    // {
    //     InputManager::Get().ClearKeyPressed('L');
    //     //GCamera = StartingCameraState;
    //     TE_LOG(LogTemp, Log, TEXT("Camera reset to world origin"));
    // }

    
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
    if (InputManager::Get().IsKeyPressed('Q'))
    {
        InputManager::Get().ClearKeyPressed('Q'); // todo: may need a better way to 'consume' input
        GEnableDebugTint = !GEnableDebugTint;
        TE_LOG(LogTemp, Log, TEXT("GShouldRenderTintColor: %d"), GEnableDebugTint);
    }
}

void FOldEngineLoop::UpdateCamera(const double DeltaTime)
{
    if (GPlayerSpeed < MinecraftSprintSpeedInMetersPerSecond)
    {
        GPlayerSpeed = MinecraftSprintSpeedInMetersPerSecond;
    }

    const float SpeedInMetersPerSecondSquared = GPlayerSpeed * GPlayerSpeed; 
    const float Acceleration = SpeedInMetersPerSecondSquared;
    const float Friction = SpeedInMetersPerSecondSquared;

    Vector UnitMovementDirectionInMeters = GPlayerController.GetMovementVector(); 

    // apply acceleration
    PlayerVelocity = PlayerVelocity + UnitMovementDirectionInMeters * Acceleration * DeltaTime;

    float timescale = 0.2f;
    float scale = exp(-DeltaTime / timescale);
    PlayerVelocity = PlayerVelocity * scale;
        
    //float PlayerSpeed = PlayerVelocity.GetMagnitude();

    Vector PlayerPosition = GCamera.GetPosition() + PlayerVelocity * DeltaTime;
    
    GCamera.SetPosition(PlayerPosition);
    GCamera.SetTarget(GCamera.GetPosition() + GPlayerController.GetForwardVector());
}


void FOldEngineLoop::UpdateWindowTitle(const double DeltaTime)
{
    constexpr float TimeBetweenUpdates = 0.1f;
    static float UpdateTimeElapsed = 0;
    UpdateTimeElapsed += static_cast<float>(DeltaTime);

    if (UpdateTimeElapsed >= TimeBetweenUpdates)
    {
        UpdateTimeElapsed = 0;
        
        int ChunkCount = static_cast<int>(GWorld->GetChunkCount());
        int MouseX;
        int MouseY;
        InputManager::Get().GetMousePosition(MouseX, MouseY);
        bool MouseDown = InputManager::Get().IsMouseButtonHeld(VK_LBUTTON);
        constexpr int buffer_count = 256;
        static WIDECHAR TempBuffer[buffer_count];

        const TCHAR* PlatformName = PlatformProcess::GetPlatformName();
        
        auto FPS = FrameTiming::GetFPS();
        swprintf(TempBuffer, buffer_count,
                 L"Voxel Game: FPS=%.2f, Chunks=%d, DrawCalls=%d, Mouse={%d, %d}, MouseDown=%d, VSync=%d, Platform=%s",
                 FPS, ChunkCount, GTotalDrawCalls, MouseX, MouseY, MouseDown, GEnableVSync, PlatformName);

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

    GRenderer.RenderScene(GWorld.get(), GCamera);

    UpdateWindowTitle(DeltaTime);
}

void FOldEngineLoop::Exit()
{
    TE_LOG(LogTemp, Log, TEXT("FOldEngineLoop::Exit()"));
}

FOldEngineLoop GOldEngineLoop;
