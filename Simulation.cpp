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
                     SimulationData::AvgData& simulationDataAvg)
{
    int year{0};
    bool singleFamilyLeft{false};
    std::vector<unsigned int> puste;

    unsigned int ostatni_el = losuj_populacje(output, generator);

    printf("%d/%d Progress:       [", number_, config_.simulationsCount_);

    while (year < config_.years_)  // kolejne lata
    {
        // zmienne przetrzymujace informacje potrzebne do statystyk
        int ilosc_osobnikow{0};
        int ilosc_narodzin{0};
        int ilosc_rodzin{0};
        int zgon{0};
        std::vector<int> rodziny;
        rodziny.resize(config_.livesOnStart_);
        //  tablice uzywane do zbierania danych z symulacji
        int rozklad_wieku[Config::bits_];
        int rozklad_bitow[Config::bits_];
        int gompertz_zgony[Config::bits_];
        for (int v = 0; v < config_.livesOnStart_; v++)
            rodziny[v] = 0;
        for (int v = 0; v < Config::bits_; v++)
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
            if (individuals_[i].ancestor_ == -1)
                continue;
            else
                ilosc_osobnikow++;

            if (!singleFamilyLeft)  // gromadz dane o rodzinach
            {
                rodziny[individuals_[i].ancestor_]++;
                if (rodziny[individuals_[i].ancestor_] == 1)
                    ilosc_rodzin++;
            }

            if (year + 1 == config_.years_)  // zgromadz dane o
                                             // bitach i wieku
            {
                rozklad_wieku[individuals_[i].age_]++;
                for (int v = 0; v < Config::bits_; v++)
                    if (individuals_[i].genome_[v / Config::intSize_] &
                        (1 << (Config::intSize_ - (v + 1) % Config::intSize_)))
                        rozklad_bitow[v]++;
            }

            // decyzja o zyciu badz smierci osobnika
            if ((individuals_[i].survivedOnes_ >=
                 config_.maxMutations_) ||                  // jedynki
                (individuals_[i].age_ >= Config::bits_) ||  // starosc
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
                gompertz_zgony[individuals_[i].age_]++;
                puste.push_back(i);
                individuals_[i].ancestor_ = -1;
                continue;
            }

            if ((individuals_[i].age_ > config_.reproductionAge_) &&
                generator.getInt(1, 100) <=
                    config_.chanceForOffspring_)  // potomstwo
            {
                for (int l{0}; l < config_.offspringCount_; l++)
                {
                    Individual osobnik{individuals_[i].offspring()};
                    ilosc_narodzin++;

                    for (int m{0}; m < config_.mutationsDelta_; m++)
                        osobnik.applyMutation(generator);

                    if (puste.empty() && ostatni_el < config_.maxPopulation_)
                    {
                        individuals_[ostatni_el] = osobnik;
                        ostatni_el++;
                    }
                    else
                    {
                        if (!puste.empty())
                        {
                            individuals_[puste.back()] = osobnik;
                            puste.pop_back();
                        }
                    }
                }
            }

            individuals_[i].age_++;     // dodaj rok do wieku
            individuals_[i].ageByOneYear(
                individuals_[i].age_);  // sprawdzanie chorob
        }
        if (ilosc_rodzin == 1)
            singleFamilyLeft = true;

        output.zapisz_kolejne(singleFamilyLeft, year, simulationDataAvg,
                              ilosc_osobnikow, ilosc_narodzin, ilosc_rodzin,
                              zgon, rozklad_wieku, rozklad_bitow,
                              gompertz_zgony);
        year++;
        if ((year % (config_.years_ / 50)) == 0)
            std::cout << "*";
    }
    std::cout << "]";
    output.zapisz_koncowa_populacje(
        individuals_, config_.simulationsCount_ + 1 - number_, ostatni_el);
}

int Simulation::losuj_populacje(Output& wyjscie, Generator& generator)
{
    for (size_t i{0}; i < config_.livesOnStart_; i++)
    {
        individuals_[i].assignRandomBits(generator, config_.startingMutations_);
        individuals_[i].ancestor_ = i;
        wyjscie.zapisz_losowana_populacje(individuals_, i);
    }
    return config_.livesOnStart_;
}
