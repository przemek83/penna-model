#include "StringOutput.h"

#include <sstream>

StringOutput::StringOutput() : Output(0, 0, 0) {}

std::shared_ptr<std::ostream> StringOutput::getStream(
    OUTPUT_TYPE outputType) const
{
    return stringStreams_.at(outputType);
}
