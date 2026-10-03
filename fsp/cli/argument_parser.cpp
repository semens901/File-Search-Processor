#include "argument_parser.h"

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

namespace fsp::cli
{
    Config ArgumentParser::config_{};
    bool ArgumentParser::help_requested_{false};

    namespace
    {
        std::size_t parse_thread_count(const char* value)
        {
            if (value == nullptr || *value == '\0')
            {
                throw std::invalid_argument("missing value for -t/--threads");
            }

            char* end = nullptr;
            const unsigned long parsed = std::strtoul(value, &end, 10);
            if (end == value || *end != '\0' || parsed == 0u)
            {
                throw std::invalid_argument("thread count must be a positive integer");
            }

            return static_cast<std::size_t>(parsed);
        }

        std::vector<std::string> normalize_arguments(int argc, char* argv[])
        {
            std::vector<std::string> normalized;
            normalized.reserve(static_cast<std::size_t>(argc));
            normalized.emplace_back(argv[0]);

            for (int i = 1; i < argc; ++i)
            {
                const std::string current = argv[i];

                if (current == "--help" || current == "-h")
                {
                    normalized.push_back("-h");
                    continue;
                }

                if (current == "--pattern")
                {
                    if (i + 1 >= argc)
                    {
                        throw std::invalid_argument("missing value for --pattern");
                    }
                    normalized.push_back("-p");
                    normalized.push_back(argv[++i]);
                    continue;
                }

                if (current == "--root")
                {
                    if (i + 1 >= argc)
                    {
                        throw std::invalid_argument("missing value for --root");
                    }
                    normalized.push_back("-r");
                    normalized.push_back(argv[++i]);
                    continue;
                }

                if (current == "--threads")
                {
                    if (i + 1 >= argc)
                    {
                        throw std::invalid_argument("missing value for --threads");
                    }
                    normalized.push_back("-t");
                    normalized.push_back(argv[++i]);
                    continue;
                }

                if (current == "--statistics" || current == "--stats")
                {
                    normalized.push_back("-s");
                    continue;
                }

                if (current.rfind("--pattern=", 0) == 0)
                {
                    normalized.push_back("-p");
                    normalized.push_back(current.substr(std::string("--pattern=").size()));
                    continue;
                }

                if (current.rfind("--root=", 0) == 0)
                {
                    normalized.push_back("-r");
                    normalized.push_back(current.substr(std::string("--root=").size()));
                    continue;
                }

                if (current.rfind("--threads=", 0) == 0)
                {
                    normalized.push_back("-t");
                    normalized.push_back(current.substr(std::string("--threads=").size()));
                    continue;
                }

                normalized.push_back(current);
            }

            return normalized;
        }
    }

    const Config& ArgumentParser::config()
    {
        return config_;
    }

    bool ArgumentParser::help_requested()
    {
        return help_requested_;
    }

    void ArgumentParser::parse(int argc, char* argv[])
    {
        config_ = Config{};
        help_requested_ = false;

        if (argc <= 1)
        {
            return;
        }

        const std::vector<std::string> normalized = normalize_arguments(argc, argv);
        std::vector<std::string> storage;
        std::vector<char*> normalized_argv;
        normalized_argv.reserve(normalized.size());
        storage.reserve(normalized.size());

        for (const auto& item : normalized)
        {
            storage.push_back(item);
            normalized_argv.push_back(storage.back().data());
        }

        opterr = 0;
        optind = 1;

        int option = 0;
        while ((option = getopt(static_cast<int>(normalized_argv.size()),
                                normalized_argv.data(),
                                ":hp:r:t:s")) != -1)
        {
            switch (option)
            {
                case 'h':
                    help_requested_ = true;
                    break;
                case 'p':
                    config_.pattern = optarg;
                    break;
                case 'r':
                    config_.root_path = optarg;
                    break;
                case 't':
                    config_.thread_count = parse_thread_count(optarg);
                    break;
                case 's':
                    config_.statistics = true;
                    break;
                case ':':
                    throw std::invalid_argument("missing value for an option");
                case '?':
                    throw std::invalid_argument(std::string("unknown option: -") + static_cast<char>(optopt));
                default:
                    break;
            }
        }

        for (int index = optind; index < static_cast<int>(normalized_argv.size()); ++index)
        {
            if (config_.pattern.empty())
            {
                config_.pattern = normalized_argv[index];
            }
            else if (config_.root_path.empty())
            {
                config_.root_path = normalized_argv[index];
            }
            else
            {
                throw std::invalid_argument("too many positional arguments");
            }
        }

        if (config_.thread_count == 0u)
        {
            config_.thread_count = std::thread::hardware_concurrency() > 0
                                      ? std::thread::hardware_concurrency()
                                      : 1u;
        }
    }
}
