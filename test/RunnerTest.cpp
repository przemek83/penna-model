#include <cpputils/StreamEater.h>
#include <catch2/catch_test_macros.hpp>

#include <src/Generator.h>
#include <src/Runner.h>

TEST_CASE("Runner - Sequential Run")
{
    StreamEater eater(std::cout);

    config::Params params;
    params.years_ = 0;
    Runner runner;
    runner.prepareSimulations(params, 0);

    std::vector<SimResults> result{runner.runSequential()};

    REQUIRE(result.size() ==
            static_cast<std::size_t>(params.simulationsCount_));
}

TEST_CASE("Runner - Parallel Run")
{
    StreamEater eater(std::cout);

    config::Params params;
    params.years_ = 0;
    Runner runner;
    runner.prepareSimulations(params, 0);

    std::vector<SimResults> result{runner.runParallel()};

    REQUIRE(result.size() ==
            static_cast<std::size_t>(params.simulationsCount_));
}
