#include <iostream>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Common.h"
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

        const std::map<Output::OUTPUT_TYPE, std::string> files{
            {Output::DEATHS_DISTRIBUTION, "proces1_symulacja1_gompertz.txt"},
            {Output::INITIAL_POPULATION,
             "proces1_symulacja1_initialPopulation.txt"},
            {Output::FINAL_POPULATION,
             "proces1_symulacja1_finalPopulation.txt"},
            {Output::FAMILIES, "proces1_symulacja1_rodziny.txt"},
            {Output::BITS_DISTRIBUTION, "proces1_symulacja1_rozklad_bitow.txt"},
            {Output::AGE_DISTRIBUTION, "proces1_symulacja1_rozklad_wieku.txt"},
            {Output::STATISTICS, "proces1_symulacja1_statystyki.txt"}};

        for (const auto& [outputType, file] : files)
            Common::compareStringWithFileContent(
                output.getContentForOutputType(outputType), file);
    }
}

TEST_CASE("Benchmark", "[penna]")
{
    SKIP();
    Config config;
    MockedGenerator generator;

    SECTION("simulation short")
    {
        // 18.01.2024 - 205.679 s
        // 19.01.2024 - 83.274 s
        // 24.01.2024 - 108.660 s

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

        config.maxPopulation_ = 200'000;
        config.years_ = 200'000;

        NullOutput output;
        Simulation simulation(config, 1, 100);
        simulation.run(generator, output);

        std::cout << std::endl;
    }
}
