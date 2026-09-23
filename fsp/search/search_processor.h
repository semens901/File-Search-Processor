#ifndef SEARCH_SEARCH_PROCESSOR_H
#define SEARCH_SEARCH_PROCESSOR_H

#include <string>
#include <memory>

#include "fsp/filesystem/log_entry.h"
#include "fsp/filesystem/file_reader.h"
#include "i_search.h"

namespace fsp::sh
{
    // SearchProcessor: base class providing a template method `process_search`
    // which constructs a concrete ISearch via `create_search()` and delegates
    // the work. Subclasses implement the factory method to supply a concrete
    // search implementation.
    class SearchProcessor
    {
    public:
        virtual ~SearchProcessor() = default;

        fsp::fs::LogEntry process_search(const std::string& text, fsp::fs::FileReader& file_reader);
    protected:
        virtual std::unique_ptr<fsp::sh::ISearch> create_search() = 0;
    };
}

#endif // SEARCH_SEARCH_PROCESSOR_H