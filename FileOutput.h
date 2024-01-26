#pragma once

#include <list>
#include <string>

#include "Output.h"

class FileOutput : public Output
{
public:
    FileOutput(float simulationStep_, int maxPopulationAge, int run);

protected:
    std::unique_ptr<std::ostream> openFile(OUTPUT_FILE file) const override;

private:
    std::string getName(OUTPUT_FILE file) const;

    const std::vector<std::string> fileNames_{
        "statystyki.txt",    "initialPopulation.txt", "finalPopulation.txt",
        "rozklad_wieku.txt", "rozklad_bitow.txt",     "gompertz.txt",
        "rodziny.txt"};
};
