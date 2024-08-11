#pragma once
#include <cmath>
#include <array>

struct PerlinNoise2D {
    // Gradient vectors for 2D Perlin noise
    static constexpr std::array<std::array<int, 2>, 8> gradients = {{
        {1, 1}, {-1, 1}, {1, -1}, {-1, -1},
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    }};

    // Hash function to get a pseudo-random gradient index
    static int hash(int x, int y) {
        return (x * 1619 + y * 31337) & 7; // Modulo 8 because we have 8 gradients
    }

    // Linear interpolation
    static float lerp(float t, float a, float b) {
        return a + t * (b - a);
    }

    // Fade function as defined by Ken Perlin
    static float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    // Dot product of the gradient and the distance vector
    static float grad(int hash, float x, float y) {
        int g = hash & 7;
        return gradients[g][0] * x + gradients[g][1] * y;
    }

    // Perlin noise sample function for 2D coordinates
    static float Sample2D(float x, float y) {
        int x0 = static_cast<int>(std::floor(x));
        int y0 = static_cast<int>(std::floor(y));
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        float sx = x - static_cast<float>(x0);
        float sy = y - static_cast<float>(y0);

        float u = fade(sx);
        float v = fade(sy);

        float n0, n1, ix0, ix1, value;

        n0 = grad(hash(x0, y0), sx, sy);
        n1 = grad(hash(x1, y0), sx - 1.0f, sy);
        ix0 = lerp(u, n0, n1);

        n0 = grad(hash(x0, y1), sx, sy - 1.0f);
        n1 = grad(hash(x1, y1), sx - 1.0f, sy - 1.0f);
        ix1 = lerp(u, n0, n1);

        value = lerp(v, ix0, ix1);
        return value;
    }
};
