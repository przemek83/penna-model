#pragma once

#include "../Output.h"

class NullOutput : public Output
{
public:
    NullOutput();

protected:
    std::shared_ptr<std::ostream> openFile(OUTPUT_FILE file) const override;
};
