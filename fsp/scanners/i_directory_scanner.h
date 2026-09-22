#ifndef I_DIRECTORY_SCANNER_H
#define I_DIRECTORY_SCANNER_H

#include <filesystem>
#include <string>
#include <vector>

namespace fsp::ss
{
    class IDirectoryScanner
    {
    public:
        virtual ~IDirectoryScanner() = default;

        virtual std::vector<std::filesystem::path> scan(const std::filesystem::path& root_path) const = 0;
    };
}

#endif // I_DIRECTORY_SCANNER_H