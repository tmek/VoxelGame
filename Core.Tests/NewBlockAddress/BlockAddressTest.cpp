// copyright

#include <csignal>

#include <gtest/gtest.h>
#include "BlockAddress.h"
#include "Misc/AssertionMacros.h"

constexpr uint32 Y_BIT = 8;
constexpr uint32 Z_BIT = 4;
constexpr uint32 X_BIT = 0;

// this conversion happens automatically in the BlockAddress struct, but explicitly here for testing.
constexpr void LocalToIndex(const uint32 X, const uint32 Z, const uint32 Y, uint32& Index) noexcept
{
    Index = Y << Y_BIT | Z << Z_BIT | X;

    check(X < CHUNK_WIDTH && Z < CHUNK_DEPTH && Y < CHUNK_HEIGHT);
    check(Index < BLOCKS_PER_CHUNK);
}

// this conversion happens automatically in the BlockAddress struct, but explicitly here for testing.
constexpr void IndexToLocal(const uint32& Index, uint32& X, uint32& Z, uint32& Y) noexcept
{
    X = Index & 0b1111;          // Index % 16;
    Z = Index >> Z_BIT & 0b1111; // (Index / 16) % 16; // the mod is needed because z must also loop every 16 
    Y = Index >> Y_BIT;          // Index / 256;

    check(X < CHUNK_WIDTH && Z < CHUNK_DEPTH && Y < CHUNK_HEIGHT);
    check(Index < BLOCKS_PER_CHUNK);
}

void PrintBlockAddress(const BlockAddress& Address) 
{
    printf("index=%d, xzy={%d,%d,%d}\n", Address.Index, Address.X, Address.Z, Address.Y);
}

TEST(BlockAddressTest, DefaultInitialization)
{
    BlockAddress addr;
    addr.X = 0;
    addr.Z = 0;
    addr.Y = 0;

    EXPECT_EQ(addr.X, 0);
    EXPECT_EQ(addr.Z, 0);
    EXPECT_EQ(addr.Y, 0);
    EXPECT_TRUE(addr.IsValid());
}

TEST(BlockAddressTest, ValidYValue)
{
    BlockAddress addr;
    addr.X = 5;
    addr.Z = 10;
    addr.Y = 383; // Max valid Y

    EXPECT_EQ(addr.X, 5);
    EXPECT_EQ(addr.Z, 10);
    EXPECT_EQ(addr.Y, 383);
    EXPECT_TRUE(addr.IsValid());
}

TEST(BlockAddressTest, InvalidYValue)
{
    BlockAddress addr;
    addr.X = 5;
    addr.Z = 10;
    addr.Y = 400; // Invalid Y

    EXPECT_EQ(addr.X, 5);
    EXPECT_EQ(addr.Z, 10);
    EXPECT_EQ(addr.Y, 400);
    EXPECT_FALSE(addr.IsValid());
}

TEST(BlockAddressTest, ColumnRowLayerAccess)
{
    BlockAddress addr;
    addr.Column = 3;
    addr.Row = 7;
    addr.Layer = 200;

    EXPECT_EQ(addr.Column, 3);
    EXPECT_EQ(addr.Row, 7);
    EXPECT_EQ(addr.Layer, 200);
    EXPECT_TRUE(addr.IsValid());
}

TEST(BlockAddressTest, IndexSetting)
{
    BlockAddress addr;
    addr.Index = 0xff33; // Example index that should set X=3, Z=3, Y=255

    EXPECT_EQ(addr.X, 3);
    EXPECT_EQ(addr.Z, 3);
    EXPECT_EQ(addr.Y, 255);
    EXPECT_TRUE(addr.IsValid());

    addr.Layer = 384; // Invalid Layer (Y)

    EXPECT_FALSE(addr.IsValid());
}

TEST(BlockAddressTest, FirstRowIndexes)
{
    BlockAddress CurrentBlock;

    for (int i = 0; i < 16; i++)
    {
        CurrentBlock.Index = i;

        EXPECT_EQ(CurrentBlock.X, i);
        EXPECT_EQ(CurrentBlock.Z, 0);
        EXPECT_EQ(CurrentBlock.Y, 0);
        EXPECT_TRUE(CurrentBlock.IsValid());
    }
}

TEST(BlockAddressTest, SecondRowIndexes)
{
    BlockAddress CurrentBlock;

    for (int i = 16; i < 32; i++)
    {
        CurrentBlock.Index = i;

        EXPECT_EQ(CurrentBlock.X, i % BLOCKS_PER_ROW);
        EXPECT_EQ(CurrentBlock.Z, 1);
        EXPECT_EQ(CurrentBlock.Y, 0);
        EXPECT_TRUE(CurrentBlock.IsValid());
    }

    for (int z = 0; z < 2; z++)
    {
        for (int x = 0; x < 16; x++)
        {
            CurrentBlock.Y = 0;
            CurrentBlock.X = x;
            CurrentBlock.Z = z;

            uint32 ExpectedIndex = CurrentBlock.Z * BLOCKS_PER_ROW + CurrentBlock.X;

            EXPECT_EQ(CurrentBlock.Index, ExpectedIndex);
            EXPECT_TRUE(CurrentBlock.IsValid());
        }
    }
}

TEST(BlockAddressTest, BoundaryChecks)
{
    BlockAddress addr;
    addr.X = 15;
    addr.Z = 15;
    addr.Y = 383;

    EXPECT_EQ(addr.X, 15);
    EXPECT_EQ(addr.Z, 15);
    EXPECT_EQ(addr.Y, 383);
    EXPECT_TRUE(addr.IsValid());

    addr.Y = 384; // Invalid Y

    EXPECT_FALSE(addr.IsValid());
}

TEST(BlockAddressTest, IndexAndLayerSynchronization)
{
    BlockAddress addr;
    LocalToIndex(15, 15, 383, addr.Index);

    EXPECT_EQ(addr.Column, 15);
    EXPECT_EQ(addr.Row, 15);
    EXPECT_EQ(addr.Layer, 383);
    EXPECT_TRUE(addr.IsValid());

    addr.Layer = 384; // Invalid Layer (Y)
    EXPECT_FALSE(addr.IsValid());
}

TEST(BlockAddressTest, XYZTiedToIndex)
{
    BlockAddress addr;

    // Set X, Z, Y and verify Index
    addr.X = 1;
    addr.Z = 2;
    addr.Y = 100;

    PrintBlockAddress(addr);
    
    uint32 ExpectedIndex;
    LocalToIndex(1, 2, 100, ExpectedIndex);
    EXPECT_EQ(addr.Index, ExpectedIndex);

    // Set Index and verify X, Z, Y
    addr.Index = 62 << 8 | 1 << 4 | 1;
    EXPECT_EQ(addr.Y, 62);
    EXPECT_EQ(addr.Z, 1);
    EXPECT_EQ(addr.X, 1);
}

TEST(BlockAddressTest, ConvertXZYToIndex)
{
    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            for (int x = 0; x < CHUNK_WIDTH; x++)
            {
                BlockAddress CurrentBlock;
                CurrentBlock.X = x;
                CurrentBlock.Z = z;
                CurrentBlock.Y = y;
                
                uint32 ExpectedIndex;
                LocalToIndex(x, z, y, ExpectedIndex);

                ASSERT_TRUE(CurrentBlock.IsValid());
                ASSERT_EQ(CurrentBlock.Index, ExpectedIndex);
            }
        }
    }
}

TEST(BlockAddressTest, ConvertIndexToXZY)
{
    BlockAddress CurrentBlock;

    uint32 TOTAL_BLOCKS = 16 * 16 * 384;

    // iterate every block in index order
    for (uint32 i = 0; i < TOTAL_BLOCKS; i++)
    {
        CurrentBlock.Index = i;

        uint32 ExpectedX;
        uint32 ExpectedZ;
        uint32 ExpectedY;

        IndexToLocal(CurrentBlock.Index, ExpectedX, ExpectedZ, ExpectedY);

        printf("index=%d, xzy={%d,%d,%d}\n", CurrentBlock.Index, CurrentBlock.X, CurrentBlock.Z, CurrentBlock.Y);

        ASSERT_TRUE(CurrentBlock.IsValid());
        ASSERT_EQ(CurrentBlock.X, ExpectedX);
        ASSERT_EQ(CurrentBlock.Z, ExpectedZ);
        ASSERT_EQ(CurrentBlock.Y, ExpectedY);
    }
}

TEST(BlockAddressTest, RangeChecks)
{
    BlockAddress CurrentBlock;

    // Test the range of the X, Z, Y values
    for (int i = 0; i < 16; i++)
    {
        CurrentBlock.X = i;
        CurrentBlock.Z = i;
        CurrentBlock.Y = i;

        EXPECT_TRUE(CurrentBlock.IsValid());
    }
    
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
