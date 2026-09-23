#ifndef DIRECTORY_SCANNER_H
#define DIRECTORY_SCANNER_H

#include "i_directory_scanner.h"

namespace fsp::ss
{
    // DirectoryScanner: concrete implementation of IDirectoryScanner that
    // recursively enumerates files under a root path and pushes them into
    // a ThreadSafeQueue for consumers.
    class DirectoryScanner : public IDirectoryScanner
    {
    public:
        ~DirectoryScanner() override;
        void scan(const std::filesystem::path& root_path,
                  fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const override;
    };
}

#endif // DIRECTORY_SCANNER_H