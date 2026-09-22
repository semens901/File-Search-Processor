#include "directory_scanner.h"

fsp::ss::DirectoryScanner::~DirectoryScanner() = default;

std::vector<std::filesystem::path> fsp::ss::DirectoryScanner::scan(const std::filesystem::path &root_path) const
{
    namespace fs = std::filesystem;
    std::vector<fs::path> file_paths;

    if (!fs::exists(root_path) || !fs::is_directory(root_path))
    {
        return file_paths;
    }

    auto options = fs::directory_options::skip_permission_denied;
    for (const auto& entry : fs::recursive_directory_iterator(root_path, options)) {
        if (entry.is_regular_file()) {
            file_paths.push_back(entry.path());
        }
    }

    return file_paths;
}