// copyright

#include "RHI/DebugLines.h"

#include <DirectXColors.h>

#include "Misc/AssertionMacros.h"


ID3D11Buffer* DebugLines::VertexBuffer = nullptr;

std::vector<LineVertex> DebugLines::LineVertices;


void DebugLines::DrawLine(const Vector& start, const Vector& end, const float color[4])
{
    float FinalColor[4] = {color[0], color[1], color[2], -1.0f};

    LineVertex a = {start, {FinalColor[0], FinalColor[1], FinalColor[2], FinalColor[3]}};
    LineVertex b = {end, {FinalColor[0], FinalColor[1], FinalColor[2], FinalColor[3]}};

    LineVertices.emplace_back(a);
    LineVertices.emplace_back(b);
}

void DebugLines::DrawBox(const Vector& center, const Vector& extents, const float color[4])
{
    // Calculate the 8 corners of the box based on the center and extents
    Vector min = center - extents;
    Vector max = center + extents;

    Vector corners[8] =
    {
        Vector(min.X, min.Y, min.Z),
        Vector(max.X, min.Y, min.Z),
        Vector(max.X, max.Y, min.Z),
        Vector(min.X, max.Y, min.Z),
        Vector(min.X, min.Y, max.Z),
        Vector(max.X, min.Y, max.Z),
        Vector(max.X, max.Y, max.Z),
        Vector(min.X, max.Y, max.Z)
    };

    // Draw the 12 edges of the box
    DrawLine(corners[0], corners[1], color);
    DrawLine(corners[1], corners[2], color);
    DrawLine(corners[2], corners[3], color);
    DrawLine(corners[3], corners[0], color);

    DrawLine(corners[4], corners[5], color);
    DrawLine(corners[5], corners[6], color);
    DrawLine(corners[6], corners[7], color);
    DrawLine(corners[7], corners[4], color);

    DrawLine(corners[0], corners[4], color);
    DrawLine(corners[1], corners[5], color);
    DrawLine(corners[2], corners[6], color);
    DrawLine(corners[3], corners[7], color);
}

void DebugLines::UpdateVertexBuffer(ID3D11Device* Device)
{
    // Release the old vertex buffer
    if (VertexBuffer)
    {
        VertexBuffer->Release();
    }

    // create the new vertex buffer and load it with line vertices
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<UINT>(LineVertices.size() * sizeof(LineVertex));
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = LineVertices.data();

    HRESULT HR = Device->CreateBuffer(&bufferDesc, &initData, &VertexBuffer);
    check(SUCCEEDED(HR));
}

void DebugLines::Draw(ID3D11DeviceContext* DeviceContext)
{
    UINT stride = sizeof(LineVertex);
    UINT offset = 0;
    DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    DeviceContext->Draw(static_cast<UINT>(LineVertices.size()), 0);
}

void DebugLines::DrawChunkBounds(int32 ChunkX, int32 ChunkZ)
{
    int32 ChunkWidth = 16;  // todo: can't leave these constants here. move this method into game or VoxelCore, "Core" should be voxel agnostic
    int32 ChunkHeight = 384;

    float XOffset = static_cast<float>(ChunkX * ChunkWidth);
    float ZOffset = static_cast<float>(ChunkZ * ChunkWidth);

    float Blue[4] = {63.f / 256.f, 63.f / 256.f, 1.0f, 1.0f};
    float Yellow[4] = {1.0f, 1.0f, 0.0f, 1.0f};
    float Cyan[4] = {0.0f, 155.f / 256.f, 155.f / 256.f, 1.0f};
    float Red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    float* LineColor;

    // draw chunk vertical lines of (0,0) chunk
    for (int LineIndex = 0; LineIndex <= ChunkWidth; LineIndex += 2)
    {
        switch (LineIndex)
        {
        case 0:
        case 16:
            LineColor = Blue;
            break;
        case 4:
        case 8:
        case 12:
            LineColor = Cyan;
            break;
        default:
            LineColor = Yellow;
        }

        float i = static_cast<float>(LineIndex);

        DrawLine({XOffset + 0, 0.0f, ZOffset + i}, {XOffset + 0.0f, 384.0f, ZOffset + i}, LineColor);
        DrawLine({XOffset + 16.0f, 0.0f, ZOffset + i}, {XOffset + 16, 384.0f, ZOffset + i}, LineColor);
        DrawLine({XOffset + i, 0.0f, ZOffset + 0}, {XOffset + i, 384.0f, ZOffset + 0}, LineColor);
        DrawLine({XOffset + i, 0.0f, ZOffset + 16}, {XOffset + i, 384.0f, ZOffset + 16}, LineColor);
    }

    // draw chunk horizontal lines
    for (int LineIndex = 0; LineIndex <= ChunkHeight; LineIndex += 2)
    {
        LineColor = Yellow;
        if ((LineIndex + 8) % 16 == 0)
        {
            LineColor = Cyan;
        }
        if ((LineIndex) % 16 == 0)
        {
            LineColor = Blue;
        }

        float y = static_cast<float>(LineIndex);
        DrawLine({XOffset + 0.0f, y, ZOffset + 0}, {XOffset + 16, y, ZOffset + 0}, LineColor);
        DrawLine({XOffset + 16, y, ZOffset + 0}, {XOffset + 16, y, ZOffset + 16}, LineColor);
        DrawLine({XOffset + 16, y, ZOffset + 16}, {XOffset + 0, y, ZOffset + 16}, LineColor);
        DrawLine({XOffset + 0, y, ZOffset + 16}, {XOffset + 0, y, ZOffset + 0}, LineColor);
    }

    LineColor = Red;
    // draw neighbor chunk lines
    for (int NCX = -1; NCX <= 2; NCX++)
    {
        for (int NCZ = -1; NCZ <= 2; NCZ++)
        {
            if (NCX > -1 && NCX < 2 && NCZ > -1 && NCZ < 2)
            {
                continue; // skip inner lines 
            }

            float XOffset = static_cast<float>((ChunkX + NCX) * ChunkWidth);
            float ZOffset = static_cast<float>((ChunkZ + NCZ) * ChunkWidth);

            float Height = static_cast<float>(ChunkHeight);
            DrawLine({XOffset, 0.0f, ZOffset}, {XOffset, Height, ZOffset}, LineColor);
            DrawLine({XOffset, 0.0f, ZOffset}, {XOffset, Height, ZOffset}, LineColor);
            DrawLine({XOffset, 0.0f, ZOffset}, {XOffset, Height, ZOffset}, LineColor);
            DrawLine({XOffset, 0.0f, ZOffset}, {XOffset, Height, ZOffset}, LineColor);
        }
    }
}
