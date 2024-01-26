#include "FileOutput.h"

#include <fstream>

FileOutput::FileOutput(float simulationStep, int maxPopulationAge, int run)
    : Output(simulationStep, maxPopulationAge, run)
{
}

std::string FileOutput::getName(OUTPUT_FILE file) const
{
    std::string fileName;

#ifdef SYMULACJA_DORSZY
    sprintf(bufor, "%3.2f", START_ODLOWOW + przedrostek * simulationStep_);
#endif

#ifndef SYMULACJA_DORSZY
    fileName.append("proces");
    fileName.append(std::to_string(1));
    fileName.append("_symulacja");
#endif
    fileName.append(std::to_string(run_));
    fileName.append("_");
    fileName.append(fileNames_[file]);

    return fileName;
}

std::shared_ptr<std::ostream> FileOutput::openFile(OUTPUT_FILE file) const
{
    auto stream{std::make_shared<std::ofstream>(getName(file))};
    return stream;
}
