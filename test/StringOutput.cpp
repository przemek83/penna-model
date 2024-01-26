#include "StringOutput.h"

#include <sstream>

StringOutput::StringOutput() : Output(0, 0, 0) {}

std::shared_ptr<std::ostream> StringOutput::openFile(OUTPUT_FILE file) const
{
    return stringStreams_.at(file);
}
