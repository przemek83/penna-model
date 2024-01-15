#include "Common.h"
#include "Individual.h"
#include "NumbersGenerator.h"
#include "Output.h"
#include "Simulation.h"
#include "SimulationData.h"

int main(int argc, char* argv[])
{
    float krok_symulacji =  // wielkosc kroku symulacji
        abs(START_ODLOWOW - KONIEC_ODLOWOW) / (float)SYMULACJI_NA_PROCES;

    Individual populacja[MAX_POP];

    SimulationData simulationDataAvg;
    Output output(krok_symulacji, MAX_POP_LAT);

    NumbersGenerator generator;

    for (int o = 1; o <= SYMULACJI_NA_PROCES + 1; o++)  // start kolejne symulacje
    {
        if (o == SYMULACJI_NA_PROCES + 1)
            output.otworz_pliki(0);
        else
        {
            if (o != SYMULACJI_NA_PROCES + 1)
                output.otworz_pliki2(o);
        }

        clock_t start{clock()};
        Simulation simulation(o, krok_symulacji);
        simulation.run(output, generator, simulationDataAvg);

        output.zapisz_srednie(SYMULACJI_NA_PROCES, simulationDataAvg);
        output.zamknij_pliki(SYMULACJI_NA_PROCES + 1 - o);

        clock_t koniec{clock()};
        if (o != SYMULACJI_NA_PROCES + 1)
        {
            printf(
                "\nCzas wykonania: %d godzin, %d minut %d "
                "sekund\n",
                (koniec - start) / (1000 * 60 * 60), ((koniec - start) % (1000 * 60 * 60)) / (1000 * 60),
                (((koniec - start) % (1000 * 60 * 60)) % (1000 * 60)) / 1000);
        }

    }  // kolejne symulacje

    return 0;
}
//============================================================//
