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

void Output::saveAverages(const SimulationAverages& simulationData)
{
    std::unique_ptr<std::ostream> families{openFile(FAMILIES)};
    *families << std::setprecision(6) << std::fixed;
    std::unique_ptr<std::ostream> ages{openFile(AGE_DISTRIBUTION)};
    *ages << std::setprecision(6) << std::fixed;
    std::unique_ptr<std::ostream> bits{openFile(BITS_DISTRIBUTION)};
    *bits << std::setprecision(2) << std::fixed;
    std::unique_ptr<std::ostream> deaths{openFile(DEATHS_DISTRIBUTION)};
    *deaths << std::setprecision(3) << std::fixed;
    std::unique_ptr<std::ostream> stats{openFile(STATISTICS)};
    *stats << std::setprecision(6) << std::fixed;

    for (size_t i{0}; i < maxPopulationAge_; i++)
    {
        if (simulationData.rodziny[i] > 1)
            *families << i << "\t" << simulationData.rodziny[i] << std::endl;

        *stats << i << "\t" << simulationData.livingAtStart_[i] << "\t"
               << simulationData.births_[i] << "\t"
               << simulationData.livingAtEnd_[i] << "\t"
               << simulationData.deaths_[i] << std::endl;
    }

    for (size_t i{0}; i < Config::bits_; i++)
    {
        *ages << i << "\t" << simulationData.wiek[i] << std::endl;
        *bits << i << "\t" << simulationData.bity[i] << std::endl;

        if (simulationData.gompertz[i] > 0)
            *deaths << i << "\t" << simulationData.gompertz[i] << std::endl;
        else
            *deaths << i << "\t" << 1 << std::endl;
    }
}

void Output::saveBasicSimulationMetrics(const SingleSimulationData& data)
{
    std::unique_ptr<std::ostream> families{openFile(FAMILIES)};
    std::unique_ptr<std::ostream> stats{openFile(STATISTICS)};

    for (size_t year{0}; year < data.livingAtStart_.size(); ++year)
    {
        if (data.rodziny[year] > 1)
            *families << year << "\t" << data.rodziny[year] << std::endl;

        *stats << year << "\t" << data.livingAtStart_[year] << "\t"
               << data.births_[year] << "\t" << data.livingAtEnd_[year] << "\t"
               << data.deaths_[year] << std::endl;
    }
}

void Output::saveInitialPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    std::unique_ptr<std::ostream> file{openFile(INITIAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        *file << counter << " " << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveFinalPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    std::unique_ptr<std::ostream> file{openFile(FINAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        *file << counter << " " << individual.getAncestor() << " "
              << individual.getAge() << " " << individual.getSurvivedMutations()
              << " " << individual.genome_.to_ullong() << " "
              << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveBitsDistribution(
    const std::array<float, Config::bits_>& bitsDistribution)
{
    std::unique_ptr<std::ostream> file{openFile(BITS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        *file << i << "\t" << std::setprecision(2) << std::fixed
              << bitsDistribution[i] << std::endl;
}

void Output::saveAgeDistribution(
    const std::array<int, Config::bits_>& ageDistribution)
{
    std::unique_ptr<std::ostream> file{openFile(AGE_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        *file << i << "\t" << ageDistribution[i] << std::endl;
}

void Output::saveDeathsDistribution(
    const std::array<float, Config::bits_>& deaths)
{
    std::unique_ptr<std::ostream> file{openFile(DEATHS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
    {
        *file << i << "\t" << std::setprecision(3) << std::fixed << deaths[i]
              << std::endl;
    }
}
