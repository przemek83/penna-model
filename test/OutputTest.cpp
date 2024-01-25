#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "Common.h"
#include "FileOutput.h"
#include "MockedGenerator.h"
#include "Simulation.h"

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

class testRunListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(
        [[maybe_unused]] Catch::TestRunInfo const& testRunInfo) override
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = 1000;

        MockedGenerator generator;
        FileOutput output(0, config.years_, 0);

        Simulation simulation1(config, 1, 50);
        const SingleSimulationData data1{simulation1.run(generator, output)};
        std::cout << std::endl;
    }
};

CATCH_REGISTER_LISTENER(testRunListener)

TEST_CASE("Output", "[penna]")
{
    SECTION("initial population")
    {
        const std::string file{"proces1_symulacja1_initialPopulation.txt"};
        Common::compareFiles(file, "TestFiles/" + file);
    }

    SECTION("final population")
    {
        const std::string file{"proces1_symulacja1_finalPopulation.txt"};
        Common::compareFiles(file, "TestFiles/" + file);
    }

    SECTION("families")
    {
        const std::string file{"proces1_symulacja1_rodziny.txt"};
        Common::compareFiles(file, "TestFiles/" + file);
    }

    SECTION("statistics")
    {
        const std::string file{"proces1_symulacja1_statystyki.txt"};
        Common::compareFiles(file, "TestFiles/" + file);
    }

    SECTION("bits distribution")
    {
        const std::string file{"proces1_symulacja1_rozklad_bitow.txt"};
        Common::compareFiles(file, "TestFiles/" + file);
    }

    SECTION("age distribution")
    {
        const std::string file{"proces1_symulacja1_rozklad_wieku.txt"};
        Common::compareFiles(file, "TestFiles/" + file);
    }

    SECTION("deaths distribution")
    {
        const std::string file{"proces1_symulacja1_gompertz.txt"};
        Common::compareFiles(file, "TestFiles/" + file);
    }
}

TEST_CASE("Output averages", "[penna]")
{
    SECTION("save averages")
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = 1000;

        MockedGenerator generator;
        FileOutput output(0, config.years_, 0);

        Simulation simulation1(config, 1, 50);
        const SingleSimulationData data1{simulation1.run(generator, output)};
        std::cout << std::endl;

        Simulation simulation2(config, 2, 50);
        const SingleSimulationData data2{simulation2.run(generator, output)};
        std::cout << std::endl;

        SimulationAverages simulationAverages{
            prepareSimulationData<float>(config.years_)};
        integrateData(simulationAverages, data1);
        integrateData(simulationAverages, data2);

        prepareFinalResults(2, config.years_, simulationAverages);

        output.saveAverages(simulationAverages);

        const std::vector<std::string> files{
            "proces1_symulacja0_gompertz.txt", "proces1_symulacja0_rodziny.txt",
            "proces1_symulacja0_rozklad_bitow.txt",
            "proces1_symulacja0_rozklad_wieku.txt",
            "proces1_symulacja0_statystyki.txt"};

        for (const auto& file : files)
            Common::compareFiles(file, "TestFiles/" + file);
    }
}
