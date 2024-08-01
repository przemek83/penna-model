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
    explicit Simulation(const config::Params& params);

    void createInitialPopulation();

    [[maybe_unused]] void saveInitialPopulation(const Output& output) const;

    [[maybe_unused]] void saveFinalPopulation(const Output& output) const;

    void setGenerator(std::unique_ptr<Generator> generator);

    SimulationData run();

    void setProgressCallback(std::function<void(int)> callback);

private:
    using BasicMetrics = SimulationData::BasicMetrics<int>;
    BasicMetrics progressByOneYear(bool singleFamily, int livesAtStart);

    std::vector<int> getAgeDistribution(
        const std::list<Individual>& individuals) const;

    std::vector<int> getBitsDistribution(
        const std::list<Individual>& individuals) const;

    int getCurrentDeathChanceInPercent(int populationCount) const;

    bool isCatched(int age) const;

    bool shouldDie(const Individual& individual,
                   int chanceForDeathInPercent) const;

    bool shouldHaveOffspring(const Individual& individual) const;

    SimulationData prepareData(std::vector<BasicMetrics> basicMetrics) const;

    std::pair<std::vector<int>, std::vector<int>> getDeathsDistributionData()
        const;

    static bool isSingleFamily(int year,
                               const std::vector<BasicMetrics>& basicMetrics);

    int getLivesOnYearStart(
        int year, const std::vector<BasicMetrics>& basicMetrics) const;

    std::list<Individual> individuals_;

    config::Params params_;
    std::unique_ptr<Generator> generator_{nullptr};

    std::function<void(int)> progressCallback_{nullptr};

    bool catchingActive_{false};
};
