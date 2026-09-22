#ifndef DIRECTORY_SCANNER_H
#define DIRECTORY_SCANNER_H

#include "i_directory_scanner.h"

namespace fsp::ss
{
    class DirectoryScanner : public IDirectoryScanner
    {
    public:
        ~DirectoryScanner() override;
        std::vector<std::filesystem::path> scan(const std::filesystem::path& root_path) const override;
    };
}

#endif // DIRECTORY_SCANNER_H