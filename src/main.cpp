#include <chrono>
#include <fstream>
#include <memory>

#include "AvgResults.h"
#include "FileOutput.h"
#include "Runner.h"
#include "SimResults.h"

namespace
{
long int getSeed()
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}

void saveOutput(const std::string& prefix, const AvgResults& averages)
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

    auto configStream{std::make_unique<std::ifstream>(configFileName)};
    const auto [paramsOK, params]{
        config::getParams(configFileName, std::move(configStream))};
    if (!paramsOK)
        return EXIT_FAILURE;

    Runner runner;
    runner.prepareSimulations(params, getSeed());
    const std::vector<SimResults> simResults{runner.runParallel()};

    const AvgResults averages{simResults,
                              static_cast<std::size_t>(params.years_),
                              config::Params::bits_};

    saveOutput(prefix, averages);

    return EXIT_SUCCESS;
}
