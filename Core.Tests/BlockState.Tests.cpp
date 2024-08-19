#include <filesystem>

#include "gtest/gtest.h"

#include "VoxelData/Block.h"

// Test rotation of a block
TEST(VoxelDataTests, RotateBlock)
{
    // Arrange
    Block Block = {};
    
    // Act
    Block.Type = 1;        // Stone
    Block.RotationIndexY = 3;         // 3 * 90 = 270 degrees
    Block.IsTransparent = true;
    Block.IsVisible = true;
    Block.IsLightSource = true;
    Block.LightLevel = 8;
    Block.SkyLightLevel = -4;
    
    // Assert
    ASSERT_EQ(Block.Type, 1);
    ASSERT_EQ(Block.RotationIndexY, 3);
    ASSERT_EQ(Block.IsFlippedVertically, false);
    ASSERT_EQ(Block.IsVisible, true);
    ASSERT_EQ(Block.IsSolid, false);
    ASSERT_EQ(Block.IsTransparent, true);
    ASSERT_EQ(Block.IsLiquid, false);
    ASSERT_EQ(Block.IsLightSource, true);
    ASSERT_EQ(Block.LightLevel, 8);
    ASSERT_EQ(Block.SkyLightLevel, 12);
}

// Test random block state (random uint32)
TEST(VoxelDataTests, RandomBlockState)
{
    // Arrange
    Block Block = {};
    
    // Act
    Block.Type = 0x1234;
    Block.RotationIndexY = 0x3;
    Block.IsFlippedVertically = true;
    Block.IsVisible = false;
    Block.IsSolid = true;
    Block.IsTransparent = false;
    Block.IsLiquid = true;
    Block.IsLightSource = false;
    Block.LightLevel = 0x8;
    Block.SkyLightLevel = 0x4;
    
    // Assert
    ASSERT_EQ(Block.Type, 0x1234);
    ASSERT_EQ(Block.RotationIndexY, 0x3);
    ASSERT_EQ(Block.IsFlippedVertically, true);
    ASSERT_EQ(Block.IsVisible, false);
    ASSERT_EQ(Block.IsSolid, true);
    ASSERT_EQ(Block.IsTransparent, false);
    ASSERT_EQ(Block.IsLiquid, true);
    ASSERT_EQ(Block.IsLightSource, false);
    ASSERT_EQ(Block.LightLevel, 0x8);
    ASSERT_EQ(Block.SkyLightLevel, 0x4);
}

TEST(VoxelDataTests, CompareBlockToInt)
{
    // Arrange
    Block Block = {};
    
    // Act
    Block.Type = 0x1234;
    Block.RotationIndexY = 0x3;
    Block.IsFlippedVertically = true;
    Block.IsVisible = false;
    Block.IsSolid = true;
    Block.IsTransparent = false;
    Block.IsLiquid = true;
    Block.IsLightSource = false;
    Block.LightLevel = 0x8;
    Block.SkyLightLevel = 0x4;

    printf("Block: %u\n", Block);

    // test
    ASSERT_EQ(Block, 1213665844 );
}

// test an setting blockstate with an uint32
TEST(VoxelDataTests, EmptyBlockState)
{
    // Arrange
    // set block type of 3 and rotation of 2 using bitwise and
    Block Block = 3 | (2 << 16) | 1 << 19; 
    
    // Assert
    ASSERT_EQ(Block.Type, 3);
    ASSERT_EQ(Block.RotationIndexY, 2);
    ASSERT_EQ(Block.IsFlippedVertically, false);
    ASSERT_EQ(Block.IsVisible, true);
    ASSERT_EQ(Block.IsSolid, false);
    ASSERT_EQ(Block.IsTransparent, false);
    ASSERT_EQ(Block.IsLiquid, false);
    ASSERT_EQ(Block.IsLightSource, false);
    ASSERT_EQ(Block.LightLevel, 0);
    ASSERT_EQ(Block.SkyLightLevel, 0);

    Block Block2(12345); // Initialize from uint32_t
    uint32 rawData = Block2;  // Convert back to uint32_t

    printf("BlockState as unsigned int: %u\n", rawData);
}

