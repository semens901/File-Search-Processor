#include <catch2/catch_test_macros.hpp>

#include <future>
#include <thread>

#include "fsp/concurrency/thread_safe_queue.h"

TEST_CASE("ThreadSafeQueue stores and retrieves values in order", "[concurrency][unit]")
{
    fsp::cy::ThreadSafeQueue<int> queue;

    REQUIRE(queue.empty());

    queue.push(10);
    queue.push(20);
    queue.push(30);

    REQUIRE_FALSE(queue.empty());

    int value = 0;
    REQUIRE(queue.try_pop(value));
    REQUIRE(value == 10);

    REQUIRE(queue.pop() == 20);
    REQUIRE(queue.pop() == 30);

    REQUIRE(queue.empty());
    REQUIRE_FALSE(queue.try_pop(value));
}

TEST_CASE("ThreadSafeQueue returns false for empty try_pop", "[concurrency][unit]")
{
    fsp::cy::ThreadSafeQueue<std::string> queue;

    std::string value;
    REQUIRE_FALSE(queue.try_pop(value));
    REQUIRE(queue.empty());

    queue.push("first");
    REQUIRE(queue.try_pop(value));
    REQUIRE(value == "first");

    REQUIRE_FALSE(queue.try_pop(value));
}

TEST_CASE("ThreadSafeQueue wakes blocked pop when stopped", "[concurrency][unit]")
{
    fsp::cy::ThreadSafeQueue<int> queue;

    auto future = std::async(std::launch::async, [&queue]() {
        return queue.pop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue.push(42);

    REQUIRE(future.get() == 42);

    auto empty_future = std::async(std::launch::async, [&queue]() {
        try
        {
            return queue.pop();
        }
        catch (const std::runtime_error&)
        {
            return -1;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue.stop();

    REQUIRE(empty_future.get() == -1);
}
