#include "search_line_engine.h"

std::unique_ptr<fsp::sh::ISearch> fsp::sh::SearchLineEngine::create_search()
{
    return std::make_unique<fsp::sh::SearchLine>();
}
