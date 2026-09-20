#ifndef FSP_CLI_ARGUMENT_PARSER_H
#define FSP_CLI_ARGUMENT_PARSER_H

#include "i_argument_parser.h"

namespace fsp::cli
{
    class ArgumentParser : public IArgumentParser
    {
    public:
        void parse(int argc, char* argv[]) override;
    };
}

#endif // FSP_CLI_ARGUMENT_PARSER_H
