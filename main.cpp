#include <iostream>
#include <stdexcept>
#include <string>

#include <spdlog/spdlog.h>

#include "fsp/cli/argument_parser.h"
#include "fsp/services/file_processing_manager.h"
#include "fsp/statistics/statistics.h"

namespace
{
    void print_usage(const char* program_name)
    {
        std::cout << "Usage:\n"
                  << "  " << program_name << " <pattern> <path>\n"
                  << "  " << program_name << " -p <pattern> -r <path> [-t <threads>]\n"
                  << "  " << program_name << " --help\n";
    }
}

int main(int argc, char* argv[])
{
    try
    {
        spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%H:%M:%S %z] [%^%l%$] %v");
        fsp::cli::ArgumentParser parser;
        parser.parse(argc, argv);

        if (fsp::cli::ArgumentParser::help_requested())
        {
            print_usage(argv[0]);
            return 0;
        }

        const auto& config = fsp::cli::ArgumentParser::config();
        if (config.pattern.empty() || config.root_path.empty())
        {
            std::cerr << "error: pattern and root path are required\n";
            print_usage(argv[0]);
            return 1;
        }

        spdlog::info("FSP demo: FileProcessingManager stub");

        fsp::sc::Statistics::enable();
        fsp::sc::Statistics::start();

        const std::size_t thread_count = config.thread_count > 0 ? config.thread_count : 1u;
        fsp::sv::FileProcessingManager manager(config.pattern, config.root_path, thread_count);

        const auto result = manager.run();

        fsp::sc::Statistics::stop();

        spdlog::info("Search result:");
        spdlog::info("line_number = {}", result.line_number);
        spdlog::info("text = {}", result.text);
        spdlog::info("file_name = {}", result.file_name);

        if (fsp::sc::Statistics::enabled())
        {
            spdlog::info("Statistics:");
            spdlog::info("files_scanned = {}", fsp::sc::Statistics::files_scanned());
            spdlog::info("files_matched = {}", fsp::sc::Statistics::files_matched());
            spdlog::info("files_with_errors = {}", fsp::sc::Statistics::files_with_errors());
            spdlog::info("directories_scanned = {}", fsp::sc::Statistics::directories_scanned());
            spdlog::info("matched_lines = {}", fsp::sc::Statistics::matched_lines());
            spdlog::info("elapsed_ms = {}", fsp::sc::Statistics::elapsed().count());
        }

        spdlog::info("FileProcessingManager mock execution completed.");
        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error: " << ex.what() << '\n';
        print_usage(argv[0]);
        return 1;
    }
}