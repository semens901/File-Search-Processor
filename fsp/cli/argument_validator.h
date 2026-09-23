#ifndef FSP_CLI_ARGUMENT_VALIDATOR_H
#define FSP_CLI_ARGUMENT_VALIDATOR_H

#include "i_argument_validator.h"

namespace fsp::cli
{
    // ArgumentValidator: validates previously parsed CLI options. This
    // class is intentionally minimal for tests and can be extended with
    // concrete rules if needed.
    class ArgumentValidator : public IArgumentValidator
    {
    public:
        void validate() override;
    };
}

#endif // FSP_CLI_ARGUMENT_VALIDATOR_H
