#include "StringOutput.h"

std::string StringOutput::getContentForOutputType(OUTPUT_TYPE outputType)
{
    return stringStreams_[outputType]->str();
}

void StringOutput::reset()
{
    for (auto& [_, stream] : stringStreams_)
        stream = std::make_shared<std::ostringstream>();
}

std::shared_ptr<std::ostream> StringOutput::getStream(
    OUTPUT_TYPE outputType) const
{
    return stringStreams_.at(outputType);
}
