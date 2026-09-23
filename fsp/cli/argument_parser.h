#ifndef FSP_CLI_ARGUMENT_PARSER_H
#define FSP_CLI_ARGUMENT_PARSER_H

#include "i_argument_parser.h"

namespace fsp::cli
{
    // ArgumentParser: concrete parser for CLI arguments. The implementation
    // is currently a stub; this type exists to satisfy the public API used
    // by the rest of the application and tests.
    class ArgumentParser : public IArgumentParser
    {
    public:
        void parse(int argc, char* argv[]) override;
    };
}

#endif // FSP_CLI_ARGUMENT_PARSER_H
