#ifndef I_FILE_SCANNER_H
#define I_FILE_SCANNER_H

#include "../search/search_line_engine.h"

namespace fsp::ss
{
    
    class IFileScanner
    {
    public:
        virtual ~IFileScanner() = default;
        virtual fsp::fs::LogEntry search(const std::string& pattern, fsp::fs::FileReader& file_reader) = 0;
    };
}

#endif // I_FILE_SCANNER_H