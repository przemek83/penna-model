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
                             long int seed)
{
    Simulation simulation(params);
    simulation.setGenerator(
        std::make_unique<NumbersGenerator>(params.bits_, seed));
    simulation.createInitialPopulation();
    auto progressCallback{
        ProgressCallback::getOverallProgressCallback(simulationNumber, params)};
    simulation.setProgressCallback(progressCallback);
    return simulation;
}

long int getSeed()
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}

}  // namespace

int main(int argc, char* argv[])
{
    const auto [parsingOK, configFileName,
                prefix]{Config::getAppArguments(argc, argv)};
    if (!parsingOK)
        return EXIT_FAILURE;

    const auto [paramsOK, params]{Config::getParams(configFileName)};
    if (!paramsOK)
        return EXIT_FAILURE;

    const long int seed{getSeed()};
    Runner runner;
    for (int i{1}; i <= params.simulationsCount_; ++i)
        runner.addSimulation(prepareSimulation(params, i, seed + i));

    const std::vector<SimulationData> simulationsData{runner.runParallel()};

    const AverageData averages{
        simulationsData, static_cast<std::size_t>(params.years_), params.bits_};

    FileOutput output(prefix);
    output.saveAverages(averages);

    return EXIT_SUCCESS;
}
