#pragma once
#include "TerrainGenerators.h"

struct ForestBeachBiomeGenerator : public TerrainGeneratorBase
{
    BlockState GetBlock(int x, int y, int z)
    {
        // bedrock
        if (y == 0) return BEDROCK;
        if (y == 1) return rand() % 3 ? BEDROCK : LAVA;
        if (y == 2) return rand() % 2 ? BEDROCK : LAVA;

        // surface samples
        int surf1 = sea_level + (int)(Sample2D(x, z, 0.01f) * 60);
        int surf2 = sea_level + (int)(Sample2D(x, z, 0.04f) * 40);
        surface_height = (surf1 + surf2) / 2;

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

        // dirt layer
        if (y > surface_height - 4)
        {
            return DIRT;
        }

        // caves
        constexpr int cave_ground_depth = 12;
        constexpr int caveSquash = 1;
        constexpr float caveFrequency = 0.05f; // 0.05f
        constexpr float caveAmplitude = 50; // 50
        constexpr int cave_radius = 8; // 8
        
        int density = static_cast<int>(Sample3D(x, y * caveSquash, z, caveFrequency) * caveAmplitude);
        bool is_cave = abs(density) < cave_radius;

        if (is_cave && y < surface_height - cave_ground_depth)
        {
            return AIR;
        }

        // stone/ores
        return GetOreBlock(y);
    }
};
