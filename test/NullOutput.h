#pragma once

#include "../Output.h"

class NullOutput : public Output
{
public:
    NullOutput();

protected:
    std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const override;
};
