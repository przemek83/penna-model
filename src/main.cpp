#include <fstream>
#include <iostream>
#include <mutex>

#include "FileOutput.h"
#include "NumbersGenerator.h"
#include "Runner.h"
#include "Simulation.h"
#include "SimulationData.h"

namespace
{

std::vector<int> getProgressVector(int simulations)
{
    std::vector<int> progresses(static_cast<std::size_t>(simulations), 0);
    return progresses;
}

const int progressLineLength{50};
const char progressLinePreffix{'['};
const char progressLineSuffix{']'};
const char progressLineMarker{'*'};

[[maybe_unused]] std::function<void(int)> createSequentialProgressCallback(
    int sim, const Config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            maxSim = params.simulationsCount_](int year)
    {
        if (year == 0)
            std::cout << simNumber << "/" << maxSim << " "
                      << progressLinePreffix;

        if ((year % (maxYears / progressLineLength)) == 0)
            std::cout << progressLineMarker;

        if (year + 1 == maxYears)
            std::cout << progressLineSuffix << std::endl;
    };
}

[[maybe_unused]] std::function<void(int)> createOverallProgressCallback(
    int sim, const Config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            simCount = params.simulationsCount_](int year)
    {
        static std::vector<int> progresses{getProgressVector(simCount + 1)};

        const int sensitivity{maxYears / 100};
        if ((year + 1) % sensitivity != 0)
            return;

        static std::mutex mutex;
        mutex.lock();
        progresses[static_cast<std::size_t>(simNumber)] =
            (year + 1) / sensitivity;
        const int currentSum{std::reduce(progresses.begin(), progresses.end())};

        const int totalSum{simCount * 100};
        if (currentSum == 1)
            std::cout << progressLinePreffix;
        if (currentSum % (totalSum / progressLineLength) == 0)
            std::cout << progressLineMarker;
        if (currentSum >= totalSum)
            std::cout << progressLineSuffix << std::endl;

        mutex.unlock();
    };
}

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
    auto progressCallback{
        createOverallProgressCallback(simulationNumber, params)};
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
