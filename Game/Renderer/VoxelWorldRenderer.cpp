// Copyright Thomas All Rights Reserved.

#include "VoxelWorldRenderer.h"

#include "BoxMeshBuilder.h"
#include "CubeMesh.h"

#include "HAL/PlatformWindow.h"

#include "Math/Math.h"

#include "RHI/DebugLines.h"
#include "RHI/DynamicRHI.h"

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
    GraphicsDevice_ = new DynamicRHI(WindowHandle);
    assert(GraphicsDevice_->IsValid());

    GraphicsDevice_->BeginPass(L"VoxelWorldRenderer:Init");

    BoxMeshBuilder boxMeshBuilder;
    BoxMesh_ = boxMeshBuilder.Build(GraphicsDevice_->GetDevice());

    CubeMesh_ = new CubeMesh(GraphicsDevice_->GetDevice(), GraphicsDevice_->GetDeviceContext());

    CubeMesh_->SetShaders(); // sets general shaders
    CubeMesh_->Select();     // sets general input layout and topology

    GraphicsDevice_->EndPass();
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

    // todo: need way to easily configure to use depth pre-pass or not

    if (GUseDepthPass)
    {
        GraphicsDevice_->BeginPass(L"VoxelGame:DepthPrePass");
        RenderDepthPrePass();
        GraphicsDevice_->EndPass();
    }

    GraphicsDevice_->BeginPass(L"VoxelGame:OpaquePass");
    RenderOpaquePass();
    GraphicsDevice_->EndPass();

    RenderDebugLines();

    GraphicsDevice_->BeginPass(L"VoxelGame:TransparencyPass");
    RenderTransparencyPass();
    GraphicsDevice_->EndPass();

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
        ChunkKeyToWorld(ChunkKey, ChunkOrigin);

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
    DrawChunks(0);
}

void VoxelWorldRenderer::RenderOpaquePass()
{
    GraphicsDevice_->SetOpaquePassState();
    DrawChunks(0);
}

void VoxelWorldRenderer::RenderTransparencyPass()
{
    GraphicsDevice_->SetTransparencyPassState();
    DrawChunks(1);
}

void VoxelWorldRenderer::RenderDebugLines()
{
    Matrix TransposedViewProj = (ViewMatrixLH * ProjectionMatrixLH).Transpose();
    XMMATRIX& XMTransposedWVPMatrix = reinterpret_cast<XMMATRIX&>(TransposedViewProj);
    GraphicsDevice_->UpdateVertexShaderConstantBuffer(XMTransposedWVPMatrix, 1.0f, 1.0f, 1.0f);

    // get camera chunk
    Vector CameraPosition = CurrentCamera_.GetPosition();
    BlockCoordinate CameraBlock = {
        static_cast<int>(floor(CameraPosition.X)),
        static_cast<int>(floor(CameraPosition.Y)), 
        static_cast<int>(floor(CameraPosition.Z))
    };
    ChunkKey CameraChunkKey;
    WorldToChunkKey(CameraBlock, CameraChunkKey);

    DebugLines::Clear();
    DebugLines::DrawChunkBounds(CameraChunkKey.X, CameraChunkKey.Z);



    DebugLines::UpdateVertexBuffer(GraphicsDevice_->GetDevice());
    DebugLines::Draw(GraphicsDevice_->GetDeviceContext());
}

void VoxelWorldRenderer::BeginFrame()
{
    GTotalDrawCalls = 0;
    constexpr float MinecraftSkyColor[] = {0.4706f, 0.6549f, 1.0f, 1.0f};
    GraphicsDevice_->Clear(MinecraftSkyColor);


    float FovYInRadians = Math::ConvertToRadians(CurrentCamera_.GetFieldOfViewY());
    float AspectRatio = CurrentCamera_.GetAspectRatio();
    float NearZ = CurrentCamera_.GetNearPlane();
    float FarZ = CurrentCamera_.GetFarPlane();

    ViewMatrixLH = Matrix::LookAtLH(CurrentCamera_.GetPosition(), CurrentCamera_.GetFocusPosition(), CurrentCamera_.GetUpDirection());
    ProjectionMatrixLH = Matrix::PerspectiveFovLH(FovYInRadians, AspectRatio, NearZ, FarZ);
}

void VoxelWorldRenderer::DrawChunks(int PassIndex)
{
    for (const ChunkKey& ChunkKey : VisibleChunks_)
    {
        BlockCoordinate ChunkOrigin;
        ChunkKeyToWorld(ChunkKey, ChunkOrigin);

        Matrix ChunkTranslation = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            static_cast<float>(ChunkOrigin.X), 0.0f, static_cast<float>(ChunkOrigin.Z), 1.0f
        };

        if (Mesh* ChunkMesh = ChunkMeshManager::GetInstance().GetChunkMesh(ChunkKey))
        {
            // set constants
            const Matrix WorldMatrix = ChunkTranslation;
            const Matrix WorldViewProjectionMatrix = WorldMatrix * ViewMatrixLH * ProjectionMatrixLH;
            Matrix TransposedWVPMatrix = WorldViewProjectionMatrix.Transpose(); // LookAtLH does a transpose and we do one here too, why?
            XMMATRIX& XMTransposedWVPMatrix = reinterpret_cast<XMMATRIX&>(TransposedWVPMatrix);
            GraphicsDevice_->UpdateVertexShaderConstantBuffer(XMTransposedWVPMatrix, ChunkMesh->DebugColor.x, ChunkMesh->DebugColor.y, ChunkMesh->DebugColor.z);

            // bind mesh
            ChunkMesh->BindToDeviceContext(GraphicsDevice_->GetDeviceContext());

            constexpr int SolidBlocksSubMesh = 0;
            constexpr int WaterBlocksSubMesh = 1;

            if (PassIndex == 1)
            {
                // draw water blocks submesh
                if (ChunkMesh->SubMeshes[WaterBlocksSubMesh].indexCount > 0)
                {
                    ChunkMesh->DrawSubMesh(GraphicsDevice_->GetDeviceContext(), WaterBlocksSubMesh);
                }
            }
            else
            {
                // draw solid blocks submesh
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
