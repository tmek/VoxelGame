#pragma once

#include <string>
#include "Tasks/Task.h"
#include "Tasks/TaskPriority.h"

/*
namespace TE::Tasks
{
    class FPipe
    {
    public:
        FPipe(const char* debugName);

        template<typename ResultType>
        TTask<ResultType>& Launch(const char* debugName, std::function<ResultType()> taskBody, ETaskPriority priority = ETaskPriority::Normal);

        FTask& Launch(const char* debugName, std::function<void()> taskBody, ETaskPriority priority = ETaskPriority::Normal);

    private:
        std::string DebugName;
    };
}
*/