// Copyright Voxel Games, Inc. All Rights Reserved.

#include "VoxelWorldRenderer.h"

#include "BoxMeshBuilder.h"
#include "CubeMesh.h"

#include "HAL/PlatformWindow.h"

#include "Math/Math.h"

#include "RHI/GraphicsDevice.h"

#include "Utils/ChunkUtils.h"

#include "VoxelMisc/ChunkMeshManager.h"

#define PRINT_MATRIX(x) PrintMatrix(x, TEXT(_CRT_STRINGIZE(x)))


void PrintMatrix(const Matrix& M, const TCHAR* Name = TEXT("Matrix"))
{
    TE_LOG(LogTemp, Log, TEXT("%s: "), Name);
    
    for (int Row = 0; Row < 4; ++Row)
    {
        TE_LOG(LogTemp, Log, TEXT("%10.4f %10.4f %10.4f %10.4f"), 
               M.M[Row][0],
               M.M[Row][1],
               M.M[Row][2],
               M.M[Row][3]);
    }
}

void PrintMatrix(const XMMATRIX& M, const TCHAR* Name = TEXT("Matrix"))
{
    TE_LOG(LogTemp, Log, TEXT("%s: "), Name);

    for (int Row = 0; Row < 4; ++Row)
    {
        TE_LOG(LogTemp, Log, TEXT("%10.4f %10.4f %10.4f %10.4f"), 
            M.r[Row].m128_f32[0],
            M.r[Row].m128_f32[1],
            M.r[Row].m128_f32[2],
            M.r[Row].m128_f32[3]);
    }
}

void VoxelWorldRenderer::Initialize(void* WindowHandle)
{
    GraphicsDevice_ = new GraphicsDevice(WindowHandle);
    assert(Graphics->IsValid());

    BoxMeshBuilder boxMeshBuilder;
    BoxMesh_ = boxMeshBuilder.Build(GraphicsDevice_->GetDevice());

    CubeMesh_ = new CubeMesh(GraphicsDevice_->GetDevice(), GraphicsDevice_->GetDeviceContext());

    CubeMesh_->SetShaders(); // sets general shaders
    CubeMesh_->Select();     // sets general input layout and topology
}

void VoxelWorldRenderer::CreateChunkMesh(const ChunkKey& Key, const ChunkRef ChunkData)
{
    ChunkMeshManager::GetInstance().RebuildChunkMesh(Key, ChunkData, GraphicsDevice_->GetDevice());
}

void VoxelWorldRenderer::DeleteChunkMesh(const ChunkKey& Key)
{
    ChunkMeshManager::GetInstance().DeleteChunkMesh(Key);
}

void VoxelWorldRenderer::RenderScene(const VoxelWorld* World, const Camera& Camera)
{
    CurrentWorld_ = World;
    CurrentCamera_ = Camera;

    BeginFrame();
    UpdateVisibility();

    RenderDepthPrePass();
    RenderMainPass();
    
    EndFrame();
}

void VoxelWorldRenderer::UpdateVisibility()
{
    if (GFreezeFrustumCulling) { return; }

    VisibleChunks_.clear();
    Plane FrustumPlanes[6];

    Matrix View = Matrix::LookAtLH(CurrentCamera_.GetPosition(), CurrentCamera_.GetFocusPosition(), CurrentCamera_.GetUpDirection());
    Matrix Proj = Matrix::PerspectiveFovLH(
        Math::ConvertToRadians(CurrentCamera_.GetFieldOfViewY()),
        CurrentCamera_.GetAspectRatio(), CurrentCamera_.GetNearPlane(), CurrentCamera_.GetFarPlane());

    Matrix ViewProjMatrix = View * Proj;
    Math::ExtractFrustumPlanes(ViewProjMatrix, FrustumPlanes);

    for (const auto& ChunkEntry : *CurrentWorld_)
    {
        const auto& ChunkKey = ChunkEntry.first;

        BlockCoordinate ChunkOrigin;
        ChunkKeyToWorldPosition(ChunkKey, ChunkOrigin);

        Vector BoundsMin = {
            static_cast<float>(ChunkOrigin.X),
            0.0f,
            static_cast<float>(ChunkOrigin.Z)
        };
        Vector BoundsMax = {
            static_cast<float>(ChunkOrigin.X + ChunkWidth),
            static_cast<float>(ChunkHeight),
            static_cast<float>(ChunkOrigin.Z + ChunkDepth)};

        if (Math::IsBoxInFrustum(FrustumPlanes, BoundsMin, BoundsMax))
        {
            VisibleChunks_.push_back(ChunkKey);
        }
    }
}

void VoxelWorldRenderer::RenderDepthPrePass()
{
    GraphicsDevice_->SetPreDepthPrePassStates();
    
    // Render all opaque geometry
    RenderOpaqueGeometry();    
}

void VoxelWorldRenderer::RenderMainPass()
{
}

void VoxelWorldRenderer::RenderOpaqueGeometry()
{
    DrawChunks(0);
}

void VoxelWorldRenderer::RenderTransparentGeometry()
{
    DrawChunks(1);
}

void VoxelWorldRenderer::BeginFrame()
{
    GTotalDrawCalls = 0;
    constexpr float MinecraftSkyColor[] = {0.4706f, 0.6549f, 1.0f, 1.0f};
    GraphicsDevice_->Clear(MinecraftSkyColor);
}

void VoxelWorldRenderer::DrawChunks(int PassIndex)
{
     for (const ChunkKey& ChunkKey : VisibleChunks_)
     {
        BlockCoordinate ChunkOrigin;
        ChunkKeyToWorldPosition(ChunkKey, ChunkOrigin);

        Matrix ChunkTranslation = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            static_cast<float>(ChunkOrigin.X), 0.0f, static_cast<float>(ChunkOrigin.Z), 1.0f
        };

        float FovYInRadians = Math::ConvertToRadians(CurrentCamera_.GetFieldOfViewY());
        float AspectRatio = CurrentCamera_.GetAspectRatio();
        float NearZ = CurrentCamera_.GetNearPlane();
        float FarZ = CurrentCamera_.GetFarPlane();          
        

        const Matrix WorldMatrix = ChunkTranslation;
        Matrix ViewMatrixLH = Matrix::LookAtLH(CurrentCamera_.GetPosition(), CurrentCamera_.GetFocusPosition(), CurrentCamera_.GetUpDirection());
        const Matrix ProjectionMatrixLH = Matrix::PerspectiveFovLH(FovYInRadians, AspectRatio, NearZ, FarZ);
        
        const Matrix WorldViewProjectionMatrix = WorldMatrix * ViewMatrixLH * ProjectionMatrixLH;

        Matrix TransposedWVPMatrix = WorldViewProjectionMatrix.Transpose(); // LookAtLH does a transpose and we do one here too, why?

        if (Mesh* ChunkMesh = ChunkMeshManager::GetInstance().GetChunkMesh(ChunkKey))
        {
            // set constants
            XMMATRIX& XMTransposedWVPMatrix = reinterpret_cast<XMMATRIX&>(TransposedWVPMatrix);
            GraphicsDevice_->UpdateVertexShaderConstantBuffer(XMTransposedWVPMatrix, ChunkMesh->DebugColor.x, ChunkMesh->DebugColor.y, ChunkMesh->DebugColor.z);

            // bind mesh
            ChunkMesh->BindToDeviceContext(GraphicsDevice_->GetDeviceContext());

            constexpr int SolidBlocksSubMesh = 0;
            constexpr int WaterBlocksSubMesh = 1;

            if(PassIndex == 1)
            {
                // draw water blocks submesh
                if (ChunkMesh->SubMeshes[WaterBlocksSubMesh].indexCount > 0)
                {
                    GraphicsDevice_->SetTransparencyPassState();
                    ChunkMesh->DrawSubMesh(GraphicsDevice_->GetDeviceContext(), WaterBlocksSubMesh);
                }
            }
            else
            {
                // draw solid blocks submesh
                GraphicsDevice_->SetOpaquePassState();
                ChunkMesh->DrawSubMesh(GraphicsDevice_->GetDeviceContext(), SolidBlocksSubMesh);
            }
            
            GTotalDrawCalls++;
        }
    }
}

void VoxelWorldRenderer::EndFrame()
{
    GraphicsDevice_->Present(GEnableVSync);
}
