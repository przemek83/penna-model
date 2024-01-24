#pragma once

#include <list>
#include <string>

#include "Individual.h"
#include "SimulationData.h"

class Output
{
public:
    Output(float simulationStep_, int maxPopulationAge, int run);

    void saveAverages(const SimulationAverages& simulationData);

    void saveBasicSimulationMetrics(const SingleSimulationData& data);

    void saveInitialPopulation(const std::list<Individual>& individuals);

    void saveFinalPopulation(const std::list<Individual>& populacja);

    void saveBitsDistribution(
        const std::array<int, Config::bits_>& bitsDistribution,
        int populationCount);

    void saveAgeDistribution(
        const std::array<int, Config::bits_>& ageDistribution);

    void saveDeathsDistribution(
        const std::array<int, Config::bits_>& deathsDistribution,
        const std::array<int, Config::bits_>& ageDistribution);

private:
    enum OUTPUT_FILE
    {
        STATISTICS = 0,
        INITIAL_POPULATION,
        FINAL_POPULATION,
        AGE_DISTRIBUTION,
        BITS_DISTRIBUTION,
        DEATHS_DISTRIBUTION,
        FAMILIES
    };

    std::ofstream openFile(OUTPUT_FILE file) const;

    std::string getName(OUTPUT_FILE file) const;

    const float simulationStep_;

    const int maxPopulationAge_;

    std::string fileNames_[7] = {"statystyki.txt",      "initialPopulation.txt",
                                 "finalPopulation.txt", "rozklad_wieku.txt",
                                 "rozklad_bitow.txt",   "gompertz.txt",
                                 "rodziny.txt"};

    const int run_;
};
