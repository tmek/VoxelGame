// copyright

#pragma once

#include <math.h>

#include "Platform.h"

class CORE_API FPlatformTime
{
public:
    static double Seconds()
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
    static double_t InitTiming();

private:
    static int64 Frequency;

    static void Initialize();
};
