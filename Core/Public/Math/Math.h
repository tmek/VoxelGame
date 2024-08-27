// Copyright

#pragma once

#include "Matrix.h"
#include "Vector.h"

// 3-D Plane
struct Plane
{
    Vector Normal;
    float Distance;
};


namespace Math
{
    // Define the constant for PI
    static constexpr float Pi = 3.14159265358979323846f;
    static constexpr double PiDouble = 3.14159265358979323846;

    // Static function to convert degrees to radians
    static float ConvertToRadians(float Degrees)
    {
        return Degrees * Pi / 180.0f;
    }

    static double ConvertToRadians(double Degrees)
    {
        return Degrees * PiDouble / 180.0;
    }


    inline void ExtractFrustumPlanes(const Matrix& ViewProjMatrixIn, Plane FrustumPlanesOut[6])
    {
        // Assuming Matrix has a method to return elements in row-major order

        const Matrix VP = ViewProjMatrixIn;

        // Left plane
        FrustumPlanesOut[0].Normal.X = VP.M[0][3] + VP.M[0][0];
        FrustumPlanesOut[0].Normal.Y = VP.M[1][3] + VP.M[1][0];
        FrustumPlanesOut[0].Normal.Z = VP.M[2][3] + VP.M[2][0];
        FrustumPlanesOut[0].Distance = VP.M[3][3] + VP.M[3][0];

        // Right plane
        FrustumPlanesOut[1].Normal.X = VP.M[0][3] - VP.M[0][0];
        FrustumPlanesOut[1].Normal.Y = VP.M[1][3] - VP.M[1][0];
        FrustumPlanesOut[1].Normal.Z = VP.M[2][3] - VP.M[2][0];
        FrustumPlanesOut[1].Distance = VP.M[3][3] - VP.M[3][0];

        // Top plane
        FrustumPlanesOut[2].Normal.X = VP.M[0][3] - VP.M[0][1];
        FrustumPlanesOut[2].Normal.Y = VP.M[1][3] - VP.M[1][1];
        FrustumPlanesOut[2].Normal.Z = VP.M[2][3] - VP.M[2][1];
        FrustumPlanesOut[2].Distance = VP.M[3][3] - VP.M[3][1];

        // Bottom plane
        FrustumPlanesOut[3].Normal.X = VP.M[0][3] + VP.M[0][1];
        FrustumPlanesOut[3].Normal.Y = VP.M[1][3] + VP.M[1][1];
        FrustumPlanesOut[3].Normal.Z = VP.M[2][3] + VP.M[2][1];
        FrustumPlanesOut[3].Distance = VP.M[3][3] + VP.M[3][1];

        // Near plane
        FrustumPlanesOut[4].Normal.X = VP.M[0][2];
        FrustumPlanesOut[4].Normal.Y = VP.M[1][2];
        FrustumPlanesOut[4].Normal.Z = VP.M[2][2];
        FrustumPlanesOut[4].Distance = VP.M[3][2];

        // Far plane
        FrustumPlanesOut[5].Normal.X = VP.M[0][3] - VP.M[0][2];
        FrustumPlanesOut[5].Normal.Y = VP.M[1][3] - VP.M[1][2];
        FrustumPlanesOut[5].Normal.Z = VP.M[2][3] - VP.M[2][2];
        FrustumPlanesOut[5].Distance = VP.M[3][3] - VP.M[3][2];

        // Normalize the planes
        for (int i = 0; i < 6; i++)
        {
            float length = sqrtf(FrustumPlanesOut[i].Normal.X * FrustumPlanesOut[i].Normal.X +
                                 FrustumPlanesOut[i].Normal.Y * FrustumPlanesOut[i].Normal.Y +
                                 FrustumPlanesOut[i].Normal.Z * FrustumPlanesOut[i].Normal.Z);
            FrustumPlanesOut[i].Normal.X /= length;
            FrustumPlanesOut[i].Normal.Y /= length;
            FrustumPlanesOut[i].Normal.Z /= length;
            FrustumPlanesOut[i].Distance /= length;
        }
    }


    static constexpr bool IsBoxInFrustum(const Plane FrustumPlanes[6], const Vector& BoundsMin, const Vector& BoundsMax) noexcept
    {
        for (int i = 0; i < 6; i++)
        {
            // Find the point in the AABB that is most opposite to the plane normal (i.e., "furthest" from the plane)
            Vector farthestPoint = BoundsMin;

            if (FrustumPlanes[i].Normal.X >= 0) farthestPoint.X = BoundsMax.X;
            if (FrustumPlanes[i].Normal.Y >= 0) farthestPoint.Y = BoundsMax.Y;
            if (FrustumPlanes[i].Normal.Z >= 0) farthestPoint.Z = BoundsMax.Z;

            // If this point is outside the plane, then the box is outside the frustum
            float distance = FrustumPlanes[i].Normal.X * farthestPoint.X +
            FrustumPlanes[i].Normal.Y * farthestPoint.Y +
            FrustumPlanes[i].Normal.Z * farthestPoint.Z + FrustumPlanes[i].Distance;

            if (distance < 0)
            {
                return false; // Box is outside this plane
            }
        }

        return true; // Box is inside or intersecting the frustum
    }
};
