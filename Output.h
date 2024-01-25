#pragma once

#include <list>
#include <string>

#include "Individual.h"
#include "SimulationData.h"

class Output
{
public:
    Output(float simulationStep_, int maxPopulationAge, int run);

    virtual ~Output() = default;

    virtual void saveAverages(const SimulationAverages& simulationData) = 0;

    virtual void saveBasicSimulationMetrics(
        const SingleSimulationData& data) = 0;

    virtual void saveInitialPopulation(
        const std::list<Individual>& individuals) = 0;

    virtual void saveFinalPopulation(
        const std::list<Individual>& populacja) = 0;

    virtual void saveBitsDistribution(
        const std::array<float, Config::bits_>& bitsDistribution) = 0;

    virtual void saveAgeDistribution(
        const std::array<int, Config::bits_>& ageDistribution) = 0;

    virtual void saveDeathsDistribution(
        const std::array<float, Config::bits_>& deaths) = 0;

protected:
    const float simulationStep_;
    const int maxPopulationAge_;
    const int run_;
};
