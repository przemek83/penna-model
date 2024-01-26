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

    void saveAverages(const SimulationAverages& simulationData);

    void saveBasicSimulationMetrics(const SingleSimulationData& data);

    void saveInitialPopulation(const std::list<Individual>& individuals);

    void saveFinalPopulation(const std::list<Individual>& individuals);

    void saveBitsDistribution(
        const std::array<float, Config::bits_>& bitsDistribution);

    void saveAgeDistribution(
        const std::array<int, Config::bits_>& ageDistribution);

    void saveDeathsDistribution(const std::array<float, Config::bits_>& deaths);

protected:
    virtual std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const = 0;

    const float simulationStep_;
    const int maxPopulationAge_;
    const int run_;
};
