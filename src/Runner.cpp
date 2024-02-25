#include "Runner.h"

#include "Timer.h"

void Runner::addSimulation(Simulation simulation)
{
    simulations_.emplace_back(std::move(simulation));
}

std::vector<SingleSimulationData> Runner::runSequential()
{
    std::vector<SingleSimulationData> dataToReturn;
    for (std::size_t i{0}; i < simulations_.size(); ++i)
    {
        const Timer timer;
        SingleSimulationData data{simulations_[i].run()};
        dataToReturn.emplace_back(std::move(data));
    }

    return dataToReturn;
}
