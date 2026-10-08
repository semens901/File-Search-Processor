#include <catch2/catch_test_macros.hpp>

#include "fsp/cli/argument_parser.h"

TEST_CASE("ArgumentParser recognizes recursive flag -R and --recursive", "[cli][unit]")
{
    {
        const char* program = "fsp";
        const char* argv[] = {program, "-R", "needle", "demo_root"};
        fsp::cli::ArgumentParser parser;
        parser.parse(4, argv);

        const auto& config = fsp::cli::ArgumentParser::config();
        REQUIRE(config.recursive == true);
    }

    {
        const char* program = "fsp";
        const char* argv[] = {program, "--recursive", "needle", "demo_root"};
        fsp::cli::ArgumentParser parser;
        parser.parse(4, argv);

        const auto& config = fsp::cli::ArgumentParser::config();
        REQUIRE(config.recursive == true);
    }
}
