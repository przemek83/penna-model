#include "FileOutput.h"
#include "NumbersGenerator.h"
#include "Simulation.h"
#include "SimulationData.h"
#include "Timer.h"

int main()
{
    const Config config;
    const float step{
#ifdef SYMULACJA_DORSZY
        static_cast<float>(abs(START_ODLOWOW - KONIEC_ODLOWOW))
#else
        100
#endif
        / static_cast<float>(config.simulationsCount_)};

    SimulationAverages averages{static_cast<std::size_t>(config.years_)};

    NumbersGenerator generator;
    for (int i{1}; i <= config.simulationsCount_; i++)
    {
        const Timer timer;
        Simulation simulation(config, i, step);
        FileOutput output(step, config.years_, i);
        const SingleSimulationData data{simulation.run(generator, output)};
        averages.integrateData(data);
    }

    averages.finalize();

    FileOutput output(step, config.years_, 0);
    output.saveAverages(averages);

    return EXIT_SUCCESS;
}
