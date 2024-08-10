#include "ChunkMesh.h"

#include "BlockMeshBuilder.h"
#include "Input/InputManager.h"
#include "Voxel/Chunk.h"

#include "DirectXMath.h"
#include "GraphicsDevice.h"
#include "MeshBuilder.h"
#include "Voxel/World.h"

ChunkMesh::ChunkMesh()
{
}


ChunkMesh::~ChunkMesh()
{
}

void ChunkMesh::BuildMesh(Chunk& chunk, ID3D11Device* device)
{
    MeshBuilder<MeshVertex> builder(device);
    builder.Clear();

    int width = chunk.Width;
    int height = chunk.Height;
    int depth = chunk.Depth;

    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            for (int z = 0; z < depth; ++z)
            {
                Block block = chunk.GetBlock(x, y, z);
                if (!block.IsAirBlock())
                {
                    AddBlock(x, y, z, block.Type, chunk, builder);
                }
            }

    m_mesh = builder.ToMesh();
    CreateBuffers(device);
}

void ChunkMesh::AddBlock(int xi, int yi, int zi, int blockType, Chunk& chunk, MeshBuilder<MeshVertex>& builder)
{
    int wx = chunk.GetWorldOffsetX() + xi;
    int wz = chunk.GetWorldOffsetZ() + zi;

    float x = static_cast<float>(xi);
    float y = static_cast<float>(yi);
    float z = static_cast<float>(zi);

    // include face if adjacent block is air
    bool IncludeNegZ = GWorld.GetBlock(wx, yi, wz - 1).Type == 0; // front
    bool IncludePosZ = GWorld.GetBlock(wx, yi, wz + 1).Type == 0; // back
    bool IncludeNegX = GWorld.GetBlock(wx - 1, yi, wz).Type == 0; // left
    bool IncludePosX = GWorld.GetBlock(wx + 1, yi, wz).Type == 0; // right
    bool IncludePosY = GWorld.GetBlock(wx, yi + 1, wz).Type == 0; // top
    bool IncludeNegY = GWorld.GetBlock(wx, yi - 1, wz).Type == 0; // bottom

    static DirectX::XMFLOAT4 TintColors[] = {
        {0.5f, 0.5f, 0.5f, 1.0f}, // stone
        {0.55f, 0.27f, 0.07f, 1.0f}, // dirt
        {0.3f, 0.6f, 0.2f, 1.0f}, // grass
        {0.0f, 0.5f, 1.0f, 1.0f}, // water
        {0.2f, 0.2f, 0.2f, 1.0f} // bedrock
    };

    XMFLOAT4 BlockColor = TintColors[blockType - 1];

    BlockMeshBuilder blockMeshBuilder(builder);
    blockMeshBuilder.SetOffset(x, y, z);
    blockMeshBuilder.SetColor(BlockColor.x, BlockColor.y, BlockColor.z, BlockColor.w);

    if(IncludePosX) blockMeshBuilder.AppendPosXFace();
    if(IncludePosY) blockMeshBuilder.AppendPosYFace();
    if(IncludePosZ) blockMeshBuilder.AppendPosZFace();
    if(IncludeNegX) blockMeshBuilder.AppendNegXFace();
    if(IncludeNegY) blockMeshBuilder.AppendNegYFace();
    if(IncludeNegZ) blockMeshBuilder.AppendNegZFace();
}


void ChunkMesh::CreateBuffers(ID3D11Device* device)
{
    // Create constant buffer
    // todo: belongs elsewhere
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(MyConstantBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
    if (FAILED(hr))
    {
        assert(false);
        //std::cerr << "Failed to create constant buffer: " << std::hex << hr << std::endl;
        return;
    }

    TintR = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    TintG = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    TintB = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

extern bool GShouldRenderTintColor;

static bool ShouldRenderTintColor = true;


void ChunkMesh::Render(const XMMATRIX& viewProjMatrix, ID3D11DeviceContext* context)
{
    // Ensure the buffers are set up correctly
    if (!m_mesh.IsValid())
    {
        return;
    }

    // update constant buffer
    MyConstantBuffer cb;
    cb.WorldViewProjection = viewProjMatrix;
    if (GShouldRenderTintColor)
    {
        cb.TintColor = {TintR, TintG, TintB, 1.0f};
    }
    else
    {
        cb.TintColor = {0.0f, 0.0f, 0.0f, 1.0f};
    }

    context->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
    context->VSSetConstantBuffers(0, 1, &constantBuffer);
    //context->VSSetConstantBuffers(0, 1, constantBuffer.get);    

    // Draw the mesh
    m_mesh.SetupPipeline(context);
    // UINT stride = sizeof(Vertex);
    // UINT offset = 0;
    // context->IASetVertexBuffers(0, 1, &m_mesh.vertexBuffer, &stride, &offset);
    // context->IASetIndexBuffer(m_mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    // context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Ensure the input layout is set
    // context->IASetInputLayout(yourInputLayout);

    // Ensure the shaders are set
    // context->VSSetShader(yourVertexShader, nullptr, 0);
    // context->PSSetShader(yourPixelShader, nullptr, 0);

    // Draw the indexed geometry
    context->DrawIndexed(m_mesh.indexCount, 0, 0);
}
