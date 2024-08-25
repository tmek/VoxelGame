// // Copyright (c) 2024 Thomas. All rights reserved.

#pragma once
#include "HAL/PlatformTime.h"
#include "Misc/Application.h"

// TODO: Engine class should live in an Engine module.
class CORE_API Engine
{
public:

    static inline void UpdateTimeAndHandleMaxTickRate();

    static void InitializeRHI();
};


void Engine::UpdateTimeAndHandleMaxTickRate()
{
    const double CurrentTime = PlatformTime::Seconds();
    const double LastTime = Application::GetLastTime();
    const double DeltaTime = CurrentTime - LastTime;

    Application::SetCurrentTime(CurrentTime);
    Application::SetDeltaTime(DeltaTime);
    Application::UpdateLastTime();
}
