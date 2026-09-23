#include "directory_scanner.h"

// DirectoryScanner: traverse a directory tree and push all regular files
// into the provided ThreadSafeQueue. Permission errors are skipped.
fsp::ss::DirectoryScanner::~DirectoryScanner() = default;

void fsp::ss::DirectoryScanner::scan(const std::filesystem::path& root_path,
                                    fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const
{
    namespace fs = std::filesystem;

    // Validate root path
    if (!fs::exists(root_path) || !fs::is_directory(root_path))
    {
        return;
    }

    // Recursively iterate and emit regular files only
    const auto options = fs::directory_options::skip_permission_denied;
    for (const auto& entry : fs::recursive_directory_iterator(root_path, options))
    {
        if (entry.is_regular_file())
        {
            queue.push(entry.path());
        }
    }
}
