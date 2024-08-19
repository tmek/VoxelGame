#include "gtest/gtest.h"

/*
#include "Logging/LogMacros.h"

#include "Tasks/Launch.h"
#include "Tasks/Task.h"


class TasksTestSuite : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Code here will be called immediately before each test.
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test.
    }

    // You can also add member variables here to use in the tests.
};


TEST(LaunchTask, LaunchSimpleTask)
{
    std::cout << "Logging a message from the Tasks test." << std::endl;

    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);

    using namespace TE::Tasks;

    std::function<int(void)> myfunc = [] { return 42; };
    
    auto task1 = Launch("SimpleTask", myfunc);
    TE_LOG(LogTemp, Log, "Task1 result: %d", task1->GetResult());

    auto task2 = Launch("SimpleTask", [] { return 123; });
    TE_LOG(LogTemp, Log, "Task2 result: %d", task2->GetResult());
}
*/