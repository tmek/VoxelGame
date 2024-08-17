#include "../GameCore/StopWatch.h"

#include "Misc/CoreDefines.h"
#include "gtest/gtest.h"

#include "HAL/PlatformProcess.h"

#include "Math/Noise/OptimizedPerlinNoise.h"
#include "Math/Noise/PerlinNoise.h"
#include "Math/Noise/SimplexNoise.h"

#define NoiseLoopCount 100000000

class MyTestSuite : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Code here will be called immediately before each test.
        printf("Logging a message from MyTestSuite::SetUp\n");
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test.
        printf("Logging a message from MyTestSuite::TearDown\n");
    }

    // You can also add member variables here to use in the tests.
};

// Example test case
TEST_F(MyTestSuite, Test1)
{
    // Test code that will run with the setup/teardown methods.
    printf("Logging a message from MyTestSuite::Test1\n");
}

TEST_F(MyTestSuite, Test2)
{
    // Test code that will run with the setup/teardown methods.
    printf("Logging a message from MyTestSuite::Test2\n");
}

TEST(TestCaseName, TestName)
{
    std::cout << "Logging a message from the test." << std::endl;

    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
    EXPECT_STREQ(EngineName, "TankEngine");
}

TEST(NoiseSuite, PerlinNoise)
{
    PerlinNoise noise;

    volatile double result = 0.0; // Declare a volatile variable

    // run and time the test several times
    for (int i = 0; i < NoiseLoopCount; i++)
    {
        double n = noise.sample3D(i * .01, i * 0.02, i * 0.03);
        result += n;
    }

    EXPECT_NE(result, 0.0); // This ensures that the result is actually computed
}

TEST(NoiseSuite, OptimizedPerlinNoise)
{
    OptimizedPerlinNoise noise;

    volatile double result = 0.0; // Declare a volatile variable

    // run and time the test several times
    for (int i = 0; i < NoiseLoopCount; i++)
    {
        double n = noise.sample3D(i * .01, i * 0.02, i * 0.03);
        result += n;
    }

    EXPECT_NE(result, 0.0); // This ensures that the result is actually computed
}

TEST(NoiseSuite, SimplexNoise)
{
    SimplexNoise noise;

    volatile double result = 0.0; // Declare a volatile variable

    // run and time the test several times
    for (int i = 0; i < NoiseLoopCount; i++)
    {
        double n = noise.sample3D(i * .01, i * 0.02, i * 0.03);
        result += n;
    }

    EXPECT_NE(result, 0.0); // This ensures that the result is actually computed
}

