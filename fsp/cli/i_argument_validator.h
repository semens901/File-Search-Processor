#ifndef I_ARGUMENT_VALIDATOR_H
#define I_ARGUMENT_VALIDATOR_H

namespace fsp::cli
{
    class IArgumentValidator
    {
    public:
        virtual ~IArgumentValidator() = default;

        virtual void validate() = 0;
    };
}

#endif // I_ARGUMENT_VALIDATOR_H
