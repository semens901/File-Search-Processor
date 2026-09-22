#include <string>
#include <thread>

#include <spdlog/spdlog.h>

#include "fsp/services/file_processing_manager.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S %z] [%^%l%$] %v");

    spdlog::info("FSP demo: FileProcessingManager stub");

    const std::string pattern = "other text";
    const std::string root_path = "demo_root";
    const std::size_t thread_count = std::thread::hardware_concurrency();

    fsp::sv::FileProcessingManager manager(pattern, root_path, thread_count);

    const auto result = manager.run();

    spdlog::info("Search result:");
    spdlog::info("line_number = {}", result.line_number);
    spdlog::info("text = {}", result.text);
    spdlog::info("file_name = {}", result.file_name);

    spdlog::info("FileProcessingManager mock execution completed.");
    return 0;
}