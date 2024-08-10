#pragma once
#include "HAL/PlatformTime.h"

class StopWatch {
public:
    StopWatch() : startTime(0.0), elapsedTime(0.0), running(false) {}

    void Start() {
        if (!running) {
            startTime = PlatformTime::GetTimeInSeconds(); // Use your PlatformTime function
            running = true;
        }
    }

    void Stop() {
        if (running) {
            elapsedTime += PlatformTime::GetTimeInSeconds() - startTime; // Calculate elapsed time
            running = false;
        }
    }

    void Reset() {
        startTime = 0.0;
        elapsedTime = 0.0;
        running = false;
    }

    double ElapsedTime() const {
        if (running) {
            return elapsedTime + (PlatformTime::GetTimeInSeconds() - startTime);
        } else {
            return elapsedTime;
        }
    }

    bool IsRunning() const {
        return running;
    }

private:
    double startTime;
    double elapsedTime;
    bool running;
};


#include <iostream>
#include <string>

class ScopedTimer {
public:
    ScopedTimer(const std::string& name) 
        : name(name), startTime(PlatformTime::GetTimeInSeconds()) {}

    ~ScopedTimer() {
        double endTime = PlatformTime::GetTimeInSeconds();
        double elapsedTime = endTime - startTime;
        std::cout << "Timer [" << name << "] elapsed time: " << elapsedTime << " seconds\n";
        std::cout << std::flush;
    }

private:
    std::string name;
    double startTime;
};

