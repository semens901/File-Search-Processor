#include "file_processing_manager.h"

#include <atomic>
#include <filesystem>
#include <future>
#include <thread>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

namespace fsp::sv
{
    // The manager coordinates two independent activities: discovering files in the
    // target tree and processing those files in parallel with a bounded worker pool.
    // A producer thread keeps filling a queue, while the pool consumes items and
    // reports whether a matching line was found.

    // Initializes the processing pipeline with the search pattern, root directory,
    // and the maximum number of worker threads allowed to run concurrently.
        FileProcessingManager::FileProcessingManager(
                std::string pattern, 
                std::filesystem::path root_path, 
                std::size_t thread_count, 
                bool recursive)
                :   pattern_(std::move(pattern)),
                    root_path_(std::move(root_path)),
                    thread_count_(thread_count),
                    recursive_(recursive),
                    thread_pool_(thread_count_)
        {
        }

    FileProcessingManager::~FileProcessingManager() = default;

    // Runs the full search lifecycle and returns the first successful match found.
    // The loop keeps the producer and consumer sides balanced until the scan is done
    // and all queued tasks have been processed.
    fsp::fs::LogEntry FileProcessingManager::run()
    {
        // Tracks whether the directory scanner has completed its traversal.
        std::atomic<bool> scanning_finished{false};

        // Producer thread: discover files and push them into the queue for later processing.
        std::thread producer([this, &scanning_finished]() {
            directoryScanner.scan(root_path_, queue_, recursive_);
            scanning_finished = true;
            queue_.stop();
        });
        producer.detach();

        std::vector<std::future<fsp::fs::LogEntry>> futures;
        futures.reserve(thread_count_);

        fsp::fs::LogEntry last_result{};
        last_result.line_number = -1;

        while (true)
        {
            // Fill the worker pool while there are queued files and we have available slots.
            while (futures.size() < thread_count_ && !queue_.empty())
            {
                std::filesystem::path file_path;
                if (!queue_.try_pop(file_path))
                {
                    break;
                }

                futures.emplace_back(thread_pool_.submit_task([this, file_path]() {
                    fsp::fs::FileReader file_reader(file_path.string());
                    if (!file_reader.is_valid())
                    {
                        spdlog::error("Failed to open file: {}", file_path.string());
                        fsp::sc::Statistics::file_error();
                        return fsp::fs::LogEntry{-1, "", file_path.string()};
                    }

                    return fileScanner.search(pattern_, file_reader);
                }));
            }

            // If the pool is empty but work may still be coming from the producer,
            // wait for the next item from the queue and schedule it as a task.
            if (futures.empty())
            {
                if (scanning_finished && queue_.empty())
                {
                    break;
                }

                std::filesystem::path file_path;
                try
                {
                    file_path = queue_.pop();
                }
                catch (const std::runtime_error&)
                {
                    break;
                }

                futures.emplace_back(thread_pool_.submit_task([this, file_path]() {
                    fsp::fs::FileReader file_reader(file_path.string());
                    if (!file_reader.is_valid())
                    {
                        spdlog::error("Failed to open file: {}", file_path.string());
                        fsp::sc::Statistics::file_error();
                        return fsp::fs::LogEntry{-1, "", file_path.string()};
                    }

                    return fileScanner.search(pattern_, file_reader);
                }));
                continue;
            }

            // Wait for the next task to finish and consume its result before continuing.
            auto result = futures.front().get();
            futures.erase(futures.begin());

            // Preserve the first successful match, while still continuing to process other files.
            if (result.line_number >= 0)
            {
                if (last_result.line_number < 0)
                {
                    last_result = result;
                }

                spdlog::info("Match found in file: {}", result.file_name);
                spdlog::info("Line number: {}", result.line_number);
                spdlog::info("Line text: {}", result.text);
            }
            else
            {
                spdlog::warn("No match found in: {}", result.file_name);
            }

            // The loop ends only after scanning is complete, the queue is drained,
            // and all in-flight tasks have been resolved.
            if (scanning_finished && queue_.empty() && futures.empty())
            {
                break;
            }
        }

        stop();
        return last_result;
    }

    // Waits until all queued file-processing tasks in the thread pool are complete.
    void FileProcessingManager::stop()
    {
        thread_pool_.wait();
    }

    // Legacy helper entry point kept for compatibility with the project structure.
    // It scans the root directory and queues matching work for consumers.
    void FileProcessingManager::filesSearch()
    {
        std::thread producer([this]() {
            directoryScanner.scan(root_path_, queue_, recursive_);
            queue_.stop();
        });
        producer.detach();
    }
}
