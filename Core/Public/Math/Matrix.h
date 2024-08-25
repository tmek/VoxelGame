// copyright

#pragma once


template <typename T>
struct TMatrix
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
        M[0][0] = m00; M[0][1] = m01; M[0][2] = m02; M[0][3] = m03;
        M[1][0] = m10; M[1][1] = m11; M[1][2] = m12; M[1][3] = m13;
        M[2][0] = m20; M[2][1] = m21; M[2][2] = m22; M[2][3] = m23;
        M[3][0] = m30; M[3][1] = m31; M[3][2] = m32; M[3][3] = m33;
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
    
    // Multiply a vector by this matrix
    // TVector<T> Transform(const TVector<T>& V) const
    // {
    //     TVector<T> Result;
    //
    //     Result.X = V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0];
    //     Result.Y = V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1];
    //     Result.Z = V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2];
    //
    //     return Result;
    // }
};

// Aliases for common matrix types
using Matrix4f = TMatrix<float>;
using Matrix4d = TMatrix<double>;
using Matrix = Matrix4f;