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
        "statistics.txt",        "initialPopulation.txt",
        "finalPopulation.txt",   "age_distribution.txt",
        "bits_distribution.txt", "deaths_distribution.txt",
        "families.txt"};
};
