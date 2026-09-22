#include "directory_scanner.h"

fsp::ss::DirectoryScanner::~DirectoryScanner() = default;

void fsp::ss::DirectoryScanner::scan(const std::filesystem::path& root_path,
                                    fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const
{
    namespace fs = std::filesystem;

    if (!fs::exists(root_path) || !fs::is_directory(root_path))
    {
        return;
    }

    const auto options = fs::directory_options::skip_permission_denied;
    for (const auto& entry : fs::recursive_directory_iterator(root_path, options))
    {
        if (entry.is_regular_file())
        {
            queue.push(entry.path());
        }
    }
}
