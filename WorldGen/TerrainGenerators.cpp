#include "TerrainGenerators.h"
#include "BlockTypes.h"




BlockState TerrainGeneratorBase::GetBlock(int x, int y, int z)
{
    if(y <= sea_level) return STONE;

    return AIR;
}



// BlockType TerrainGenerators::GetPlainsBlockType(int x, int y, int z)
// {
//     int scale = 10;
//     int amplitude = 20;
//     
//     int terrain_height = SeaLevel + (sin(x) * 10.0f + cos(z) * 10.0f) * amplitude;
//
//     if(y <= terrain_height)
//     {
//         if(y < SeaLevel)
//         {
//             return (uint16_t)BlockNames::Water;
//         }
//         else if(y == SeaLevel)
//         {
//             return (uint16_t)BlockNames::Sand;
//         }
//         else if(y < terrain_height - 3)
//         {
//             return (uint16_t)BlockNames::Stone;
//         }
//         else
//         {
//             return (uint16_t)BlockNames::Grass;
//         }
//     }
//     else
//     {
//         return (uint16_t)BlockNames::Air;
//     }
// }
