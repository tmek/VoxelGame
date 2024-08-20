#include "SimplexNoise.h"

const std::array<std::array<int, 3>, 12> SimplexNoise::grad3b = {
    {
        {1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
        {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
        {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}
    }
};


//
// const std::array<std::array<int, 3>, 12> SimplexNoise::grad3 = {{
//     {1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
//     {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
//     {0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
// }};
//
// // Permutation modulo 12
// const std::array<int, 12> SimplexNoise::permMod12 = {
//     0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
// };
//
