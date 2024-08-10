#pragma once
#include "Core.h"

struct FrameTiming
{
    static void Initialize()
    {
        
    }

    static double GetFPS()
    {
        return FPS;
    }

    static double GetFrameTime()
    {
        return FrameTime;
    }

    static double GetFrameTimeInMS()
    {
        return FrameTime * 1000.0;
    }
    
    static void Update(double DeltaTime)
    {
        constexpr double SamplePeriod = 1.0;
        SampleDuration += DeltaTime;
        SampleFrames++;
        
        if(SampleDuration >= SamplePeriod)
        {
            FPS = SampleFrames / SampleDuration;
            FrameTime = SampleDuration / SampleFrames;
            SampleDuration = 0.0;
            SampleFrames = 0;
        }    
    }
    
private:    
    static double FPS;
    static double FrameTime;
    static double SampleDuration;
    static int32 SampleFrames;
};