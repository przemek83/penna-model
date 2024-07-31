#pragma once

#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "Individual.h"
#include "OutputType.h"
#include "SimulationData.h"

class AverageData;

class Output
{
public:
    Output() = default;
    virtual ~Output() = default;

    Output(const Output&) = delete;
    Output(Output&&) = delete;
    Output& operator=(const Output&) = delete;
    Output& operator=(Output&&) = delete;

    [[maybe_unused]] void saveSimulationData(const SimulationData& data);

    void saveAverages(const AverageData& data);

    void saveInitialPopulation(const std::list<Individual>& individuals) const;

    void saveFinalPopulation(const std::list<Individual>& individuals) const;

protected:
    virtual std::shared_ptr<std::ostream> getStream(
        OutputType outputType) const = 0;

private:
    template <typename T>
    void saveData(const T& data, std::map<OutputType, int> precisions)
    {
        const std::shared_ptr<std::ostream> families{
            getStream(OutputType::FAMILIES)};
        *families << std::setprecision(precisions[OutputType::FAMILIES])
                  << std::fixed;
        data.saveFamilies(*families, separator_);

        const std::shared_ptr<std::ostream> stats{
            getStream(OutputType::STATISTICS)};
        *stats << std::setprecision(precisions[OutputType::STATISTICS])
               << std::fixed;
        data.saveBasicMetrics(*stats, separator_);

        const std::shared_ptr<std::ostream> bits{
            getStream(OutputType::BITS_DISTRIBUTION)};
        *bits << std::setprecision(2) << std::fixed;
        data.saveBitsDistibution(*bits, separator_);

        const std::shared_ptr<std::ostream> age{
            getStream(OutputType::AGE_DISTRIBUTION)};
        *age << std::setprecision(precisions[OutputType::AGE_DISTRIBUTION])
             << std::fixed;
        data.saveAgeDistibution(*age, separator_);

        const std::shared_ptr<std::ostream> deaths{
            getStream(OutputType::DEATHS_DISTRIBUTION)};
        *deaths << std::setprecision(3) << std::fixed;
        data.saveDeathsDistibution(*deaths, separator_);
    }

    char separator_{'\t'};
};
