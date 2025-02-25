#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;

public:
    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        cond_.notify_one();
    }

    bool pop(T &value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty())
            return false;
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [&]
                   { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::queue<T> emptyQueue;
        std::swap(queue_, emptyQueue);
    }
};