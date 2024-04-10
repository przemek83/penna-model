#include <fstream>

#include "FileOutput.h"
#include "NumbersGenerator.h"
#include "ProgressCallback.h"
#include "Runner.h"
#include "Simulation.h"
#include "SimulationData.h"

namespace
{
Simulation prepareSimulation(const Config::Params& params, int simulationNumber,
                             std::shared_ptr<NumbersGenerator> generator)
{
    Simulation simulation(params);
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
    FileOutput output(params.years_);
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
