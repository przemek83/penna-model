#include "AverageData.h"
#include "FileOutput.h"
#include "ProgressBarOverall.h"
#include "Runner.h"
#include "SimulationData.h"

namespace
{
long int getSeed()
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}

void saveOutput(const std::string& prefix, const AverageData& averages)
{
    FileOutput output(prefix);
    output.saveAverages(averages);
}
}  // namespace

int main(int argc, const char* argv[])
{
    const auto [parsingOK, configFileName,
                prefix]{config::getAppArguments(argc, argv)};
    if (!parsingOK)
        return EXIT_FAILURE;

    const auto [paramsOK, params]{config::getParams(configFileName)};
    if (!paramsOK)
        return EXIT_FAILURE;

    Runner runner;
    runner.prepareSimulations(params, getSeed());
    const std::vector<SimulationData> simulationsData{runner.runParallel()};

    const AverageData averages{simulationsData,
                               static_cast<std::size_t>(params.years_),
                               config::Params::bits_};

    saveOutput(prefix, averages);

    return EXIT_SUCCESS;
}
