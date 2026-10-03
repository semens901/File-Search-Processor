#include <catch2/catch_test_macros.hpp>

#include "fsp/statistics/statistics.h"

TEST_CASE("Statistics can be enabled and queried", "[statistics][unit]")
{
    fsp::sc::Statistics::enable();

    REQUIRE(fsp::sc::Statistics::enabled());
}
