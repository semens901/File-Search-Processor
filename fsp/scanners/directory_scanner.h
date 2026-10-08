#ifndef DIRECTORY_SCANNER_H
#define DIRECTORY_SCANNER_H

#include "i_directory_scanner.h"
#include "fsp/statistics/statistics.h"

namespace fsp::ss
{
    // DirectoryScanner: concrete implementation of IDirectoryScanner that
    // recursively enumerates files under a root path and pushes them into
    // a ThreadSafeQueue for consumers.
    class DirectoryScanner : public IDirectoryScanner
    {
    public:
        ~DirectoryScanner() override;
        void scan(
            const std::filesystem::path& root_path,
            fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue,
            const bool& recursive) const override;
    
    private:
            template<typename begin, typename end>
            void directory_scan(
                begin begin_, 
                const end end_, 
                fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue, 
                std::error_code ec) const;
    };

    template <typename begin, typename end>
    inline void DirectoryScanner::directory_scan(
        begin begin_, 
        const end end_,
        fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue, 
        std::error_code ec) const
    {
        auto it = begin_;
        while (it != end_)
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
}

#endif // DIRECTORY_SCANNER_H