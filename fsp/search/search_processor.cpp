#include "search_processor.h"

fsp::fs::LogEntry fsp::sh::SearchProcessor::process_search(const std::string &text, fsp::fs::FileReader& file_reader)
{
    auto search = create_search();
    return search->search_text(text, file_reader);
}
