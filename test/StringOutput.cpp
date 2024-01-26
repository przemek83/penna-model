#include "StringOutput.h"

StringOutput::StringOutput() : Output(0, 0, 0) {}

std::string StringOutput::getContentForOutputType(OUTPUT_TYPE outputType)
{
    return stringStreams_[outputType]->str();
}

std::shared_ptr<std::ostream> StringOutput::getStream(
    OUTPUT_TYPE outputType) const
{
    return stringStreams_.at(outputType);
}
