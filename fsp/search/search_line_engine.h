#ifndef SEARCH_SEARCH_LINE_ENGINE_H
#define SEARCH_SEARCH_LINE_ENGINE_H

#include <memory>
#include <string>

#include "fsp/filesystem/log_entry.h"
#include "i_search.h"
#include "search_line.h"
#include "search_processor.h"

namespace fsp::sh
{

    class SearchLineEngine : public SearchProcessor
    {
    public:
        virtual ~SearchLineEngine() = default;
    protected:
        std::unique_ptr<fsp::sh::ISearch> create_search() override;
    };

}
#endif // SEARCH_SEARCH_LINE_ENGINE_H