#pragma once

#include <functional>
#include <list>

#include "Config.h"
#include "Individual.h"
#include "SimulationData.h"

class Output;
class Generator;

class Simulation
{
public:
    Simulation(const Config& config, float step);

    void createInitialPopulation(Generator& generator);

    void saveInitialPopulation(Output& output);

    void saveFinalPopulation(Output& output);

    SingleSimulationData run(
        Generator& generator,
        std::function<void(std::size_t)> progressCallback = nullptr);

private:
    using BasicMetrics = SingleSimulationData::BasicMetrics;
    BasicMetrics progressByOneYear(Generator& generator, bool singleFamily,
                                   int livesAtStart);

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

    std::pair<std::vector<int>, std::vector<int>> getDeathsDistributionData(
        Generator& generator) const;

    static bool isSingleFamily(std::size_t year,
                               const std::vector<BasicMetrics>& basicMetrics);

    int getLivesOnYearStart(
        std::size_t year, const std::vector<BasicMetrics>& basicMetrics) const;

    std::list<Individual> individuals_;

    Config config_;
    float step_;
};