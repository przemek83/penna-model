#pragma once

#include <list>
#include <string>

#include "Output.h"

class FileOutput : public Output
{
public:
    explicit FileOutput(std::string prefix);

    std::string getName(OutputType outputType) const;

protected:
    std::shared_ptr<std::ostream> getStream(
        OutputType outputType) const override;

private:
    const std::map<OutputType, std::string> fileNames_{
        {OutputType::STATISTICS, "statistics.txt"},
        {OutputType::INITIAL_POPULATION, "initialPopulation.txt"},
        {OutputType::FINAL_POPULATION, "finalPopulation.txt"},
        {OutputType::AGE_DISTRIBUTION, "age_distribution.txt"},
        {OutputType::BITS_DISTRIBUTION, "bits_distribution.txt"},
        {OutputType::DEATHS_DISTRIBUTION, "deaths_distribution.txt"},
        {OutputType::FAMILIES, "families.txt"}};

    std::string prefix_;
};
