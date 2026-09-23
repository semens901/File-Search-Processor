#ifndef I_DIRECTORY_SCANNER_H
#define I_DIRECTORY_SCANNER_H

#include <filesystem>
#include <string>
#include <vector>

#include "../concurrency/thread_safe_queue.h"

namespace fsp::ss
{
    // IDirectoryScanner: interface for components that enumerate file paths
    // under a root directory. Implementations push discovered file paths into
    // the provided ThreadSafeQueue for downstream processing.
    class IDirectoryScanner
    {
    public:
        virtual ~IDirectoryScanner() = default;

        virtual void scan(const std::filesystem::path& root_path,
                          fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const = 0;
    };
}

#endif // I_DIRECTORY_SCANNER_H