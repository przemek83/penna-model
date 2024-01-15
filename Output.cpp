#include "Output.h"

#include "Common.h"
#include "Individual.h"
#include "SimulationData.h"

Output::Output(float simulationStep, int maxPopulationAge) : simulationStep_{simulationStep}, maxPopulationAge_{maxPopulationAge} {}

std::string Output::nazwa(int przedrostek, int numer)
{
    std::string plik_nazwa;
    char bufor[10], bufor2[10];

#ifdef SYMULACJA_DORSZY
    sprintf(bufor, "%3.2f", START_ODLOWOW + przedrostek * simulationStep_);
#else
    _itoa(przedrostek, bufor, 10);
#endif
    _itoa(1, bufor2, 10);

    plik_nazwa.erase();
#ifndef SYMULACJA_DORSZY
    plik_nazwa.append("proces");
    plik_nazwa.append(bufor2);
    plik_nazwa.append("_symulacja");
#endif
    plik_nazwa.append(bufor);
    plik_nazwa.append("_");
    plik_nazwa.append(fileNames_[numer]);

    return plik_nazwa;
}

void Output::otworz_pliki(int przedrostek)
{
    fopen_s(&plik_statystyki, nazwa(przedrostek, STATYSTYKI).data(), "w");
    if (przedrostek != 0)
        fopen_s(&plik_osobniki, nazwa(przedrostek, POPULACJE).data(), "w");
    fopen_s(&plik_rozklad_wieku, nazwa(przedrostek, ROZKLAD_WIEKU).data(), "w");
    fopen_s(&plik_rozklad_bitow, nazwa(przedrostek, ROZKLAD_JEDYNEK).data(), "w");
    fopen_s(&plik_gompertz, nazwa(przedrostek, GOMPERTZ).data(), "w");
    fopen_s(&plik_rodziny, nazwa(przedrostek, RODZINY).data(), "w");
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
    fclose(plik_gompertz);
}

void Output::zapisz_srednie(int symulacji, SimulationData& simulationData)
{
    przelicz_srednie_konwencjonalnie((float)symulacji, simulationData);

    for (int v = 0; v < maxPopulationAge_; v++)
    {
        if (simulationData.rodziny[v] > 1)
            fprintf(plik_rodziny, "%d\t%f\n", v, simulationData.rodziny[v]);
        fprintf(plik_statystyki, "%d\t%f\t%f\t%f\t%f\n", v, simulationData.stat[v][0], simulationData.stat[v][1], simulationData.stat[v][2],
                simulationData.stat[v][3]);
    }

    for (int v = 0; v < WIELKOSC * INT_W; v++)
    {
        fprintf(plik_rozklad_wieku, "%d\t%f\n", v, simulationData.wiek[v]);
        fprintf(plik_rozklad_bitow, "%d\t%.2f\n", v, simulationData.bity[v]);
        if (simulationData.gompertz[v] > 0)
            fprintf(plik_gompertz, "%d\t%.3f\n", v, simulationData.gompertz[v]);
        else
            fprintf(plik_gompertz, "%d\t1\n", v);
    }
}

void Output::zapisz_kolejne(bool rodzina1, int rok, SimulationData& simulationData, int ilosc_osobnikow, int ilosc_narodzin, int ilosc_rodzin, int zgon,
                            int* rozklad_wieku, int* rozklad_bitow, int* gompertz_zgony)
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
    simulationData.stat[rok][0] += ilosc_osobnikow;
    simulationData.stat[rok][1] += ilosc_narodzin;
    simulationData.stat[rok][2] += ilosc_osobnikow - zgon;
    simulationData.stat[rok][3] += zgon;

    if (rok + 1 == maxPopulationAge_)
    {
        for (int v = 0; v < WIELKOSC * INT_W; v++)
        {
#ifdef CALE_WYJSCIE
            fprintf(plik_rozklad_wieku, "%d\t%d\n", v, rozklad_wieku[v]);
#endif
            simulationData.wiek[v] += rozklad_wieku[v];
        }

        for (int v = 0; v < WIELKOSC * INT_W; v++)
        {
#ifdef CALE_WYJSCIE
            fprintf(plik_rozklad_bitow, "%d\t%.2f\n", v, rozklad_bitow[v] * 1.0 / ilosc_osobnikow);
#endif
            simulationData.bity[v] += (float)rozklad_bitow[v] / (float)ilosc_osobnikow;
        }

        for (int v = 0; v < WIELKOSC * INT_W; v++)
            if (rozklad_wieku[v] > 0)
            {
#ifdef CALE_WYJSCIE
                fprintf(plik_gompertz, "%d\t%.3f\n", v,
                        gompertz_zgony[v] * 1.0 / rozklad_wieku[v]);
#endif
                simulationData.gompertz[v] +=
                    (float)gompertz_zgony[v] / (float)rozklad_wieku[v];
            }
            else
            {
#ifdef CALE_WYJSCIE
                fprintf(plik_gompertz, "%d\t1\n", v);
#endif
                simulationData.gompertz[v] += 1;
            }
    }
}

void Output::zapisz_losowana_populacje(Individual* populacja, int numer)
{
#ifdef CALE_WYJSCIE
    fprintf(plik_osobniki, "%u ", numer - 1);
    populacja[numer - 1].itob(plik_osobniki);
#endif
}

void Output::zapisz_koncowa_populacje(Individual* populacja, int x,
                                      unsigned int ostatni_el)
{
#ifdef CALE_WYJSCIE
    for (unsigned d = 0; d < ostatni_el; d++)
        if (populacja[d].przodek != -1)
        {
            fprintf(plik_osobniki, "%u %d %d %d %u ", d, populacja[d].przodek, populacja[d].wiek, populacja[d].ilosc_1, populacja[d].ciag[0]);
            populacja[d].itob(plik_osobniki);
        }
    zamknij_pliki(x);
#endif
}

void Output::przelicz_srednie_konwencjonalnie(float dzielnik,
                                              SimulationData& simulationData)
{
    for (int v = 0; v < WIELKOSC * INT_W; v++)
    {
        simulationData.gompertz[v] = simulationData.gompertz[v] / dzielnik;
        simulationData.bity[v] = simulationData.bity[v] / dzielnik;
        simulationData.wiek[v] = simulationData.wiek[v] / dzielnik;
    }

    for (int v = 0; v < maxPopulationAge_; v++)
        simulationData.rodziny[v] = simulationData.rodziny[v] / dzielnik;

    for (int v = 0; v < maxPopulationAge_; v++)
        for (int w = 0; w < 4; w++)
        {
            simulationData.stat[v][w] = simulationData.stat[v][w] / dzielnik;
        }
}
