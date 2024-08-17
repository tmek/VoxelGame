#pragma once

#include <vector>
#include <atomic>
#include <string>
#include "Tasks/Task.h"
/*
namespace TE::Tasks
{
    class FTaskEvent
    {
    public:
        FTaskEvent(const char* debugName);

        void Trigger();

        void AddPrerequisites(std::vector<FTask> prerequisiteTasks);

        void Wait();

    private:
        std::vector<std::shared_ptr<FTask>> Prerequisites;
        std::atomic<bool> bIsTriggered;
        std::string DebugName;
    };
}
*/