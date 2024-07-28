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

    void saveInitialPopulation(const std::list<Individual>& individuals);

    void saveFinalPopulation(const std::list<Individual>& individuals);

protected:
    virtual std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const = 0;

private:
    template <typename T>
    void saveData(const T& data, std::map<OUTPUT_TYPE, int> precisions)
    {
        const std::shared_ptr<std::ostream> families{getStream(FAMILIES)};
        *families << std::setprecision(precisions[FAMILIES]) << std::fixed;
        data.saveFamilies(*families, separator_);

        const std::shared_ptr<std::ostream> stats{getStream(STATISTICS)};
        *stats << std::setprecision(precisions[STATISTICS]) << std::fixed;
        data.saveBasicMetrics(*stats, separator_);

        const std::shared_ptr<std::ostream> bits{getStream(BITS_DISTRIBUTION)};
        *bits << std::setprecision(2) << std::fixed;
        data.saveBitsDistibution(*bits, separator_);

        const std::shared_ptr<std::ostream> age{getStream(AGE_DISTRIBUTION)};
        *age << std::setprecision(precisions[AGE_DISTRIBUTION]) << std::fixed;
        data.saveAgeDistibution(*age, separator_);

        const std::shared_ptr<std::ostream> deaths{
            getStream(DEATHS_DISTRIBUTION)};
        *deaths << std::setprecision(3) << std::fixed;
        data.saveDeathsDistibution(*deaths, separator_);
    }

    char separator_{'\t'};
};
