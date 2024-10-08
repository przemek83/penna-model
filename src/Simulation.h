#pragma once

#include <list>
#include <memory>

#include "Config.h"
#include "Generator.h"
#include "Individual.h"
#include "Metrics.h"
#include "SimResults.h"

class Output;
class ProgressBar;

class Simulation
{
public:
    Simulation(const config::Params& params, int simId);

    void createInitialPopulation();

    [[maybe_unused]] void saveInitialPopulation(const Output& output) const;

    [[maybe_unused]] void saveFinalPopulation(const Output& output) const;

    void setGenerator(std::unique_ptr<Generator> generator);

    SimResults run();

    void setProgressBar(std::shared_ptr<ProgressBar> progressBar);

private:
    using BasicMetrics = metrics::BasicMetrics<int>;
    BasicMetrics progressByOneYear(bool singleFamily, int livesAtStart);

    std::vector<int> getAgeDistribution(
        const std::list<Individual>& individuals) const;

    std::vector<int> getBitsDistribution(
        const std::list<Individual>& individuals) const;

    int getDeathChance(int populationCount) const;

    bool isCatched(int age) const;

    bool shouldDie(const Individual& individual, int deathChance) const;

    bool shouldHaveOffspring(const Individual& individual) const;

    SimResults prepareData(std::vector<BasicMetrics> basicMetrics) const;

    std::pair<std::vector<int>, std::vector<int>> getDeathsDistributionData()
        const;

    static bool isSingleFamily(int year,
                               const std::vector<BasicMetrics>& basicMetrics);

    int getLivesOnYearStart(
        int year, const std::vector<BasicMetrics>& basicMetrics) const;

    void handleOffspring(const Individual& individual,
                         BasicMetrics& yearMetrics);

    void handleFamilies(std::vector<int>& families,
                        const Individual& individual,
                        BasicMetrics& yearMetrics) const;

    std::list<Individual> individuals_;

    config::Params params_;
    std::unique_ptr<Generator> generator_{nullptr};

    std::shared_ptr<ProgressBar> progressBar_{nullptr};

    bool catchingActive_{false};

    const int simId_;

    const int familyCounted_{1};
    const int familyNotCounted_{0};
};
