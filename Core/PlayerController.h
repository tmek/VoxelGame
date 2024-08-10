#pragma once

#include "DirectXMath.h"

class PlayerController
{
public:
    void ProcessInput(float DeltaTime);

    DirectX::XMVECTOR GetForwardVector();
    DirectX::XMVECTOR GetRightVector();
    DirectX::XMVECTOR GetUpVector();
    // movement vector
    DirectX::XMVECTOR MovementVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Yaw in degrees
    float Yaw;
};
