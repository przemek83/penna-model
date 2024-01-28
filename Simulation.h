#pragma once

#include <list>
#include <random>

#include "Config.h"
#include "Individual.h"
#include "SimulationData.h"

class Output;
class Generator;

class Simulation
{
public:
    Simulation(const Config& config, int number, float step);

    SingleSimulationData run(Generator& generator, Output& output);

private:
    void createInitialPopulation(Generator& generator);

    static std::array<int, Config::bits_> getAgeDistribution(
        const std::list<Individual>& individuals);

    static std::array<int, Config::bits_> getBitsDistribution(
        const std::list<Individual>& individuals);

    static void fillDistributions(
        SingleSimulationData& data,
        const std::array<int, Config::bits_>& ageDistribution,
        const std::array<int, Config::bits_>& bitsDistribution,
        const std::array<int, Config::bits_>& gompertzDeathsDistribution,
        const std::array<int, Config::bits_>& gompertzAgeDistribution,
        int populationCount);

    std::list<Individual> individuals_;

    Config config_;
    int number_;
    float step_;
};
