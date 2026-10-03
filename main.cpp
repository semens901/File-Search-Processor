#include <iostream>
#include <stdexcept>
#include <string>

#include <spdlog/spdlog.h>

#include "fsp/cli/argument_parser.h"
#include "fsp/services/file_processing_manager.h"
#include "fsp/statistics/statistics.h"

namespace
{
    // Prints a short usage summary to help the user understand the accepted CLI forms.
    void print_usage(const char* program_name)
    {
        std::cout << "Usage:\n"
                  << "  " << program_name << " <pattern> <path>\n"
                  << "  " << program_name << " -p <pattern> -r <path> [-t <threads>] [-s]\n"
                  << "  " << program_name << " --help\n";
    }
}

// Program entry point. It validates command-line input, runs the search workflow,
// and prints a compact result summary together with collected runtime statistics.
int main(int argc, char* argv[])
{
    try
    {
        // Configure application-level logging so runtime diagnostics are visible when debugging.
        spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%H:%M:%S %z] [%^%l%$] %v");

        // Parse CLI arguments and normalize supported short/long forms.
        fsp::cli::ArgumentParser parser;
        parser.parse(argc, argv);

        // If the user requested help, print usage and exit immediately.
        if (fsp::cli::ArgumentParser::help_requested())
        {
            print_usage(argv[0]);
            return 0;
        }

        // Validate the mandatory search configuration.
        const auto& config = fsp::cli::ArgumentParser::config();
        if (config.pattern.empty() || config.root_path.empty())
        {
            std::cerr << "error: pattern and root path are required\n";
            print_usage(argv[0]);
            return 1;
        }

        spdlog::info("FSP demo: FileProcessingManager stub");

        // Enable the runtime statistics tracker only when the user explicitly asks for it.
        if (config.statistics)
        {
            fsp::sc::Statistics::enable();
        }
        fsp::sc::Statistics::start();

        // Create the manager with a safe thread count fallback for the current machine.
        const std::size_t thread_count = config.thread_count > 0 ? config.thread_count : 1u;
        fsp::sv::FileProcessingManager manager(config.pattern, config.root_path, thread_count);

        // Run the actual file-processing flow and collect the first match, if any.
        const auto result = manager.run();

        // Stop the timer and report the final results to the user.
        fsp::sc::Statistics::stop();

        spdlog::info("Search result:");
        spdlog::info("line_number = {}", result.line_number);
        spdlog::info("text = {}", result.text);
        spdlog::info("file_name = {}\n", result.file_name);

        if (fsp::sc::Statistics::enabled())
        {
            spdlog::info("==================================================");
            spdlog::info("Statistics summary");
            spdlog::info("--------------------------------------------------");
            spdlog::info("files_scanned = {}", fsp::sc::Statistics::files_scanned());
            spdlog::info("files_matched = {}", fsp::sc::Statistics::files_matched());
            spdlog::info("files_with_errors = {}", fsp::sc::Statistics::files_with_errors());
            spdlog::info("directories_scanned = {}", fsp::sc::Statistics::directories_scanned());
            spdlog::info("matched_lines = {}", fsp::sc::Statistics::matched_lines());
            spdlog::info("elapsed_ms = {}", fsp::sc::Statistics::elapsed().count());
            spdlog::info("==================================================");
        }

        spdlog::info("FileProcessingManager mock execution completed.");
        return 0;
    }
    catch (const std::exception& ex)
    {
        // Report user-facing failures and show usage guidance for the next run.
        std::cerr << "error: " << ex.what() << '\n';
        print_usage(argv[0]);
        return 1;
    }
}