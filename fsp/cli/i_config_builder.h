#ifndef I_CONFIG_BUILDER_H
#define I_CONFIG_BUILDER_H

namespace fsp::cli
{
    // IConfigBuilder: abstraction for assembling runtime configuration
    // from parsed arguments and environment. Implementations can populate
    // configuration objects used by the application.
    class IConfigBuilder
    {
    public:
        virtual ~IConfigBuilder() = default;

        virtual void build() = 0;
    };
}

#endif // I_CONFIG_BUILDER_H
