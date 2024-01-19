#pragma once

#include <string>
#include <vector>

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

    void zapisz_kolejne(bool rodzina1, int rok,
                        SimulationData::AvgData& simulationData,
                        int ilosc_osobnikow, int ilosc_narodzin,
                        int ilosc_rodzin, int zgon, int* rozklad_wieku,
                        int* rozklad_bitow, int* gompertz_zgony);

    void zapisz_losowana_populacje(const std::vector<Individual>& individuals,
                                   int count);

    void zapisz_koncowa_populacje(std::vector<Individual>& populacja, int x,
                                  unsigned int ostatni_el);

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
