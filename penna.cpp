#include "Common.h"
#include "NumbersGenerator.h"
#include "Output.h"
#include "Simulation.h"
#include "SimulationData.h"

int main()
{
    const float krok_symulacji =  // wielkosc kroku symulacji
        abs(START_ODLOWOW - KONIEC_ODLOWOW) /
        static_cast<float>(SYMULACJI_NA_PROCES);

    SimulationData simulationDataAvg;
    Output output(krok_symulacji, MAX_POP_LAT);

    NumbersGenerator generator;

    for (int o = 1; o <= SYMULACJI_NA_PROCES + 1;
         o++)  // start kolejne symulacje
    {
        if (o == SYMULACJI_NA_PROCES + 1)
            output.otworz_pliki(0);
        else
        {
            if (o != SYMULACJI_NA_PROCES + 1)
                output.otworz_pliki2(o);
        }

        const clock_t start{clock()};
        Simulation simulation(o, krok_symulacji);
        simulation.run(output, generator, simulationDataAvg);

        output.zapisz_srednie(SYMULACJI_NA_PROCES, simulationDataAvg);
        output.zamknij_pliki(SYMULACJI_NA_PROCES + 1 - o);

        const clock_t koniec{clock()};
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
