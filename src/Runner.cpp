#include "Runner.h"

#include <future>

#include "NumbersGenerator.h"
#include "ProgressBarOverall.h"
#include "Timer.h"

void Runner::prepareSimulations(const config::Params& params, long int seed)
{
    auto progressBar{std::make_shared<ProgressBarOverall>(
        params.years_, params.simulationsCount_)};
    for (int i{0}; i < params.simulationsCount_; ++i)
    {
        Simulation simulation(params, i);
        simulation.setGenerator(std::make_unique<NumbersGenerator>(
            config::Params::bits_, seed + i));
        simulation.createInitialPopulation();
        simulation.setProgressBar(progressBar);
        simulations_.emplace_back(std::move(simulation));
    }
}

std::vector<SimulationData> Runner::runSequential()
{
    const Timer timer;
    std::vector<SimulationData> dataToReturn;
    const std::size_t simCount{simulations_.size()};
    dataToReturn.reserve(simCount);

    for (std::size_t i{0}; i < simCount; ++i)
        dataToReturn.push_back(simulations_[i].run());

    return dataToReturn;
}

std::vector<SimulationData> Runner::runParallel()
{
    const Timer timer;
    std::vector<std::future<SimulationData>> futures;
    const std::size_t simCount{simulations_.size()};
    for (std::size_t i{0}; i < simCount; ++i)
        futures.emplace_back(std::async(std::launch::async,
                                        [&simulation = simulations_[i]]
                                        { return simulation.run(); }));

    std::vector<SimulationData> dataToReturn;
    for (auto& future : futures)
    {
        future.wait();
        dataToReturn.push_back(future.get());
    }

    return dataToReturn;
}
