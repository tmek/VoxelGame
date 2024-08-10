#pragma once
#include "Platform.h"
#include "Windows.h"

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
    static int64 Frequency; // high-resolution performance counter frequency
    
    static void Initialize()
    {
        QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));
    }

    static int64 GetTime()
    {
        int64 time;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
        return time;
    }
};
