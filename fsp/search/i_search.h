#ifndef I_SEARCH_H
#define I_SEARCH_H

#include <string>

#include "fsp/filesystem/log_entry.h"
#include "fsp/filesystem/file_reader.h"

namespace fsp::sh
{
    class ISearch
    {
    public:
        virtual ~ISearch() = default;
        virtual fsp::fs::LogEntry search_text(std::string text, fsp::fs::FileReader& file_reader) = 0;
        virtual fsp::fs::LogEntry get_log_entry() const = 0;
    };
}

#endif // I_SEARCH_H