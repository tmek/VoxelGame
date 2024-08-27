// copyright

#pragma once

#include <cmath>
#include <DirectXMath.h>
#include <type_traits>

// forward
template <typename T>
struct TVector;


// A 4X4 Matrix for floating point types (float, double).
template <typename T>
struct alignas(16) TMatrix
{
    static_assert(std::is_floating_point_v<T>, "TMatrix should only be used with floating point types");

public:

    T M[4][4];

    // Default constructor - initializes to identity matrix
    TMatrix()
    {
        SetIdentity();
    }

    // Constructor with initial values
    TMatrix(T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33)
    {
        M[0][0] = m00;
        M[0][1] = m01;
        M[0][2] = m02;
        M[0][3] = m03;
        M[1][0] = m10;
        M[1][1] = m11;
        M[1][2] = m12;
        M[1][3] = m13;
        M[2][0] = m20;
        M[2][1] = m21;
        M[2][2] = m22;
        M[2][3] = m23;
        M[3][0] = m30;
        M[3][1] = m31;
        M[3][2] = m32;
        M[3][3] = m33;
    }

    // Matrix multiplication
    TMatrix<T> operator*(const TMatrix<T>& Other) const
    {
        TMatrix<T> result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.M[i][j] = M[i][0] * Other.M[0][j] +
                M[i][1] * Other.M[1][j] +
                M[i][2] * Other.M[2][j] +
                M[i][3] * Other.M[3][j];
            }
        }

        return result;
    }

    // Transpose the matrix
    TMatrix<T> Transpose() const
    {
        TMatrix<T> result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.M[i][j] = M[j][i];
            }
        }

        return result;
    }

    // Set the matrix to identity
    void SetIdentity()
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                M[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }
    }

    // Calculate the determinant of the matrix
    T Determinant() const
    {
        return
        M[0][3] * M[1][2] * M[2][1] * M[3][0] - M[0][2] * M[1][3] * M[2][1] * M[3][0] -
        M[0][3] * M[1][1] * M[2][2] * M[3][0] + M[0][1] * M[1][3] * M[2][2] * M[3][0] +
        M[0][2] * M[1][1] * M[2][3] * M[3][0] - M[0][1] * M[1][2] * M[2][3] * M[3][0] -
        M[0][3] * M[1][2] * M[2][0] * M[3][1] + M[0][2] * M[1][3] * M[2][0] * M[3][1] +
        M[0][3] * M[1][0] * M[2][2] * M[3][1] - M[0][0] * M[1][3] * M[2][2] * M[3][1] -
        M[0][2] * M[1][0] * M[2][3] * M[3][1] + M[0][0] * M[1][2] * M[2][3] * M[3][1] +
        M[0][3] * M[1][1] * M[2][0] * M[3][2] - M[0][1] * M[1][3] * M[2][0] * M[3][2] -
        M[0][3] * M[1][0] * M[2][1] * M[3][2] + M[0][0] * M[1][3] * M[2][1] * M[3][2] +
        M[0][1] * M[1][0] * M[2][3] * M[3][2] - M[0][0] * M[1][1] * M[2][3] * M[3][2] -
        M[0][2] * M[1][1] * M[2][0] * M[3][3] + M[0][1] * M[1][2] * M[2][0] * M[3][3] +
        M[0][2] * M[1][0] * M[2][1] * M[3][3] - M[0][0] * M[1][2] * M[2][1] * M[3][3] -
        M[0][1] * M[1][0] * M[2][2] * M[3][3] + M[0][0] * M[1][1] * M[2][2] * M[3][3];
    }

    // Multiply the matrix by a scalar
    TMatrix<T> operator*(T Scalar) const
    {
        TMatrix<T> result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.M[i][j] = M[i][j] * Scalar;
            }
        }

        return result;
    }

    // Static method to create a rotation matrix from yaw (Y), pitch (X), and roll (Z)
    static TMatrix<T> RotationRollPitchYaw(T Pitch, T Yaw, T Roll)
    {
        T cosPitch = std::cos(Pitch);
        T sinPitch = std::sin(Pitch);
        T cosYaw = std::cos(Yaw);
        T sinYaw = std::sin(Yaw);
        T cosRoll = std::cos(Roll);
        T sinRoll = std::sin(Roll);

        TMatrix<T> rotationMatrix;

        rotationMatrix.M[0][0] = cosYaw * cosRoll;
        rotationMatrix.M[0][1] = cosYaw * sinRoll;
        rotationMatrix.M[0][2] = -sinYaw;
        rotationMatrix.M[0][3] = static_cast<T>(0);

        rotationMatrix.M[1][0] = sinPitch * sinYaw * cosRoll - cosPitch * sinRoll;
        rotationMatrix.M[1][1] = sinPitch * sinYaw * sinRoll + cosPitch * cosRoll;
        rotationMatrix.M[1][2] = sinPitch * cosYaw;
        rotationMatrix.M[1][3] = static_cast<T>(0);

        rotationMatrix.M[2][0] = cosPitch * sinYaw * cosRoll + sinPitch * sinRoll;
        rotationMatrix.M[2][1] = cosPitch * sinYaw * sinRoll - sinPitch * cosRoll;
        rotationMatrix.M[2][2] = cosPitch * cosYaw;
        rotationMatrix.M[2][3] = static_cast<T>(0);

        rotationMatrix.M[3][0] = static_cast<T>(0);
        rotationMatrix.M[3][1] = static_cast<T>(0);
        rotationMatrix.M[3][2] = static_cast<T>(0);
        rotationMatrix.M[3][3] = static_cast<T>(1);

        return rotationMatrix;
    }

    //Multiply a vector by this matrix
    TVector<T> Transform(const TVector<T>& V) const
    {
        TVector<T> Result;

        Result.X = V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0];
        Result.Y = V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1];
        Result.Z = V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2];

        return Result;
    }

    // LookAt function for left-handed (LH) coordinate system, row-major order
    static TMatrix<T> LookAtLH(const TVector<T>& EyePosition, const TVector<T>& FocusPosition, const TVector<T>& UpDirection)
    {
        // -------------------------------------------------------------------------------
        // Construct the base X, Y, Z axes that determine the camera's view orientation.
        // These axes represent the camera's local coordinate system (right, up, forward),
        // but are oriented relative to the world space coordinates.
        // -------------------------------------------------------------------------------

        // Step 1: Calculate the forward (Z) vector, which is the direction from the eye to the focus
        TVector<T> ZAxis = (FocusPosition - EyePosition).GetNormalized();

        // Step 2: Calculate the right (X) vector, which is perpendicular to the up direction and ZAxis
        TVector<T> XAxis = UpDirection.GetCrossProduct(ZAxis).GetNormalized();

        // Step 3: Calculate the corrected up (Y) vector, which is perpendicular to both the X and Z axes
        TVector<T> YAxis = ZAxis.GetCrossProduct(XAxis);

        /* -------------------------------------------------------------------------------
         * After rotating the world such that the camera's orientation matches the canonical view (looking down the Z-axis),
         * we need to translate everything so that the camera is at the origin.
         *
         * Each row's dot product computes the projection of the camera's position onto the corresponding axis (X, Y, Z).
         * Which is negated to give the necessary translation to shift the camera's position to the origin.
         * ------------------------------------------------------------------------------- */

        TVector<T> Translation = {
            -XAxis.GetDotProduct(EyePosition),
            -YAxis.GetDotProduct(EyePosition),
            -ZAxis.GetDotProduct(EyePosition)};

        /* ------------------------------------------------------------------------------- 
        *   Finally we construct the matrix:
            
               | XAxis.x  XAxis.y  XAxis.z  Translation.x |
               | YAxis.x  YAxis.y  YAxis.z  Translation.y |
               | ZAxis.x  ZAxis.y  ZAxis.z  Translation.z |
               |    0        0        0         1         | // homogeneous coordinates
               
         * ------------------------------------------------------------------------------- */
        

        // Step 4: Construct the LookAt matrix
        TMatrix<T> LookAtMatrix;
        LookAtMatrix.M[0][0] = XAxis.X;
        LookAtMatrix.M[0][1] = XAxis.Y;
        LookAtMatrix.M[0][2] = XAxis.Z;
        LookAtMatrix.M[0][3] = Translation.X;

        LookAtMatrix.M[1][0] = YAxis.X;
        LookAtMatrix.M[1][1] = YAxis.Y;
        LookAtMatrix.M[1][2] = YAxis.Z;
        LookAtMatrix.M[1][3] = Translation.Y;

        LookAtMatrix.M[2][0] = ZAxis.X;
        LookAtMatrix.M[2][1] = ZAxis.Y;
        LookAtMatrix.M[2][2] = ZAxis.Z;
        LookAtMatrix.M[2][3] = Translation.Z;

        LookAtMatrix.M[3][0] = 0.0f;
        LookAtMatrix.M[3][1] = 0.0f;
        LookAtMatrix.M[3][2] = 0.0f;
        LookAtMatrix.M[3][3] = 1.0f;

        // CPU side we work in row-major layout, but HLSL expects column-major layout.
        return LookAtMatrix.Transpose();
        //return LookAtMatrix;

        TVector<T> EyeDirection = FocusPosition - EyePosition;
        return LookToLH(EyePosition, EyeDirection, UpDirection);
    }

    // LookAt function for right-handed (RH) coordinate system
    static TMatrix<T> LookAtRH(const TVector<T>& eye, const TVector<T>& target, const TVector<T>& up)
    {
        TVector<T> forward = (eye - target).GetNormalized();
        TVector<T> right = up.GetCrossProduct(forward).GetNormalized();
        TVector<T> upCorrected = forward.GetCrossProduct(right);

        TMatrix<T> result;
        result.M[0][0] = right.X;
        result.M[1][0] = right.Y;
        result.M[2][0] = right.Z;
        result.M[3][0] = -right.GetDotProduct(eye);

        result.M[0][1] = upCorrected.X;
        result.M[1][1] = upCorrected.Y;
        result.M[2][1] = upCorrected.Z;
        result.M[3][1] = -upCorrected.GetDotProduct(eye);

        result.M[0][2] = forward.X;
        result.M[1][2] = forward.Y;
        result.M[2][2] = forward.Z;
        result.M[3][2] = -forward.GetDotProduct(eye);

        result.M[0][3] = 0.0f;
        result.M[1][3] = 0.0f;
        result.M[2][3] = 0.0f;
        result.M[3][3] = 1.0f;

        return result;
    }

    // LookTo function for left-handed (LH) coordinate system
    static TMatrix<T> LookToLH(const TVector<T>& EyePosition, const TVector<T>& EyeDirection, const TVector<T>& UpDirection)
    {
        //XMVECTOR R2 = XMVector3Normalize(EyeDirection);
        TVector<T> R2 = EyeDirection.GetNormalized();
        // XMVECTOR R0 = XMVector3Cross(UpDirection, R2);
        // R0 = XMVector3Normalize(R0);
        TVector<T> R0 = UpDirection.GetCrossProduct(R2).GetNormalized();
        //
        // XMVECTOR R1 = XMVector3Cross(R2, R0);
        TVector<T> R1 = R2.GetCrossProduct(R0);
        //
        // XMVECTOR NegEyePosition = XMVectorNegate(EyePosition);
        TVector<T> NegEyePosition = -EyePosition;
        //
        // XMVECTOR D0 = XMVector3Dot(R0, NegEyePosition);
        T D0 = R0.GetDotProduct(NegEyePosition);
        // XMVECTOR D1 = XMVector3Dot(R1, NegEyePosition);
        T D1 = R1.GetDotProduct(NegEyePosition);
        // XMVECTOR D2 = XMVector3Dot(R2, NegEyePosition);
        T D2 = R2.GetDotProduct(NegEyePosition);
        //
        // XMMATRIX M;
        TMatrix<T> M;
        // M.r[0] = XMVectorSelect(D0, R0, g_XMSelect1110.v);
        M.M[0][0] = D0;
        M.M[1][0] = D0;
        M.M[2][0] = D0;
        M.M[3][0] = R0[3];
        // [1]M.r = XMVectorSelect(D1, R1, g_XMSelect1110.v);
        M.M[0][1] = D1;
        M.M[1][1] = D1;
        M.M[2][1] = D1;
        M.M[3][1] = R1[3];
        // [2]M.r = XMVectorSelect(D2, R2, g_XMSelect1110.v);
        M.M[0][2] = D2;
        M.M[1][2] = D2;
        M.M[2][2] = D2;
        M.M[3][2] = R2[3];
        // [3]M.r = g_XMIdentityR3.v;
        M.M[0][3] = 0;
        M.M[1][3] = 0;
        M.M[2][3] = 0;
        M.M[3][3] = 1;
        //
        // M = XMMatrixTranspose(M);
        M = M.Transpose();

        return M;
    }

    // LookTo function for right-handed (RH) coordinate system
    static TMatrix<T> LookToRH(const TVector<T>& eye, const TVector<T>& direction, const TVector<T>& up)
    {
        return LookAtRH(eye, eye + direction, up);
    }

public:

    // PerspectiveFov function for left-handed (LH) coordinate system
    static TMatrix<T> PerspectiveFovLH(T FovAngleY, T AspectRatio, T NearZ, T FarZ)
    {
/*
        |   1/(aspect * tan(fovY/2))     0                    0                               0   |
        |   0                            1/tan(fovY/2)        0                               0   |
        |   0                            0                    farZ/(farZ-nearZ)               1   |
        |   0                            0                    -(farZ * nearZ)/(farZ-nearZ)    0   /

        Explanation of Each Term:
        1/(aspect * tan(fovY/2)): Scales the X coordinates based on the aspect ratio and field of view.
        1/tan(fovY/2): Scales the Y coordinates based on the field of view.
        farZ/(farZ-nearZ): Scales the Z coordinates to fit within the [0, 1] range.
        -(farZ * nearZ)/(farZ-nearZ): Translates the Z coordinates so that nearZ maps to 0 and farZ maps to 1.
        1 in the third row, fourth column: Projects the Z coordinate into the W component for perspective division.
        0 in the last row: Ensures that the W component is correctly handled for the perspective divide, maintaining the homogeneous coordinate system.        

        //XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
        
*/
        assert(NearZ > 0.f && FarZ > 0.f);
        assert(!XMScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
        assert(!XMScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
        assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));
        
        float    SinFov;
        float    CosFov;

        const float HalfFovAngleY = 0.5f * FovAngleY;
        SinFov = sinf(HalfFovAngleY);
        CosFov = cosf(HalfFovAngleY);
        float Tan = SinFov / CosFov;
        float InvTan = 1.0f / Tan;

        float Height = InvTan;  // inv tan?
        float Width = Height / AspectRatio;
        
        float fRange = FarZ / (FarZ - NearZ);

        TMatrix<T> M;
        M.M[0][0] = Width;
        M.M[0][1] = 0.0f;
        M.M[0][2] = 0.0f;
        M.M[0][3] = 0.0f;
          
        M.M[1][0] = 0.0f;
        M.M[1][1] = Height;
        M.M[1][2] = 0.0f;
        M.M[1][3] = 0.0f;
          
        M.M[2][0] = 0.0f;
        M.M[2][1] = 0.0f;
        M.M[2][2] = fRange;
        M.M[2][3] = 1.0f;
          
        M.M[3][0] = 0.0f;
        M.M[3][1] = 0.0f;
        M.M[3][2] = -fRange * NearZ;
        M.M[3][3] = 0.0f;
        
        return M;

        

        TMatrix<T> Result;
    
        // Step 1: Calculate the vertical scaling factor based on the field of view
        T yScale = static_cast<T>(1) / std::tan(FovAngleY / static_cast<T>(2));

        // Step 2: Calculate the horizontal scaling factor based on the aspect ratio
        T xScale = yScale / AspectRatio;

        // Step 3: Calculate the scaling factor for Z
        T zRange = FarZ - NearZ;

        // Step 4: Populate the perspective matrix
        Result.M[0][0] = xScale;         // X scaling
        Result.M[0][1] = 0.0f;           // No skewing in Y direction
        Result.M[0][2] = 0.0f;           // No translation in X
        Result.M[0][3] = 0.0f;           // Perspective scaling

        Result.M[1][0] = 0.0f;           // No skewing in X direction
        Result.M[1][1] = yScale;         // Y scaling
        Result.M[1][2] = 0.0f;           // No translation in Y
        Result.M[1][3] = 0.0f;           // Perspective scaling

        Result.M[2][0] = 0.0f;           // No translation in X
        Result.M[2][1] = 0.0f;           // No translation in Y
        Result.M[2][2] = FarZ / zRange;  // Z scaling
        Result.M[2][3] = 1.0f;           // Perspective division

        Result.M[3][0] = 0.0f;           // No translation in X
        Result.M[3][1] = 0.0f;           // No translation in Y
        Result.M[3][2] = -(FarZ * NearZ) / zRange;  // Z translation
        Result.M[3][3] = 0.0f;           // Final row for homogeneous coordinates

        return Result;        
 
        
     
    }

    // PerspectiveFov function for right-handed (RH) coordinate system
    static TMatrix<T> PerspectiveFovRH(T fovY, T aspect, T nearZ, T farZ)
    {
        return PerspectiveFovImpl(fovY, aspect, nearZ, farZ, -1.0f);
    }

private:

    static TMatrix<T> PerspectiveFovImpl(T fovY, T aspect, T nearZ, T farZ, T zSign)
    {
        TMatrix<T> result;
        T tanHalfFovY = std::tan(fovY / 2.0f);

        result.M[0][0] = 1.0f / (aspect * tanHalfFovY);
        result.M[1][1] = 1.0f / tanHalfFovY;
        result.M[2][2] = farZ / (nearZ - farZ);
        result.M[2][3] = zSign;
        result.M[3][2] = -(nearZ * farZ) / (farZ - nearZ);
        result.M[3][3] = 0.0f;

        return result;
    }
};

// Aliases for common matrix types
using Matrix4f = TMatrix<float>;
using Matrix4d = TMatrix<double>;
using Matrix = Matrix4f;
