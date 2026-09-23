#include "file_scanner.h"

// FileScanner: thin facade that delegates searching logic to the
// configured search engine (e.g., SearchLineEngine). Keeps the public
// API stable while allowing engines to be swapped during testing.
fsp::ss::FileScanner::FileScanner()
{
}

fsp::ss::FileScanner::~FileScanner() = default;

fsp::fs::LogEntry fsp::ss::FileScanner::search(const std::string &pattern, fsp::fs::FileReader &file_reader)
{
    return search_engine.process_search(pattern, file_reader);
}
