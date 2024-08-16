#include "Misc/CoreDefines.h"
#include "gtest/gtest.h"

#include "HAL/PlatformProcess.h"


class MyTestSuite : public ::testing::Test {
protected:
  void SetUp() override {
    // Code here will be called immediately before each test.
    printf("Logging a message from MyTestSuite::SetUp\n");
    PlatformProcess::SleepMS(1000);
  }

  void TearDown() override {
    // Code here will be called immediately after each test.
    printf("Logging a message from MyTestSuite::TearDown\n");
  }

  // You can also add member variables here to use in the tests.
};

// Example test case
TEST_F(MyTestSuite, Test1) {
  // Test code that will run with the setup/teardown methods.
  printf("Logging a message from MyTestSuite::Test1\n");
}

TEST_F(MyTestSuite, Test2) {
  // Test code that will run with the setup/teardown methods.
  printf("Logging a message from MyTestSuite::Test2\n");
  
}

TEST(TestCaseName, TestName)
{
  std::cout << "Logging a message from the test." << std::endl;
  
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
  EXPECT_STREQ(EngineName, "TankEngine");

  PlatformProcess::SleepMS(1000);
}