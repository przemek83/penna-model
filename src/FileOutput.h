#pragma once

#include <list>
#include <string>

#include "Output.h"

class FileOutput : public Output
{
public:
    FileOutput(int maxPopulationAge, int run);

    std::string getName(OUTPUT_TYPE outputType) const;

protected:
    std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const override;

private:
    std::vector<std::string> fileNames_{
        "statistics.txt",        "initialPopulation.txt",
        "finalPopulation.txt",   "age_distribution.txt",
        "bits_distribution.txt", "deaths_distribution.txt",
        "families.txt"};
};
