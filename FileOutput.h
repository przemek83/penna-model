#pragma once

#include <list>
#include <string>

#include "Output.h"

class FileOutput : public Output
{
public:
    FileOutput(float simulationStep_, int maxPopulationAge, int run);

protected:
    std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const override;

private:
    std::string getName(OUTPUT_TYPE outputType) const;

    const std::vector<std::string> fileNames_{
        "statystyki.txt",    "initialPopulation.txt", "finalPopulation.txt",
        "rozklad_wieku.txt", "rozklad_bitow.txt",     "gompertz.txt",
        "rodziny.txt"};
};
