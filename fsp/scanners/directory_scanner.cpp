#include "directory_scanner.h"

// DirectoryScanner: traverse a directory tree and push all regular files
// into the provided ThreadSafeQueue. Permission errors are skipped.
fsp::ss::DirectoryScanner::~DirectoryScanner() = default;

void fsp::ss::DirectoryScanner::scan(const std::filesystem::path& root_path,
                                    fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const
{
    namespace fs = std::filesystem;

    std::error_code ec;

    if (!fs::exists(root_path, ec) ||
        !fs::is_directory(root_path, ec))
    {
        fsp::sc::Statistics::file_error();
        fsp::sc::Statistics::file_skipped();
        return;
    }

    fsp::sc::Statistics::directory_scanned();

    fs::recursive_directory_iterator it(root_path, {}, ec);
    const fs::recursive_directory_iterator end;

    while (it != end)
    {
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
            fsp::sc::Statistics::file_error();
            fsp::sc::Statistics::file_skipped();
        }

        it.increment(ec);
    }
}
