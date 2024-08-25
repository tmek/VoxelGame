// // Copyright (c) 2024 Thomas. All rights reserved.

#pragma once
#include "Generated/CoreExports.h"


/** Provides information about the application. */
class Application
{
public:

    /** Gets current time in seconds.
     *
     *  @return Current time in seconds.
     */
    inline static double GetCurrentTime()
    {
        return CurrentTime;
    }

    /**
     * Sets current time in seconds.
     *
     * @param Seconds - Time in seconds.
     */
    static void SetCurrentTime(double Seconds)
    {
        CurrentTime = Seconds;
    }

    /** Gets previous value of CurrentTime.
     *
     *  @return Previous value of CurrentTime.
     */
    inline static double GetLastTime()
    {
        return LastTime;
    }

    /** Updates last time to CurrentTime. */
    static void UpdateLastTime()
    {
        LastTime = CurrentTime;
    }

    /** Gets time delta in seconds.
     *
     *  @return Time delta in seconds.
     */
    inline static double GetDeltaTime()
    {
        return DeltaTime;
    }

    /** Sets time delta in seconds.
     *
     * @param Seconds - Time delta in seconds.
     */
    static void SetDeltaTime(double Seconds)
    {
        DeltaTime = Seconds;
    }

private:

    /** Holds current time. */
    static CORE_API double CurrentTime;

    /** Holds previous value of CurrentTime. */
    static CORE_API double LastTime;

    /** Holds delta time in seconds. */
    static CORE_API double DeltaTime;
};
