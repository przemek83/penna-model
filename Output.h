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

    void zapisz_kolejne(int year, int populationCount, int birthsCount,
                        int familiesCount, int deaths);

    void saveInitialPopulation(const std::list<Individual>& individuals,
                               int run);

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
    void otworz_pliki(int przedrostek);

    void zamknij_pliki(int przedrostek);

    std::string nazwa(int przedrostek, int numer);

    const float simulationStep_;

    const int maxPopulationAge_;

    std::string fileNames_[7] = {"statystyki.txt",      "initialPopulation.txt",
                                 "finalPopulation.txt", "rozklad_wieku.txt",
                                 "rozklad_bitow.txt",   "gompertz.txt",
                                 "rodziny.txt"};

    FILE* plik_statystyki;
    FILE* plik_rozklad_wieku;
    FILE* plik_rozklad_bitow;
    FILE* plik_gompertz;
    FILE* plik_rodziny;

    enum
    {
        STATISTICS = 0,
        INITIAL_POPULATION,
        FINAL_POPULATION,
        AGE_DISTRIBUTION,
        BITS_DISTRIBUTION,
        DEATHS_DISTRIBUTION,
        FAMILIES
    };

    const int run_;
};
