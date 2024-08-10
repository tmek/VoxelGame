#include "FrameTiming.h"

double FrameTiming::FPS = 0.0;
double FrameTiming::FrameTime = 0.0;
double FrameTiming::SampleDuration = 0.0;
int32 FrameTiming::SampleFrames = 0;

void FrameTiming::Initialize()
{
        
}

double FrameTiming::GetFPS()
{
    return FPS;
}

double FrameTiming::GetFrameTime()
{
    return FrameTime;
}

double FrameTiming::GetFrameTimeInMS()
{
    return FrameTime * 1000.0;
}

void FrameTiming::Update(double DeltaTime)
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
