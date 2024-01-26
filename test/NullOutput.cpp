#include "NullOutput.h"

#include <fstream>

NullOutput::NullOutput() : Output(0, 0, 0) {}

std::unique_ptr<std::ostream> NullOutput::openFile(
    [[maybe_unused]] OUTPUT_FILE file) const
{
    auto stream{std::make_unique<std::ofstream>()};
    return stream;
}
