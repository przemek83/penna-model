#pragma once

#include <list>
#include <string>

#include "Output.h"

class FileOutput : public Output
{
public:
    explicit FileOutput(std::string prefix);

    std::string getName(OUTPUT_TYPE outputType) const;

protected:
    std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const override;

private:
    const std::map<OUTPUT_TYPE, std::string> fileNames_{
        {OUTPUT_TYPE::STATISTICS, "statistics.txt"},
        {OUTPUT_TYPE::INITIAL_POPULATION, "initialPopulation.txt"},
        {OUTPUT_TYPE::FINAL_POPULATION, "finalPopulation.txt"},
        {OUTPUT_TYPE::AGE_DISTRIBUTION, "age_distribution.txt"},
        {OUTPUT_TYPE::BITS_DISTRIBUTION, "bits_distribution.txt"},
        {OUTPUT_TYPE::DEATHS_DISTRIBUTION, "deaths_distribution.txt"},
        {OUTPUT_TYPE::FAMILIES, "families.txt"}};

    std::string prefix_;
};
