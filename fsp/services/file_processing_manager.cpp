#include "file_processing_manager.h"

#include <filesystem>
#include <future>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

namespace fsp::sv
{
    FileProcessingManager::FileProcessingManager(std::string pattern, std::filesystem::path root_path, std::size_t thread_count)
        : pattern_(std::move(pattern)),
          fileScanner(pattern_),
          root_path_(std::move(root_path)),
          thread_count_(thread_count),
          thread_pool_(thread_count_)
    {
    }

    FileProcessingManager::~FileProcessingManager() = default;

    fsp::fs::LogEntry FileProcessingManager::run()
    {
        filesSearch();

        std::vector<std::future<fsp::fs::LogEntry>> futures;
        futures.reserve(thread_count_);

        fsp::fs::LogEntry last_result{};
        last_result.line_number = -1;

        while (!queue_.empty())
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

            for (auto& future : futures)
            {
                const auto result = future.get();
                last_result = result;

                if (result.line_number >= 0)
                {
                    spdlog::info("Match found in file: {}", result.file_name);
                    spdlog::info("Line number: {}", result.line_number);
                    spdlog::info("Line text: {}", result.text);
                }
                else
                {
                    spdlog::warn("No match found in: {}", result.file_name);
                }
            }

            futures.clear();
        }

        thread_pool_.wait();
        return last_result;
    }

    void FileProcessingManager::stop()
    {
        thread_pool_.wait();
    }

    void FileProcessingManager::filesSearch()
    {
        std::vector<std::filesystem::path> files = directoryScanner.scan(root_path_);
        for (const auto& file : files)
        {
            queue_.push(file);
        }
    }
}
