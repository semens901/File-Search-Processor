#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <string>

#include "fsp/services/file_processing_manager.h"
#include "../utils/fake_directory_creator.h"

namespace fs = std::filesystem;

static std::string make_file_content(std::size_t index, std::size_t match_index)
{
    std::string content;
    for (std::size_t line = 0; line < 120; ++line)
    {
        content += "alpha beta gamma delta\n";
    }

    if (index == match_index)
    {
        content += "needle_here_in_central_file\n";
    }

    return content;
}

static void populate_directory(FakeDirectoryCreator& dir, std::size_t file_count)
{
    const auto match_index = file_count / 2;
    for (std::size_t i = 0; i < file_count; ++i)
    {
        dir.create_file("file_" + std::to_string(i) + ".txt",
                        make_file_content(i, match_index));
    }
}

static std::pair<fsp::fs::LogEntry, std::chrono::milliseconds> run_manager(const fs::path& root,
                                                                        std::size_t thread_count)
{
    const auto start = std::chrono::steady_clock::now();
    fsp::sv::FileProcessingManager manager("needle_here_in_central_file", root, thread_count);
    const auto result = manager.run();
    const auto end = std::chrono::steady_clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    INFO("threads=" << thread_count << ", file_name=" << result.file_name
         << ", line_number=" << result.line_number
         << ", duration_ms=" << duration.count());

    return {result, duration};
}

TEST_CASE("FileProcessingManager finds the match with 1 thread", "[service][unit]")
{
    FakeDirectoryCreator dir("fpm_single");
    populate_directory(dir, 40);

    const auto [result, duration] = run_manager(dir.root_path(), 1);

    REQUIRE(result.file_name.find("file_") != std::string::npos);
    REQUIRE(result.line_number == 121);
    REQUIRE(result.text.find("needle_here_in_central_file") != std::string::npos);
}

TEST_CASE("FileProcessingManager finds the match with 4 threads", "[service][unit]")
{
    FakeDirectoryCreator dir("fpm_multi");
    populate_directory(dir, 40);

    const auto [result, duration] = run_manager(dir.root_path(), 4);

    REQUIRE(result.file_name.find("file_") != std::string::npos);
    REQUIRE(result.line_number == 121);
    REQUIRE(result.text.find("needle_here_in_central_file") != std::string::npos);
}

TEST_CASE("FileProcessingManager benchmark: 1 thread vs 4 threads", "[service][performance]")
{
    FakeDirectoryCreator dir("fpm_perf");

    // increase work so parallelism advantage is measurable
    populate_directory(dir, 10000);

    const auto [result_1, duration_1] = run_manager(dir.root_path(), 1);
    const auto [result_4, duration_4] = run_manager(dir.root_path(), 4);


    REQUIRE(result_1.file_name.find("file_") != std::string::npos);
    REQUIRE(result_4.file_name.find("file_") != std::string::npos);
    REQUIRE(result_1.line_number == 121);
    REQUIRE(result_4.line_number == 121);
    REQUIRE(result_1.text.find("needle_here_in_central_file") != std::string::npos);
    REQUIRE(result_4.text.find("needle_here_in_central_file") != std::string::npos);

    INFO("1-thread duration: " << duration_1.count() << " ms");
    INFO("4-thread duration: " << duration_4.count() << " ms");

    // Require that 4 threads complete faster than 1 thread
    REQUIRE(duration_4 < duration_1);
}
