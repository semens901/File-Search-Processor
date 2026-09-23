#ifndef FSP_CLI_CONFIG_BUILDER_H
#define FSP_CLI_CONFIG_BUILDER_H

#include "i_config_builder.h"

namespace fsp::cli
{
    // ConfigBuilder: assemble runtime `Config` from parsed arguments and
    // environment. Left as a small extension point for future CLI options.
    class ConfigBuilder : public IConfigBuilder
    {
    public:
        void build() override;
    };
}

#endif // FSP_CLI_CONFIG_BUILDER_H
