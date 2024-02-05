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
    using BasicMetrics = SingleSimulationData::BasicMetrics;
    BasicMetrics progressByOneYear(Generator& generator, bool singleFamily,
                                   int livesAtStart);

    void createInitialPopulation(Generator& generator);

    static std::vector<int> getAgeDistribution(
        const std::list<Individual>& individuals);

    static std::vector<int> getBitsDistribution(
        const std::list<Individual>& individuals);

    int getCurrentDeathChanceInPercent(int populationCount) const;

    bool shouldDie(const Individual& individual, Generator& generator,
                   int chanceForDeathInPercent) const;

    bool shouldHaveOffspring(const Individual& individual,
                             Generator& generator) const;

    SingleSimulationData prepareData(
        std::vector<BasicMetrics> basicMetrics,
        const std::vector<int>& gompertzDeathsDistribution,
        const std::vector<int>& gompertzAgeDistribution) const;

    void saveSimulationData(const SingleSimulationData& data,
                            Output& output) const;

    std::pair<std::vector<int>, std::vector<int>> getDeathsDistributionData(
        Generator& generator) const;

    static bool isSingleFamily(std::size_t year,
                               const std::vector<BasicMetrics>& basicMetrics);

    int getLivesOnYearStart(
        std::size_t year, const std::vector<BasicMetrics>& basicMetrics) const;

    std::list<Individual> individuals_;

    Config config_;
    int number_;
    float step_;
};
