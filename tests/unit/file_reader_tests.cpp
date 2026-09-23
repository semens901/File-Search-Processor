#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "../utils/fake_file_creator.h"
#include "fsp/filesystem/file_reader.h"

TEST_CASE("FileReader opens an existing file and exposes its content", "[filesystem][unit]")
{
    FakeFileCreator fake_creator("first line\nsecond line\n");

    fsp::fs::FileReader file_reader(fake_creator.get_path());

    REQUIRE(file_reader.is_valid());
    REQUIRE(file_reader.get_file_name() == fake_creator.get_path());

    std::string first_line;
    std::getline(file_reader.get_stream(), first_line);
    REQUIRE(first_line == "first line");
}

TEST_CASE("FileReader can reopen a different file", "[filesystem][unit]")
{
    FakeFileCreator first_file("alpha\n");
    FakeFileCreator second_file("beta\n");

    fsp::fs::FileReader file_reader;
    REQUIRE_FALSE(file_reader.is_valid());

    REQUIRE(file_reader.open(first_file.get_path()));
    REQUIRE(file_reader.get_file_name() == first_file.get_path());

    std::string line;
    std::getline(file_reader.get_stream(), line);
    REQUIRE(line == "alpha");

    REQUIRE(file_reader.open(second_file.get_path()));
    REQUIRE(file_reader.get_file_name() == second_file.get_path());

    std::getline(file_reader.get_stream(), line);
    REQUIRE(line == "beta");
}

TEST_CASE("FileReader marks itself invalid when the file cannot be opened", "[filesystem][unit]")
{
    const auto missing_path = std::filesystem::temp_directory_path() /
        ("missing_file_reader_" + std::to_string(std::rand()) + ".txt");

    fsp::fs::FileReader file_reader("/tmp/this_file_should_not_exist_for_test.txt");
    REQUIRE_FALSE(file_reader.is_valid());

    file_reader.open(missing_path.string());
    REQUIRE_FALSE(file_reader.is_valid());

    fsp::fs::FileReader valid_reader("/tmp/this_file_should_not_exist_for_test.txt");
    REQUIRE_FALSE(valid_reader.is_valid());

    const auto valid_file = FakeFileCreator("valid\n");
    REQUIRE(valid_reader.open(valid_file.get_path()));
    REQUIRE(valid_reader.is_valid());

    REQUIRE_FALSE(valid_reader.open(missing_path.string()));
    REQUIRE_FALSE(valid_reader.is_valid());
}
