#include "Output.h"

#include "Common.h"
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
    if (przedrostek != 0)
        plik_osobniki = fopen(nazwa(przedrostek, POPULACJE).data(), "w");
    plik_rozklad_wieku = fopen(nazwa(przedrostek, ROZKLAD_WIEKU).data(), "w");
    plik_rozklad_bitow = fopen(nazwa(przedrostek, ROZKLAD_JEDYNEK).data(), "w");
    plik_gompertz = fopen(nazwa(przedrostek, GOMPERTZ).data(), "w");
    plik_rodziny = fopen(nazwa(przedrostek, RODZINY).data(), "w");
}

void Output::otworz_pliki2(int przedrostek)
{
#ifdef CALE_WYJSCIE
    otworz_pliki(przedrostek);
#endif
}

void Output::zamknij_pliki(int przedrostek)
{
    if (przedrostek != 0)
        fclose(plik_osobniki);
    fclose(plik_statystyki);
    fclose(plik_rozklad_wieku);
    fclose(plik_rozklad_bitow);
    fclose(plik_rodziny);
    fclose(plik_gompertz);
}

void Output::zapisz_srednie(int symulacji,
                            SimulationData::AvgData& simulationData)
{
    przelicz_srednie_konwencjonalnie((float)symulacji, simulationData);

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
#ifdef CALE_WYJSCIE
        fprintf(plik_rodziny, "%d\t%d\n", rok, ilosc_rodzin);
#endif
        simulationData.rodziny[rok] += ilosc_rodzin;
    }
#ifdef CALE_WYJSCIE
    fprintf(plik_statystyki, "%d\t%d\t%d\t%d\t%d\n", rok, ilosc_osobnikow, ilosc_narodzin, ilosc_osobnikow - zgon, zgon);
#endif
    simulationData.livingAtStart_[rok] += ilosc_osobnikow;
    simulationData.births_[rok] += ilosc_narodzin;
    simulationData.livingAtEnd_[rok] += ilosc_osobnikow - zgon;
    simulationData.deaths_[rok] += zgon;
}

void Output::zapisz_losowana_populacje(const std::list<Individual>& individuals)
{
#ifdef CALE_WYJSCIE
    int counter{0};
    for (const auto& individual : individuals)
    {
        fprintf(plik_osobniki, "%u %s\n", counter,
                individual.asBitString().c_str());
        counter++;
    }
#endif
}

void Output::zapisz_koncowa_populacje(std::list<Individual>& individuals, int x)
{
#ifdef CALE_WYJSCIE
    int counter{0};
    for (const auto& individual : individuals)
    {
        fprintf(plik_osobniki, "%u %d %d %d %u %s\n", counter,
                individual.getAncestor(), individual.getAge(),
                individual.getSurvivedMutations(),
                individual.genome_.to_ullong(),
                individual.asBitString().c_str());
        counter++;
    }
    zamknij_pliki(x);
#endif
}

void Output::saveBitsDistribution(
    const std::array<int, Config::bits_>& bitsDistribution,
    SimulationData::AvgData& avgData, int populationCount)
{
    for (int v = 0; v < Config::bits_; v++)
    {
#ifdef CALE_WYJSCIE
        fprintf(plik_rozklad_bitow, "%d\t%.2f\n", v,
                bitsDistribution[v] * 1.0 / populationCount);
#endif
        avgData.bity[v] += (float)bitsDistribution[v] / (float)populationCount;
    }
}

void Output::saveAgeDistribution(
    const std::array<int, Config::bits_>& ageDistribution,
    SimulationData::AvgData& avgData)
{
    for (int v = 0; v < Config::bits_; v++)
    {
#ifdef CALE_WYJSCIE
        fprintf(plik_rozklad_wieku, "%d\t%d\n", v, ageDistribution[v]);
#endif
        avgData.wiek[v] += ageDistribution[v];
    }
}

void Output::saveDeathsDistribution(
    const std::array<int, Config::bits_>& deathsDistribution,
    const std::array<int, Config::bits_>& ageDistribution,
    SimulationData::AvgData& avgData)
{
    for (int v = 0; v < Config::bits_; v++)
    {
        if (ageDistribution[v] > 0)
        {
#ifdef CALE_WYJSCIE
            fprintf(plik_gompertz, "%d\t%.3f\n", v,
                    deathsDistribution[v] * 1.0 / ageDistribution[v]);
#endif
            avgData.gompertz[v] +=
                (float)deathsDistribution[v] / (float)ageDistribution[v];
        }
        else
        {
#ifdef CALE_WYJSCIE
            fprintf(plik_gompertz, "%d\t1\n", v);
#endif
            avgData.gompertz[v] += 1;
        }
    }
}

void Output::przelicz_srednie_konwencjonalnie(
    float dzielnik, SimulationData::AvgData& simulationData)
{
    for (size_t v = 0; v < Config::bits_; v++)
    {
        simulationData.gompertz[v] /= dzielnik;
        simulationData.bity[v] /= dzielnik;
        simulationData.wiek[v] /= dzielnik;
    }

    for (size_t v = 0; v < maxPopulationAge_; v++)
        simulationData.rodziny[v] /= dzielnik;

    for (size_t v = 0; v < maxPopulationAge_; v++)
    {
        simulationData.livingAtStart_[v] /= dzielnik;
        simulationData.births_[v] /= dzielnik;
        simulationData.livingAtEnd_[v] /= dzielnik;
        simulationData.deaths_[v] /= dzielnik;
    }
}
