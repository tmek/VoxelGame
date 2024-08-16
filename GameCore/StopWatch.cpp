﻿#include "StopWatch.h"
#include <iostream>
#include <string>
#include "HAL/PlatformTime.h"


StopWatch::StopWatch(): startTime(0.0), elapsedTime(0.0), running(false)
{}

void StopWatch::Start()
{
    if (!running) {
        startTime = PlatformTime::GetTimeInSeconds(); // Use your PlatformTime function
        running = true;
    }
}

void StopWatch::Stop()
{
    if (running) {
        elapsedTime += PlatformTime::GetTimeInSeconds() - startTime; // Calculate elapsed time
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
        return elapsedTime + (PlatformTime::GetTimeInSeconds() - startTime);
    } else {
        return elapsedTime;
    }
}

bool StopWatch::IsRunning() const
{
    return running;
}

ScopedTimer::ScopedTimer(const std::string& name): name(name), startTime(PlatformTime::GetTimeInSeconds())
{}

ScopedTimer::~ScopedTimer()
{
    double endTime = PlatformTime::GetTimeInSeconds();
    double elapsedTime = endTime - startTime;
    std::cout << "Timer [" << name << "] elapsed time: " << elapsedTime << " seconds\n";
    std::cout << std::flush;
}
