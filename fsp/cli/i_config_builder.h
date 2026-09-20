#ifndef I_CONFIG_BUILDER_H
#define I_CONFIG_BUILDER_H

namespace fsp::cli
{
    class IConfigBuilder
    {
    public:
        virtual ~IConfigBuilder() = default;

        virtual void build() = 0;
    };
}

#endif // I_CONFIG_BUILDER_H
