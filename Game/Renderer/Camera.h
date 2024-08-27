// Copyright (C) [Year] [Your Name or Company]. All rights reserved.

#pragma once

#include "Math/Vector.h"

class Camera
{
public:

    // Constructors
    inline Camera();

    inline Camera(const Vector& Position, const Vector& Target, float FieldOfViewY, float AspectRatio, float NearPlane, float FarPlane);

    // Getters
    [[nodiscard]] inline const Vector& GetPosition() const;

    [[nodiscard]] inline const Vector& GetFocusPosition() const;

    [[nodiscard]] inline const Vector& GetUpDirection() const;

    [[nodiscard]] inline float GetFieldOfViewY() const;

    [[nodiscard]] inline float GetAspectRatio() const;

    [[nodiscard]] inline float GetNearPlane() const;

    [[nodiscard]] inline float GetFarPlane() const;

    // Setters
    inline void SetPosition(const Vector& NewPosition);

    inline void SetTarget(const Vector& NewTarget);

    inline void SetUp(const Vector& NewUp);

    inline void SetFieldOfViewY(float NewFieldOfViewY);

    inline void SetAspectRatio(float NewAspectRatio);

    inline void SetNearPlane(float NewNearPlane);

    inline void SetFarPlane(float NewFarPlane);

private:

    Vector Position_;
    Vector Target_;
    Vector Up_;

    float FieldOfViewY_;
    float AspectRatio_;
    float NearPlane_;
    float FarPlane_;
};

// Inline method definitions

Camera::Camera()
    : Position_(Vector::ZeroVector), // zero
      Target_(Vector::ForwardVector), // forward 
      Up_(Vector::UpVector),
      FieldOfViewY_(45.0f),
      AspectRatio_(16.0f / 9.0f),
      NearPlane_(0.1f),
      FarPlane_(1000.0f)
{
}

Camera::Camera(const Vector& Position, const Vector& Target, const float FieldOfViewY, const float AspectRatio, const float NearPlane, const float FarPlane)
    : Position_(Position),
      Target_(Target),
      Up_(Vector::UpVector),
      FieldOfViewY_(FieldOfViewY),
      AspectRatio_(AspectRatio),
      NearPlane_(NearPlane),
      FarPlane_(FarPlane)
{
}

const Vector& Camera::GetPosition() const { return Position_; }
const Vector& Camera::GetFocusPosition() const { return Target_; }
const Vector& Camera::GetUpDirection() const { return Up_; }
float Camera::GetFieldOfViewY() const { return FieldOfViewY_; }
float Camera::GetAspectRatio() const { return AspectRatio_; }
float Camera::GetNearPlane() const { return NearPlane_; }
float Camera::GetFarPlane() const { return FarPlane_; }

void Camera::SetPosition(const Vector& NewPosition) { Position_ = NewPosition; }
void Camera::SetTarget(const Vector& NewTarget) { Target_ = NewTarget; }
void Camera::SetUp(const Vector& NewUp) { Up_ = NewUp; }
void Camera::SetFieldOfViewY(float NewFieldOfViewY) { FieldOfViewY_ = NewFieldOfViewY; }
void Camera::SetAspectRatio(float NewAspectRatio) { AspectRatio_ = NewAspectRatio; }
void Camera::SetNearPlane(float NewNearPlane) { NearPlane_ = NewNearPlane; }
void Camera::SetFarPlane(float NewFarPlane) { FarPlane_ = NewFarPlane; }
