#include "file_scanner.h"

// FileScanner is a thin facade that delegates the actual pattern matching to the
// configured search engine while keeping the public API stable for the rest of the app.
fsp::ss::FileScanner::FileScanner()
{
}

fsp::ss::FileScanner::~FileScanner() = default;

// Searches a single file for the requested pattern and updates the runtime stats.
// A successful match increments both the file and line counters.
fsp::fs::LogEntry fsp::ss::FileScanner::search(const std::string &pattern, fsp::fs::FileReader &file_reader)
{
    // Count every file examined by the search engine.
    fsp::sc::Statistics::file_scanned();

    auto res = search_engine.process_search(pattern, file_reader);
    if(res.line_number >= 0)
    {
        // A positive line number means the file contains a match.
        fsp::sc::Statistics::file_matched();
        fsp::sc::Statistics::line_matched();
    }
    return res;
}
