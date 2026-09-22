#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace fsp::cy
{
    template <typename T>
    class ThreadSafeQueue
    {
    public:
        ThreadSafeQueue() = default;
        ~ThreadSafeQueue() = default;

        ThreadSafeQueue(const ThreadSafeQueue&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

        void push(T value)
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                queue_.push(std::move(value));
            }
            condition_.notify_one();
        }

        bool try_pop(T& value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (queue_.empty())
            {
                return false;
            }

            value = std::move(queue_.front());
            queue_.pop();
            return true;
        }

        T pop()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait(lock, [this]() { return !queue_.empty() || stopped_; });

            if (stopped_ && queue_.empty())
            {
                throw std::runtime_error("Queue is stopped and empty");
            }

            T value = std::move(queue_.front());
            queue_.pop();
            return value;
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

        void stop()
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                stopped_ = true;
            }
            condition_.notify_all();
        }

    private:
        mutable std::mutex mutex_;
        std::queue<T> queue_;
        std::condition_variable condition_;
        bool stopped_{false};
    };
}

#endif // THREAD_SAFE_QUEUE_H
