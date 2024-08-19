/*
#include "Voxel/Conversions.h"

#include "Voxel/ChunkConstants.h"
#include "gtest/gtest.h"

#include "VoxelData/Chunk.h"
#include "VoxelData/ChunkKey.h"

// test size of layers
TEST(ChunkConstantsTestCase, ChunkConstants)
{
    ASSERT_EQ(ChunkWidth, 16);
    ASSERT_EQ(ChunkDepth, 16);
    ASSERT_EQ(ChunkHeight, 384);

    ASSERT_EQ(ChunkLayerSize, 16*16);
    ASSERT_EQ(ChunkSize, 16*384*16);
    
    ASSERT_EQ(MaxBlockHeight, 383);
}

TEST(ChunkConstantsTestCase, FillChunkRand)
{
    srand(time(0));
    
    Chunk NewChunk = {};
    ChunkKey NewChunkKey = { 3,  -2 };
    

    // fill chunk multiple times
    unsigned long long total = 0;
    for(int j = 0; j < 10; j++)
    {
        for (int i = 0; i < ChunkSize; i++)
        {
            LocalToWorld(NewChunkKey, i, NewChunk.Blocks[i]);
            
            uint32 NewBlockType = static_cast<uint32>(rand());
            NewChunk.Blocks[i] = NewBlockType;
            total += NewBlockType;
        }
    }

    // print total
    printf("Total: %llu\n", total);

    SUCCEED();
}
*/