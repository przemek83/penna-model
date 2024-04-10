#include "FileOutput.h"

#include <fstream>

FileOutput::FileOutput(int maxPopulationAge) : Output(maxPopulationAge) {}

std::string FileOutput::getName(OUTPUT_TYPE outputType) const
{
    std::string fileName;
    fileName.append("output_");
    fileName.append(fileNames_[outputType]);

    return fileName;
}

std::shared_ptr<std::ostream> FileOutput::getStream(
    OUTPUT_TYPE outputType) const
{
    auto stream{std::make_shared<std::ofstream>(getName(outputType))};
    return stream;
}
