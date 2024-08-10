#include "PlayerController.h"

#include "Input/InputManager.h"


void PlayerController::ProcessInput(float DeltaTime)
{
    InputManager& Input = InputManager::Get();

    MovementVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    
    if(Input.IsKeyHeld('W'))
    {
        MovementVector = DirectX::XMVectorAdd(MovementVector, GetForwardVector());
    }
    if(Input.IsKeyHeld('S'))
    {
        MovementVector = DirectX::XMVectorSubtract(MovementVector, GetForwardVector());
    }
    if(Input.IsKeyHeld('A'))
    {
        MovementVector = DirectX::XMVectorSubtract(MovementVector, GetRightVector());
    }
    if(Input.IsKeyHeld('D'))
    {
        MovementVector = DirectX::XMVectorAdd(MovementVector, GetRightVector());
    }

    // up
    if(Input.IsKeyHeld('E'))
    {
        MovementVector = DirectX::XMVectorAdd(MovementVector, GetUpVector());
    }
    // down
    if(Input.IsKeyHeld('Q'))
    {
        MovementVector = DirectX::XMVectorSubtract(MovementVector, GetUpVector());
    }

    // turn left
    float TurnSpeed = 90.0f; // degrees per second
    
    if(Input.IsKeyHeld('J') || Input.IsKeyHeld(VK_LEFT))
    {
        Yaw -= TurnSpeed * DeltaTime;
    }
    // turn right
    if(Input.IsKeyHeld('L') || Input.IsKeyHeld(VK_RIGHT))
    {
        Yaw += TurnSpeed * DeltaTime;
    }
}

DirectX::XMVECTOR PlayerController::GetForwardVector()
{
    DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(Yaw));
    return DirectX::XMVector3Transform(Forward, RotationMatrix);
}

DirectX::XMVECTOR PlayerController::GetRightVector()
{
    DirectX::XMVECTOR Right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(Yaw));
    return DirectX::XMVector3Transform(Right, RotationMatrix);
}

DirectX::XMVECTOR PlayerController::GetUpVector()
{
    DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    //DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(Yaw));
    //return DirectX::XMVector3Transform(Up, RotationMatrix);
    return Up;
}
