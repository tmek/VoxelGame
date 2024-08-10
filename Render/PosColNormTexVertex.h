#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct PosColNormTexVertex {
    XMFLOAT3 Position;
    XMFLOAT4 Color;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
};
