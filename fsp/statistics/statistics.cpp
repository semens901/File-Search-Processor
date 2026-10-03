#include "statistics.h"

// Runtime statistics are kept in a lightweight singleton-like class so the search
// pipeline can report file counts, traversal counts, and timing without passing
// additional state through the entire call chain.

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

    // Starts the wall-clock timer used to report execution duration.
    void Statistics::start() noexcept
    {
        start_time_ = std::chrono::steady_clock::now();
    }

    // Stops the measurement and stores the end timestamp for elapsed() queries.
    void Statistics::stop() noexcept
    {
        end_time_ = std::chrono::steady_clock::now();
    }

    // Enables the statistics collector. All counters remain inactive until this flag is set.
    void Statistics::enable() noexcept
    {
        enabled_ = true;
    }

    // Returns the collection state so the main application can decide whether to print the summary.
    bool Statistics::enabled() noexcept
    {
        return enabled_;
    }

    // Counts a file processed by the search engine.
    void Statistics::file_scanned() noexcept
    {
        if(enabled_)
            files_scanned_.fetch_add(1, std::memory_order_relaxed);
    }

    // Counts a file that produced at least one successful line match.
    void Statistics::file_matched() noexcept
    {
        if(enabled_)
            files_matched_.fetch_add(1, std::memory_order_relaxed);
    }

    // Counts files or entries that were intentionally skipped during traversal.
    void Statistics::file_skipped() noexcept
    {
        if(enabled_)
            files_skipped_.fetch_add(1, std::memory_order_relaxed);
    }

    // Counts real runtime issues, such as unreadable files, access problems, or traversal failures.
    void Statistics::file_error() noexcept
    {
        if(enabled_)
            files_with_errors_.fetch_add(1, std::memory_order_relaxed);
    }

    // Counts directories encountered during scan so the report reflects traversal depth.
    void Statistics::directory_scanned() noexcept
    {
        if(enabled_)
            directories_scanned_.fetch_add(1, std::memory_order_relaxed);
    }

    // Counts each matched line, not just each file, to give more detailed search metrics.
    void Statistics::line_matched() noexcept
    {
        if(enabled_)
            matched_lines_.fetch_add(1, std::memory_order_relaxed);
    }

    // Returns the current number of files examined by the search engine.
    std::uint64_t Statistics::files_scanned() noexcept
    {
        return files_scanned_.load(std::memory_order_relaxed);
    }

    // Returns the number of files in which at least one match was found.
    std::uint64_t Statistics::files_matched() noexcept
    {
        return files_matched_.load(std::memory_order_relaxed);
    }

    // Returns the number of entries intentionally skipped while scanning.
    std::uint64_t Statistics::files_skipped() noexcept
    {
        return files_skipped_.load(std::memory_order_relaxed);
    }

    // Returns the number of files or entries that caused handling errors.
    std::uint64_t Statistics::files_with_errors() noexcept
    {
        return files_with_errors_.load(std::memory_order_relaxed);
    }

    // Returns the number of directories traversed during the scan.
    std::uint64_t Statistics::directories_scanned() noexcept
    {
        return directories_scanned_.load(std::memory_order_relaxed);
    }

    // Returns the total number of matched lines discovered across all processed files.
    std::uint64_t Statistics::matched_lines() noexcept
    {
        return matched_lines_.load(std::memory_order_relaxed);
    }

    // Returns elapsed execution time between start() and stop() calls.
    std::chrono::milliseconds Statistics::elapsed() noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end_time_ - start_time_);
    }
}