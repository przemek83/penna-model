#pragma once

#include "../Output.h"

class NullOutput : public Output
{
public:
    NullOutput();

    void saveAverages(
        [[maybe_unused]] const SimulationAverages& simulationData) override
    {
    }

    void saveBasicSimulationMetrics(
        [[maybe_unused]] const SingleSimulationData& data) override
    {
    }

    void saveInitialPopulation(
        [[maybe_unused]] const std::list<Individual>& individuals) override
    {
    }

    void saveFinalPopulation(
        [[maybe_unused]] const std::list<Individual>& populacja) override
    {
    }

    void saveBitsDistribution(
        [[maybe_unused]] const std::array<float, Config::bits_>&
            bitsDistribution) override
    {
    }

    void saveAgeDistribution(
        [[maybe_unused]] const std::array<int, Config::bits_>& ageDistribution)
        override
    {
    }

    void saveDeathsDistribution(
        [[maybe_unused]] const std::array<float, Config::bits_>& deaths)
        override
    {
    }
};
