#ifndef I_DIRECTORY_SCANNER_H
#define I_DIRECTORY_SCANNER_H

#include <filesystem>
#include <string>
#include <vector>

#include "../concurrency/thread_safe_queue.h"

namespace fsp::ss
{
    class IDirectoryScanner
    {
    public:
        virtual ~IDirectoryScanner() = default;

        virtual void scan(const std::filesystem::path& root_path,
                          fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const = 0;
    };
}

#endif // I_DIRECTORY_SCANNER_H