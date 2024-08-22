#pragma once
#include "BlockTypes.h"

#include "Block/Block.h"

#include "HAL/PlatformTime.h"

#include "Math/Noise/OptimizedPerlinNoise.h"
#include "Math/Noise/PerlinNoise.h"
#include "Math/Noise/SimplexNoise.h"

#include "Render/BlockMeshBuilder.h"

#define sea_level 64

struct TerrainGeneratorBase
{
    int surface_height = 64;
    OptimizedPerlinNoise optimizedPerlinNoise;
    PerlinNoise perlinNoise;
    SimplexNoise simplexNoise;

    virtual const Block GetBlock(int x, int y, int z);

    // PlainsBiomeGenerator() : TerrainGeneratorBase()
    // {
    //     VG_LOG(LOG_CATEGORY_GENERAL, LOG_INFO, "PlainsGenerator::PlainsGenerator() - amplitude: %d", amplitude);
    // }
    
    TerrainGeneratorBase()
    {
        // seed the random number generator with time
        std::srand((uint32)FPlatformTime::GetCPUCounter());
    }

    TerrainGeneratorBase(uint32_t seed)
    {
        srand(seed);
    }

    float Sample2D(int x, int z, float freq)
    {
        return (float)simplexNoise.sample3D(x * freq, 0, z * freq);
    }

    float Sample3D(int x, int y, int z, float freq)
    {
        //return (float)perlinNoise.sample3D(x * freq, y * freq, z * freq);
        return (float)simplexNoise.sample3D(x * freq, y * freq, z * freq);
    }

    Block GetOreBlock(const int y)
    {
        // diamonds
        if (y < 16)
        {
            if (rand() % 1182 == 0)
            {
                return DIAMOND_BLOCK;
            }
        }

        // gold
        if (y < 32 && rand() % 100 == 0)
        {
            return GOLD_BLOCK;
        }

        // iron
        if (rand() % 20 == 0)
        {
            return IRON_BLOCK;
        }

        // everything else
        return rand() % 50 ? STONE : COAL_ORE; // todo: rand not threadsafe
        
    }

    virtual ~TerrainGeneratorBase() = default;
};
