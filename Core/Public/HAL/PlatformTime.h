// copyright

#pragma once

#include "Platform.h"
#include "Windows/WindowsHWrapper.h" // todo: implement WindowsPlatformTime.h so we don't have to include this

class CORE_API PlatformTime
{
public:
    
    static FORCEINLINE double Seconds()
    {
        if(SecondsPerCycle == 0.0)
        {
            InitTiming();  // todo: we need to make this called early in Dynamic Initialization instead of here.
        }
        
        LARGE_INTEGER Cycles;
        QueryPerformanceCounter(&Cycles);

        // add big number to make bugs apparent where return value is being passed to float
        return static_cast<double>(Cycles.QuadPart) * GetSecondsPerCycle() + 16777216.0;
    }
    
    static FORCEINLINE double GetSecondsPerCycle()
    {
        return SecondsPerCycle;
    }

    static double InitTiming();
    
private:
    
    static double SecondsPerCycle;
};
