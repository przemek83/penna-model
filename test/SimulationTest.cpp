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
    const int simId{0};

    SECTION("single simulation")
    {
        config::Params params;
        params.population_.max_ = 5'000;
        params.years_ = 1'000;

        StringOutput output;

        Simulation simulation(params, simId);
        simulation.setGenerator(
            common::getTestGenerator(config::Params::bits_));
        simulation.createInitialPopulation();
        simulation.saveInitialPopulation(output);
        auto data{simulation.run()};

        simulation.saveFinalPopulation(output);
        output.saveSimulationData(data);

        const std::vector<OutputType> outputTypes{
            OutputType::DEATHS_DISTRIBUTION, OutputType::INITIAL_POPULATION,
            OutputType::FINAL_POPULATION,    OutputType::FAMILIES,
            OutputType::BITS_DISTRIBUTION,   OutputType::AGE_DISTRIBUTION,
            OutputType::STATISTICS};

        const FileOutput fileOutput("sim1");
        for (const auto outputType : outputTypes)
        {
            CAPTURE(outputType);
            common::compareStringWithFileContent(
                output.getContentForOutputType(outputType),
                fileOutput.getName(outputType));
        }
    }
}

TEST_CASE("Benchmark", "[penna]")
{
    SKIP();
    config::Params params;
    const int simId{0};

    SECTION("simulation short")
    {
        // 02.02.2024 - 26.010 s
        // 04.07.2024 - 16.734 s

        params.population_.max_ = 100'000;
        params.years_ = 100'000;

        Simulation simulation(params, simId);
        simulation.setGenerator(
            common::getTestGenerator(config::Params::bits_));
        simulation.createInitialPopulation();
        simulation.run();
    }

    SECTION("simulation long")
    {
        // 02.02.2024 - 114.283 s
        // 04.07.2024 - 84.883 s

        params.population_.max_ = 200'000;
        params.years_ = 200'000;

        Simulation simulation(params, simId);
        simulation.setGenerator(
            common::getTestGenerator(config::Params::bits_));
        simulation.createInitialPopulation();
        simulation.run();
    }
}
