#pragma once

#include "Task.h"
#include <type_traits>

namespace TE::Tasks
{
    template<typename Callable>
    auto Launch(const char* debugName, Callable&& taskBody)
        -> std::shared_ptr<TTask<std::invoke_result_t<Callable>>>
    {
        using ResultType = std::invoke_result_t<Callable>;

        // Create a shared_ptr to a new TTask instance
        auto task = std::make_shared<TTask<ResultType>>(
            debugName,
            std::function<ResultType()>(std::forward<Callable>(taskBody))
        );

        // Execute the task immediately
        task->Execute();

        return task;
    }
}
