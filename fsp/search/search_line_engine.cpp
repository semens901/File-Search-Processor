#include "search_line_engine.h"

// SearchLineEngine: factory for creating SearchLine instances. The engine
// centralizes how search objects are constructed which simplifies testing
// and allows swapping implementations without changing call sites.
std::unique_ptr<fsp::sh::ISearch> fsp::sh::SearchLineEngine::create_search()
{
    return std::make_unique<fsp::sh::SearchLine>();
}
