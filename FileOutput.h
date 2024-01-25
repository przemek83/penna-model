#pragma once

#include <list>
#include <string>

#include "Individual.h"
#include "Output.h"
#include "SimulationData.h"

class FileOutput : public Output
{
public:
    FileOutput(float simulationStep_, int maxPopulationAge, int run);

    void saveAverages(const SimulationAverages& simulationData) override;

    void saveBasicSimulationMetrics(const SingleSimulationData& data) override;

    void saveInitialPopulation(
        const std::list<Individual>& individuals) override;

    void saveFinalPopulation(const std::list<Individual>& populacja) override;

    void saveBitsDistribution(
        const std::array<float, Config::bits_>& bitsDistribution) override;

    void saveAgeDistribution(
        const std::array<int, Config::bits_>& ageDistribution) override;

    void saveDeathsDistribution(
        const std::array<float, Config::bits_>& deaths) override;

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

    const std::vector<std::string> fileNames_{
        "statystyki.txt",    "initialPopulation.txt", "finalPopulation.txt",
        "rozklad_wieku.txt", "rozklad_bitow.txt",     "gompertz.txt",
        "rodziny.txt"};
};
