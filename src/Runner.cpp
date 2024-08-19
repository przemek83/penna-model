#include "Runner.h"

#include <future>

#include "NumbersGenerator.h"
// #include "ProgressBarSequential.h"
#include "Timer.h"

void Runner::prepareSimulations(const config::Params& params, long int seed)
{
    //    auto progressBar{std::make_shared<ProgressBarSequential>(
    //       params.years_, params.simulationsCount_)};
    for (int i{0}; i < params.simulationsCount_; ++i)
    {
        Simulation simulation(params, i);
        simulation.setGenerator(std::make_unique<NumbersGenerator>(
            config::Params::bits_, seed + i));
        simulation.createInitialPopulation();
        //        simulation.setProgressBar(progressBar);
        simulations_.emplace_back(std::move(simulation));
    }
}

std::vector<SimResults> Runner::runSequential()
{
    const Timer timer;
    std::vector<SimResults> dataToReturn;
    const std::size_t simCount{simulations_.size()};
    dataToReturn.reserve(simCount);

    for (std::size_t i{0}; i < simCount; ++i)
        dataToReturn.push_back(simulations_[i].run());

    return dataToReturn;
}

std::vector<SimResults> Runner::runParallel()
{
    const Timer timer;
    std::vector<std::future<SimResults>> futures;
    const std::size_t simCount{simulations_.size()};
    for (std::size_t i{0}; i < simCount; ++i)
    {
        const auto operation{[&sim = simulations_[i]] { return sim.run(); }};
        futures.emplace_back(std::async(operation));
    }

    std::vector<SimResults> dataToReturn;
    for (auto& future : futures)
    {
        future.wait();
        dataToReturn.push_back(future.get());
    }

    return dataToReturn;
}
