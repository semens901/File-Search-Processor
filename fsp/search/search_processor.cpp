#include "search_processor.h"

// SearchProcessor: coordinates creating a concrete ISearch instance and
// delegates the actual search. Acts as a small indirection to decouple
// callers from specific search implementations.
fsp::fs::LogEntry fsp::sh::SearchProcessor::process_search(const std::string &text, fsp::fs::FileReader& file_reader)
{
    auto search = create_search();
    return search->search_text(text, file_reader);
}
