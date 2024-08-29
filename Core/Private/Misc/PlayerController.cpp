// Copyright

#include "Misc/PlayerController.h"

#include "Input/InputManager.h"

#include "Math/Math.h"


constexpr float MaxPitch = 89.0f; 


void PlayerController::Tick(const float DeltaTime)
{
    InputManager& Input = InputManager::Get();

    MovementVector_ = Vector::ZeroVector;

    // move forward
    if (Input.IsKeyHeld('W'))
    {
        MovementVector_ = MovementVector_ + GetForwardVector();
    }

    // move backward
    if (Input.IsKeyHeld('S'))
    {
        MovementVector_ = MovementVector_ - GetForwardVector();
    }

    // move left
    if (Input.IsKeyHeld('A'))
    {
        MovementVector_ = MovementVector_ - GetRightVector();
    }

    // move right
    if (Input.IsKeyHeld('D'))
    {
        MovementVector_ = MovementVector_ + GetRightVector();
    }

    // move up
    if (Input.IsKeyHeld('E'))
    {
        MovementVector_ = MovementVector_ + GetUpVector();
    }

    // move down
    if (Input.IsKeyHeld('Q'))
    {
        MovementVector_ = MovementVector_ - GetUpVector();
    }

    // turn left
    float TurnSpeed = 90.0f; // degrees per second

    if (Input.IsKeyHeld('J') || Input.IsKeyHeld(VK_LEFT))
    {
        Yaw_ -= TurnSpeed * DeltaTime;
    }

    // turn right
    if (Input.IsKeyHeld('L') || Input.IsKeyHeld(VK_RIGHT))
    {
        Yaw_ += TurnSpeed * DeltaTime;
    }

    // turn up
    if (Input.IsKeyHeld('I') || Input.IsKeyHeld(VK_UP))
    {
        Pitch_ -= TurnSpeed * DeltaTime * 0.5f;

        // clamp pitch 
        Pitch_ = Pitch_ > MaxPitch ? MaxPitch :
                 Pitch_ < -MaxPitch ? -MaxPitch :
                 Pitch_;
    }

    // turn down
    if (Input.IsKeyHeld('K') || Input.IsKeyHeld(VK_DOWN))
    {
        Pitch_ += TurnSpeed * DeltaTime * 0.5f;

        // clamp pitch
        Pitch_ = Pitch_ > MaxPitch ? MaxPitch :
                 Pitch_ < -MaxPitch ? -MaxPitch :
                 Pitch_;
    }
}

Vector PlayerController::GetRightVector() const
{
    const Vector Right = {1, 0, 0};
    const Matrix RotationMatrix = Matrix::RotationRollPitchYaw(0, Math::ConvertToRadians(Yaw_), 0);
    return Vector::Transform(Right, RotationMatrix);
}

Vector PlayerController::GetUpVector() const
{
    return {0, 1, 0};
}

Vector PlayerController::GetForwardVector() const
{
    const Vector Forward = {0, 0, 1};
    const Matrix RotationMatrix = Matrix::RotationRollPitchYaw(Math::ConvertToRadians(Pitch_), Math::ConvertToRadians(Yaw_), 0.0f);
    return Vector::Transform(Forward, RotationMatrix);
}
