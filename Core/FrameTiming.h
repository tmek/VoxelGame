#pragma once
#include "Core.h"

struct FrameTiming
{
    static void Initialize();
    static double GetFPS();
    static double GetFrameTime();
    static double GetFrameTimeInMS();
    static void Update(double DeltaTime);

private:    
    static double FPS;
    static double FrameTime;
    static double SampleDuration;
    static int32 SampleFrames;
};