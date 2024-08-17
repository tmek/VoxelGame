#include "Tasks/Pipe.h"
#include "Tasks/Task.h"
/*
namespace TE::Tasks
{
    FPipe::FPipe(const char* debugName)
        : DebugName(debugName)
    {
    }

    template<typename ResultType>
    TTask<ResultType>& FPipe::Launch(const char* debugName, std::function<ResultType()> taskBody, ETaskPriority priority)
    {
        auto task = std::make_shared<TTask<ResultType>>(debugName, std::move(taskBody), priority);
        task->Execute();  // Execute sequentially in the pipe
        return *task;
    }

    FTask& FPipe::Launch(const char* debugName, std::function<void()> taskBody, ETaskPriority priority)
    {
        auto task = std::make_shared<FTask>(debugName, std::move(taskBody), priority);
        task->Execute();  // Execute sequentially in the pipe
        return *task;
    }
}
*/