#ifndef FILE_PROCESSING_MANAGER_H
#define FILE_PROCESSING_MANAGER_H

#include <cstddef>
#include <string>

#include "../scanners/file_scanner.h"
#include "../scanners/directory_scanner.h"
#include "../concurrency/thread_safe_queue.h"
#include "BS_thread_pool.hpp"

namespace fsp::sv
{
    class FileProcessingManager
    {
    public:
        FileProcessingManager(std::string pattern, std::filesystem::path root_path, std::size_t thread_count);
        ~FileProcessingManager();

        fsp::fs::LogEntry run();
        void stop();

    private:
        void filesSearch();

    private:
        std::string pattern_;
        fsp::ss::FileScanner fileScanner;
        fsp::ss::DirectoryScanner directoryScanner;
        std::filesystem::path root_path_;
        std::size_t thread_count_;
        BS::thread_pool<> thread_pool_;
        fsp::cy::ThreadSafeQueue<std::filesystem::path> queue_;
    };
}

#endif // FILE_PROCESSING_MANAGER_H