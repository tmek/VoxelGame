// Copyright (C) [Year] [Your Name or Company]. All rights reserved.

#pragma once

#include <cassert>
#include <type_traits>

// 3D vector template for integer types (int, long, etc.)
template <typename T>
struct TIntVector
{
    static_assert(std::is_integral_v<T>, "TIntVector should only be used with integer types");

public:

    union
    {
        struct
        {
            /** Vector's X component. */
            T X;

            /** Vector's Y component. */
            T Y;

            /** Vector's Z component. */
            T Z;
        };

        /** Indexable union of vector components. */
        T XYZ[3];
    };

    // Default constructor
    TIntVector() = default;

    // Copy constructor
    TIntVector(const TIntVector<T>& Other)
        : X(Other.X), Y(Other.Y), Z(Other.Z)
    {
    }

    // Constructor with initial values
    TIntVector(T InX, T InY, T InZ)
        : X(InX), Y(InY), Z(InZ)
    {
    }

    // Move constructor
    TIntVector(TIntVector<T>&& Other) noexcept = default;

    // Move assignment operator
    TIntVector<T>& operator=(TIntVector<T>&& Other) noexcept = default;

    // Access operators
    T& operator[](size_t Index)
    {
        assert(Index < 3 && "Index out of bounds in TIntVector");
        return XYZ[Index];
    }

    const T& operator[](size_t Index) const
    {
        assert(Index < 3 && "Index out of bounds in TIntVector");
        return XYZ[Index];
    }

    // Addition
    TIntVector<T> operator+(const TIntVector<T>& Other) const
    {
        return TIntVector<T>(X + Other.X, Y + Other.Y, Z + Other.Z);
    }

    // Subtraction
    TIntVector<T> operator-(const TIntVector<T>& Other) const
    {
        return TIntVector<T>(X - Other.X, Y - Other.Y, Z - Other.Z);
    }

    // Scalar multiplication
    TIntVector<T> operator*(T Scalar) const
    {
        return TIntVector<T>(X * Scalar, Y * Scalar, Z * Scalar);
    }

    // Uncomment and define in implementation file if needed:
    // static const TIntVector<T> ZeroVector;
    // static const TIntVector<T> OneVector;
    // static const TIntVector<T> XAxisVector;
    // static const TIntVector<T> YAxisVector;
    // static const TIntVector<T> ZAxisVector;
};

// Aliases for integer 3D vectors
using Vector3i = TIntVector<int>;
using IntVector = Vector3i;
using Vector3l = TIntVector<long>;
