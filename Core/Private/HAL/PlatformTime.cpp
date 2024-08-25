#include "HAL/PlatformTime.h"
#include "HAL/Platform.h"

#include "CoreMinimal.h"

#include "Windows/WindowsHWrapper.h"

double PlatformTime::SecondsPerCycle = 0.0;

double PlatformTime::InitTiming()
{
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency); // todo: make verify macro and wrap this
    SecondsPerCycle = 1.0 / static_cast<double>(Frequency.QuadPart);

    check(SecondsPerCycle != 0.0);
    
    return Seconds();
}
