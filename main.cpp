#include "NumbersGenerator.h"
#include "Output.h"
#include "Simulation.h"
#include "SimulationData.h"
#include "Timer.h"

int main()
{
    const Config config;
    const float krok_symulacji{
#ifdef SYMULACJA_DORSZY
        static_cast<float>(abs(START_ODLOWOW - KONIEC_ODLOWOW))
#else
        100
#endif
        / static_cast<float>(config.simulationsCount_)};

    SimulationAverages averages{prepareSimulationAverages(config.years_)};

    Output output(krok_symulacji, config.years_);

    NumbersGenerator generator;

    for (int i{1}; i <= config.simulationsCount_; i++)
    {
        const Timer timer;
        Simulation simulation(config, i, krok_symulacji);
        simulation.run(output, generator, averages);
    }

    output.otworz_pliki(0);
    prepareFinalResults(config.simulationsCount_, config.years_, averages);
    output.saveAvgs(averages);
    output.zamknij_pliki(0);

    return EXIT_SUCCESS;
}
