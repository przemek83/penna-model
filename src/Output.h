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

    void saveAverages(const SimulationAverages& data);

    void saveBasicSimulationMetrics(const SingleSimulationData& data);

    void saveInitialPopulation(const std::list<Individual>& individuals);

    void saveFinalPopulation(const std::list<Individual>& individuals);

    void saveBitsDistribution(const std::vector<float>& bitsDistribution);

    void saveAgeDistribution(const std::vector<int>& ageDistribution);

    void saveDeathsDistribution(const std::vector<float>& deaths);

protected:
    virtual std::shared_ptr<std::ostream> getStream(
        OUTPUT_TYPE outputType) const = 0;

    float simulationStep_;
    int maxPopulationAge_;
    int run_;
};