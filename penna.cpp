#include <iostream>

#include "Common.h"
#include "NumbersGenerator.h"
#include "Output.h"
#include "Simulation.h"
#include "SimulationData.h"

int main()
{
    Config config;
    const float krok_symulacji =
#ifdef SYMULACJA_DORSZY
        static_cast<float>(abs(START_ODLOWOW - KONIEC_ODLOWOW))
#else
        100
#endif
        / static_cast<float>(config.simulationsCount_);

    SimulationData simulationDataAvg{};
    simulationDataAvg.rodziny.resize(static_cast<size_t>(config.years_));
    simulationDataAvg.livingAtStart_.resize(static_cast<size_t>(config.years_));
    simulationDataAvg.births_.resize(static_cast<size_t>(config.years_));
    simulationDataAvg.livingAtEnd_.resize(static_cast<size_t>(config.years_));
    simulationDataAvg.deaths_.resize(static_cast<size_t>(config.years_));

    Output output(krok_symulacji, config.years_);

    NumbersGenerator generator;

    for (int i = 1; i <= config.simulationsCount_; i++)
    {
        output.otworz_pliki2(i);

        const clock_t start{clock()};
        Simulation simulation(config, i, krok_symulacji);
        simulation.run(output, generator, simulationDataAvg);

        const clock_t koniec{clock()};

        std::cout << std::endl
                  << "Execution time: " << (koniec - start) / (1000 * 60 * 60)
                  << "h, "
                  << ((koniec - start) % (1000 * 60 * 60)) / (1000 * 60)
                  << "m, "
                  << (((koniec - start) % (1000 * 60 * 60)) % (1000 * 60)) /
                         1000
                  << "s." << std::endl;
    }

    output.otworz_pliki(0);
    output.zapisz_srednie(config.simulationsCount_, simulationDataAvg);
    output.zamknij_pliki(0);

    return EXIT_SUCCESS;
}
//============================================================//
