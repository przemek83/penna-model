#include "Output.h"

#include <filesystem>
#include <iomanip>
#include <iostream>

Output::Output(float simulationStep, int maxPopulationAge, int run)
    : simulationStep_{simulationStep},
      maxPopulationAge_{maxPopulationAge},
      run_{run}
{
}

void Output::saveAverages(const SimulationAverages& data)
{
    const std::shared_ptr<std::ostream> families{getStream(FAMILIES)};
    *families << std::setprecision(6) << std::fixed;
    const std::shared_ptr<std::ostream> ages{getStream(AGE_DISTRIBUTION)};
    *ages << std::setprecision(6) << std::fixed;
    const std::shared_ptr<std::ostream> bits{getStream(BITS_DISTRIBUTION)};
    *bits << std::setprecision(2) << std::fixed;
    const std::shared_ptr<std::ostream> deaths{getStream(DEATHS_DISTRIBUTION)};
    *deaths << std::setprecision(3) << std::fixed;
    const std::shared_ptr<std::ostream> stats{getStream(STATISTICS)};
    *stats << std::setprecision(6) << std::fixed;

    for (std::size_t i{0}; i < maxPopulationAge_; i++)
    {
        const SimulationData<float>::BasicData& basicData{data.getBasicData(i)};
        if (basicData.families_ > 1)
            *families << i << "\t" << basicData.families_ << std::endl;

        *stats << i << "\t" << basicData.livingAtStart_ << "\t"
               << basicData.births_ << "\t" << basicData.livingAtEnd_ << "\t"
               << basicData.deaths_ << std::endl;
    }

    const std::vector<float>& deathsDistribution{data.getDeathsDistribution()};
    const std::vector<float>& bitsDistribution{data.getBitsDistribution()};
    const std::vector<float>& ageDistribution{data.getAgeDistribution()};

    for (size_t i{0}; i < Config::bits_; i++)
    {
        *ages << i << "\t" << ageDistribution[i] << std::endl;
        *bits << i << "\t" << bitsDistribution[i] << std::endl;

        if (deathsDistribution[i] > 0)
            *deaths << i << "\t" << deathsDistribution[i] << std::endl;
        else
            *deaths << i << "\t" << 1 << std::endl;
    }
}

void Output::saveBasicSimulationMetrics(const SingleSimulationData& data)
{
    const std::shared_ptr<std::ostream> families{getStream(FAMILIES)};
    const std::shared_ptr<std::ostream> stats{getStream(STATISTICS)};

    for (size_t year{0}; year < maxPopulationAge_; ++year)
    {
        const SingleSimulationData::BasicData& basicData{
            data.getBasicData(year)};
        if (basicData.families_ > 1)
            *families << year << "\t" << basicData.families_ << std::endl;

        *stats << year << "\t" << basicData.livingAtStart_ << "\t"
               << basicData.births_ << "\t" << basicData.livingAtEnd_ << "\t"
               << basicData.deaths_ << std::endl;
    }
}

void Output::saveInitialPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    const std::shared_ptr<std::ostream> file{getStream(INITIAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        *file << counter << " " << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveFinalPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    const std::shared_ptr<std::ostream> file{getStream(FINAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        *file << counter << " " << individual.getAncestor() << " "
              << individual.getAge() << " " << individual.getSurvivedMutations()
              << " " << individual.getGenomeAsNumber() << " "
              << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveBitsDistribution(const std::vector<float>& bitsDistribution)
{
    const std::shared_ptr<std::ostream> file{getStream(BITS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        *file << i << "\t" << std::setprecision(2) << std::fixed
              << bitsDistribution[i] << std::endl;
}

void Output::saveAgeDistribution(const std::vector<int>& ageDistribution)
{
    const std::shared_ptr<std::ostream> file{getStream(AGE_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        *file << i << "\t" << ageDistribution[i] << std::endl;
}

void Output::saveDeathsDistribution(const std::vector<float>& deaths)
{
    const std::shared_ptr<std::ostream> file{getStream(DEATHS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
    {
        *file << i << "\t" << std::setprecision(3) << std::fixed << deaths[i]
              << std::endl;
    }
}
