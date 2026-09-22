#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include "i_file_scanner.h"
#include "fsp/search/search_line_engine.h"

namespace fsp::ss
{
    class FileScanner : public IFileScanner
    {
    public:
        FileScanner(std::string pattern);
        ~FileScanner() override;
        fsp::fs::LogEntry search(const std::string& pattern, fsp::fs::FileReader& file_reader) override;
    private:
        std::string pattern_;
        fsp::fs::FileReader file_reader;
        fsp::sh::SearchLineEngine search_engine;
    };
}

#endif // FILE_SCANNER_H