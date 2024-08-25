// Copyright

#pragma once

#include <cmath>
#include <numbers>

struct Math
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
};
