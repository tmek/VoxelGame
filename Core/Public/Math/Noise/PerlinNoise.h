﻿#pragma once
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

    double sample3D(double x, double y, double z) const
    {
        int fx = static_cast<int>(std::floor(x));
        int fy = static_cast<int>(std::floor(y));
        int fz = static_cast<int>(std::floor(z));
        
        int X = fx & 255;
        int Y = fy & 255;
        int Z = fz & 255;

        x -= fx;
        y -= fy;
        z -= fz;

        double u = fade(x);
        double v = fade(y);
        double w = fade(z);

        int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

        return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)),
                            lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))),
                    lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)),
                         lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }

private:
    std::vector<int> p;

    double fade(double t) const { return t * t * t * (t * (t * 6 - 15) + 10); }
    double lerp(double t, double a, double b) const { return a + t * (b - a); }

    double grad(int hash, double x, double y, double z) const
    {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};
