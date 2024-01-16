#include "Simulation.h"

#include <iostream>
#include <vector>

#include "Generator.h"
#include "Output.h"

Simulation::Simulation(const Config& config, int number, float step)
    : config_(config), number_(number), step_{step}
{
    individuals_.resize(config_.maxPopulation_);
}

void Simulation::run(Output& output, Generator& generator,
                     SimulationData& simulationDataAvg)
{
    unsigned int rok = 0;  // aktualny rok w symulacji
    bool singleFamily =
        false;             // flaga pokazujaca istnienie tylko jednej rodziny
    std::vector<unsigned int> puste;

    unsigned int ostatni_el = losuj_populacje(output, generator);

    printf("%d/%d Progress:       [", number_, config_.simulationsCount_);

    while (rok < config_.years_)  // kolejne lata
    {
        // zmienne przetrzymujace informacje potrzebne do statystyk
        int ilosc_osobnikow{0};
        int ilosc_narodzin{0};
        int ilosc_rodzin{0};
        int zgon{0};
        std::vector<int> rodziny;
        rodziny.resize(config_.livesOnStart_);
        //  tablice uzywane do zbierania danych z symulacji
        int rozklad_wieku[WIELKOSC * INT_W];
        int rozklad_bitow[WIELKOSC * INT_W];
        int gompertz_zgony[WIELKOSC * INT_W];
        for (int v = 0; v < config_.livesOnStart_; v++)
            rodziny[v] = 0;
        for (int v = 0; v < WIELKOSC * INT_W; v++)
        {
            rozklad_wieku[v] = 0;
            rozklad_bitow[v] = 0;
            gompertz_zgony[v] = 0;
        }

#ifdef SYMULACJA_DORSZY
        if (rok + 1 == MAX_POP_LAT)
        {
            if (ostatni_el - puste.size() == 0)
                return;
        }
#endif

        for (unsigned int i = 0; i < ostatni_el; i++)
        {
            if (individuals_[i].przodek == -1)
                continue;
            else
                ilosc_osobnikow++;

            if (!singleFamily)  // gromadz dane o rodzinach
            {
                rodziny[individuals_[i].przodek]++;
                if (rodziny[individuals_[i].przodek] == 1)
                    ilosc_rodzin++;
            }

            if (rok + 1 == config_.years_)  // zgromadz dane o
                                            // bitach i wieku
            {
                rozklad_wieku[individuals_[i].wiek]++;
                for (int v = 0; v < WIELKOSC * INT_W; v++)
                    if (individuals_[i].ciag[v / INT_W] &
                        (1 << (INT_W - (v + 1) % INT_W)))
                        rozklad_bitow[v]++;
            }

            // decyzja o zyciu badz smierci osobnika
            if ((individuals_[i].ilosc_1 >=
                 config_.maxMutations_) ||                     // jedynki
                (individuals_[i].wiek >= WIELKOSC * INT_W) ||  // starosc
                ((float)generator.getInt(0, 100) <=
                 (float)(ostatni_el - puste.size()) / config_.maxPopulation_ *
                     100.0)  // verhulst
#ifdef SYMULACJA_DORSZY
                || ((rok > ODLOWY_OD) &&
                    (individuals_[i].wiek >= MINIMALNY_WIEK) &&
                    ((float)generator.getInt(0, 10000) / 100 <=
                     START_ODLOWOW + number_ * step_))
#endif
                    )  // smierc
            {
                zgon++;
                gompertz_zgony[individuals_[i].wiek]++;
                puste.push_back(i);
                individuals_[i].przodek = -1;
                continue;
            }
            else  // zycie
            {
                if ((individuals_[i].wiek > config_.reproductionAge_) &&
                    generator.getInt(1, 100) <=
                        config_.chanceForOffspring_)  // potomstwo
                {
                    Individual osobnik;
                    for (unsigned int l = 0; l < config_.offspringCount_;
                         l++)  // ile potomstwa
                    {
                        ilosc_narodzin++;
                        for (int x = 0; x < WIELKOSC; x++)  // przepisz genom
                                                            // rodzica
                            osobnik.ciag[x] = individuals_[i].ciag[x];
                        for (unsigned m = 0; m < config_.mutationsDelta_;
                             m++)  // mutacje
                        {
                            unsigned int liczba_losowa =
                                generator.getInt(0, INT_W - 1);
                            unsigned int ktore =
                                generator.getInt(0, WIELKOSC - 1);
                            unsigned int temp = 1;
                            temp <<= liczba_losowa;
                            osobnik.ciag[ktore] = (osobnik.ciag[ktore] | temp);
                        }

                        if (puste.size() == 0 &&
                            ostatni_el < config_.maxPopulation_)  // dodaj
                                                                  // osobnika
                                                                  // na
                                                                  // nowym
                                                                  // miejscu
                        {
                            individuals_[ostatni_el].inicjuj(
                                individuals_[i].przodek, osobnik.ciag);
                            ostatni_el++;
                        }
                        else  // dodaj osobnika w
                              // miejsce puste
                        {
                            if (puste.size() > 0)
                            {
                                individuals_[puste.back()].inicjuj(
                                    individuals_[i].przodek, osobnik.ciag);
                                puste.pop_back();
                            }
                        }
                    }
                }
            }
            individuals_[i].wiek++;                      // dodaj rok do wieku
            individuals_[i].czy1(individuals_[i].wiek);  // sprawdzanie chorob
        }
        if (ilosc_rodzin == 1)
            singleFamily = true;

        output.zapisz_kolejne(singleFamily, rok, simulationDataAvg,
                              ilosc_osobnikow, ilosc_narodzin, ilosc_rodzin,
                              zgon, rozklad_wieku, rozklad_bitow,
                              gompertz_zgony);
        rok++;
        if ((rok % (config_.years_ / 50)) == 0)
            std::cout << "*";
    }  // kolejne lata
    std::cout << "]";
    output.zapisz_koncowa_populacje(
        individuals_, config_.simulationsCount_ + 1 - number_, ostatni_el);
}

int Simulation::losuj_populacje(Output& wyjscie, Generator& generator)
{
    unsigned int nowy[WIELKOSC];
    unsigned int liczba_losowa = 0;
    unsigned int ktore = 0;
    unsigned int temp = 0;
    unsigned int numer = 0;

    for (int i = 0; i < config_.livesOnStart_; i++)
    {
        for (int x = 0; x < WIELKOSC; x++)
            nowy[x] = 0;
        for (int j = 0; j < config_.startingMutations_; j++)
        {
            do
            {
                liczba_losowa = generator.getInt(0, INT_W - 1);
                ktore = generator.getInt(0, WIELKOSC - 1);
                temp = 1;
                temp <<= liczba_losowa;
            } while (nowy[ktore] == (nowy[ktore] | temp));

            nowy[ktore] = (nowy[ktore] | temp);
        }
        numer++;
        individuals_[numer - 1].inicjuj(i, nowy);
        wyjscie.zapisz_losowana_populacje(individuals_, numer);
    }
    return numer;
}
