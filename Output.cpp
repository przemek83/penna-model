#include "Output.h"

#include <fstream>

#include "Individual.h"
#include "SimulationData.h"

Output::Output(float simulationStep, int maxPopulationAge) : simulationStep_{simulationStep}, maxPopulationAge_{maxPopulationAge} {}

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
    plik_statystyki = fopen(nazwa(przedrostek, STATYSTYKI).data(), "w");
    plik_rozklad_wieku = fopen(nazwa(przedrostek, ROZKLAD_WIEKU).data(), "w");
    plik_rozklad_bitow = fopen(nazwa(przedrostek, ROZKLAD_JEDYNEK).data(), "w");
    plik_gompertz = fopen(nazwa(przedrostek, GOMPERTZ).data(), "w");
    plik_rodziny = fopen(nazwa(przedrostek, RODZINY).data(), "w");
}

void Output::zamknij_pliki(int przedrostek)
{
    fclose(plik_statystyki);
    fclose(plik_rozklad_wieku);
    fclose(plik_rozklad_bitow);
    fclose(plik_rodziny);
    fclose(plik_gompertz);
}

void Output::saveAvgs(SimulationData::AvgData& simulationData)
{
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
}

void Output::zapisz_kolejne(bool rodzina1, int rok,
                            SimulationData::AvgData& simulationData,
                            int ilosc_osobnikow, int ilosc_narodzin,
                            int ilosc_rodzin, int zgon)
{
    if (!rodzina1)
    {
        fprintf(plik_rodziny, "%d\t%d\n", rok, ilosc_rodzin);
        simulationData.rodziny[rok] += ilosc_rodzin;
    }
    fprintf(plik_statystyki, "%d\t%d\t%d\t%d\t%d\n", rok, ilosc_osobnikow, ilosc_narodzin, ilosc_osobnikow - zgon, zgon);
    simulationData.livingAtStart_[rok] += ilosc_osobnikow;
    simulationData.births_[rok] += ilosc_narodzin;
    simulationData.livingAtEnd_[rok] += ilosc_osobnikow - zgon;
    simulationData.deaths_[rok] += zgon;
}

void Output::saveInitialPopulation(const std::list<Individual>& individuals,
                                   int run)
{
    int counter{0};
    std::ofstream file{nazwa(run, INITIAL_POPULATION).c_str()};
    for (const auto& individual : individuals)
    {
        file << counter << " " << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveFinalPopulation(const std::list<Individual>& individuals,
                                 int run)
{
    int counter{0};
    std::ofstream file{nazwa(run, FINAL_POPULATION).c_str()};
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
    for (int v = 0; v < Config::bits_; v++)
        fprintf(plik_rozklad_bitow, "%d\t%.2f\n", v,
                bitsDistribution[v] * 1.0 / populationCount);
}

void Output::saveAgeDistribution(
    const std::array<int, Config::bits_>& ageDistribution)
{
    for (int v = 0; v < Config::bits_; v++)
        fprintf(plik_rozklad_wieku, "%d\t%d\n", v, ageDistribution[v]);
}

void Output::saveDeathsDistribution(
    const std::array<int, Config::bits_>& deathsDistribution,
    const std::array<int, Config::bits_>& ageDistribution)
{
    for (int v = 0; v < Config::bits_; v++)
    {
        if (ageDistribution[v] > 0)
        {
            fprintf(plik_gompertz, "%d\t%.3f\n", v,
                    deathsDistribution[v] * 1.0 / ageDistribution[v]);
        }
        else
        {
            fprintf(plik_gompertz, "%d\t1\n", v);
        }
    }
}
