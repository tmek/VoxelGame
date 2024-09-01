// // Copyright (c) 2024 Thomas. All rights reserved.

#pragma once

#include <d3d11.h>
#include <vector>

#include "HAL/Platform.h"

#include "Math/Vector.h"


struct CORE_API LineVertex
{
    Vector Position;
    float Color[4];
};

class CORE_API DebugLines
{
public:

    static void Clear() { LineVertices.clear(); }

    static void DrawLine(const Vector& start, const Vector& end, const float color[4]);

    static void DrawBox(const Vector& center, const Vector& extents, const float color[4]);

    static void UpdateVertexBuffer(ID3D11Device* Device);

    static void Draw(ID3D11DeviceContext* DeviceContext);

    static void DrawChunkBounds(int32 ChunkX, int32 ChunkZ);

    // void DrawArrow(const Vector& start, const Vector& end, float radius, const Vector& color);
    // void DrawCoordinateSystem(const Vector& origin, const Quaternion& orientation, float size);
    //
    // void DrawFrustum(const Frustum& frustum, const Vector& color);
    // void DrawSphere(const Vector& center, float radius, const Vector& color);
    // void DrawCylinder(const Vector& start, const Vector& end, float radius, const Vector& color);
    // void DrawCone(const Vector& start, const Vector& end, float radius, const Vector& color);

private:

    static std::vector<LineVertex> LineVertices;
    static ID3D11Buffer* VertexBuffer;
};
