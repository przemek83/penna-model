#include <fstream>
#include <iostream>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Generator.h"
#include "Output.h"
#include "Simulation.h"
#include "SimulationData.h"
#include "catch2/matchers/catch_matchers_string.hpp"

namespace
{
class MockedGenerator : public Generator
{
public:
    int getInt(int low, int high) override
    {
        // Following SplitMix64 algorithm described in
        // https://thompsonsed.co.uk/random-number-generators-for-c-performance-tested
        uint64_t z = (x_ += UINT64_C(0x9E3779B97F4A7C15));
        z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
        return low + ((z ^ (z >> 31)) % (high - low + 1));
    }

private:
    uint64_t x_{12};
};

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

TEST_CASE("E2E", "[penna]")
{
    SECTION("single simulation")
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = 1000;
        Output output(100, config.years_);
        output.otworz_pliki2(1);

        SimulationData simulationDataAvg{};
        simulationDataAvg.rodziny.resize(static_cast<size_t>(config.years_));
        simulationDataAvg.livingAtStart_.resize(
            static_cast<size_t>(config.years_));
        simulationDataAvg.births_.resize(static_cast<size_t>(config.years_));
        simulationDataAvg.livingAtEnd_.resize(
            static_cast<size_t>(config.years_));
        simulationDataAvg.deaths_.resize(static_cast<size_t>(config.years_));

        MockedGenerator generator;
        Simulation simulation(config, 0, 100);
        simulation.run(output, generator, simulationDataAvg);

        std::vector<std::string> files{"proces1_symulacja1_gompertz.txt",
                                       "proces1_symulacja1_osobniki.txt",
                                       "proces1_symulacja1_rodziny.txt",
                                       "proces1_symulacja1_rozklad_bitow.txt",
                                       "proces1_symulacja1_rozklad_wieku.txt",
                                       "proces1_symulacja1_statystyki.txt"};
        for (const auto& file : files)
            compareFiles(file, "testFiles/" + file);
    }
}
