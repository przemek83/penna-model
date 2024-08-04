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
Simulation prepareSimulation(const config::Params& params, int simulationNumber,
                             long int seed)
{
    Simulation simulation(params);
    simulation.setGenerator(
        std::make_unique<NumbersGenerator>(config::Params::bits_, seed));
    simulation.createInitialPopulation();
    auto progressCallback{progress_callback::getOverallProgressCallback(
        simulationNumber, params)};
    simulation.setProgressCallback(progressCallback);
    return simulation;
}

long int getSeed()
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}

std::vector<SimulationData> runSimulations(const config::Params& params)
{
    const long int seed{getSeed()};
    Runner runner;
    for (int i{1}; i <= params.simulationsCount_; ++i)
        runner.addSimulation(prepareSimulation(params, i, seed + i));

    return runner.runSequential();
}

void saveOutput(const std::string& prefix, const AverageData& averages)
{
    FileOutput output(prefix);
    output.saveAverages(averages);
}

}  // namespace

int main(int argc, char* argv[])
{
    const auto [parsingOK, configFileName,
                prefix]{config::getAppArguments(argc, argv)};
    if (!parsingOK)
        return EXIT_FAILURE;

    const auto [paramsOK, params]{config::getParams(configFileName)};
    if (!paramsOK)
        return EXIT_FAILURE;

    const std::vector<SimulationData> simulationsData{runSimulations(params)};

    const AverageData averages{simulationsData,
                               static_cast<std::size_t>(params.years_),
                               config::Params::bits_};

    saveOutput(prefix, averages);

    return EXIT_SUCCESS;
}
