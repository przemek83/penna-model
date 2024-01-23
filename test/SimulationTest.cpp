#include <fstream>
#include <iostream>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers_string.hpp"

#include "MockedGenerator.h"
#include "Simulation.h"
#include "SimulationData.h"

namespace
{
std::string getFileContent(const std::string& file)
{
    std::ifstream ifstream(file);
    std::string content{(std::istreambuf_iterator<char>(ifstream)),
                        (std::istreambuf_iterator<char>())};
    return content;
}

void compareFiles(const std::string& currentFile,
                  const std::string& expectedFile)
{
    const std::string currentContent{getFileContent(currentFile)};
    const std::string expectedContent{getFileContent(expectedFile)};
    CAPTURE(currentFile);
    REQUIRE_THAT(currentContent, Catch::Matchers::Equals(expectedContent));
}

};  // namespace

TEST_CASE("Simulation", "[penna]")
{
    SECTION("single simulation")
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = 1000;

        SimulationAverages simulationAverages{
            prepareSimulationAverages(config.years_)};

        MockedGenerator generator;
        Simulation simulation(config, 1, 100);
        simulation.run(generator, simulationAverages);
        std::cout << std::endl;

        std::vector<std::string> files{
            "proces1_symulacja1_gompertz.txt",
            "proces1_symulacja1_initialPopulation.txt",
            "proces1_symulacja1_finalPopulation.txt",
            "proces1_symulacja1_rodziny.txt",
            "proces1_symulacja1_rozklad_bitow.txt",
            "proces1_symulacja1_rozklad_wieku.txt",
            "proces1_symulacja1_statystyki.txt"};

        for (const auto& file : files)
            compareFiles(file, "TestFiles/" + file);
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

        config.maxPopulation_ = 100'000;
        config.years_ = 100'000;

        SimulationAverages simulationAverages{
            prepareSimulationAverages(config.years_)};

        Simulation simulation(config, 1, 100);
        simulation.run(generator, simulationAverages);

        std::cout << std::endl;
    }

    SECTION("simulation long")
    {
        // 18.01.2024 - 846.279 s
        // 19.01.2024 - 333.807 s

        config.maxPopulation_ = 200'000;
        config.years_ = 200'000;

        SimulationAverages simulationAverages{
            prepareSimulationAverages(config.years_)};

        Simulation simulation(config, 1, 100);
        simulation.run(generator, simulationAverages);

        std::cout << std::endl;
    }
}
