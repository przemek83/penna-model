#include <iostream>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Common.h"
#include "FileOutput.h"
#include "MockedGenerator.h"
#include "NullOutput.h"
#include "Simulation.h"
#include "SimulationData.h"
#include "StringOutput.h"

TEST_CASE("Simulation", "[penna]")
{
    SECTION("single simulation")
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = 1000;

        MockedGenerator generator;
        StringOutput output(0, config.years_, 1);

        Simulation simulation(config, 1, 100);
        simulation.run(generator, output);
        std::cout << std::endl;

        const std::vector<Output::OUTPUT_TYPE> outputTypes{
            Output::DEATHS_DISTRIBUTION, Output::INITIAL_POPULATION,
            Output::FINAL_POPULATION,    Output::FAMILIES,
            Output::BITS_DISTRIBUTION,   Output::AGE_DISTRIBUTION,
            Output::STATISTICS};

        const FileOutput fileOutput(0, config.years_, 1);
        for (const auto outputType : outputTypes)
        {
            CAPTURE(outputType);
            Common::compareStringWithFileContent(
                output.getContentForOutputType(outputType),
                fileOutput.getName(outputType));
        }
    }
}

TEST_CASE("Benchmark", "[penna]")
{
    //    SKIP();
    Config config;
    MockedGenerator generator;

    SECTION("simulation short")
    {
        // 18.01.2024 - 205.679 s
        // 19.01.2024 - 83.274 s
        // 24.01.2024 - 108.660 s
        // 30.01.2024 - 105.830 s

        config.maxPopulation_ = 100'000;
        config.years_ = 100'000;

        NullOutput output;
        Simulation simulation(config, 1, 100);
        simulation.run(generator, output);

        std::cout << std::endl;
    }

    SECTION("simulation long")
    {
        // 18.01.2024 - 846.279 s
        // 19.01.2024 - 333.807 s
        // 24.01.2024 - 430.467 s
        // 30.01.2024 - 417.025 s

        config.maxPopulation_ = 200'000;
        config.years_ = 200'000;

        NullOutput output;
        Simulation simulation(config, 1, 100);
        simulation.run(generator, output);

        std::cout << std::endl;
    }
}
