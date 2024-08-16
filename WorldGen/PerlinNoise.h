#pragma once
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <random>
#include <vector>

// Perlin Noise class
class PerlinNoise
{
public:
    PerlinNoise(unsigned int seed = std::random_device{}())
    {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::shuffle(p.begin(), p.end(), std::default_random_engine(seed));
        p.insert(p.end(), p.begin(), p.end());
    }

    double sample3D(int32_t x, int32_t y, int32_t z) const
    {
        int X = x & 255;
        int Y = y & 255;
        int Z = z & 255;

        double u = fade(x - X);
        double v = fade(y - Y);
        double w = fade(z - Z);

        int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

        return lerp(w, lerp(v, lerp(u, grad(p[AA], x - X, y - Y, z - Z), grad(p[BA], x - X - 1, y - Y, z - Z)),
                            lerp(u, grad(p[AB], x - X, y - Y - 1, z - Z), grad(p[BB], x - X - 1, y - Y - 1, z - Z))),
                    lerp(v, lerp(u, grad(p[AA + 1], x - X, y - Y, z - Z - 1), grad(p[BA + 1], x - X - 1, y - Y, z - Z - 1)),
                         lerp(u, grad(p[AB + 1], x - X, y - Y - 1, z - Z - 1), grad(p[BB + 1], x - X - 1, y - Y - 1, z - Z - 1))));
    }



private:
    std::vector<int> p;

    inline double fade(double t) const { return t * t * t * (t * (t * 6 - 15) + 10); }
    inline double lerp(double t, double a, double b) const { return a + t * (b - a); }
    inline double grad(int hash, double x, double y, double z) const
    {
        // Simplified gradient computation with fewer branches
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
    
};
