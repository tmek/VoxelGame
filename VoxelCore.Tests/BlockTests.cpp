// Copyright

#include <numeric>
#include <thread>

#include "gtest/gtest.h"
#include "Block/Block.h"

#include "Chunk/Chunk.h"
#include "Voxel/Conversions.h"


TEST(BlockTests, BlockRotation)
{
    // Arrange
    Block Block = {};

    // Act
    Block.Type = 1; // Stone
    Block.RotationIndexY = 3; // 3 * 90 = 270 degrees
    Block.IsTransparent = true;
    Block.IsVisible = true;
    Block.IsLightSource = true;
    Block.LightLevel = 8;
    Block.SkyLightLevel = -4; // should wrap around to +12

    // Assert
    EXPECT_EQ(Block.Type, 1);
    EXPECT_EQ(Block.RotationIndexY, 3);
    EXPECT_EQ(Block.IsFlipped, false);
    EXPECT_EQ(Block.IsVisible, true);
    EXPECT_EQ(Block.IsSolid, false);
    EXPECT_EQ(Block.IsTransparent, true);
    EXPECT_EQ(Block.IsLiquid, false);
    EXPECT_EQ(Block.IsLightSource, true);
    EXPECT_EQ(Block.LightLevel, 8);
    EXPECT_EQ(Block.SkyLightLevel, 12);

    EXPECT_EQ(Block.GetRotationYAngle(), 270.00001f);
}


// test size of layers
TEST(ChunkTests, ChunkConstants)
{
    EXPECT_EQ(ChunkWidth, 16);
    EXPECT_EQ(ChunkDepth, 16);
    EXPECT_EQ(ChunkHeight, 384);
    EXPECT_EQ(ChunkLayerSize, 16*16);
    EXPECT_EQ(ChunkSize, 16*384*16);
    EXPECT_EQ(MaxBlockHeight, 383);
}

TEST(ChunkTests, ChunkIsCacheLineAligned)
{
    // ARRANGE
    Chunk NewChunk = {};

    // ACT
    // assert Chunk is cache-line aligned
    void* pChunk = &NewChunk;
    EXPECT_EQ(reinterpret_cast<uintptr_t>(pChunk) % 64, 0);
}

/** Verify that the address of a BlockRef of the first block in chunk is the same as the chunk address */
TEST(ChunkTests, BlockRefOfFirstBlockIsSameAsChunkAddress)
{
    // ARRANGE
    Chunk NewChunk = {};

    // ACT
    BlockRef FirstBlock = NewChunk.Blocks[0];

    // ASSERT
    // assert Block address is same as first block in chunk
    EXPECT_EQ(&FirstBlock, &NewChunk.Blocks[0]);

    // assert Block address is same as chunk address
    void* pBlock = &FirstBlock;
    void* pChunk = &NewChunk;
    EXPECT_EQ(pBlock, pChunk);
}

TEST(ChunkTests, FillChunkWithRandomBlockTypes)
{
    srand(static_cast<uint32>(time(nullptr)));

    // ARRANGE
    Chunk NewChunk = {};
    ChunkKey NewChunkKey;
    NewChunkKey = {3, -2};

    // ACT
    // fill chunk multiple times
    unsigned long long total = 0;
    for (int j = 0; j < 10; j++)
    {
        for (int BlockIndex = 0; BlockIndex < ChunkSize; BlockIndex++)
        {
            //LocalToWorld(NewChunkKey, i, NewChunk.Blocks[i]);

            BlockType RandomBlockType = static_cast<BlockType>(rand());

            BlockRef CurrentBlock = NewChunk.Blocks[BlockIndex];

            if(j==0 && BlockIndex==0)
            {
                // assert Block address is same as first block in chunk
                EXPECT_EQ(&CurrentBlock, &NewChunk.Blocks[0]);

                // assert Block address is same as chunk address
                void* pBlock = &CurrentBlock;
                void* pChunk = &NewChunk;
                EXPECT_EQ(pBlock, pChunk);
            }
            CurrentBlock.Type = RandomBlockType;
            
            total += CurrentBlock.Type;
        }
    }

    // print total
    printf("Total: %llu\n", total);
}
