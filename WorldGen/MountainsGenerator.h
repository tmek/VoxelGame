#pragma once
#include "TerrainGenerators.h"

struct MountainsGenerator : TerrainGeneratorBase
{
    BlockState GetBlock(int x, int y, int z)
    {
        // surface samples
        // implement multiple octaves of perlin noise
        int octave1 = Sample2D(x, z, 0.02f) * 40;
        int octave2 = Sample2D(x, z, 0.04f) * 20;
        int octave3 = Sample2D(x, z, 0.08f) * 10;
        int additional_offset = 8;
        surface_height = octave1 + octave2 + octave3;
        surface_height += sea_level + additional_offset;

        // above surface (water or air)
        if (y > surface_height)
        {
            if (y > sea_level)
            {
                return AIR;
            }
            else
            {
                return WATER;
            }
        }

        return STONE;
        
        // surface (grass or sand)
        if (y == surface_height)
        {
            // underwater surface
            if (y <= sea_level)
            {
                return rand() % 10 || y == sea_level ? SAND : GRAVEL;
            }

            if (y <= sea_level + 2)
            {
                return SAND;
            }
            return GRASS;
        }

        // the 3 blocks below surface (dirt)
        if (y > surface_height - 3)
        {
            return DIRT;
        }

        // caves
        constexpr int cave_ground_depth = 6;
        constexpr float caveFrequency = 0.1f;
        constexpr float caveAmplitude = 10;
        constexpr float caveSquash = 1;

        constexpr int cave_radius = 2;
        int density = Sample3D(x, y * caveSquash, z, caveFrequency) * caveAmplitude;
        bool is_cave = abs(density) < cave_radius;

        if (is_cave && y < surface_height - cave_ground_depth)
        {
            return AIR;
        }

        // stone/ores
        return GetOreBlock(y);
    }    
};
