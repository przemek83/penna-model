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

    void otworz_pliki2(int przedrostek);

    void zamknij_pliki(int przedrostek);

    void zapisz_srednie(int symulacji, SimulationData::AvgData& simulationData);

    void zapisz_kolejne(
        bool rodzina1, int rok, SimulationData::AvgData& simulationData,
        int ilosc_osobnikow, int ilosc_narodzin, int ilosc_rodzin, int zgon,
        const std::array<int, Config::bits_>& ageDistribution,
        const std::array<int, Config::bits_>& bitsDistribution,
        const std::array<int, Config::bits_>& gompertzDeathsDistribution);

    void zapisz_losowana_populacje(const std::list<Individual>& individuals);

    void zapisz_koncowa_populacje(std::list<Individual>& populacja, int x);

private:
    std::string nazwa(int przedrostek, int numer);

    void przelicz_srednie_konwencjonalnie(
        float dzielnik, SimulationData::AvgData& simulationData);

    const float simulationStep_;

    const int maxPopulationAge_;

    std::string fileNames_[6] = {"statystyki.txt", "osobniki.txt", "rozklad_wieku.txt", "rozklad_bitow.txt", "gompertz.txt", "rodziny.txt"};

    FILE* plik_statystyki;
    FILE* plik_osobniki;
    FILE* plik_rozklad_wieku;
    FILE* plik_rozklad_bitow;
    FILE* plik_gompertz;
    FILE* plik_rodziny;

    enum
    {
        STATYSTYKI = 0,
        POPULACJE,
        ROZKLAD_WIEKU,
        ROZKLAD_JEDYNEK,
        GOMPERTZ,
        RODZINY
    };
};
