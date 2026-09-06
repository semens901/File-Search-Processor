#ifndef SEARCH_SEARCH_LINE_H
#define SEARCH_SEARCH_LINE_H

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

#include "fsp/filesystem/log_entry.h"
#include "fsp/filesystem/file_reader.h"
#include "i_search.h"

namespace fsp::sh
{
    class SearchLine : public ISearch
    {
    public:
        virtual ~SearchLine() = default;
        
        fsp::fs::LogEntry search_text(std::string text, fsp::fs::FileReader& file_reader) override;

        fsp::fs::LogEntry get_log_entry() const override;

    private:
        std::string file_name;
        fsp::fs::FileReader file_reader;
        fsp::fs::LogEntry log_entry;
    };
}

#endif // SEARCH_SEARCH_LINE_H