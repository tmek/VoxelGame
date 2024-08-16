#pragma once

#include <functional>
#include <future>
#include <type_traits>
#include <utility>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    template<class F, class... Args>
    auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};

inline ThreadPool::ThreadPool(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}

template<class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
    using returnType = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<returnType()>>(
        [func = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable {
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
