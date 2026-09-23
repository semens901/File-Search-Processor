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
    // Implementation notes:
    // - A producer thread fills a thread-safe queue with file paths discovered
    //   by DirectoryScanner.
    // - A BS::thread_pool is used to schedule file search tasks concurrently.
    // - The manager collects the first matching LogEntry and returns it.
    // - `stop()` waits for the pool to finish outstanding tasks.

    FileProcessingManager::FileProcessingManager(std::string pattern, std::filesystem::path root_path, std::size_t thread_count)
        : pattern_(std::move(pattern)),
          root_path_(std::move(root_path)),
          thread_count_(thread_count),
          thread_pool_(thread_count_)
    {
    }

    FileProcessingManager::~FileProcessingManager() = default;

    fsp::fs::LogEntry FileProcessingManager::run()
    {
        std::atomic<bool> scanning_finished{false};
        std::thread producer([this, &scanning_finished]() {
            directoryScanner.scan(root_path_, queue_);
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
                        return fsp::fs::LogEntry{-1, "", file_path.string()};
                    }

                    return fileScanner.search(pattern_, file_reader);
                }));
            }

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
                        return fsp::fs::LogEntry{-1, "", file_path.string()};
                    }

                    return fileScanner.search(pattern_, file_reader);
                }));
                continue;
            }

            auto result = futures.front().get();
            futures.erase(futures.begin());

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

            if (scanning_finished && queue_.empty() && futures.empty())
            {
                break;
            }
        }

        stop();
        return last_result;
    }

    void FileProcessingManager::stop()
    {
        thread_pool_.wait();
    }

    void FileProcessingManager::filesSearch()
    {
        std::thread producer([this]() {
            directoryScanner.scan(root_path_, queue_);
            queue_.stop();
        });
        producer.detach();
    }
}
