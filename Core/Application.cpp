#include "Application.h"

HINSTANCE Application::InstanceHandle = nullptr;

void Application::SetInstanceHandle(HINSTANCE NewInstanceHandle)
{
    InstanceHandle = NewInstanceHandle;
}

HINSTANCE Application::GetInstanceHandle()
{
    return InstanceHandle;
}
