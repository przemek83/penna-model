#include "Runner.h"
#include <future>

#include "Timer.h"

void Runner::addSimulation(Simulation simulation)
{
    simulations_.emplace_back(std::move(simulation));
}

std::vector<SimulationData> Runner::runSequential()
{
    const Timer timer;
    std::vector<SimulationData> dataToReturn;
    dataToReturn.reserve(simulations_.size());

    for (std::size_t i{0}; i < simulations_.size(); ++i)
        dataToReturn.push_back(simulations_[i].run());

    return dataToReturn;
}

std::vector<SimulationData> Runner::runParallel()
{
    const Timer timer;
    std::vector<std::future<SimulationData>> futures;
    for (std::size_t i{0}; i < simulations_.size(); ++i)
    {
        futures.emplace_back(
            std::async(std::launch::async, &Simulation::run, simulations_[i]));
    }

    std::vector<SimulationData> dataToReturn;
    for (auto& future : futures)
    {
        future.wait();
        dataToReturn.push_back(future.get());
    }

    return dataToReturn;
}
