#include "directory_scanner.h"

// DirectoryScanner traverses a directory tree and enqueues regular files for later
// search work. It also records access and traversal issues in the statistics layer.
fsp::ss::DirectoryScanner::~DirectoryScanner() = default;

// Recursively walks the supplied root directory, pushes valid regular files into the
// queue, and records skipped or inaccessible entries for analysis in the final report.
void fsp::ss::DirectoryScanner::scan(
    const std::filesystem::path& root_path,
    fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue,
    const bool& recursive) const
{
    namespace fs = std::filesystem;

    std::error_code ec;

    // If the root path is invalid or not a directory, treat it as a runtime error.
    if (!fs::exists(root_path, ec) ||
        !fs::is_directory(root_path, ec))
    {
        fsp::sc::Statistics::file_error();
        fsp::sc::Statistics::file_skipped();
        return;
    }

    // Count the root directory itself as a scanned directory.
    fsp::sc::Statistics::directory_scanned();
    if(!recursive)
    {
        fs::directory_iterator it(root_path, {}, ec);
        fs::directory_iterator end;
        directory_scan(it, end, queue, ec);
    }
    else
    {
        fs::recursive_directory_iterator it(root_path, {}, ec);
        const fs::recursive_directory_iterator end;
        directory_scan(it, end, queue, ec);
    }
}
