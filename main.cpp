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

    SimulationAverages averages{prepareSimulationData<float>(config.years_)};

    NumbersGenerator generator;

    for (int i{1}; i <= config.simulationsCount_; i++)
    {
        const Timer timer;
        Simulation simulation(config, i, krok_symulacji);
        SingleSimulationData data{simulation.run(generator)};
        integrateData(averages, data);
    }

    Output output(krok_symulacji, config.years_, 0);
    prepareFinalResults(config.simulationsCount_, config.years_, averages);
    output.saveAverages(averages);

    return EXIT_SUCCESS;
}
