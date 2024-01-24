#include "Output.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Individual.h"
#include "SimulationData.h"

Output::Output(float simulationStep, int maxPopulationAge, int run)
    : simulationStep_{simulationStep},
      maxPopulationAge_{maxPopulationAge},
      run_{run}
{
}

std::string Output::nazwa(int przedrostek, int numer) const
{
    std::string plik_nazwa;

#ifdef SYMULACJA_DORSZY
    sprintf(bufor, "%3.2f", START_ODLOWOW + przedrostek * simulationStep_);
#endif

    plik_nazwa.erase();
#ifndef SYMULACJA_DORSZY
    plik_nazwa.append("proces");
    plik_nazwa.append(std::to_string(1));
    plik_nazwa.append("_symulacja");
#endif
    plik_nazwa.append(std::to_string(przedrostek));
    plik_nazwa.append("_");
    plik_nazwa.append(fileNames_[numer]);

    return plik_nazwa;
}

void Output::saveAverages(const SimulationAverages& simulationData)
{
    std::ofstream families{openFile(FAMILIES)};
    families << std::setprecision(6) << std::fixed;
    std::ofstream ages{openFile(AGE_DISTRIBUTION)};
    ages << std::setprecision(6) << std::fixed;
    std::ofstream bits{openFile(BITS_DISTRIBUTION)};
    bits << std::setprecision(2) << std::fixed;
    std::ofstream deaths{openFile(DEATHS_DISTRIBUTION)};
    deaths << std::setprecision(3) << std::fixed;
    std::ofstream stats{openFile(STATISTICS)};
    stats << std::setprecision(6) << std::fixed;

    for (size_t i{0}; i < maxPopulationAge_; i++)
    {
        if (simulationData.rodziny[i] > 1)
            families << i << "\t" << simulationData.rodziny[i] << std::endl;

        stats << i << "\t" << simulationData.livingAtStart_[i] << "\t"
              << simulationData.births_[i] << "\t"
              << simulationData.livingAtEnd_[i] << "\t"
              << simulationData.deaths_[i] << std::endl;
    }

    for (size_t i{0}; i < Config::bits_; i++)
    {
        ages << i << "\t" << simulationData.wiek[i] << std::endl;
        bits << i << "\t" << simulationData.bity[i] << std::endl;

        if (simulationData.gompertz[i] > 0)
            deaths << i << "\t" << simulationData.gompertz[i] << std::endl;
        else
            deaths << i << "\t" << 1 << std::endl;
    }
}

void Output::saveBasicSimulationMetrics(const SingleSimulationData& data)
{
    std::ofstream families{openFile(FAMILIES)};
    std::ofstream stats{openFile(STATISTICS)};

    for (size_t year{0}; year < data.livingAtStart_.size(); ++year)
    {
        if (data.rodziny[year] > 1)
            families << year << "\t" << data.rodziny[year] << std::endl;

        stats << year << "\t" << data.livingAtStart_[year] << "\t"
              << data.births_[year] << "\t" << data.livingAtEnd_[year] << "\t"
              << data.deaths_[year] << std::endl;
    }
}

void Output::saveInitialPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    std::ofstream file{openFile(INITIAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        file << counter << " " << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveFinalPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    std::ofstream file{openFile(FINAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        file << counter << " " << individual.getAncestor() << " "
             << individual.getAge() << " " << individual.getSurvivedMutations()
             << " " << individual.genome_.to_ullong() << " "
             << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveBitsDistribution(
    const std::array<int, Config::bits_>& bitsDistribution, int populationCount)
{
    std::ofstream file{openFile(BITS_DISTRIBUTION)};

    for (int i{0}; i < Config::bits_; i++)
        file << i << "\t" << std::setprecision(2) << std::fixed
             << bitsDistribution[i] * 1.0 / populationCount << std::endl;
}

void Output::saveAgeDistribution(
    const std::array<int, Config::bits_>& ageDistribution)
{
    std::ofstream file{openFile(AGE_DISTRIBUTION)};
    for (int i{0}; i < Config::bits_; i++)
        file << i << "\t" << ageDistribution[i] << std::endl;
}

void Output::saveDeathsDistribution(
    const std::array<int, Config::bits_>& deathsDistribution,
    const std::array<int, Config::bits_>& ageDistribution)
{
    std::ofstream file{openFile(DEATHS_DISTRIBUTION)};
    for (int i{0}; i < Config::bits_; i++)
    {
        if (ageDistribution[i] > 0)
        {
            file << i << "\t" << std::setprecision(3) << std::fixed
                 << deathsDistribution[i] * 1.0 / ageDistribution[i]
                 << std::endl;
        }
        else
        {
            file << i << "\t"
                 << "1" << std::endl;
        }
    }
}

std::ofstream Output::openFile(OUTPUT_FILE file) const
{
    std::ofstream stream{nazwa(run_, file)};
    return stream;
}
