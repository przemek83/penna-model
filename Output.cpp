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
    std::filesystem::remove(nazwa(run, FAMILIES));
    std::filesystem::remove(nazwa(run, STATISTICS));
}

std::string Output::nazwa(int przedrostek, int numer)
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

void Output::otworz_pliki(int przedrostek)
{
    plik_statystyki = fopen(nazwa(przedrostek, STATISTICS).data(), "w");
    plik_rozklad_wieku =
        fopen(nazwa(przedrostek, AGE_DISTRIBUTION).data(), "w");
    plik_rozklad_bitow =
        fopen(nazwa(przedrostek, BITS_DISTRIBUTION).data(), "w");
    plik_gompertz = fopen(nazwa(przedrostek, DEATHS_DISTRIBUTION).data(), "w");
    plik_rodziny = fopen(nazwa(przedrostek, FAMILIES).data(), "w");
}

void Output::zamknij_pliki(int przedrostek)
{
    fclose(plik_statystyki);
    fclose(plik_rozklad_wieku);
    fclose(plik_rozklad_bitow);
    fclose(plik_rodziny);
    fclose(plik_gompertz);
}

void Output::saveAverages(const SimulationAverages& simulationData)
{
    otworz_pliki(run_);

    for (int v = 0; v < maxPopulationAge_; v++)
    {
        if (simulationData.rodziny[v] > 1)
            fprintf(plik_rodziny, "%d\t%f\n", v, simulationData.rodziny[v]);
        fprintf(plik_statystyki, "%d\t%f\t%f\t%f\t%f\n", v,
                simulationData.livingAtStart_[v], simulationData.births_[v],
                simulationData.livingAtEnd_[v], simulationData.deaths_[v]);
    }

    for (int v = 0; v < Config::bits_; v++)
    {
        fprintf(plik_rozklad_wieku, "%d\t%f\n", v, simulationData.wiek[v]);
        fprintf(plik_rozklad_bitow, "%d\t%.2f\n", v, simulationData.bity[v]);
        if (simulationData.gompertz[v] > 0)
            fprintf(plik_gompertz, "%d\t%.3f\n", v, simulationData.gompertz[v]);
        else
            fprintf(plik_gompertz, "%d\t1\n", v);
    }

    zamknij_pliki(run_);
}

void Output::zapisz_kolejne(bool rodzina1, int rok, int ilosc_osobnikow,
                            int ilosc_narodzin, int ilosc_rodzin, int zgon)
{
    std::ofstream families{nazwa(run_, FAMILIES), std::ios_base::app};
    std::ofstream stats{nazwa(run_, STATISTICS), std::ios_base::app};

    if (!rodzina1)
        families << rok << "\t" << ilosc_rodzin << std::endl;

    stats << rok << "\t" << ilosc_osobnikow << "\t" << ilosc_narodzin << "\t"
          << ilosc_osobnikow - zgon << "\t" << zgon << std::endl;
}

void Output::saveInitialPopulation(const std::list<Individual>& individuals,
                                   int run)
{
    int counter{0};
    std::ofstream file{nazwa(run, INITIAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        file << counter << " " << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveFinalPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    std::ofstream file{nazwa(run_, FINAL_POPULATION)};
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
    std::ofstream file{nazwa(run_, BITS_DISTRIBUTION)};

    for (int i{0}; i < Config::bits_; i++)
        file << i << "\t" << std::setprecision(2) << std::fixed
             << bitsDistribution[i] * 1.0 / populationCount << std::endl;
}

void Output::saveAgeDistribution(
    const std::array<int, Config::bits_>& ageDistribution)
{
    std::ofstream file{nazwa(run_, AGE_DISTRIBUTION)};
    for (int i{0}; i < Config::bits_; i++)
        file << i << "\t" << ageDistribution[i] << std::endl;
}

void Output::saveDeathsDistribution(
    const std::array<int, Config::bits_>& deathsDistribution,
    const std::array<int, Config::bits_>& ageDistribution)
{
    std::ofstream file{nazwa(run_, DEATHS_DISTRIBUTION)};
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
