#pragma once
#include <array>
#include <numeric>
#include <random>
#include <vector>



class SimplexNoise
{
public:
    SimplexNoise(unsigned int seed = std::random_device{}())
    {
        perm.resize(256);
        std::iota(perm.begin(), perm.end(), 0);
        std::shuffle(perm.begin(), perm.end(), std::default_random_engine(seed));
        perm.insert(perm.end(), perm.begin(), perm.end());
    }

    double sample3D(double x, double y, double z) const
    {
        // Skew the input space to determine which simplex cell we're in
        double s = (x + y + z) * F3;
        int i = fastFloor(x + s);
        int j = fastFloor(y + s);
        int k = fastFloor(z + s);

        // Unskew the cell origin back to (x,y,z) space
        double t = (i + j + k) * G3;
        double X0 = i - t;
        double Y0 = j - t;
        double Z0 = k - t;
        double x0 = x - X0;
        double y0 = y - Y0;
        double z0 = z - Z0;

        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k)
        int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k)
        if (x0 >= y0) {
            if (y0 >= z0) {
                i1 = 1; j1 = 0; k1 = 0;
                i2 = 1; j2 = 1; k2 = 0;
            } else if (x0 >= z0) {
                i1 = 1; j1 = 0; k1 = 0;
                i2 = 1; j2 = 0; k2 = 1;
            } else {
                i1 = 0; j1 = 0; k1 = 1;
                i2 = 1; j2 = 0; k2 = 1;
            }
        } else {
            if (y0 < z0) {
                i1 = 0; j1 = 0; k1 = 1;
                i2 = 0; j2 = 1; k2 = 1;
            } else if (x0 < z0) {
                i1 = 0; j1 = 1; k1 = 0;
                i2 = 0; j2 = 1; k2 = 1;
            } else {
                i1 = 0; j1 = 1; k1 = 0;
                i2 = 1; j2 = 1; k2 = 0;
            }
        }

        // Offsets for remaining corners
        double x1 = x0 - i1 + G3;
        double y1 = y0 - j1 + G3;
        double z1 = z0 - k1 + G3;
        double x2 = x0 - i2 + 2.0 * G3;
        double y2 = y0 - j2 + 2.0 * G3;
        double z2 = z0 - k2 + 2.0 * G3;
        double x3 = x0 - 1.0 + 3.0 * G3;
        double y3 = y0 - 1.0 + 3.0 * G3;
        double z3 = z0 - 1.0 + 3.0 * G3;

        // Calculate the hashed gradient indices of the four simplex corners
        int ii = i & 255;
        int jj = j & 255;
        int kk = k & 255;
        int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
        int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
        int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
        int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

        // Calculate the contributions from the four corners
        double n0 = contribution(gi0, x0, y0, z0);
        double n1 = contribution(gi1, x1, y1, z1);
        double n2 = contribution(gi2, x2, y2, z2);
        double n3 = contribution(gi3, x3, y3, z3);

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to stay just inside [-1,1]
        return 32.0 * (n0 + n1 + n2 + n3);
    }

private:
    std::vector<int> perm;

    static inline int fastFloor(double x) {
        return x > 0 ? (int)x : (int)x - 1;
    }

    static inline double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static inline double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    static inline double grad(int hash, double x, double y, double z) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    static inline double contribution(int gi, double x, double y, double z) {
        double t = 0.5 - x * x - y * y - z * z;
        if (t < 0) return 0.0;
        t *= t;
        return t * t * dot(grad3[gi], x, y, z);
    }

    static inline double dot(const std::array<int, 3>& g, double x, double y, double z) {
        return g[0] * x + g[1] * y + g[2] * z;
    }

    static inline const double F3 = 1.0 / 3.0;
    static inline const double G3 = 1.0 / 6.0;

    static inline const std::array<std::array<int, 3>, 12> grad3 = {{
        {1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
        {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
        {0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
    }};
};
