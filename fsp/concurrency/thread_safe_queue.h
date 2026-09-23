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

        // push: enqueue a value and notify waiting threads
        void push(T value)
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                queue_.push(std::move(value));
            }
            condition_.notify_one();
        }

        // try_pop: attempt to dequeue without blocking. Returns false if empty.
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

        // pop: block until a value is available or the queue is stopped.
        // If the queue is stopped and empty, throws runtime_error.
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

        // empty: thread-safe check for emptiness
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

        // stop: atomically mark the queue stopped and wake waiting threads.
        // Consumers will receive an exception from `pop()` if the queue is
        // stopped and empty.
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
