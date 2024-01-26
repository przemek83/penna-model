#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include "Common.h"
#include "MockedGenerator.h"
#include "NullOutput.h"
#include "Simulation.h"
#include "StringOutput.h"

namespace
{
const int years{1000};
StringOutput output(0, years, 0);
}  // namespace

class testRunListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(
        [[maybe_unused]] Catch::TestRunInfo const& testRunInfo) override
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = years;

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
            output.getContentForOutputType(Output::INITIAL_POPULATION), file);
    }

    SECTION("final population")
    {
        const std::string file{"proces1_symulacja1_finalPopulation.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::FINAL_POPULATION), file);
    }

    SECTION("families")
    {
        const std::string file{"proces1_symulacja1_rodziny.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::FAMILIES), file);
    }

    SECTION("statistics")
    {
        const std::string file{"proces1_symulacja1_statystyki.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::STATISTICS), file);
    }

    SECTION("bits distribution")
    {
        const std::string file{"proces1_symulacja1_rozklad_bitow.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::BITS_DISTRIBUTION), file);
    }

    SECTION("age distribution")
    {
        const std::string file{"proces1_symulacja1_rozklad_wieku.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::AGE_DISTRIBUTION), file);
    }

    SECTION("deaths distribution")
    {
        const std::string file{"proces1_symulacja1_gompertz.txt"};
        Common::compareStringWithFileContent(
            output.getContentForOutputType(Output::DEATHS_DISTRIBUTION), file);
    }
}

TEST_CASE("Output averages", "[penna]")
{
    SECTION("save averages")
    {
        Config config;
        config.maxPopulation_ = 5000;
        config.years_ = years;

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

        output.reset();
        output.saveAverages(simulationAverages);

        const std::map<Output::OUTPUT_TYPE, std::string> files{
            {Output::DEATHS_DISTRIBUTION, "proces1_symulacja0_gompertz.txt"},
            {Output::FAMILIES, "proces1_symulacja0_rodziny.txt"},
            {Output::BITS_DISTRIBUTION, "proces1_symulacja0_rozklad_bitow.txt"},
            {Output::AGE_DISTRIBUTION, "proces1_symulacja0_rozklad_wieku.txt"},
            {Output::STATISTICS, "proces1_symulacja0_statystyki.txt"}};

        for (const auto& [outputType, file] : files)
            Common::compareStringWithFileContent(
                output.getContentForOutputType(outputType), file);
    }
}
