#include "Tasks/Task.h"
#include <iostream>
/*
namespace TE::Tasks
{
    template<typename ResultType>
    TTask<ResultType>::TTask(const char* debugName, std::function<ResultType()> taskBody, ETaskPriority priority, std::vector<std::shared_ptr<FTask>> prerequisites)
        : TaskBody(std::move(taskBody)), Prerequisites(std::move(prerequisites)), DebugName(debugName), Priority(priority), bIsCompleted(false)
    {
    }

    template<typename ResultType>
    bool TTask<ResultType>::IsCompleted() const
    {
        return bIsCompleted.load();
    }

    template<typename ResultType>
    void TTask<ResultType>::Wait() const
    {
        while (!bIsCompleted.load())
        {
            // Busy-wait loop
        }
    }

    template<typename ResultType>
    bool TTask<ResultType>::Wait(std::chrono::milliseconds timeout) const
    {
        auto start = std::chrono::steady_clock::now();
        while (!bIsCompleted.load())
        {
            if (std::chrono::steady_clock::now() - start >= timeout)
            {
                return false;  // Timeout occurred
            }
        }
        return true;
    }

    template<typename ResultType>
    ResultType TTask<ResultType>::GetResult()
    {
        Wait();  // Ensure the task is complete
        return TaskFuture.get();
    }

    template<typename ResultType>
    void TTask<ResultType>::AddDependency(std::shared_ptr<FTask> prerequisiteTask)
    {
        Prerequisites.push_back(prerequisiteTask);  // Simply add the shared pointer to the vector
    }

    template<typename ResultType>
    template<typename NestedResultType>
    TTask<NestedResultType> TTask<ResultType>::LaunchNested(const char* debugName, std::function<NestedResultType()> nestedTaskBody, ETaskPriority priority)
    {
        TTask<NestedResultType> nestedTask(debugName, std::move(nestedTaskBody), priority);
        AddNested(nestedTask);
        nestedTask.Execute();
        return nestedTask;
    }

    template<typename ResultType>
    void TTask<ResultType>::AddNested(std::shared_ptr<FTask> nestedTask)
    {
        NestedTasks.push_back(nestedTask);  // Add the shared pointer to the nested tasks
    }

    template<typename ResultType>
    void TTask<ResultType>::Reset()
    {
        bIsCompleted.store(false);
        Prerequisites.clear();
        NestedTasks.clear();
        TaskBody = nullptr;
    }

    template<typename ResultType>
    void TTask<ResultType>::Execute()
    {
        for (const auto& prerequisite : Prerequisites)
        {
            prerequisite->Wait();  // Ensure all prerequisites are completed
        }

        // Execute the task body and store the result in TaskFuture
        TaskFuture = std::async(std::launch::deferred, TaskBody);  // Launch the task synchronously in this single-threaded example
        bIsCompleted.store(true);
    }

    // Specialization for void tasks
    TTask<void>::TTask(const char* debugName, std::function<void()> taskBody, ETaskPriority priority, std::vector<std::shared_ptr<FTask>> prerequisites)
        : TaskBody(std::move(taskBody)), Prerequisites(prerequisites), DebugName(debugName), Priority(priority), bIsCompleted(false)
    {
    }

    bool TTask<void>::IsCompleted() const
    {
        return bIsCompleted.load();
    }

    void TTask<void>::Wait() const
    {
        while (!bIsCompleted.load())
        {
            // Busy-wait loop
        }
    }

    bool TTask<void>::Wait(std::chrono::milliseconds timeout) const
    {
        auto start = std::chrono::steady_clock::now();
        while (!bIsCompleted.load())
        {
            if (std::chrono::steady_clock::now() - start >= timeout)
            {
                return false;  // Timeout occurred
            }
        }
        return true;
    }

    void TTask<void>::GetResult()
    {
        Wait();  // Ensure the task is complete
        TaskFuture.get();
    }

    void TTask<void>::AddDependency(std::shared_ptr<FTask> prerequisiteTask)
    {
        Prerequisites.push_back(prerequisiteTask);
    }

    template<typename NestedResultType>
    TTask<NestedResultType> TTask<void>::LaunchNested(const char* debugName, std::function<NestedResultType()> nestedTaskBody, ETaskPriority priority)
    {
        TTask<NestedResultType> nestedTask(debugName, std::move(nestedTaskBody), priority);
        AddNested(nestedTask);
        nestedTask.Execute();
        return nestedTask;
    }

    void TTask<void>::AddNested(std::shared_ptr<FTask> nestedTask)
    {
        NestedTasks.push_back(nestedTask);
    }

    void TTask<void>::Reset()
    {
        bIsCompleted.store(false);
        Prerequisites.clear();
        NestedTasks.clear();
        TaskBody = nullptr;
    }

    void TTask<void>::Execute()
    {
        for (const auto& prerequisite : Prerequisites)
        {
            prerequisite->Wait();  // Ensure all prerequisites are completed
        }

        TaskFuture = std::async(std::launch::deferred, TaskBody);  // Launch the task synchronously in this single-threaded example
        bIsCompleted.store(true);
    }

    // Explicit template instantiation for void type
    template class TTask<void>;
}
*/