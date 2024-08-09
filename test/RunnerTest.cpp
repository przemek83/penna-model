#include <catch2/catch_test_macros.hpp>

#include <src/Generator.h>
#include <src/Runner.h>

TEST_CASE("Runner - Sequential Run")
{
    config::Params params;
    params.years_ = 0;
    Runner runner;
    Simulation simulation1(params, 0);
    Simulation simulation2(params, 1);
    runner.addSimulation(std::move(simulation1));
    runner.addSimulation(std::move(simulation2));

    std::vector<SimulationData> result{runner.runSequential()};

    REQUIRE(result.size() ==
            static_cast<std::size_t>(params.simulationsCount_));
}

TEST_CASE("Runner - Parallel Run")
{
    config::Params params;
    params.years_ = 0;
    Runner runner;
    Simulation simulation1(params, 0);
    Simulation simulation2(params, 1);
    runner.addSimulation(std::move(simulation1));
    runner.addSimulation(std::move(simulation2));

    std::vector<SimulationData> result{runner.runParallel()};

    REQUIRE(result.size() ==
            static_cast<std::size_t>(params.simulationsCount_));
}
