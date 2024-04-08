#pragma once

#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "Individual.h"
#include "SimulationData.h"

class Output
{
public:
    Output(int years, int runNumber);

    virtual ~Output() = default;

    enum OUTPUT_TYPE
    {
        STATISTICS = 0,
        INITIAL_POPULATION,
        FINAL_POPULATION,
        AGE_DISTRIBUTION,
        BITS_DISTRIBUTION,
        DEATHS_DISTRIBUTION,
        FAMILIES
    };

    void saveSimulationData(const SingleSimulationData& data);

    void saveAverages(const SimulationAverages& data);

    void saveInitialPopulation(const std::list<Individual>& individuals);

    void saveFinalPopulation(const std::list<Individual>& individuals);

protected:
    virtual std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const = 0;

    int getRunNumber() const;

private:
    template <typename T>
    void saveData(T data, std::map<OUTPUT_TYPE, int> precisions)
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

    void saveBitsDistribution(const std::vector<float>& bitsDistribution);

    void saveAgeDistribution(const std::vector<int>& ageDistribution);

    void saveDeathsDistribution(const std::vector<float>& deaths);

    int years_;
    int runNumber_;

    char separator_{'\t'};
};
