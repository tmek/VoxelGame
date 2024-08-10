#include "PlatformTime.h"
#include "HAL/Windows/PlatformWindows.h"

// Define the static member
int64 PlatformTime::Frequency = 0;

void PlatformTime::Initialize()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));
}

int64 PlatformTime::GetTime()
{
    int64 time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
    return time;
}
