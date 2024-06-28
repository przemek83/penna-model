#include <iostream>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <src/FileOutput.h>
#include <src/Generator.h>
#include <src/Simulation.h>
#include <src/SimulationData.h>

#include "Common.h"
#include "StringOutput.h"

TEST_CASE("Simulation", "[penna]")
{
    SECTION("single simulation")
    {
        Config::Params params;
        params.population_.max_ = 5000;
        params.years_ = 1000;

        StringOutput output;

        Simulation simulation(params);
        simulation.setGenerator(Common::getTestGenerator(params.bits_));
        simulation.createInitialPopulation();
        simulation.saveInitialPopulation(output);
        auto data{simulation.run()};

        simulation.saveFinalPopulation(output);
        output.saveSimulationData(data);

        std::cout << std::endl;

        const std::vector<OUTPUT_TYPE> outputTypes{
            DEATHS_DISTRIBUTION, INITIAL_POPULATION, FINAL_POPULATION, FAMILIES,
            BITS_DISTRIBUTION,   AGE_DISTRIBUTION,   STATISTICS};

        const FileOutput fileOutput("sim1");
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
    Config::Params params;

    SECTION("simulation short")
    {
        // 02.02.2024 - 26.010 s

        params.population_.max_ = 100'000;
        params.years_ = 100'000;

        Simulation simulation(params);
        simulation.setGenerator(Common::getTestGenerator(params.bits_));
        simulation.createInitialPopulation();
        simulation.run();

        std::cout << std::endl;
    }

    SECTION("simulation long")
    {
        // 02.02.2024 - 114.283 s

        params.population_.max_ = 200'000;
        params.years_ = 200'000;

        Simulation simulation(params);
        simulation.setGenerator(Common::getTestGenerator(params.bits_));
        simulation.createInitialPopulation();
        simulation.run();

        std::cout << std::endl;
    }
}
