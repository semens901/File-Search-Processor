#ifndef FSP_CLI_CONFIG_H
#define FSP_CLI_CONFIG_H

#include <cstddef>
#include <string>

namespace fsp::cli
{
    // Config: structure holding runtime options parsed from the command line.
    // Fields provide defaults so the rest of the program can rely on valid
    // values even when specific options are omitted.
    struct Config
    {
        std::string pattern;
        std::string root_path;
        std::size_t thread_count{1};
        bool recursive{true};
        bool case_sensitive{false};
    };
}

#endif // FSP_CLI_CONFIG_H