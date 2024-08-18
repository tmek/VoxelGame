#pragma once

#include <functional>
#include <future>
#include <type_traits>
#include <utility>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "HAL/Windows/WindowsProcess.h"

class ThreadPool
{
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    template <class F, class... Args>
    auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;

private:
    std::vector<std::jthread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};

inline ThreadPool::ThreadPool(size_t numThreads)
{
    VG_LOG(LogCategoryGeneral, LOG_INFO, "Creating ThreadPool with %zu threads...", numThreads);
    for (size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this, i]
        {
            // Worker thread init
            wchar_t threadName[32];
            swprintf(threadName, 32, L"Worker Thread %zu", i);
            WindowsPlatformProcess::SetThreadName(threadName);

            // worker thread's loop
            while (true)
            {
                std::function<void()> task;

                // lock the queue, check if there are any tasks, and pop the task off the queue
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                // start the task
                task();
            }
        });
    }

    // // queue a task on each worker thread to log the thread id
    // todo: there's not a way yet to enqueue a task to run on a specific thread
    // for (size_t i = 0; i < numThreads; ++i)
    // {
    //     Enqueue([i]
    //     {
    //         VG_LOG(LogCategoryGeneral, LOG_INFO, "Logged from worker thread id: %d", std::this_thread::get_id());
    //     });
    // }
    
    // Log the thread IDs after all threads are created and initialized.
    for (std::jthread& worker : workers)
    {
        // Each thread should be joinable immediately after being created and added to the `workers` vector
        VG_LOG(LogCategoryGeneral, LOG_INFO, "Worker thread id: %d", worker.get_id());
    }    
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::jthread& worker : workers)
    {
        VG_LOG(LogCategoryGeneral, LOG_INFO, "Joining worker thread id: %d", worker.get_id());        
        worker.join();
    }
}

template <class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>
{
    using returnType = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<returnType()>>(
        [func = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable
        {
            return func(std::forward<Args>(args)...);
        }
    );

    std::future<returnType> res = task->get_future();
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}
