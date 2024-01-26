#include "NullOutput.h"

#include <fstream>

NullOutput::NullOutput() : Output(0, 0, 0) {}

std::shared_ptr<std::ostream> NullOutput::getStream(
    [[maybe_unused]] OUTPUT_TYPE outputType) const
{
    auto stream{std::make_shared<std::ofstream>()};
    return stream;
}
