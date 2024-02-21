#pragma once

#include <functional>
#include <list>
#include <memory>

#include "Config.h"
#include "Individual.h"
#include "SimulationData.h"

class Output;
class Generator;

class Simulation
{
public:
    Simulation(const Config::Params& config, float step);

    void createInitialPopulation();

    void saveInitialPopulation(Output& output);

    void saveFinalPopulation(Output& output);

    void setGenerator(std::shared_ptr<Generator> generator);

    SingleSimulationData run(
        std::function<void(int)> progressCallback = nullptr);

private:
    using BasicMetrics = SingleSimulationData::BasicMetrics;
    BasicMetrics progressByOneYear(bool singleFamily, int livesAtStart);

    static std::vector<int> getAgeDistribution(
        const std::list<Individual>& individuals);

    static std::vector<int> getBitsDistribution(
        const std::list<Individual>& individuals);

    int getCurrentDeathChanceInPercent(int populationCount) const;

    bool shouldDie(const Individual& individual,
                   int chanceForDeathInPercent) const;

    bool shouldHaveOffspring(const Individual& individual) const;

    SingleSimulationData prepareData(
        std::vector<BasicMetrics> basicMetrics) const;

    std::pair<std::vector<int>, std::vector<int>> getDeathsDistributionData()
        const;

    static bool isSingleFamily(int year,
                               const std::vector<BasicMetrics>& basicMetrics);

    int getLivesOnYearStart(
        int year, const std::vector<BasicMetrics>& basicMetrics) const;

    std::list<Individual> individuals_;

    Config::Params config_;
    float step_;
    std::shared_ptr<Generator> generator_;
};
