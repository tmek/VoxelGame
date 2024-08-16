#pragma once
#include "CoreTypes.h"

class CORE_API PlatformTime
{
public:
    static double GetTimeInSeconds()
    {
        static bool initialized = false;
        if (!initialized)
        {
            Initialize();
            initialized = true;
        }

        double time = static_cast<double>(GetCPUCounter());
        double frequency = static_cast<double>(Frequency);
        return time / frequency;
    }
    
    static int64 GetCPUCounter();

private:
    static int64 Frequency;

    static void Initialize();
};
