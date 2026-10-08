#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

#include "fsp/cli/argument_parser.h"

TEST_CASE("ArgumentParser parses short POSIX flags", "[cli][unit]")
{
    char program[] = "fsp";
    char pattern[] = "needle";
    char root[] = "demo_root";
    char threads[] = "4";
    const char* argv[] = {program, "-p", pattern, "-r", root, "-t", threads};

    fsp::cli::ArgumentParser parser;
    parser.parse(7, argv);

    const auto& config = fsp::cli::ArgumentParser::config();
    REQUIRE(config.pattern == "needle");
    REQUIRE(config.root_path == "demo_root");
    REQUIRE(config.thread_count == 4u);
    REQUIRE_FALSE(fsp::cli::ArgumentParser::help_requested());
}

TEST_CASE("ArgumentParser parses long POSIX flags", "[cli][unit]")
{
    char program[] = "fsp";
    const char* argv[] = {program, "--pattern", "needle", "--root", "demo_root", "--threads", "8"};

    fsp::cli::ArgumentParser parser;
    parser.parse(7, argv);

    const auto& config = fsp::cli::ArgumentParser::config();
    REQUIRE(config.pattern == "needle");
    REQUIRE(config.root_path == "demo_root");
    REQUIRE(config.thread_count == 8u);
}

TEST_CASE("ArgumentParser parses equals-style long flags", "[cli][unit]")
{
    char program[] = "fsp";
    const char* argv[] = {program, "--pattern=needle", "--root=demo_root", "--threads=12"};

    fsp::cli::ArgumentParser parser;
    parser.parse(4, argv);

    const auto& config = fsp::cli::ArgumentParser::config();
    REQUIRE(config.pattern == "needle");
    REQUIRE(config.root_path == "demo_root");
    REQUIRE(config.thread_count == 12u);
}

TEST_CASE("ArgumentParser accepts positional arguments", "[cli][unit]")
{
    char program[] = "fsp";
    char pattern[] = "needle";
    char root[] = "demo_root";
    const char* argv[] = {program, pattern, root};

    fsp::cli::ArgumentParser parser;
    parser.parse(3, argv);

    const auto& config = fsp::cli::ArgumentParser::config();
    REQUIRE(config.pattern == "needle");
    REQUIRE(config.root_path == "demo_root");
}

TEST_CASE("ArgumentParser accepts mixed short flags and positional values", "[cli][unit]")
{
    char program[] = "fsp";
    char pattern[] = "needle";
    char root[] = "demo_root";
    const char* argv[] = {program, "-p", pattern, root};

    fsp::cli::ArgumentParser parser;
    parser.parse(4, argv);

    const auto& config = fsp::cli::ArgumentParser::config();
    REQUIRE(config.pattern == "needle");
    REQUIRE(config.root_path == "demo_root");
}

TEST_CASE("ArgumentParser handles help flags", "[cli][unit]")
{
    char program[] = "fsp";
    const char* argv[] = {program, "-h"};

    fsp::cli::ArgumentParser parser;
    parser.parse(2, argv);

    REQUIRE(fsp::cli::ArgumentParser::help_requested());
}

TEST_CASE("ArgumentParser handles stats display flag", "[cli][unit]")
{
    char program[] = "fsp";
    char pattern[] = "needle";
    char root[] = "demo_root";
    const char* argv[] = {program, "-p", pattern, "-r", root, "-s", "-R"};

    fsp::cli::ArgumentParser parser;
    parser.parse(7, argv);

    const auto& config = fsp::cli::ArgumentParser::config();
    REQUIRE(config.pattern == "needle");
    REQUIRE(config.root_path == "demo_root");
    REQUIRE(config.statistics);
}

TEST_CASE("ArgumentParser handles long help flag", "[cli][unit]")
{
    char program[] = "fsp";
    const char* argv[] = {program, "--help"};

    fsp::cli::ArgumentParser parser;
    parser.parse(2, argv);

    REQUIRE(fsp::cli::ArgumentParser::help_requested());
}

TEST_CASE("ArgumentParser rejects missing option values", "[cli][unit]")
{
    char program[] = "fsp";
    const char* argv[] = {program, "-p", "needle", "-r"};

    fsp::cli::ArgumentParser parser;
    REQUIRE_THROWS_AS(parser.parse(4, argv), std::invalid_argument);
}

TEST_CASE("ArgumentParser rejects invalid thread count", "[cli][unit]")
{
    char program[] = "fsp";
    char invalid_threads[] = "0";
    const char* argv[] = {program, "-p", "needle", "-r", "demo_root", "-t", invalid_threads};

    fsp::cli::ArgumentParser parser;
    REQUIRE_THROWS_AS(parser.parse(7, argv), std::invalid_argument);
}
