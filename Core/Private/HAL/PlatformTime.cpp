#include "HAL/PlatformTime.h"
#include "HAL/Platform.h"

#include "CoreMinimal.h"

#include "Windows/WindowsHWrapper.h"

// Define the static member
int64 FPlatformTime::Frequency = 0;

void FPlatformTime::Initialize()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));
}

int64 FPlatformTime::GetCPUCounter()
{
    int64 time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
    return time;
}
double_t FPlatformTime::InitTiming()
{
    return Seconds();
}
