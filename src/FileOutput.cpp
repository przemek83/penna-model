#include "FileOutput.h"

#include <fstream>

FileOutput::FileOutput(std::string prefix) : prefix_(std::move(prefix)) {}

std::string FileOutput::getName(OUTPUT_TYPE outputType) const
{
    std::string fileName;
    fileName.append(prefix_);
    fileName.append("_");
    fileName.append(fileNames_[outputType]);

    return fileName;
}

std::shared_ptr<std::ostream> FileOutput::getStream(
    OUTPUT_TYPE outputType) const
{
    auto stream{std::make_shared<std::ofstream>(getName(outputType))};
    return stream;
}
