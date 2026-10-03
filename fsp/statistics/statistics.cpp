#include "statistics.h"

// statistics: placeholder implementation file. Statistics-related logic can
// be implemented here; currently the module is intentionally minimal.

namespace fsp::sc
{
    std::atomic<std::uint64_t> Statistics::files_scanned_ = 0;
    std::atomic<std::uint64_t> Statistics::files_matched_ = 0;
    std::atomic<std::uint64_t> Statistics::files_skipped_ = 0;
    std::atomic<std::uint64_t> Statistics::files_with_errors_ = 0;

    std::atomic<std::uint64_t> Statistics::directories_scanned_ = 0;

    std::atomic<std::uint64_t> Statistics::matched_lines_ = 0;

    Statistics::clock::time_point Statistics::start_time_;
    Statistics::clock::time_point Statistics::end_time_;

    bool Statistics::enabled_ = false;

    void Statistics::start() noexcept
    {
        start_time_ = std::chrono::steady_clock::now();
    }

    void Statistics::stop() noexcept
    {
        end_time_ = std::chrono::steady_clock::now();
    }

    void Statistics::enable() noexcept
    {
        enabled_ = true;
    }

    bool Statistics::enabled() noexcept
    {
        return enabled_;
    }

    void Statistics::file_scanned() noexcept
    {
        if(enabled_)
            files_scanned_.fetch_add(1, std::memory_order_relaxed);
    }

    void Statistics::file_matched() noexcept
    {
        if(enabled_)
            files_matched_.fetch_add(1, std::memory_order_relaxed);
    }

    void Statistics::file_skipped() noexcept
    {
        if(enabled_)
            files_skipped_.fetch_add(1, std::memory_order_relaxed);
    }

    void Statistics::file_error() noexcept
    {
        if(enabled_)
            files_with_errors_.fetch_add(1, std::memory_order_relaxed);
    }

    void Statistics::directory_scanned() noexcept
    {
        if(enabled_)
            directories_scanned_.fetch_add(1, std::memory_order_relaxed);
    }

    void Statistics::line_matched() noexcept
    {
        if(enabled_)
            matched_lines_.fetch_add(1, std::memory_order_relaxed);
    }

    std::uint64_t Statistics::files_scanned() noexcept
    {
        return files_scanned_.load(std::memory_order_relaxed);
    }

    std::uint64_t Statistics::files_matched() noexcept
    {
        return files_matched_.load(std::memory_order_relaxed);
    }

    std::uint64_t Statistics::files_skipped() noexcept
    {
        return files_skipped_.load(std::memory_order_relaxed);
    }

    std::uint64_t Statistics::files_with_errors() noexcept
    {
        return files_with_errors_.load(std::memory_order_relaxed);
    }

    std::uint64_t Statistics::directories_scanned() noexcept
    {
        return directories_scanned_.load(std::memory_order_relaxed);
    }

    std::uint64_t Statistics::matched_lines() noexcept
    {
        return matched_lines_.load(std::memory_order_relaxed);
    }

    std::chrono::milliseconds Statistics::elapsed() noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end_time_ - start_time_);
    }
}