#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "Common.h"
#include "MockedGenerator.h"
#include "Output.h"
#include "Simulation.h"

TEST_CASE("Output", "[penna]")
{
    SECTION("save averages")
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = 1000;

        SimulationAverages simulationAverages{
            prepareSimulationAverages(config.years_)};

        MockedGenerator generator;
        Simulation simulation1(config, 1, 50);
        simulation1.run(generator, simulationAverages);
        std::cout << std::endl;

        Simulation simulation2(config, 2, 50);
        simulation2.run(generator, simulationAverages);
        std::cout << std::endl;

        prepareFinalResults(2, config.years_, simulationAverages);

        Output output(0, config.years_, 0);

        output.saveAverages(simulationAverages);

        std::vector<std::string> files{"proces1_symulacja0_gompertz.txt",
                                       "proces1_symulacja0_rodziny.txt",
                                       "proces1_symulacja0_rozklad_bitow.txt",
                                       "proces1_symulacja0_rozklad_wieku.txt",
                                       "proces1_symulacja0_statystyki.txt"};

        for (const auto& file : files)
            Common::compareFiles(file, "TestFiles/" + file);
    }
}
