﻿
#include <iostream>
#include <string>

#include "Misc/StopWatch.h"
#include "HAL/PlatformTime.h"


StopWatch::StopWatch(): startTime(0.0), elapsedTime(0.0), running(false)
{}

void StopWatch::Start()
{
    if (!running) {
        startTime = PlatformTime::Seconds(); // Use your PlatformTime function
        running = true;
    }
}

void StopWatch::Stop()
{
    if (running) {
        elapsedTime += PlatformTime::Seconds() - startTime; // Calculate elapsed time
        running = false;
    }
}

void StopWatch::Reset()
{
    startTime = 0.0;
    elapsedTime = 0.0;
    running = false;
}

double StopWatch::GetElapsedSeconds() const
{
    if (running) {
        return elapsedTime + (PlatformTime::Seconds() - startTime);
    } else {
        return elapsedTime;
    }
}

bool StopWatch::IsRunning() const
{
    return running;
}

ScopedTimer::ScopedTimer(const std::string& name): name(name), startTime(PlatformTime::Seconds())
{}

ScopedTimer::~ScopedTimer()
{
    double endTime = PlatformTime::Seconds();
    double elapsedTime = endTime - startTime;
    std::cout << "Timer [" << name << "] elapsed time: " << elapsedTime << " seconds\n";
    std::cout << std::flush;
}
