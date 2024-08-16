#include "HAL/PlatformTime.h"
#include "HAL/Platform.h"
#include "Windows/WindowsIncludes.h"

// Define the static member
int64 PlatformTime::Frequency = 0;

void PlatformTime::Initialize()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));
}

int64 PlatformTime::GetCPUCounter()
{
    int64 time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
    return time;
}
