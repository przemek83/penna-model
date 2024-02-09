#include <iostream>

#include "FileOutput.h"
#include "NumbersGenerator.h"
#include "Simulation.h"
#include "SimulationData.h"
#include "Timer.h"

namespace
{
std::function<void(int)> createProgressCallback(int sim, const Config& config)
{
    return [maxYears = config.years_, simNumber = sim,
            maxSim = config.simulationsCount_](std::size_t year)
    {
        if (year == 1)
            std::cout << simNumber << "/" << maxSim << " Progress:       [";

        if ((year % (maxYears / 50)) == 0)
            std::cout << "*";

        if (year == maxYears)
            std::cout << "]";
    };
}
}  // namespace

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

    auto initialPopulationGenerator{std::make_shared<NumbersGenerator>()};
    for (int i{1}; i <= config.simulationsCount_; i++)
    {
        const Timer timer;
        Simulation simulation(config, step);
        simulation.setGenerator(initialPopulationGenerator);
        simulation.createInitialPopulation();
        auto progressCallback{createProgressCallback(i, config)};
        const SingleSimulationData data{simulation.run(progressCallback)};
        averages.integrateData(data);
    }

    averages.finalize();

    FileOutput output(step, config.years_, 0);
    output.saveAverages(averages);

    return EXIT_SUCCESS;
}
