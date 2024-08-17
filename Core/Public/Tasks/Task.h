#pragma once

#include <functional>
#include <memory>

namespace TE::Tasks
{
    template<typename ResultType>
    class TTask
    {
    public:
        // Retrieves the result of the task
        ResultType GetResult()
        {
            return Result;
        }

        // Executes the task's function body
        void Execute()
        {
            Result = TaskBody();
            bIsCompleted = true;
        }

    private:
        // Make Launch function a friend of TTask to access the private constructor
        template<typename Callable>
        friend auto Launch(const char* debugName, Callable&& taskBody)
            -> std::shared_ptr<TTask<std::invoke_result_t<Callable>>>;

        template<typename Callable>
        auto Launch(const char* debugName, Callable&& taskBody)
            -> std::shared_ptr<TTask<std::invoke_result_t<Callable>>>
        

        // Private constructor (only accessible via the Launch function)
        TTask(const char* debugName, std::function<ResultType()> taskBody)
            : TaskBody(taskBody), bIsCompleted(false), DebugName(debugName)
        {
        }

        std::function<ResultType()> TaskBody;
        ResultType Result;
        bool bIsCompleted;
        const char* DebugName;
    };

    // // Specialization for void tasks
    // template<>
    // class TTask<void>
    // {
    // public:
    //     // Retrieves the result of the task (no-op for void)
    //     void GetResult()
    //     {
    //         // Task just completes with no return value
    //     }
    //
    //     // Executes the task's function body
    //     void Execute()
    //     {
    //         TaskBody();
    //         bIsCompleted = true;
    //     }
    //
    // private:
    //     template<typename Callable>
    //     friend auto Launch(const char* debugName, Callable&& taskBody)
    //         -> std::shared_ptr<TTask<std::invoke_result_t<Callable>>>;
    //
    //     TTask(const char* debugName, std::function<void()> taskBody)
    //         : TaskBody(taskBody), bIsCompleted(false), DebugName(debugName)
    //     {
    //     }
    //
    //     std::function<void()> TaskBody;
    //     bool bIsCompleted;
    //     const char* DebugName;
    // };
}
