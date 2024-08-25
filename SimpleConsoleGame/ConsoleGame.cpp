#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
#include <execution>
#include <sstream>

#include "SimpleGameLoop.h"

#include "../Game/WorldGen/BlockTypes.h"

#include "Block/Block.h"

#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"
#include "Misc/CoreDefines.h"
#include "Logging/LogMacros.h"

#include "Misc/AssertionMacros.h"

class TestClassMain
{
public:
    TestClassMain()
    {
        TE_LOG(LogTemp, Warning, TEXT("Hello From (SimpleConsoleGame): %s"), "TestClassMain::TestClassMain()");
    }
};

TestClassMain testClassMain;


int main(int Argc, char* ArgV[])
{
    TE_LOG(LogTemp, Log, TEXT("main() entry point"));

    TE_LOG(LogTemp, Log, TEXT("argc: %d"), Argc);
    for (int i = 0; i < Argc; i++)
    {
        TE_LOG(LogTemp, Log, TEXT("argv[%d]: %s"), i, ArgV[i]);
    }

    Block block = {};
    block.TypeIndex = STONE;
    
    
    // log hardware concurrency
    TE_LOG(LogTemp, Log, TEXT("Hardware Concurrency: %d"), std::thread::hardware_concurrency());
    
    std::thread t([]() {
        TE_LOG(LogTemp, Log, TEXT("Hello from thread"));
        PlatformProcess::SleepMS(3000);
        TE_LOG(LogTemp, Log, TEXT("done sleeping in thread"));
    });


    TE_LOG(LogTemp, Log, TEXT("Sleeping for 1 second... %f"), PlatformTime::Seconds());
    PlatformProcess::SleepMS(1000);
    
    PlatformProcess::ShowConsole();

    TE_LOG(LogTemp, Log, TEXT("Done Sleeping. %f"), PlatformTime::Seconds());

    // TE_LOG(LogTemp, Log, "Joining thread t.");
    // t.join();
    
    check(true);

    // wait for key
    // std::cout << "Press any key to exit...";
    // std::cin.get();

    //t.detach();

    PlatformProcess::SleepMS(5000);

    // log if thread is joinable
    TE_LOG(LogTemp, Log, TEXT("Thread t is joinable: %d"), t.joinable());

    if(t.joinable())
    {
        t.join();
    }

    GameLoop gameLoop;
    
    std::thread t2([&gameLoop]() {
        TE_LOG(LogTemp, Log, TEXT("Stopping game loop in 3 seconds"));
        PlatformProcess::SleepMS(3100);
        gameLoop.bRequestExit = true;
        TE_LOG(LogTemp, Log, TEXT("done in thread 2"));
    });

    t2.detach();
    
    gameLoop.Run();

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Parallel for_each using the execution policy
    std::for_each(std::execution::par_unseq, data.begin(), data.end(), [](int& n) {
        n *= n;  // Square each element
        // std::thread::id ThreadId = std::this_thread::get_id();
        // printf("Thread ID: %uul\n", ThreadId );

        std::thread::id ThreadId = std::this_thread::get_id();
        std::stringstream ss;
        ss << ThreadId;
        printf("Thread ID: %s\n", ss.str().c_str());
        
        PlatformProcess::SleepMS(1000); // to verify parallelism
    });

    for (int n : data) {
        std::cout << n << " ";
    }
    
    return 0;
}
