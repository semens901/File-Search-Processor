#ifndef DIRECTORY_SCANNER_H
#define DIRECTORY_SCANNER_H

#include "i_directory_scanner.h"

namespace fsp::ss
{
    class DirectoryScanner : public IDirectoryScanner
    {
    public:
        ~DirectoryScanner() override;
        void scan(const std::filesystem::path& root_path,
                  fsp::cy::ThreadSafeQueue<std::filesystem::path>& queue) const override;
    };
}

#endif // DIRECTORY_SCANNER_H