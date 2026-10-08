#ifndef FSP_CLI_ARGUMENT_PARSER_H
#define FSP_CLI_ARGUMENT_PARSER_H

#include "config.h"
#include "i_argument_parser.h"

namespace fsp::cli
{
    class ArgumentParser : public IArgumentParser
    {
    public:
        void parse(int argc, const char* argv[]) override;

        static const Config& config();
        static bool help_requested();

    private:
        static Config config_;
        static bool help_requested_;
    };
}

#endif // FSP_CLI_ARGUMENT_PARSER_H
