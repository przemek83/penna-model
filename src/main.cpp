#include <fstream>
#include <iostream>

#include "FileOutput.h"
#include "NumbersGenerator.h"
#include "Simulation.h"
#include "SimulationData.h"
#include "Timer.h"

namespace
{
std::function<void(int)> createProgressCallback(int sim,
                                                const Config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            maxSim = params.simulationsCount_](int year)
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
    std::ifstream configFile("config.yaml");
    const Config::Params params{Config::loadConfig(configFile)};

    if (!Config::isValid(params))
        return EXIT_FAILURE;

    const float step{
#ifdef SYMULACJA_DORSZY
        static_cast<float>(abs(START_ODLOWOW - KONIEC_ODLOWOW))
#else
        100
#endif
        / static_cast<float>(params.simulationsCount_)};

    SimulationAverages averages{static_cast<std::size_t>(params.years_)};

    auto initialPopulationGenerator{std::make_shared<NumbersGenerator>()};
    std::vector<Simulation> simulations;
    for (int i{1}; i <= params.simulationsCount_; i++)
    {
        Simulation& simulation{simulations.emplace_back(params, step)};
        simulation.setGenerator(initialPopulationGenerator);
        simulation.createInitialPopulation();
    }

    for (std::size_t i{0}; i < simulations.size(); ++i)
    {
        const Timer timer;
        auto progressCallback{createProgressCallback(i + 1, params)};
        const SingleSimulationData data{simulations[i].run(progressCallback)};
        averages.integrateData(data);
    }

    averages.finalize();

    FileOutput output(step, params.years_, 0);
    output.saveAverages(averages);

    return EXIT_SUCCESS;
}
