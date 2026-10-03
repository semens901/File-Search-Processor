#include "directory_scanner.h"

// DirectoryScanner traverses a directory tree and enqueues regular files for later
// search work. It also records access and traversal issues in the statistics layer.
fsp::ss::DirectoryScanner::~DirectoryScanner() = default;

// Recursively walks the supplied root directory, pushes valid regular files into the
// queue, and records skipped or inaccessible entries for analysis in the final report.
void fsp::ss::DirectoryScanner::scan(const std::filesystem::path& root_path,
                                    fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const
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

    fs::recursive_directory_iterator it(root_path, {}, ec);
    const fs::recursive_directory_iterator end;

    while (it != end)
    {
        // An iterator error means the scan encountered an access or traversal problem.
        if (ec)
        {
            fsp::sc::Statistics::file_error();
            fsp::sc::Statistics::file_skipped();
            ec.clear();

            it.increment(ec);
            continue;
        }

        const auto& entry = *it;

        std::error_code entry_ec;

        // Count directories encountered during traversal to provide a basic directory metric.
        if (entry.is_directory(entry_ec))
        {
            if (!entry_ec)
            {
                fsp::sc::Statistics::directory_scanned();
            }
            else
            {
                fsp::sc::Statistics::file_error();
                fsp::sc::Statistics::file_skipped();
            }
        }
        else if (entry.is_regular_file(entry_ec))
        {
            // Regular files are queued for processing; unreadable files contribute to error statistics.
            if (entry_ec)
            {
                fsp::sc::Statistics::file_error();
                fsp::sc::Statistics::file_skipped();
            }
            else
            {
                queue.push(entry.path());
            }
        }
        else if (entry_ec)
        {
            // Other kinds of entries with access issues are recorded as errors as well.
            fsp::sc::Statistics::file_error();
            fsp::sc::Statistics::file_skipped();
        }

        it.increment(ec);
    }
}
