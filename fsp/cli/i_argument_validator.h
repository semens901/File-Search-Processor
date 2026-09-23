#ifndef I_ARGUMENT_VALIDATOR_H
#define I_ARGUMENT_VALIDATOR_H

namespace fsp::cli
{
    // IArgumentValidator: interface for performing validation of CLI
    // arguments after parsing. Implementations should throw or set error
    // state when validation fails.
    class IArgumentValidator
    {
    public:
        virtual ~IArgumentValidator() = default;

        virtual void validate() = 0;
    };
}

#endif // I_ARGUMENT_VALIDATOR_H
