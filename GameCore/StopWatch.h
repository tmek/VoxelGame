#pragma once
#include <string>

class StopWatch {
public:
    StopWatch();

    void Start();
    void Stop();
    void Reset();

    double ElapsedTime() const;
    bool IsRunning() const;

private:
    double startTime;
    double elapsedTime;
    bool running;
};

class ScopedTimer {
public:
    ScopedTimer(const std::string& name);
    ~ScopedTimer();

private:
    std::string name;
    double startTime;
};

