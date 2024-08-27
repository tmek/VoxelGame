// Copyright (C) [Year] [Your Name or Company]. All rights reserved.

#pragma once

#pragma warning(disable: 4251) // 'type' needs to have dll-interface to be used by clients of 'type'								https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4251

#include <cassert>
#include <cmath>
#include <type_traits> // Ensure to include this for static_assert
#include "Generated/CoreExports.h"

// Forward declaration
template <typename T>
struct TMatrix;


// 3D vector template for floating-point types (float, double)
template <typename T>
struct TVector
{
    static_assert(std::is_floating_point_v<T>, "TVector should only be used with floating-point types");

public:

    using FReal = T;

    union
    {
        struct
        {
            T X, Y, Z;
        };

        T XYZ[3];
    };

    // Default constructor
    TVector() = default;

    // Copy constructor
    TVector(const TVector<T>& Other)
        : X(Other.X), Y(Other.Y), Z(Other.Z)
    {
    }

    // Constructor from individual components
    TVector(T InX, T InY, T InZ)
        : X(InX), Y(InY), Z(InZ)
    {
    }

    // Cross product
    [[nodiscard]] TVector<T> GetCrossProduct(const TVector<T>& Other) const
    {
        return TVector<T>(
            Y * Other.Z - Z * Other.Y,
            Z * Other.X - X * Other.Z,
            X * Other.Y - Y * Other.X
        );
    }

    // Dot product
    [[nodiscard]] T GetDotProduct(const TVector<T>& Other) const
    {
        return X * Other.X + Y * Other.Y + Z * Other.Z;
    }

    // Access operators
    T& operator[](size_t Index)
    {
        assert(Index < 3);
        return XYZ[Index];
    }

    const T& operator[](size_t Index) const
    {
        assert(Index < 3);
        return XYZ[Index];
    }

    // Addition
    TVector<T> operator+(const TVector<T>& Other) const
    {
        return TVector<T>(X + Other.X, Y + Other.Y, Z + Other.Z);
    }

    // Negate
    TVector<T> operator-() const
    {
        return TVector<T>(-X, -Y, -Z);
    }
    
    // Subtraction
    TVector<T> operator-(const TVector<T>& Other) const
    {
        return TVector<T>(X - Other.X, Y - Other.Y, Z - Other.Z);
    }

    // Multiplication
    TVector<T> operator*(const TVector<T>& Other) const
    {
        return TVector<T>(X * Other.X, Y * Other.Y, Z * Other.Z);
    }

    // Scalar multiplication
    TVector<T> operator*(const FReal Scalar) const
    {
        return TVector<T>(X * Scalar, Y * Scalar, Z * Scalar);
    }

    // Magnitude
    [[nodiscard]] T GetMagnitude() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    // Normalize
    [[nodiscard]] TVector<T> GetNormalized() const
    {
        T Magnitude = GetMagnitude();
        return *this * (T(1) / Magnitude);
    }

    // Transform vector by matrix
    [[nodiscard]] static TVector<T> Transform(const TVector<T>& V, const TMatrix<T>& M)
    {
        TVector<T> Result;
        Result.X = V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0] + M.M[3][0];
        Result.Y = V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1] + M.M[3][1];
        Result.Z = V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2] + M.M[3][2];
        return Result;
    }

public:

    /** A zero vector (0,0,0) */
    CORE_API static const TVector<T> ZeroVector;

    /** One vector (1,1,1) */
    CORE_API static const TVector<T> OneVector;

    /** Up vector (0,1,0) */
    CORE_API static const TVector<T> UpVector;

    /** Down vector (0,-1,0) */
    CORE_API static const TVector<T> DownVector;

    /** Forward vector (0,0,1) */
    CORE_API static const TVector<T> ForwardVector;

    /** Backward vector (0,0,-1) */
    CORE_API static const TVector<T> BackwardVector;

    /** Right vector (1,0,0) */
    CORE_API static const TVector<T> RightVector;

    /** Left vector (-1,0,0) */
    CORE_API static const TVector<T> LeftVector;

    /** Unit X axis vector (1,0,0) */
    CORE_API static const TVector<T> XAxisVector;

    /** Unit Y axis vector (0,1,0) */
    CORE_API static const TVector<T> YAxisVector;

    /** Unit Z axis vector (0,0,1) */
    CORE_API static const TVector<T> ZAxisVector;
};

// Aliases for floating-point 3D vectors
using Vector3f = CORE_API TVector<float>;
using Vector3d = CORE_API TVector<double>;
using Vector = CORE_API Vector3f; // Generally, Vector is assumed to be float in many systems


