#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "Common.h"
#include "FileOutput.h"
#include "MockedGenerator.h"
#include "NullOutput.h"
#include "Simulation.h"
#include "test/StringOutput.h"

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

namespace
{
StringOutput output;
}

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
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::INITIAL_POPULATION),
            "TestFiles/" + file);
    }

    SECTION("final population")
    {
        const std::string file{"proces1_symulacja1_finalPopulation.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::FINAL_POPULATION),
            "TestFiles/" + file);
    }

    SECTION("families")
    {
        const std::string file{"proces1_symulacja1_rodziny.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::FAMILIES),
            "TestFiles/" + file);
    }

    SECTION("statistics")
    {
        const std::string file{"proces1_symulacja1_statystyki.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::STATISTICS),
            "TestFiles/" + file);
    }

    SECTION("bits distribution")
    {
        const std::string file{"proces1_symulacja1_rozklad_bitow.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::BITS_DISTRIBUTION),
            "TestFiles/" + file);
    }

    SECTION("age distribution")
    {
        const std::string file{"proces1_symulacja1_rozklad_wieku.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::AGE_DISTRIBUTION),
            "TestFiles/" + file);
    }

    SECTION("deaths distribution")
    {
        const std::string file{"proces1_symulacja1_gompertz.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::DEATHS_DISTRIBUTION),
            "TestFiles/" + file);
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
        NullOutput nullOutput;

        Simulation simulation1(config, 1, 50);
        const SingleSimulationData data1{
            simulation1.run(generator, nullOutput)};
        std::cout << std::endl;

        Simulation simulation2(config, 2, 50);
        const SingleSimulationData data2{
            simulation2.run(generator, nullOutput)};
        std::cout << std::endl;

        SimulationAverages simulationAverages{
            prepareSimulationData<float>(config.years_)};
        integrateData(simulationAverages, data1);
        integrateData(simulationAverages, data2);

        prepareFinalResults(2, config.years_, simulationAverages);

        FileOutput output(0, config.years_, 0);
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
