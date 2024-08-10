#pragma once
#include "Platform.h"

class PlatformTime
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

        double time = static_cast<double>(GetTime());
        double frequency = static_cast<double>(Frequency);
        return time / frequency;
    }

private:
    static int64 Frequency;
    static int64 GetTime();

    static void Initialize();
};
