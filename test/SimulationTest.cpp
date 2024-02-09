#include <iostream>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <src/FileOutput.h>
#include <src/Simulation.h>
#include <src/SimulationData.h>

#include "Common.h"
#include "MockedGenerator.h"
#include "StringOutput.h"

TEST_CASE("Simulation", "[penna]")
{
    SECTION("single simulation")
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = 1000;

        auto generator{std::make_shared<MockedGenerator>()};
        StringOutput output(0, config.years_, 1);

        Simulation simulation(config, 100);
        simulation.setGenerator(generator);
        simulation.createInitialPopulation();
        simulation.saveInitialPopulation(output);
        auto data{simulation.run()};

        simulation.saveFinalPopulation(output);
        output.saveSimulationData(data);

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
    SKIP();
    Config config;
    auto generator{std::make_shared<MockedGenerator>()};

    SECTION("simulation short")
    {
        // 02.02.2024 - 26.010 s

        config.maxPopulation_ = 100'000;
        config.years_ = 100'000;

        Simulation simulation(config, 100);
        simulation.setGenerator(generator);
        simulation.createInitialPopulation();
        simulation.run();

        std::cout << std::endl;
    }

    SECTION("simulation long")
    {
        // 02.02.2024 - 114.283 s

        config.maxPopulation_ = 200'000;
        config.years_ = 200'000;

        Simulation simulation(config, 100);
        simulation.setGenerator(generator);
        simulation.createInitialPopulation();
        simulation.run();

        std::cout << std::endl;
    }
}
