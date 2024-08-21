#pragma once

#include <functional>
#include <future>
#include <type_traits>
#include <utility>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Windows/WindowsPlatformProcess.h"
#include "Windows.h" // todo: this is temporary. implement generic platform version.  

#include "Logging/LogMacros.h"


class CORE_API ThreadPool
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
    TE_LOG(LogGeneral, Log, "Creating ThreadPool with %zu threads...", numThreads);
    for (size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this, i]
        {
            // Worker thread init
            wchar_t threadName[32];
            swprintf(threadName, 32, L"Worker Thread %zu", i);
            //WindowsPlatformProcess::SetThreadName(threadName);

            //VG_LOG(LogCategoryGeneral, LOG_INFO, "Setting worker thread priority to THREAD_PRIORITY_BELOW_NORMAL");
            //::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_NORMAL);

            // Calculate the core to assign based on thread ID

            int MaxCoresToUseForThreadPool = 4; // todo: make this configurable
            int AssignedCore = i % MaxCoresToUseForThreadPool; // 4 cores
            int SkipCores = 0; // todo: make this configurable
            int FinalCore =  AssignedCore + SkipCores;
            uint64_t affinityMask = 1ull << FinalCore;
            
            // Set thread affinity to improve cache locality, reducing cache misses by keeping the thread on the same core.
            //SetThreadAffinityMask(::GetCurrentThread(), affinityMask);

            TE_LOG(LogGeneral, Log, "Worker thread id: %d (%d), affinity mask: %d", i, ::GetCurrentThreadId(), affinityMask);
            
            // worker thread's loop
            while (true)
            {
                // give some time to OS
                // it's important to do this before starting a task to make sure this thread
                // is not running on the same core as the main thread and it's not competing for resources
                std::this_thread::yield();
                
                std::function<void()> task;

                // lock the queue, check if there are any tasks, and pop the task off the queue
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                // pin the thread affinity
                // // get current thread affinity (todo: is this possible? basically we the core that currently has the least load) 
                // auto threadid = ::GetCurrentThread();
                // DWORD_PTR CurrentAffinityMask = SetThreadAffinityMask(threadid, 0);
                //
                // if(CurrentAffinityMask != 0)
                // {
                //     VG_LOG(LogCategoryGeneral, LOG_INFO, "Worker thread id: %d (%d), current affinity mask: %d", i, ::GetCurrentThreadId(), CurrentAffinityMask);
                // }
                // else
                // {
                //     VG_LOG(LogCategoryGeneral, LOG_INFO, "Worker thread id: %d (%d), current affinity mask: %d", i, ::GetCurrentThreadId(), CurrentAffinityMask);
                // }
                //
                // // pin to current core
                // SetThreadAffinityMask(threadid, CurrentAffinityMask);
                
                // start the task
                task();

                // unpin
                // SetThreadAffinityMask(threadid, 0);

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
        TE_LOG(LogGeneral, Log, "Worker thread id: %d", worker.get_id());
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
        TE_LOG(LogGeneral, Log, "Joining worker thread id: %d", worker.get_id());        
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
