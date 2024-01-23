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

    void run(Output& output, Generator& generator,
             SimulationData<float>& simulationDataAvg);

private:
    void createInitialPopulation(Generator& generator);

    static std::array<int, Config::bits_> getAgeDistribution(
        const std::list<Individual>& individuals);

    static std::array<int, Config::bits_> getBitsDistribution(
        const std::list<Individual>& individuals);

    std::list<Individual> individuals_;

    const Config& config_;
    const int number_;
    const float step_;

    static void updateAvgDistributions(
        SimulationData<float>& simulationDataAvg,
        const std::array<int, Config::bits_>& ageDistribution,
        const std::array<int, Config::bits_>& bitsDistribution,
        const std::array<int, Config::bits_>& gompertzDeathsDistribution,
        const std::array<int, Config::bits_>& gompertzAgeDistribution,
        int populationCount);
};
