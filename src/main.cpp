#include <fstream>

#include "FileOutput.h"
#include "NumbersGenerator.h"
#include "ProgressCallback.h"
#include "Runner.h"
#include "Simulation.h"
#include "SimulationData.h"

namespace
{
float getStep(const Config::Params& params)
{
    const float step{
#ifdef SYMULACJA_DORSZY
        static_cast<float>(abs(START_ODLOWOW - KONIEC_ODLOWOW))
#else
        100
#endif
        / static_cast<float>(params.simulationsCount_)};

    return step;
}

Simulation prepareSimulation(const Config::Params& params, int simulationNumber,
                             std::shared_ptr<NumbersGenerator> generator)
{
    const float step{getStep(params)};

    Simulation simulation(params, step);
    simulation.setGenerator(generator);
    simulation.createInitialPopulation();
    simulation.setGenerator(std::make_shared<NumbersGenerator>());
    auto progressCallback{ProgressCallback::createOverallProgressCallback(
        simulationNumber, params)};
    simulation.setProgressCallback(progressCallback);
    return simulation;
}

SimulationAverages calculateAverages(
    const std::vector<SingleSimulationData>& simulationsData, int years)
{
    SimulationAverages averages{static_cast<std::size_t>(years)};
    for (const auto& data : simulationsData)
        averages.integrateData(data);

    averages.finalize();
    return averages;
}

void saveAverages(const SimulationAverages& averages,
                  const Config::Params& params)
{
    const float step{getStep(params)};
    FileOutput output(step, params.years_, 0);
    output.saveAverages(averages);
}

}  // namespace

int main()
{
    std::ifstream configFile("config.yaml");
    const Config::Params params{Config::loadConfig(configFile)};

    if (!Config::isValid(params))
        return EXIT_FAILURE;

    Runner runner;
    auto generator{std::make_shared<NumbersGenerator>()};

    for (int i{1}; i <= params.simulationsCount_; i++)
        runner.addSimulation(prepareSimulation(params, i, generator));

    const std::vector<SingleSimulationData> simulationsData{
        runner.runParallel()};

    const SimulationAverages averages{
        calculateAverages(simulationsData, params.years_)};

    saveAverages(averages, params);

    return EXIT_SUCCESS;
}
