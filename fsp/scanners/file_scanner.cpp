#include "file_scanner.h"

fsp::ss::FileScanner::FileScanner()
{
}

fsp::ss::FileScanner::~FileScanner() = default;

fsp::fs::LogEntry fsp::ss::FileScanner::search(const std::string &pattern, fsp::fs::FileReader &file_reader)
{
    return search_engine.process_search(pattern, file_reader);
}
