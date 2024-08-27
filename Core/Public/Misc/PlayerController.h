// Copyright

#pragma once

#include "Math/Vector.h"

class CORE_API PlayerController
{
public:

    // Tick the player controller
    void Tick(float DeltaTime);

    // Get the right vector based on player controller orientation
    [[nodiscard]] Vector GetRightVector() const;

    // Get the up vector based on player controller orientation
    [[nodiscard]] Vector GetUpVector() const;

    // Get the forward vector based on player controller orientation
    [[nodiscard]] Vector GetForwardVector() const;

    [[nodiscard]] Vector GetMovementVector() const { return MovementVector_; }

    [[nodiscard]] float GetYaw() const { return Yaw_; }
    
    [[nodiscard]] float GetPitch() const { return Pitch_; }

private:

    TVector<float> MovementVector_ = {0,0,0};
    float Yaw_ = 0.0f;
    float Pitch_ = 0.0f;
};
