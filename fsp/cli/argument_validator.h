#ifndef FSP_CLI_ARGUMENT_VALIDATOR_H
#define FSP_CLI_ARGUMENT_VALIDATOR_H

#include "i_argument_validator.h"

namespace fsp::cli
{
    class ArgumentValidator : public IArgumentValidator
    {
    public:
        void validate() override;
    };
}

#endif // FSP_CLI_ARGUMENT_VALIDATOR_H
