#include <filesystem>
#include <fstream>
#include <iostream>

#include <argparse/argparse.hpp>

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
    simulation.setGenerator(std::make_shared<NumbersGenerator>());
    auto progressCallback{ProgressCallback::createOverallProgressCallback(
        simulationNumber, params)};
    simulation.setProgressCallback(progressCallback);
    return simulation;
}

AverageData calculateAverages(
    const std::vector<SimulationData>& simulationsData, int years)
{
    AverageData averages{static_cast<std::size_t>(years)};
    for (const auto& data : simulationsData)
        averages.integrateData(data);

    averages.finalize();
    return averages;
}

void saveAverages(const AverageData& averages, const Config::Params& params)
{
    FileOutput output(params.years_);
    output.saveAverages(averages);
}

[[noreturn]] void exitWithMsg(const std::exception& e,
                              const argparse::ArgumentParser& parser)
{
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    abort();
}

void fillParser(argparse::ArgumentParser& parser)
{
    parser.add_argument("config")
        .default_value(std::string("config.yaml"))
        .help("name of config file to use.");
    parser.add_description(
        "Implementation of Penna model of population aging.");
    parser.add_epilog(
        "After simulation one can use script for graph generation using "
        "result files.");
}

std::string getConfigFileName(int argc, char* argv[])
{
    argparse::ArgumentParser parser("penna", "1.0",
                                    argparse::default_arguments::help);
    fillParser(parser);

    std::string configFileName;
    try
    {
        parser.parse_args(argc, argv);
        configFileName = parser.get<std::string>("config");
    }
    catch (const std::exception& e)
    {
        exitWithMsg(e, parser);
    }

    return configFileName;
}

Config::Params getParams(const std::string& configFileName)
{
    std::ifstream configFileStream(configFileName);
    if (configFileStream.fail())
    {
        std::cerr << "Cannot read config file " + configFileName << std::endl;
        abort();
    }

    const Config::Params params{Config::loadConfig(configFileStream)};

    if (!Config::isValid(params))
        abort();

    return params;
}

}  // namespace

int main(int argc, char* argv[])
{
    const std::string configFileName{getConfigFileName(argc, argv)};
    const Config::Params params{getParams(configFileName)};

    Runner runner;
    auto generator{std::make_shared<NumbersGenerator>()};

    for (int i{1}; i <= params.simulationsCount_; i++)
        runner.addSimulation(prepareSimulation(params, i, generator));

    const std::vector<SimulationData> simulationsData{runner.runParallel()};

    const AverageData averages{
        calculateAverages(simulationsData, params.years_)};

    saveAverages(averages, params);

    return EXIT_SUCCESS;
}
