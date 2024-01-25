#include "FileOutput.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Individual.h"
#include "SimulationData.h"

FileOutput::FileOutput(float simulationStep, int maxPopulationAge, int run)
    : Output(simulationStep, maxPopulationAge, run)
{
}

std::string FileOutput::getName(OUTPUT_FILE file) const
{
    std::string fileName;

#ifdef SYMULACJA_DORSZY
    sprintf(bufor, "%3.2f", START_ODLOWOW + przedrostek * simulationStep_);
#endif

#ifndef SYMULACJA_DORSZY
    fileName.append("proces");
    fileName.append(std::to_string(1));
    fileName.append("_symulacja");
#endif
    fileName.append(std::to_string(run_));
    fileName.append("_");
    fileName.append(fileNames_[file]);

    return fileName;
}

void FileOutput::saveAverages(const SimulationAverages& simulationData)
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

void FileOutput::saveBasicSimulationMetrics(const SingleSimulationData& data)
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

void FileOutput::saveInitialPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    std::ofstream file{openFile(INITIAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        file << counter << " " << individual.asBitString() << std::endl;
        counter++;
    }
}

void FileOutput::saveFinalPopulation(const std::list<Individual>& individuals)
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

void FileOutput::saveBitsDistribution(
    const std::array<float, Config::bits_>& bitsDistribution)
{
    std::ofstream file{openFile(BITS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        file << i << "\t" << std::setprecision(2) << std::fixed
             << bitsDistribution[i] << std::endl;
}

void FileOutput::saveAgeDistribution(
    const std::array<int, Config::bits_>& ageDistribution)
{
    std::ofstream file{openFile(AGE_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        file << i << "\t" << ageDistribution[i] << std::endl;
}

void FileOutput::saveDeathsDistribution(
    const std::array<float, Config::bits_>& deaths)
{
    std::ofstream file{openFile(DEATHS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
    {
        file << i << "\t" << std::setprecision(3) << std::fixed << deaths[i]
             << std::endl;
    }
}

std::ofstream FileOutput::openFile(OUTPUT_FILE file) const
{
    std::ofstream stream{getName(file)};
    return stream;
}