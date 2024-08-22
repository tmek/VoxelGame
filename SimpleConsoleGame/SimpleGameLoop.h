#pragma once
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"

#include "Logging/LogMacros.h"


struct GameLoop
{
    bool bRequestExit = false;
    
    void Run()
    {
        while (bRequestExit == false)
        {
            // Process input
            // Update game state
            // Render
            
            TE_LOG(LogTemp, Log, TEXT("GameLoop::Run() %f"), FPlatformTime::Seconds());
            PlatformProcess::SleepMS(1000);
        }
    }
};
