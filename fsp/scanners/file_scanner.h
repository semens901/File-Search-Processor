#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include "i_file_scanner.h"
#include "fsp/search/search_line_engine.h"

namespace fsp::ss
{
    // FileScanner: concrete IFileScanner that performs file-level searches
    // by delegating to an underlying search engine. It encapsulates the
    // engine instance to keep the public interface simple.
    class FileScanner : public IFileScanner
    {
    public:
        FileScanner();
        ~FileScanner() override;
        fsp::fs::LogEntry search(const std::string& pattern, fsp::fs::FileReader& file_reader) override;
    private:
        std::string pattern_;
        fsp::fs::FileReader file_reader;
        fsp::sh::SearchLineEngine search_engine;
    };
}

#endif // FILE_SCANNER_H