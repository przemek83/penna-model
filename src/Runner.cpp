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
    for (std::size_t i{0}; i < simulations_.size(); ++i)
    {
        SimulationData data{simulations_[i].run()};
        dataToReturn.emplace_back(std::move(data));
    }

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
        SimulationData data{future.get()};
        dataToReturn.emplace_back(std::move(data));
    }

    return dataToReturn;
}
