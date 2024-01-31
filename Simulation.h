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

    static std::vector<int> getAgeDistribution(
        const std::list<Individual>& individuals);

    static std::vector<int> getBitsDistribution(
        const std::list<Individual>& individuals);

    int getCurrentDeathChanceInPercent(int populationCount) const;

    bool shouldDie(const Individual& individual, Generator& generator,
                   int chanceForDeathInPercent) const;

    SingleSimulationData prepareData(
        std::vector<SingleSimulationData::BasicData> basicData,
        const std::vector<int>& gompertzDeathsDistribution,
        const std::vector<int>& gompertzAgeDistribution) const;

    std::list<Individual> individuals_;

    Config config_;
    int number_;
    float step_;
};
