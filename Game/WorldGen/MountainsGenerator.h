﻿#pragma once
#include "TerrainGenerators.h"

struct MountainsGenerator : TerrainGeneratorBase
{
    const Block GetBlock(int x, int y, int z) override
    {
        // surface samples
        // implement multiple octaves of perlin noise
        int octave1 = (int)Sample2D(x, z, 0.02f) * 40;
        int octave2 = (int)Sample2D(x, z, 0.04f) * 20;
        int octave3 = (int)Sample2D(x, z, 0.08f) * 10;
        int additional_offset = 8;
        surface_height = octave1 + octave2 + octave3;
        surface_height += sea_level + additional_offset;

        // surface samples
        // implement multiple octaves of perlin noise
        // float v = (float)x / 20;
        // float octave1 = sin(v);
        // float octave2 = sin(v*2)/2;
        // float octave3 = sin(v*4)/4;
        // float additional_offset = 8;
        //
        // octave1 *= 20;
        // octave2 *= 20;
        // octave3 *= 20;
        // surface_height = octave1 + octave2 + octave3;
        // surface_height += sea_level + additional_offset;
        
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

        //return STONE;
        
        // surface (grass or sand)
        if (y == surface_height)
        {
            // underwater surface
            if (y <= sea_level)
            {
                return rand() % 10 || y == sea_level ? ICE : GRAVEL;
            }

            if (y <= sea_level + 2)
            {
                return SNOW;
            }
            return SNOW;
        }

        // the 3 blocks below surface (dirt)
        if (y > surface_height - 3)
        {
            return STONE;
        }

        // caves
        constexpr int cave_ground_depth = 6;
        constexpr float caveFrequency = 0.1f;
        constexpr float caveAmplitude = 10;
        constexpr int caveSquash = 1;

        constexpr int cave_radius = 2;
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
