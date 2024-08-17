#include "Tasks/Launch.h"
#include "Tasks/Task.h"

namespace TE::Tasks
{

    // FTask& Launch(const char* debugName, std::function<void()> taskBody, ETaskPriority priority, std::vector<FTask> prerequisites)
    // {
    //     auto task = std::make_shared<FTask>(debugName, std::move(taskBody), priority, std::move(prerequisites));
    //     task->Execute();  // Execute the task immediately in this single-threaded implementation
    //     return *task;
    // }
}
