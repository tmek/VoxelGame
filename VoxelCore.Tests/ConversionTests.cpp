// copyright

#include "gtest/gtest.h"
#include "Block/Block.h"
#include "Voxel/Conversions.h"

#define PlaceholderFunction WorldToChunkGrid

void PrintBlockIndexAndOffset(const BlockIndex& Index, const BlockCoordinate& CurrentBlock)
{
    printf("index=%d, xzy={%d,%d,%d}\n", Index, CurrentBlock.X, CurrentBlock.Y, CurrentBlock.Z);
}

TEST(BlockTests, WorldPositionToChunkKeyTest)
{

    int32 ChunkGridCoordinate;

    PlaceholderFunction(0, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, 0);

    PlaceholderFunction(1, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, 0);

    PlaceholderFunction(15, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, 0);
    
    PlaceholderFunction(16, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, 1);

    PlaceholderFunction(31, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, 1);

    PlaceholderFunction(32, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, 2);

    PlaceholderFunction(47, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, 2);

    // negatives
    PlaceholderFunction(-1, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, -1);

    PlaceholderFunction(-16, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, -1);

    PlaceholderFunction(-17, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, -2);

    PlaceholderFunction(-32, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, -2);
    
    PlaceholderFunction(-33, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, -3);

    PlaceholderFunction(-48, ChunkGridCoordinate);
    EXPECT_EQ(ChunkGridCoordinate, -3);
}

TEST(BlockAddressTest, ConvertXZYToIndexAndIndexToXZY)
{
    int ExpectedIndex = 0;
    
    for (int ExpectedY = 0; ExpectedY < ChunkHeight; ExpectedY++)
    {
        for (int ExpectedZ = 0; ExpectedZ < ChunkDepth; ExpectedZ++)
        {
            for (int ExpectedX = 0; ExpectedX < ChunkWidth; ExpectedX++, ExpectedIndex++)
            {
                BlockCoordinate Coordinate;
                BlockIndex Index;
                
                LocalToIndex(ExpectedX, ExpectedY, ExpectedZ, Index);
                IndexToLocal(ExpectedIndex, Coordinate);

                ASSERT_EQ(ExpectedIndex, Index);
                ASSERT_EQ(ExpectedX, Coordinate.X);
                ASSERT_EQ(ExpectedY, Coordinate.Y);
                ASSERT_EQ(ExpectedZ, Coordinate.Z);
            }
        }
    }
}

TEST(BlockAddressTest, BlocksAreInChunkRange)
{
    uint32 TOTAL_BLOCKS = 16 * 16 * 384;

    // iterate every block in index order
    for (uint32 ExpectedIndex = 0; ExpectedIndex < TOTAL_BLOCKS; ExpectedIndex++)
    {
        BlockCoordinate CurrentBlock;
        BlockIndex Index;

        // convert index to xzy and back
        IndexToLocal(ExpectedIndex, CurrentBlock);
        LocalToIndex(CurrentBlock, Index);

        PrintBlockIndexAndOffset(Index, CurrentBlock);

        
        ASSERT_TRUE(IsBlockInChunk(CurrentBlock.X, CurrentBlock.Y, CurrentBlock.Z));
        ASSERT_EQ(ExpectedIndex, Index);
    }
}

TEST(BlockAddressTest, WorldToChunkSpace)
{
    int LocalCoordinate;
    
    WorldToLocal(0, LocalCoordinate);
    EXPECT_EQ(LocalCoordinate, 0);

    WorldToLocal(1, LocalCoordinate);
    EXPECT_EQ(LocalCoordinate, 1);

    WorldToLocal(16, LocalCoordinate);
    EXPECT_EQ(LocalCoordinate, 0);

    WorldToLocal(-1, LocalCoordinate);
    EXPECT_EQ(LocalCoordinate, 15);

    WorldToLocal(-16, LocalCoordinate);
    EXPECT_EQ(LocalCoordinate, 0);

    WorldToLocal(-17, LocalCoordinate);
    EXPECT_EQ(LocalCoordinate, 15);

    WorldToLocal(-18, LocalCoordinate);
    EXPECT_EQ(LocalCoordinate, 14);
    
    for(int i=-45; i<45; i++)
    {
        WorldToLocal(i, LocalCoordinate);
        int a = i;
        int b = 16;
        printf("i=%d, local=%d, imod16=%d\n", i, LocalCoordinate, (a % b + b) % b); // ((i%16)+16)%16);
    }
}
