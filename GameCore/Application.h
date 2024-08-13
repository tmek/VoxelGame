#pragma once
#include "minwindef.h"

struct Application 
{
    // Begin Platform Specific
    static void SetInstanceHandle(HINSTANCE NewInstanceHandle);

    static HINSTANCE GetInstanceHandle();

private:
    static HINSTANCE InstanceHandle;
    // End Platform Specific
};
