#include "Tasks/TaskEvent.h"
#include "Tasks/Task.h"

/*
namespace TE::Tasks
{
    FTaskEvent::FTaskEvent(const char* debugName)
        : DebugName(debugName), bIsTriggered(false)
    {
    }

    void FTaskEvent::Trigger()
    {
        bIsTriggered.store(true);
    }

    void FTaskEvent::AddPrerequisites(std::vector<FTask> prerequisiteTasks)
    {
        Prerequisites.insert(Prerequisites.end(), prerequisiteTasks.begin(), prerequisiteTasks.end());
    }

    void FTaskEvent::Wait()
    {
        for (const auto& prerequisite : Prerequisites)
        {
            prerequisite->Wait();  // Dereference shared_ptr to call Wait()
        }

        while (!bIsTriggered.load())
        {
            // Busy-wait until the event is triggered
        }
    }
}
*/