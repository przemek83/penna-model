#pragma once

#include <list>
#include <string>

#include "Individual.h"
#include "SimulationData.h"

class Output
{
public:
    Output(float simulationStep_, int maxPopulationAge);

    void otworz_pliki(int przedrostek);

    void zamknij_pliki(int przedrostek);

    void saveAvgs(int symulacji, SimulationData::AvgData& simulationData);

    void zapisz_kolejne(bool rodzina1, int rok,
                        SimulationData::AvgData& simulationData,
                        int ilosc_osobnikow, int ilosc_narodzin,
                        int ilosc_rodzin, int zgon);

    void saveInitialPopulation(const std::list<Individual>& individuals,
                               int run);

    void saveFinalPopulation(const std::list<Individual>& populacja, int run);

    void saveBitsDistribution(
        const std::array<int, Config::bits_>& bitsDistribution,
        int populationCount);

    void saveAgeDistribution(
        const std::array<int, Config::bits_>& ageDistribution);

    void saveDeathsDistribution(
        const std::array<int, Config::bits_>& deathsDistribution,
        const std::array<int, Config::bits_>& ageDistribution);

private:
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
        STATYSTYKI = 0,
        INITIAL_POPULATION,
        FINAL_POPULATION,
        ROZKLAD_WIEKU,
        ROZKLAD_JEDYNEK,
        GOMPERTZ,
        RODZINY
    };
};
