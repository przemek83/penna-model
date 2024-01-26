#pragma once

#include <list>
#include <memory>
#include <string>

#include "Individual.h"
#include "SimulationData.h"

class Output
{
public:
    Output(float simulationStep_, int maxPopulationAge, int run);

    virtual ~Output() = default;

    void saveAverages(const SimulationAverages& simulationData);

    void saveBasicSimulationMetrics(const SingleSimulationData& data);

    void saveInitialPopulation(const std::list<Individual>& individuals);

    void saveFinalPopulation(const std::list<Individual>& populacja);

    void saveBitsDistribution(
        const std::array<float, Config::bits_>& bitsDistribution);

    void saveAgeDistribution(
        const std::array<int, Config::bits_>& ageDistribution);

    void saveDeathsDistribution(const std::array<float, Config::bits_>& deaths);

protected:
    enum OUTPUT_FILE
    {
        STATISTICS = 0,
        INITIAL_POPULATION,
        FINAL_POPULATION,
        AGE_DISTRIBUTION,
        BITS_DISTRIBUTION,
        DEATHS_DISTRIBUTION,
        FAMILIES
    };

    virtual std::shared_ptr<std::ostream> openFile(OUTPUT_FILE file) const = 0;

    const float simulationStep_;
    const int maxPopulationAge_;
    const int run_;
};
