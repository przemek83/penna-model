#include <filesystem>

#include "AverageData.h"
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
    simulation.setGenerator(std::make_shared<NumbersGenerator>(params.bits_));
    auto progressCallback{ProgressCallback::createOverallProgressCallback(
        simulationNumber, params)};
    simulation.setProgressCallback(progressCallback);
    return simulation;
}

}  // namespace

int main(int argc, char* argv[])
{
    const std::string configFileName{Config::getConfigFileName(argc, argv)};
    const Config::Params params{Config::getParams(configFileName)};

    Runner runner;
    auto generator{std::make_shared<NumbersGenerator>(params.bits_)};

    for (int i{1}; i <= params.simulationsCount_; i++)
        runner.addSimulation(prepareSimulation(params, i, generator));

    const std::vector<SimulationData> simulationsData{runner.runParallel()};

    const AverageData averages{
        simulationsData, static_cast<std::size_t>(params.years_), params.bits_};

    FileOutput output("averages");
    output.saveAverages(averages);

    return EXIT_SUCCESS;
}
