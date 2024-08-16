#pragma once
#include <vector>
#include <cmath>
#include <numeric>
#include <random>

class OptimizedPerlinNoise
{
public:
    OptimizedPerlinNoise(unsigned int seed = std::random_device{}())
    {
        // Initialize and shuffle permutation vector
        perm.resize(256);
        std::iota(perm.begin(), perm.end(), 0);
        std::shuffle(perm.begin(), perm.end(), std::default_random_engine(seed));
        perm.insert(perm.end(), perm.begin(), perm.end());
    }

    double sample3D(double x, double y, double z) const
    {
        // Cache the results of fastFloor to avoid redundant calculations
        int X = fastFloor(x) & 255;
        int Y = fastFloor(y) & 255;
        int Z = fastFloor(z) & 255;

        double xf = x - fastFloor(x); // x0 in the original implementation
        double yf = y - fastFloor(y); // y0 in the original implementation
        double zf = z - fastFloor(z); // z0 in the original implementation


        // Compute fade curves for x, y, z
        double u = fade(xf);
        double v = fade(yf);
        double w = fade(zf);

        // Hash coordinates of the 8 cube corners
        int A  = perm[X] + Y, AA = perm[A] + Z, AB = perm[A + 1] + Z;
        int B  = perm[X + 1] + Y, BA = perm[B] + Z, BB = perm[B + 1] + Z;

        // Blend results from 8 corners of the cube
        return lerp(w, lerp(v, lerp(u, grad(perm[AA], xf, yf, zf), grad(perm[BA], xf - 1, yf, zf)),
                            lerp(u, grad(perm[AB], xf, yf - 1, zf), grad(perm[BB], xf - 1, yf - 1, zf))),
                    lerp(v, lerp(u, grad(perm[AA + 1], xf, yf, zf - 1), grad(perm[BA + 1], xf - 1, yf, zf - 1)),
                         lerp(u, grad(perm[AB + 1], xf, yf - 1, zf - 1), grad(perm[BB + 1], xf - 1, yf - 1, zf - 1))));
    }

private:
    std::vector<int> perm;

    // Faster floor function
    static int fastFloor(double x)
    {
        return (x >= 0) ? static_cast<int>(x) : static_cast<int>(x) - 1;
    }

    // Fade function with fewer multiplications
    static double fade(double t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    // Linear interpolation
    static double lerp(double t, double a, double b)
    {
        return a + t * (b - a);
    }

    // Gradient function optimized to reduce branching
    static double grad(int hash, double x, double y, double z)
    {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};
