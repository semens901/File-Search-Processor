#ifndef I_ARGUMENT_PARSER_H
#define I_ARGUMENT_PARSER_H

namespace fsp::cli
{
    class IArgumentParser
    {
    public:
        virtual ~IArgumentParser() = default;

        virtual void parse(int argc, char* argv[]) = 0;
    };
}

#endif // I_ARGUMENT_PARSER_H
