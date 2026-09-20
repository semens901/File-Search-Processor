#ifndef FSP_CLI_CONFIG_BUILDER_H
#define FSP_CLI_CONFIG_BUILDER_H

#include "i_config_builder.h"

namespace fsp::cli
{
    class ConfigBuilder : public IConfigBuilder
    {
    public:
        void build() override;
    };
}

#endif // FSP_CLI_CONFIG_BUILDER_H
