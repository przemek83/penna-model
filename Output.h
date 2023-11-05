#pragma once

#include <string>
#include "Individual.h"

class Output
{
public:
    Output(float simulationStep_, int processId, int maxPopulationAge);

    void otworz_pliki(int przedrostek);

    void otworz_pliki2(int przedrostek);

    void zamknij_pliki(int przedrostek);

    void zapisz_srednie(int symulacji, float* sr_gompertz_final, float* sr_rodziny_final, float* sr_bity_final, float* sr_wiek_final, float sr_stat_final[][4]);

    void zapisz_kolejne(bool rodzina1, int rok, float* sr_gompertz, float* sr_rodziny, float* sr_bity, float* sr_wiek, float sr_stat[][4], int ilosc_osobnikow,
                        int ilosc_narodzin, int ilosc_rodzin, int zgon, int* rozklad_wieku, int* rozklad_bitow, int* gompertz_zgony);

    void zapisz_losowana_populacje(Individual* populacja, int numer);

    void zapisz_koncowa_populacje(Individual* populacja, int x, unsigned int ostatni_el);

private:
    std::string nazwa(int przedrostek, int numer);

    void przelicz_srednie_konwencjonalnie(float dzielnik, float* sr_gompertz_final, float* sr_rodziny_final, float* sr_bity_final, float* sr_wiek_final,
                                          float sr_stat_final[][4]);

    const float simulationStep_;

    const int maxPopulationAge_;

    const int processId_;

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
